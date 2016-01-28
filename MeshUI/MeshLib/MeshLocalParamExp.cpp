#include "stdafx.h"
#include "MeshLocalParamExp.h"
#include <set>
#include <queue>
#include <map>
//////////////////////////////////////////////////////////////////////
// CNormalCoordinate Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNormalCoordinate::CNormalCoordinate()
{
        this->m_Center_Vtx_Id = -1;
        this->m_Center_Vtx_Pos = Coord(0, 0, 0);
        this->m_X_Coord = Coord(0, 0, 0);
        this->m_Y_Coord = Coord(0, 0, 0);
        this->m_N_Coord = Coord(0, 0, 0);
        this->m_NeighVtx_On3D.clear();
}
CNormalCoordinate::~CNormalCoordinate()
{
        this->m_NeighVtx_On3D.clear();
}
//////////////////////////////////////////////////////////////////////
// CNormalCoordinate Methods
//////////////////////////////////////////////////////////////////////
void CNormalCoordinate::SetNC(int cvid, Coord& cvpos, Coord& cvnormal)
{
        this->m_Center_Vtx_Id = cvid;
        this->m_Center_Vtx_Pos = cvpos;
        this->m_N_Coord = cvnormal.unit();
        this->m_X_Coord = Coord(cvnormal[2], 0, -cvnormal[0]).unit();
        this->m_Y_Coord = cross(m_X_Coord, m_N_Coord).unit();
}
void CNormalCoordinate::AddNeighVertex(int vid, Coord& vpos)
{
        Coord pq = vpos - m_Center_Vtx_Pos;
        double pq_len = pq.abs();
        pq.normalize();

        Coord p_pq = pq - m_N_Coord * dot(pq, m_N_Coord);
        p_pq.normalize();
        p_pq *= pq_len;

        m_NeighVtx_On3D.push_back(make_pair(vid, p_pq));
}
void CNormalCoordinate::Rotate(Coord& othernormal)
{
        if (this->m_N_Coord == othernormal)
        {
                return;
        }

        // rotate normal;
        Coord axis = cross(othernormal, this->m_N_Coord);
        double rangle = acos(dot(othernormal, this->m_N_Coord));

        for (size_t i = 0; i < m_NeighVtx_On3D.size(); i++)
        {
                pair<int, Coord>& nvtx3D = m_NeighVtx_On3D[i];
                double dlen = nvtx3D.second.abs();
                Coord rcoord = nvtx3D.second.Spin(axis, Coord(0, 0, 0), rangle);
                nvtx3D.second = rcoord.unit() * dlen;
        }
}


//////////////////////////////////////////////////////////////////////
// CMeshLocalParamExp Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMeshLocalParamExp::CMeshLocalParamExp()
{
}
CMeshLocalParamExp::~CMeshLocalParamExp()
{
}
//////////////////////////////////////////////////////////////////////
// CMeshLocalParamExp public methods
//////////////////////////////////////////////////////////////////////
void CMeshLocalParamExp::Attach(MeshModel* pModel)
{
        ASSERT(pModel != NULL);
        mesh = pModel;
}
void CMeshLocalParamExp::LocalParam(int cvid, vector<int>& tri_index, vector<int>& vtx_index, vector<double>& param_uv)
{
        PolyIndexArray& faceIndex = mesh->m_Kernel.GetFaceInfo().GetIndex();
        PolyIndexArray& vtxadjVtxArray = mesh->m_Kernel.GetVertexInfo().GetAdjVertices();
        CoordArray& vtxNormalArray = mesh->m_Kernel.GetVertexInfo().GetNormal();
        CoordArray& vtxCoordArray = mesh->m_Kernel.GetVertexInfo().GetCoord();

        //
        set<int> vtxidSet;
        for (size_t i = 0; i < tri_index.size(); i++)
        {
                IntArray& face = faceIndex[tri_index[i]];
                for (size_t j = 0; j < face.size(); j ++)
                {
                        vtxidSet.insert(face[j]);
                }
        }
        size_t vtxnum = vtxidSet.size();

        //
        map<int, int> vid2indexMap;
        int vindex = 0;
        for (set<int>::iterator iter = vtxidSet.begin(); iter != vtxidSet.end(); iter++)
        {
                vtx_index.push_back(*iter);
                vid2indexMap.insert(make_pair(*iter, vindex++));
        }
        vector<pair<bool, Coord> > dijkstraVector(vtxnum);
        fill(dijkstraVector.begin(), dijkstraVector.end(), make_pair(false, Coord(0, 0, 0)));
        dijkstraVector[vid2indexMap[cvid]].first = true;                                                                

        //
        queue<int> vtxidQueue;
        vtxidQueue.push(cvid);
        Coord baseNormal = vtxNormalArray[cvid].unit();

        while (!vtxidQueue.empty())
        {
                int vid = vtxidQueue.front();
                vtxidQueue.pop();


                CNormalCoordinate nc;
                nc.SetNC(vid, vtxCoordArray[vid], vtxNormalArray[vid]);

                IntArray& adjVtx = vtxadjVtxArray[vid];
                for (size_t i = 0; i < adjVtx.size(); i++)
                {
                        if (vtxidSet.find(adjVtx[i]) != vtxidSet.end())
                        {
                                nc.AddNeighVertex(adjVtx[i], vtxCoordArray[adjVtx[i]]);
                        }
                }
                nc.Rotate(baseNormal);

                Coord perCoord = dijkstraVector[vid2indexMap[vid]].second;
                for (size_t i = 0; i < nc.m_NeighVtx_On3D.size(); i++)
                {
                        pair<int, Coord>& nvtx3D = nc.m_NeighVtx_On3D[i];
                        pair<bool, Coord>& dijdis = dijkstraVector[vid2indexMap[nvtx3D.first]];

                        if (!dijdis.first)
                        {
                                Coord addCoord = perCoord + nvtx3D.second;
                                dijkstraVector[vid2indexMap[nvtx3D.first]] = make_pair(true, addCoord);

                                vtxidQueue.push(adjVtx[i]);
                        }
                }
        }

        Project(baseNormal, dijkstraVector, param_uv);
}
void CMeshLocalParamExp::Project(Coord& ncnormal, vector<pair<bool, Coord> >& dijvectors, vector<double>& param_uv)
{
        Coord nCoord = ncnormal.unit();
        Coord xCoord = Coord(ncnormal[2], 0, -ncnormal[0]).unit();
        Coord yCoord = cross(xCoord, nCoord).unit();

        CMatrix M33(3, 3);
        CMatrix M31(3, 1);

        for (int i = 0; i < 3; i++)
        {
                M33.SetElement(i, 0, xCoord[i]);
                M33.SetElement(i, 1, yCoord[i]);
                M33.SetElement(i, 2, nCoord[i]);
        }
        BOOL rst = M33.InvertGaussJordan();
        ASSERT(rst);

        for (size_t i = 0; i < dijvectors.size(); i++)
        {
                pair<bool, Coord>& dijvec = dijvectors[i];
                for (int j = 0; j < 3; j++)
                {
                        M31.SetElement(j, 0, dijvec.second[j]);
                }
                CMatrix Mb = M33 * M31;
                double pu = Mb.GetElement(0, 0);
                double pv = Mb.GetElement(1, 0);
                param_uv.push_back(pu);
                param_uv.push_back(pv);
        }
}
#include "stdafx.h"
#include "CriticalPoint.h"

CriticalPoint::CriticalPoint(vector<M_Point*>& vtx) :
m_vtx(vtx),p_SadPointID(NULL), p_MaxPointID(NULL),p_MinPointID(NULL)
{}

CriticalPoint::~CriticalPoint()
{}

CriticalPoint::CriticalPoint(CommonData& cd) :
m_vtx(cd.m_vtx),
p_SadPointID(&cd.m_SadPointID),
p_MaxPointID(&cd.m_MaxPointID),
p_MinPointID(&cd.m_MinPointID),
p_DualConn(&cd.m_DualConn),
p_EigenValue(&cd.m_EigenValue)
{}

bool CriticalPoint::LoadEigenField(string filename)
{
        ifstream ifs(filename.c_str());
        if(ifs.fail()) return false;

        int num;
        ifs >> num;

        p_EigenValue->clear();
        double eigen;

        for (int i = 0; i < num; i++)
        {
                ifs >> eigen;
                p_EigenValue->push_back(eigen);
        }

        ifs.close();

        printf("read eigenvector data ok...\n");
        VtxValue2VtxColor(*p_EigenValue);

        FindCriticalPoints();
        return true;
}

Global::POINTTYPE CriticalPoint::CheckVertexType(vector<bool>& adjFlag)
{
        bool flag1 = adjFlag[0];
        bool flag2 = adjFlag[0];
        int cNum = 0;
        size_t adjNum = adjFlag.size();

        for (size_t i = 1; i <= adjFlag.size(); i++)
        {
                size_t index = i % adjNum;

                flag1 = (flag1 && adjFlag[index]);
                flag2 = (flag2 || adjFlag[index]);
                if (adjFlag[index] != adjFlag[i - 1])
                {
                        cNum++;
                }
        }

        if (flag1)      return Global::MAXPOINT;
        if (!flag2)     return Global::MINPOINT;
        if (cNum == 4)  return Global::SADDLEPOINT;
        return Global::REGULARPOINT;
}

void CriticalPoint::FindCriticalPoints()
{
        p_MaxPointID->clear();  p_MinPointID->clear();  p_SadPointID->clear();

        PolyIndexArray& adjVerticesArray = *(Global::p_adjVtxArray);
        CoordArray& vCoord = *(Global::p_coord);
        size_t nVertex = vCoord.size();

        m_vtx.clear();
        m_vtx.resize(Global::vtxNum);

        vector<bool> adjFlag;
        for (size_t i = 0; i < nVertex; ++i)
        {
                // set the adj vertex flag array.
                double value = (*p_EigenValue)[i];
                IndexArray& adjVertices = adjVerticesArray[i];

                adjFlag.clear();
                adjFlag.resize(adjVertices.size());
                fill(adjFlag.begin(), adjFlag.end(), false);

                for (size_t j = 0; j < adjVertices.size(); j++)
                {
                        if (value > (*p_EigenValue)[adjVertices[j]])
                                adjFlag[j] = true;
                }

                if (Global::mesh->m_BasicOp.IsBoundaryVertex((int)i))
                {
                        vector<bool> tmpadjFlag(adjFlag);
                        for (int k = (int)tmpadjFlag.size() - 1; k >=0; k--)
                        {
                                adjFlag.push_back(tmpadjFlag[k]);
                        }
                }

                // check the point type.
                Global::POINTTYPE type= CheckVertexType(adjFlag);
                // set the point's type
                
                if(type == Global::MAXPOINT)
                {
                        m_vtx[i] = new MaxPoint(i);
                        p_MaxPointID->insert(i); 
                }else if(type == Global::MINPOINT)
                {
                        m_vtx[i] = new MinPoint(i);
                        p_MinPointID->insert(i);
                }else if(type == Global::SADDLEPOINT)
                {
                        m_vtx[i] = new SadPoint(i);
                        p_SadPointID->insert(i);
                }else{
                        m_vtx[i] = new M_Point(i);
                }
                
        }
        //      SetCPVertexFlag();
        //      fill(m_cp_vtx_mapping.begin(), m_cp_vtx_mapping.end(), -1);

        //      set_dual_max_min_vertex_mapping();

        printf("FindCriticalPoint ok\n");

        TraceLine();
        printf("max: %d, min: %d, saddle: %d\n", p_MaxPointID->size(), p_MinPointID->size(), p_SadPointID->size());
}

void CriticalPoint::TraceLine()
{
        /* TraceLine : from each saddle point, find its corresponding four max-min points */
        for(set<size_t>::iterator it=p_SadPointID->begin(); it!=p_SadPointID->end(); ++it)
        {
                M_Point& sadPoint = *(m_vtx[*it]);
                TraceLineFromSaddle(dynamic_cast<SadPoint&>(sadPoint));
                TraceLine4MaxMin(dynamic_cast<SadPoint&>(sadPoint));
        }


        for(map<MaxMinPair,IndexArray>::iterator im = p_DualConn->begin(); im!=p_DualConn->end(); ++im)
        {
                //Global::mesh->m_BasicOp.GetShortestPath(
                //      (VertexID)(im->first).first, (VertexID)(im->first).second, im->second);
                FindShortestPath(m_vtx,(im->first).first, (im->first).second, im->second);
        }
}

void CriticalPoint::TraceLineFromSaddle(SadPoint& sadP)
{
        bool rst = FindMaxMinFromSadNeigh(sadP);

        if (rst)
        {
                TraceMaxLineFromSaddle(sadP,sadP.m_MaxPoints[0],sadP.m_MaxPath[0]);
                TraceMaxLineFromSaddle(sadP,sadP.m_MaxPoints[1],sadP.m_MaxPath[1]);
                TraceMinLineFromSaddle(sadP,sadP.m_MinPoints[0],sadP.m_MinPath[0]);
                TraceMinLineFromSaddle(sadP,sadP.m_MinPoints[1],sadP.m_MinPath[1]);
        }

}

void CriticalPoint::TraceMaxLineFromSaddle(SadPoint& sadP, size_t& maxP, IntArray& pathArray)
{
        /* TraceMaxLineFromSaddle : from a saddle point, find a path to a max point
         * maxP : sadP.maxPoints*
         * pathArray : sadP.maxPath*
         */
        CoordArray& vCoord = *(Global::p_coord);
        size_t nextVID = maxP;
        pathArray.clear();
        pathArray.push_back(sadP.getId());

        double lensum = 0;
        while( ((m_vtx)[nextVID])->getType() != Global::MAXPOINT)
        {
                pathArray.push_back(nextVID);
                double len = (vCoord[pathArray[pathArray.size() - 2]] - vCoord[pathArray[pathArray.size() - 1]]).abs();
                lensum += len;

                if (lensum < Global::maxLineLength)
                {
                        nextVID = FindMaxEigenValueFromVertexNeigh(nextVID);
                }
                else return;
        }
        
        pathArray.push_back(nextVID);
        maxP = nextVID;
        
}

void CriticalPoint::TraceMinLineFromSaddle(SadPoint& sadP,size_t& minP,IntArray& pathArray)
{
        CoordArray& vCoord = *(Global::p_coord);
        size_t nextVID = minP;
        pathArray.push_back(sadP.getId());

        double lensum = 0;
        while (m_vtx[nextVID]->getType() != Global::MINPOINT)
        {
                pathArray.push_back(nextVID);
                double len = (vCoord[pathArray[pathArray.size() - 2]] - vCoord[pathArray[pathArray.size() - 1]]).abs();
                lensum += len;

                if (lensum < Global::maxLineLength)
                {
                        nextVID = FindMinEigenValueFromVertexNeigh(nextVID);
                }
                else return;
        }
        pathArray.push_back(nextVID);
        minP = nextVID;
}

size_t CriticalPoint::FindMaxEigenValueFromVertexNeigh(size_t vid)
{
        IndexArray& adjVertices = ((*Global::p_adjVtxArray))[vid];

        double maxValue = -1e20;
        size_t maxcpVid = 0;
        for (size_t i = 0; i < adjVertices.size(); i++)
        {
                size_t vid = adjVertices[i];
                if ((*p_EigenValue)[vid] > maxValue)
                {
                        maxValue = (*p_EigenValue)[vid];
                        maxcpVid = vid;
                }                 
        }
        return maxcpVid;
}

size_t CriticalPoint::FindMinEigenValueFromVertexNeigh(size_t vid)
{
        IndexArray& adjVertices = ((*Global::p_adjVtxArray))[vid];

        double minValue = 1e20;
        size_t mincpVid = 0;
        for (size_t i = 0; i < adjVertices.size(); i++)
        {
                int vid = adjVertices[i];

                if ((*p_EigenValue)[vid] < minValue)
                {
                        minValue = (*p_EigenValue)[vid];
                        mincpVid = vid;
                }                 
        }

        return mincpVid;
}

void CriticalPoint::FindMaxMinMapping2Sad()
{
        size_t sadVid, maxVid, minVid;
        
        for(set<size_t>::iterator it=p_MaxPointID->begin(); it!=p_MaxPointID->end();++it)
        {
                dynamic_cast<MaxPoint*>(m_vtx[*it])->m_SadPoints.clear();
        }
        
        for(set<size_t>::iterator it=p_MinPointID->begin(); it!=p_MinPointID->end();++it)
        {
                dynamic_cast<MinPoint*>(m_vtx[*it])->m_SadPoints.clear();
        }

        for(set<size_t>::iterator it=p_SadPointID->begin(); it!=p_SadPointID->end();++it)
        {
                sadVid = m_vtx[*it]->getId();
                SadPoint& sadP = dynamic_cast<SadPoint&>(*m_vtx[sadVid]);
                for(size_t k=0;k<sadP.m_MaxPoints.size(); ++k)
                {
                        maxVid = sadP.m_MaxPoints[k];
                        MaxPoint& maxP = dynamic_cast<MaxPoint&>(*m_vtx[maxVid]);
                        maxP.m_SadPoints.push_back(sadVid);
                }
                for(size_t k=0;k<sadP.m_MinPoints.size(); ++k)
                {
                        minVid = sadP.m_MinPoints[k];
                        MinPoint& minP = dynamic_cast<MinPoint&>(*m_vtx[minVid]);
                        minP.m_SadPoints.push_back(sadVid);
                }
        }

}

bool CriticalPoint::FindMaxMinFromSadNeigh(SadPoint& sadP)
{
        /* FindMaxMniFromSadNeigh : from a saddle point,find its corresponding 
         * four max-min points in its neigh
         */
        PolyIndexArray& adjVerticesArray = Global::mesh->m_Kernel.GetVertexInfo().GetAdjVertices();
        size_t sadVid = sadP.m_vid;
        double value = (*p_EigenValue)[sadVid];
        IndexArray& adjVertices = adjVerticesArray[sadVid];

        // find 4 range index here.
        vector<size_t> rangeIndex;
        size_t adjNum = adjVertices.size();
        for (size_t j = 1; j <= adjNum; j++)
        {
                size_t nowIndex = j % adjNum;
                size_t beforeIndex = j - 1;

                if ((*p_EigenValue)[adjVertices[nowIndex]] < value && (*p_EigenValue)[adjVertices[beforeIndex]] > value)
                {
                        rangeIndex.push_back(nowIndex);
                }
                else if((*p_EigenValue)[adjVertices[nowIndex]] > value && (*p_EigenValue)[adjVertices[beforeIndex]] < value)
                {
                        rangeIndex.push_back(nowIndex);
                }
        }

        if (rangeIndex.size() == 4)
        {
                size_t min1 = FindMinPInRange(sadVid, make_pair(rangeIndex[0], rangeIndex[1]));
                size_t max1 = FindMaxPInRange(sadVid, make_pair(rangeIndex[1], rangeIndex[2]));
                size_t min2 = FindMinPInRange(sadVid, make_pair(rangeIndex[2], rangeIndex[3]));
                size_t max2 = FindMaxPInRange(sadVid, make_pair(rangeIndex[3], rangeIndex[0]));

                sadP.m_MaxPoints.push_back(max1); sadP.m_MaxPoints.push_back(max2);
                sadP.m_MinPoints.push_back(min1); sadP.m_MinPoints.push_back(min2);
                sadP.m_MaxPath.resize(2); sadP.m_MinPath.resize(2);
                return true;
        }

        return false;
}

size_t CriticalPoint::FindMinPInRange(size_t vid, const pair<size_t,size_t>& range)
{
        IndexArray& adjVertices = (*(Global::p_adjVtxArray))[vid];

        double minValue = 1e20;
        size_t mincpVid = 0;
        size_t adjNum = adjVertices.size();
        size_t sIndex = range.first;
        size_t eIndex = range.second < range.first ? range.second + adjNum: range.second;

        for (size_t i = sIndex; i <= eIndex; i++)
        {
                int vid = adjVertices[i % adjNum];
                if ((*p_EigenValue)[vid] < minValue)
                {
                        minValue = (*p_EigenValue)[vid];
                        mincpVid = vid;
                }                 
        }

        return mincpVid;
}

size_t CriticalPoint::FindMaxPInRange(size_t vid, const pair<size_t,size_t>& range)
{
        IndexArray& adjVertices = (*(Global::p_adjVtxArray))[vid];
        
        double maxValue = -1e20;
        size_t maxcpVid = 0;
        size_t adjNum = adjVertices.size();
        size_t startIndex = range.first;
        size_t endIndex = range.second < range.first ? range.second + adjNum : range.second;

        for (size_t i = startIndex; i <= endIndex; i++)
        {
                size_t vid = adjVertices[i % adjNum];
                if ((*p_EigenValue)[vid] > maxValue)
                {
                        maxValue = (*p_EigenValue)[vid];
                        maxcpVid = vid;
                }                 
        }

        return maxcpVid;
}


void CriticalPoint::TraceLine4MaxMin(SadPoint& sadP)
{
        /* TraceLine4MaxMin : from the saddle point corresponding max-min points
         * connect the max-min pair
         */
        size_t maxVid, minVid;
        for(size_t i=0; i<sadP.m_MaxPoints.size(); ++i)
        {
                maxVid = sadP.m_MaxPoints[i];
                for(size_t j=0; j<sadP.m_MinPoints.size(); ++j)
                {
                        minVid = sadP.m_MinPoints[j];
                        (*p_DualConn)[make_pair(maxVid,minVid)] = IndexArray();
                }
        }
}

void CriticalPoint::clear()
{
        p_EigenValue->clear();
        
        p_EigenValue = NULL;
        p_SadPointID = NULL;
        p_MaxPointID = NULL;
        p_MinPointID = NULL;

}

void CriticalPoint::VtxValue2VtxColor(vector<double>& vertexValue)
{
        /* VertexValue2VertexColor : 
        * vertexValue - the eigenvalue of every vertex
        */
        Global::mesh->m_AuxData.ClearData();
        ColorArray& colorArray = Global::mesh->m_Kernel.GetVertexInfo().GetColor();
        colorArray.clear();

        CHSVColor color;
        color.RGBtoHSV(1, 0, 0);
        color.m_S = 0.9f;
        color.m_V = 0.9f;

        size_t vNum = Global::vtxNum;

        double min = 1e20, max = -1e20;
        double avg = 0;
        for(size_t i = 0; i < vNum; ++i)
        {
                if(vertexValue[i] < min)        min = vertexValue[i];
                if(vertexValue[i] > max)        max = vertexValue[i];
        }

        double range = (max - min) * 1.1;

        bool eq = ALMOST_EQUAL_LARGE(range, 0.0);

        for(size_t i = 0; i < vertexValue.size();++i)
        {
                float R, G, B;
                if (eq)
                {
                        color.m_H = (float) 0.5 * 255;
                }
                else
                {
                        double prop = (vertexValue[i] - min) / range;

                        //color.m_S = prop;
                        color.m_H = (1 - prop) * 255;
                }
                color.HSVtoRGB(&R, &G, &B);
                Color c(R, G, B);
                colorArray.push_back(c);
        }
}
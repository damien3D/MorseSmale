#include "stdafx.h"
#include "CommonData.h"
#include <queue>

#pragma warning(disable: 4267)

void CommonData::clear()
{
        m_vtx.clear();
        m_SadPointID.clear();
        m_MaxPointID.clear();
        m_MinPointID.clear();

        m_DualConn.clear();
        m_EigenValue.clear();

}

void FindShortestPath(vector<M_Point*>& m_vtx,size_t start, size_t end, IntArray& path)
{
        path.clear();
        if(start==end) {
                path.push_back(start); return;
        }
        PolyIndexArray& adjVtxArray = Global::mesh->m_Kernel.GetVertexInfo().GetAdjVertices();
        map<int,int> preVtx; // 
        preVtx[end] = -1;
        map<int,double> dist; // 
        dist[end] = 0.0;
        set<int> vtxInQueue; // vertexes in queue
        queue<int> q;
        q.push(end);
        vtxInQueue.insert(end);

        M_Point* mp;
        double curLen;
        map<int,double>::iterator im;
        while(!q.empty())
        {
                int u = q.front(); q.pop(); 
                vtxInQueue.erase( vtxInQueue.find(u));
                if(u==start) break;

                double len = dist[u];
                IndexArray& adjVtxes = adjVtxArray[u];
                int nv = (int)adjVtxes.size();
                for(int k=0; k<nv; ++k)
                {
                        int vid = adjVtxes[k];
                        mp = m_vtx[vid];
                //      if(mp->m_isPathVtx==true && mp->getType() == Global::REGULARPOINT ) continue;

                        im = dist.find(vid);
                        if(im == dist.end()) curLen=INFINITE_DISTANCE;
                        else curLen = im->second;
                        double d = ((*Global::p_coord)[vid] - (*Global::p_coord)[u]).abs();
                        if( len + d < curLen){
                                dist[vid] = len+d;
                                preVtx[vid] = u;
                                if(vtxInQueue.find(vid) == vtxInQueue.end()){
                                        vtxInQueue.insert(vid);
                                        q.push(vid);
                                }
                        }
                }
        }

        int curVID = start, preVID = preVtx[curVID];
        while(preVID!= -1){
                m_vtx[curVID]->m_isPathVtx = true;
                path.push_back(curVID);
                curVID = preVID;
                preVID = preVtx[curVID];
        }
        path.push_back(end);
        (m_vtx[end])->m_isPathVtx = true;
/*
        printf("\n Path = { ");
        for(size_t k=0; k<path.size(); ++k)
        {
                printf("%d ", path[k]);
        }
        printf("}\n");
        */
}

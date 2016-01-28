#include "stdafx.h"
#include "NoiseRemoval.h"
#include <queue>
#include <cstdio>

#pragma warning(disable:4267)

NoiseRemoval::NoiseRemoval():
p_vtx(NULL),p_SadPointID(NULL), p_MaxPointID(NULL),p_MinPointID(NULL),p_EigenValue(NULL)
{}

NoiseRemoval::NoiseRemoval(CommonData& cd):
p_vtx(&cd.m_vtx),
p_MaxPointID(&cd.m_MaxPointID),
p_MinPointID(&cd.m_MinPointID),
p_SadPointID(&cd.m_SadPointID),
p_EigenValue(&cd.m_EigenValue),
p_DualConn(&cd.m_DualConn)
{}

NoiseRemoval::~NoiseRemoval(){
        p_vtx                   = NULL;
        p_SadPointID    = NULL;
        p_MaxPointID    = NULL;
        p_MinPointID    = NULL;
        p_EigenValue    = NULL;
        p_DualConn              = NULL;
}

void NoiseRemoval::CalPersistence()
{
        //CalPersistence : Calculate the persistence between saddle-point and maxmin-point
        m_persistence.clear();
        size_t sadVid, maxVid, minVid;
        for(set<size_t>::iterator it=p_SadPointID->begin(); it!=p_SadPointID->end(); ++it)
        {
                SadPoint& sadP = dynamic_cast<SadPoint&> (*(*p_vtx)[*it]);
                sadVid = sadP.getId();
                for(size_t k=0;k<sadP.m_MaxPoints.size(); ++k)
                {
                        maxVid = sadP.m_MaxPoints[k];
                        m_persistence[make_pair(sadVid,maxVid)] = 
                                fabs((*p_EigenValue)[sadVid] - (*p_EigenValue)[maxVid]);
                }
                for(size_t k=0;k<sadP.m_MinPoints.size(); ++k)
                {
                        minVid = sadP.m_MinPoints[k];
                        m_persistence[make_pair(sadVid,minVid)] =
                                fabs((*p_EigenValue)[sadVid] - (*p_EigenValue)[minVid]);
                }
        }

        double sumPersistence=0,maxPersistence=-1e10, minPersistence=1e10;
        for(map<pair<size_t,size_t>,double> ::iterator it = m_persistence.begin();it!=m_persistence.end();it++)
        {
                sumPersistence += (it->second)*(it->second);
                if(it->second > maxPersistence) maxPersistence = it->second;
                if(it->second < minPersistence) minPersistence = it->second;
        }

        sumPersistence = sqrt(sumPersistence);
        maxPersistence /= sumPersistence; 
        minPersistence /= sumPersistence;
        printf("MinPersistence : %.6f \nMaxPersistence : %.6f\nSumPersistence: %.6f\n",minPersistence, maxPersistence, sumPersistence);
        for(map<pair<size_t,size_t>,double> ::iterator it = m_persistence.begin();it!=m_persistence.end();it++)
        {
                it->second = (it->second)/sumPersistence;
        }
}

void NoiseRemoval::RemoveNoise()
{
        disjoinSet.reSize(p_vtx->size());
        disjoinSet.Init(0);
        
        m_badPoints.clear();
        m_badPointCluster.clear();

        // find the bad critical points and union adj points
        for(map<pair<size_t,size_t>,double>::iterator it = m_persistence.begin(); it!=m_persistence.end(); it++)
        {
                if(it->second<m_threshold.first || it->second>m_threshold.second)
                {
                        int vid1 = (int) (it->first).first, vid2 = (int)(it->first).second;
                        disjoinSet.Union(vid1,vid2);
                        m_badPoints.insert(vid1);
                        m_badPoints.insert(vid2);
                }
        }
        // make cluster 
        for(set<size_t>::iterator is = m_badPoints.begin(); is!=m_badPoints.end();is++)
        {
                int vid = (int) *is;
                size_t fv = (size_t) disjoinSet.Find(vid);
                m_badPointCluster[fv].push_back(vid);
        }
        // make the cluster
        m_NoiseCluster.clear(); m_NoiseCluster.resize(m_badPointCluster.size());
        size_t nC=0;
        for(map<size_t,vector<size_t>>::iterator im=m_badPointCluster.begin(); im!=m_badPointCluster.end(); ++im,++nC)
        {
                m_NoiseCluster[nC].noisePoints.assign(im->second.begin(), im->second.end());
        }

        RemoveBadCriticalPoints();
        CheckDualBadConnLine();
}

void NoiseRemoval::RemoveBadCriticalPoints()
{
        m_center_cluster_mapping.clear();       m_badvtx_cluster_mapping.clear();
        m_newAddSad.clear();
        for(vector<NoiseCluster>::iterator it = m_NoiseCluster.begin(); it!=m_NoiseCluster.end(); ++it)
        {
                size_t numMaxPoints(0), numMinPoints(0), numSadPoints(0);
                vector<size_t>& cluster = it->noisePoints;
                vector<Global::POINTTYPE>& pointType = it->noiseType;
                pointType.resize(cluster.size());
                for(size_t k=0; k<cluster.size(); ++k)
                {
                        size_t vid = cluster[k];
                        M_Point* mp = (*p_vtx)[vid];
                        Global::POINTTYPE type = mp->getType();
                        pointType[k] = type;
                        if(type == Global::MAXPOINT){
                                p_MaxPointID->erase( p_MaxPointID->find(vid));  numMaxPoints++;
                        }else if(type == Global::MINPOINT){
                                p_MinPointID->erase( p_MinPointID->find(vid));  numMinPoints++;
                        }else if(type == Global::SADDLEPOINT){
                                p_SadPointID->erase( p_SadPointID->find(vid));  numSadPoints++;
                        }
                        mp->setType(Global::REGULARPOINT);
                }
                size_t tmp_maxNum = max(numMaxPoints,max(numMinPoints,numSadPoints));
                size_t& centerP = it->centerPoint;
                if(numMaxPoints == tmp_maxNum){
                        it->type = Global::MAXPOINT;
                        centerP = FindCenterInCluster(*it, Global::MAXPOINT);
                        (*p_vtx)[centerP]->setType(Global::MAXPOINT);   p_MaxPointID->insert(centerP);
                }else if(numMinPoints == tmp_maxNum){
                        it->type = Global::MINPOINT;
                        centerP = FindCenterInCluster(*it, Global::MINPOINT);
                        (*p_vtx)[centerP]->setType(Global::MINPOINT);   p_MinPointID->insert(centerP);
                }else if(numSadPoints == tmp_maxNum){
                        it->type = Global::SADDLEPOINT;
                        centerP = FindCenterInCluster(*it, Global::SADDLEPOINT);
                        (*p_vtx)[centerP]->setType(Global::SADDLEPOINT); p_SadPointID->insert(centerP);
                        m_newAddSad.push_back(centerP);
                }
                m_center_cluster_mapping[centerP] = it-m_NoiseCluster.begin();
                for(size_t k=0; k<cluster.size(); ++k) 
                {
                        m_badvtx_cluster_mapping[cluster[k]] = it-m_NoiseCluster.begin();
                }
        }
        
        ModifyConnLine();
        ModifyDualConnLine();

        TestConnection();
}

void NoiseRemoval::ModifyConnLine()
{
        /* ModifyConnLine : After remove noise, we should modify the connection 
         * First, we modify the new add saddle point(have done)
         */
        

        set <size_t> unSolvedSadID;
        M_Point* pMPoint;
        size_t sadVid, maxVid, minVid;
        for(vector<size_t>::iterator it=m_newAddSad.begin(); it!=m_newAddSad.end(); ++it)
        {
                pMPoint = (*p_vtx)[*it];
                SadPoint& sadPoint = dynamic_cast<SadPoint&> (*pMPoint);
                ModifySadConnLine(sadPoint);
        }

        for(set<size_t>::iterator is=p_SadPointID->begin(); is!=p_SadPointID->end(); ++is)
        {
                sadVid = *is;
                pMPoint = (*p_vtx)[sadVid];
                ASSERT(pMPoint->getType() == Global::SADDLEPOINT);
                SadPoint& sadPoint = dynamic_cast<SadPoint&> (*pMPoint);
                ASSERT(sadPoint.m_MaxPoints.size()==2 && sadPoint.m_MinPoints.size()==2);
                // the max point may have been removed, so we check if the mapping vertex is a max point.
                // if it is, then set it as the max point ; else, we need find another max point
                for(size_t k=0; k<2; ++k)
                {
                        maxVid = sadPoint.m_MaxPoints[k];
                        if((*p_vtx)[maxVid]->getType() != Global::MAXPOINT)
                        {
                                // this vertex's mapping
                                size_t clusterID = m_badvtx_cluster_mapping[maxVid];
                                maxVid = m_NoiseCluster[clusterID].centerPoint;
                                if((*p_vtx)[maxVid]->getType() != Global::MAXPOINT)
                                {
                                        if((*p_vtx)[maxVid]->getType() == Global::SADDLEPOINT)
                                        {
                                                // this should be a new add saddle point
                                                SadPoint& newSad = dynamic_cast<SadPoint&>(*(*p_vtx)[maxVid]);
                                                size_t maxID1=newSad.m_MaxPoints[0],maxID2=newSad.m_MaxPoints[1];
                                                size_t another = sadPoint.m_MaxPoints[1-k];
                                                IntArray path1, path2;
                                                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)maxID1,path1);
                                                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)maxID2,path2);
                                                maxVid = (path1.size()>path2.size())?maxID2:maxID1;
                                        }else{
                                                // a complex case (there are three types point in the cluster)
                                                // we solve it late
                                                unSolvedSadID.insert(sadVid);
                                                continue;
                                        }
                                }
                                sadPoint.m_MaxPoints[k] = maxVid;
                                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)maxVid,sadPoint.m_MaxPath[k]);
                        }
                }

                for(size_t k=0; k<2; ++k)
                {
                        minVid = sadPoint.m_MinPoints[k];
                        if((*p_vtx)[minVid]->getType() != Global::MINPOINT)
                        {
                                size_t clusterID = m_badvtx_cluster_mapping[minVid];
                                minVid = m_NoiseCluster[clusterID].centerPoint;
                                if((*p_vtx)[minVid]->getType() != Global::MINPOINT)
                                {
                                        if((*p_vtx)[minVid]->getType() == Global::SADDLEPOINT)
                                        {
                                                SadPoint& newSad = dynamic_cast<SadPoint&>(*(*p_vtx)[minVid]);
                                                size_t minID1=newSad.m_MinPoints[0],minID2=newSad.m_MinPoints[1];
                                                size_t another = sadPoint.m_MinPoints[1-k];
                                                IntArray path1, path2;
                                                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)minID1,path1);
                                                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)minID2,path2);
                                                minVid = (path1.size()>path2.size())?minID2:minID1;
                                                //double agl1 = Angle(sadVid,another,minID1);
                                                //double agl2 = Angle(sadVid,another,minID2);
                                                //minVid = (agl1>agl2)?minID1:minID2;
                                        }else{
                                                unSolvedSadID.insert(sadVid);
                                                continue;
                                        }
                                }
                                sadPoint.m_MinPoints[k] = minVid;
                                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)minVid,sadPoint.m_MinPath[k]);
                        }
                }
        }
        
        if(unSolvedSadID.size() != 0) printf("UnSolved Saddle Points: ");
        for(set<size_t>::iterator is = unSolvedSadID.begin(); is!=unSolvedSadID.end(); ++is)
        {
                printf("%d ",*is);
        }
        printf("\n");
        /*
        for(set<size_t>::iterator is = unSolvedSadID.begin(); is!=unSolvedSadID.end(); ++is)
        {
                sadVid = *is;
                pMPoint = (*p_vtx)[sadVid];
                ASSERT(pMPoint->getType() == Global::SADDLEPOINT);
                SadPoint& sadPoint = dynamic_cast<SadPoint&> (*pMPoint);
                
                for(size_t k=0; k<2; ++k)
                {
                        maxVid = sadPoint.m_MaxPoints[k];
                        if((*p_vtx)[maxVid]->getType() != Global::MAXPOINT)
                        {
                                size_t centerP = m_center_vtx_mapping[maxVid];
                                ASSERT((*p_vtx)[centerP]->getType() == Global::MINPOINT);
                                MinPoint* minP = dynamic_cast<MinPoint*>((*p_vtx)[centerP]);
                                vector<size_t>& sadPs = minP->m_SadPoints;
                                set<size_t> maxPs;
                                for(vector<size_t>::iterator it = sadPs.begin(); it!=sadPs.end(); ++it)
                                {
                                        SadPoint* sadP = dynamic_cast<SadPoint*> ((*p_vtx)[*it]);
                                        maxPs.insert(sadP->m_MaxPoints[0]);
                                        maxPs.insert(sadP->m_MaxPoints[1]);
                                }
                                size_t another = sadPoint.m_MaxPoints[1-k];
                                double maxAgl=0.0;
                                size_t bestVid;
                                for(set<size_t>::iterator is=maxPs.begin(); is!=maxPs.end(); ++is)
                                {
                                        double agl = Angle(sadVid, *is, another);
                                        if(agl>maxAgl){
                                                maxAgl = agl;
                                                bestVid = *is;
                                        }
                                }
                                sadPoint.m_MaxPoints[k] = bestVid;
                                Global::mesh->m_BasicOp.GetShortestPath(
                                        (VertexID)sadVid, (VertexID)sadPoint.m_MaxPoints[k],sadPoint.m_MaxPath[k] );
                        }
                        minVid = sadPoint.m_MinPoints[k];
                        if((*p_vtx)[maxVid]->getType() != Global::MINPOINT)
                        {
                                size_t centerP = m_center_vtx_mapping[minVid];
                                ASSERT(((*p_vtx)[centerP])->getType() == Global::MAXPOINT);
                                MaxPoint* maxP = dynamic_cast<MaxPoint*> ((*p_vtx)[centerP]);
                                vector<size_t>& sadPs = maxP->m_SadPoints;
                                set<size_t> minPs;
                                for(vector<size_t>::iterator it = sadPs.begin(); it!=sadPs.end(); ++it)
                                {
                                        SadPoint* sadP = dynamic_cast<SadPoint*>((*p_vtx)[*it]);
                                        minPs.insert(sadP->m_MinPoints[0]);
                                        minPs.insert(sadP->m_MinPoints[1]);
                                }
                                size_t another = sadPoint.m_MinPoints[1-k];
                                double maxAgl = 0.0;
                                size_t bestVid;
                                for(set<size_t>::iterator is=minPs.begin(); is!=minPs.end(); ++is)
                                {
                                        double agl = Angle(sadVid, *is, another);
                                        if(agl>maxAgl){
                                                maxAgl = agl;
                                                bestVid = *is;
                                        }
                                }
                                sadPoint.m_MinPoints[k] = bestVid;
                                Global::mesh->m_BasicOp.GetShortestPath(
                                        (VertexID)sadVid, (VertexID)sadPoint.m_MinPoints[k],sadPoint.m_MinPath[k] );
                        }
                }
        }
        */
}


void NoiseRemoval::ModifySadConnLine(SadPoint& sadPoint)
{       
        // set this saddle point's connection. First, find all max-min
        // point connecting with this cluster, then choose two best
        // max-min-pair
        size_t sadVid, maxVid, minVid;
        sadVid = sadPoint.getId();
        set<size_t> maxVtxes,minVtxes;
        // find all max-point and min-point which connect with this cluster
        size_t clusterID = m_center_cluster_mapping[sadVid];
        NoiseCluster& noiseCluster = m_NoiseCluster[clusterID];
        vector<size_t>& cluster = noiseCluster.noisePoints;
        vector<Global::POINTTYPE>& noiseTypes = noiseCluster.noiseType;

        for(size_t k=0; k<cluster.size(); ++k)
        {
                size_t c_id = cluster[k];
                M_Point* mp = (*p_vtx)[c_id];
                if(noiseTypes[k] == Global::SADDLEPOINT)
                {
                        SadPoint& sP = dynamic_cast<SadPoint&> (*mp);
                        for(size_t i=0;i<sP.m_MaxPoints.size();++i) 
                        {
                                maxVid = sP.m_MaxPoints[i];             mp = (*p_vtx)[maxVid];
                                if(mp->getType() == Global::MAXPOINT) maxVtxes.insert(maxVid);
                                else {
                                        size_t clusterID = m_badvtx_cluster_mapping[maxVid];
                                        if(m_NoiseCluster[clusterID].type == Global::MAXPOINT) 
                                                maxVtxes.insert(m_NoiseCluster[clusterID].centerPoint);
                                }
                        }
                        for(size_t i=0;i<sP.m_MinPoints.size();++i)
                        {
                                minVid = sP.m_MinPoints[i];             mp = (*p_vtx)[minVid];
                                if(mp->getType() == Global::MINPOINT) minVtxes.insert(minVid);
                                else{
                                        size_t clusterID = m_badvtx_cluster_mapping[minVid];
                                        if(m_NoiseCluster[clusterID].type == Global::MINPOINT)
                                                minVtxes.insert(m_NoiseCluster[clusterID].centerPoint);
                                }
                        }
                }
        }
        
        if(maxVtxes.size()<2){
                for(size_t k=0; k<cluster.size(); ++k)
                {
                        M_Point* mp = (*p_vtx)[cluster[k]];
                        if(noiseTypes[k] == Global::MAXPOINT){
                                MaxPoint& maxPoint = dynamic_cast<MaxPoint&> (*mp);
                                vector<size_t>& sadPs = maxPoint.m_SadPoints;
                                for(vector<size_t>::iterator it = sadPs.begin(); it!=sadPs.end(); ++it)
                                {
                                        if(find(cluster.begin(), cluster.end(), *it) != cluster.end() ) continue;
                                        sadVid = *it;
                                        SadPoint& sadPoint = dynamic_cast<SadPoint&>(*(*p_vtx)[sadVid]);
                                        for(size_t i=0;i<2; ++i)
                                        {
                                                maxVid = sadPoint.m_MaxPoints[i]; mp = (*p_vtx)[maxVid];
                                                if(mp->getType() == Global::MAXPOINT) maxVtxes.insert(maxVid);
                                                else{
                                                        size_t clusterID = m_badvtx_cluster_mapping[maxVid];
                                                        if(m_NoiseCluster[clusterID].type == Global::MAXPOINT) 
                                                                maxVtxes.insert(m_NoiseCluster[clusterID].centerPoint);
                                                }
                                        }
                                }
                        }
                }
        }

        if(minVtxes.size()<2){
                for(size_t k=0; k<cluster.size(); ++k)
                {
                        M_Point* mp = (*p_vtx)[cluster[k]];
                        if(noiseTypes[k] == Global::MINPOINT){
                                MinPoint& minPoint = dynamic_cast<MinPoint&> (*mp);
                                vector<size_t>& sadPs = minPoint.m_SadPoints;
                                for(vector<size_t>::iterator it=sadPs.begin(); it!=sadPs.end(); ++it)
                                {
                                        if(find(cluster.begin(), cluster.end(), *it) != cluster.end() ) continue;
                                        sadVid = *it;
                                        SadPoint& sadPoint = dynamic_cast<SadPoint&>(*(*p_vtx)[sadVid]);
                                        for(size_t i=0; i<2; ++i)
                                        {
                                                minVid = sadPoint.m_MinPoints[i]; mp = (*p_vtx)[minVid];
                                                if(mp->getType() == Global::MINPOINT) minVtxes.insert(minVid);
                                                else {
                                                        size_t clusterID = m_badvtx_cluster_mapping[minVid];
                                                        if(m_NoiseCluster[clusterID].type == Global::MINPOINT)
                                                                minVtxes.insert(m_NoiseCluster[clusterID].centerPoint);
                                                }
                                        }
                                }
                        }
                }
        }

        vector<size_t>& bestMax = sadPoint.m_MaxPoints; bestMax.clear(); 
        vector<size_t>& bestMin = sadPoint.m_MinPoints; bestMin.clear();
        

        size_t maxMeasure = SelectMeasureOnNormal(sadVid, maxVtxes);
        size_t minMeasure = SelectMeasureOnNormal(sadVid, minVtxes);

        if(maxMeasure == 0) FindBestMMPairOnLength(sadVid, maxVtxes, bestMax);
        else if(maxMeasure == 1) FindBestMMPairOnAngle(sadVid, maxVtxes, bestMax);
        
        if(minMeasure == 0) FindBestMMPairOnLength(sadVid, minVtxes, bestMin);
        else if(minMeasure == 1) FindBestMMPairOnAngle(sadVid, minVtxes, bestMin);

        sadPoint.m_MaxPath.clear();     sadPoint.m_MaxPath.resize(2);
        Global::mesh->m_BasicOp.GetShortestPath(
                (VertexID)sadVid, (VertexID)sadPoint.m_MaxPoints[0],sadPoint.m_MaxPath[0] );
        Global::mesh->m_BasicOp.GetShortestPath(
                (VertexID)sadVid, (VertexID)sadPoint.m_MaxPoints[1],sadPoint.m_MaxPath[1] );

        sadPoint.m_MinPath.clear();     sadPoint.m_MinPath.resize(2);
        Global::mesh->m_BasicOp.GetShortestPath(
                (VertexID)sadVid, (VertexID)sadPoint.m_MinPoints[0],sadPoint.m_MinPath[0] );
        Global::mesh->m_BasicOp.GetShortestPath(
                (VertexID)sadVid, (VertexID)sadPoint.m_MinPoints[1],sadPoint.m_MinPath[1] );

        for(size_t k=0;k<2;++k)
        {
                M_Point* mp = (*p_vtx)[bestMax[k]];
                MaxPoint& maxPoint = dynamic_cast<MaxPoint&> (*mp);
                for(size_t i=0; i<maxPoint.m_SadPoints.size(); ++i)
                {
                        size_t sid = maxPoint.m_SadPoints[i];
                        if(find(cluster.begin(),cluster.end(),sid)!=cluster.end())
                        {
                                maxPoint.m_SadPoints[i] = sadVid;
                        }       
                }
                mp = (*p_vtx)[bestMin[k]];
                MinPoint& minPoint = dynamic_cast<MinPoint&> (*mp);
                for(size_t i=0; i<minPoint.m_SadPoints.size(); ++i)
                {
                        size_t sid = minPoint.m_SadPoints[i];
                        if(find(cluster.begin(),cluster.end(),sid)!=cluster.end())
                        {
                                minPoint.m_SadPoints[i] = sadVid;
                        }
                }
        }

}

void NoiseRemoval::ModifyDualConnLine()
{
        set<MaxMinPair> mmPSet;
        size_t maxVid, minVid;
        for(map<MaxMinPair, IndexArray>::iterator im = p_DualConn->begin(); im!=p_DualConn->end(); ++im)
        {
                maxVid = (im->first).first;
                minVid = (im->first).second;
                M_Point* maxp = (*p_vtx)[maxVid];
                M_Point* minp = (*p_vtx)[minVid];
                if(maxp->getType()!=Global::MAXPOINT || minp->getType()!=Global::MINPOINT)
                        p_DualConn->erase(im);
        }
        for(set<size_t>::iterator is = p_SadPointID->begin(); is!=p_SadPointID->end(); ++is)
        {
                M_Point* mp = (*p_vtx)[*is];
                SadPoint& sadPoint = dynamic_cast<SadPoint&> (*mp);
                for(size_t k=0; k<2; ++k)
                {
                        maxVid = sadPoint.m_MaxPoints[k];
                        for(size_t i=0; i<2; ++i)
                        {
                                minVid = sadPoint.m_MinPoints[i];
                                MaxMinPair mmP = make_pair(maxVid, minVid);
                                mmPSet.insert(mmP);
                                if(p_DualConn->find(mmP) == p_DualConn->end())
                                {
                                        (*p_DualConn)[mmP] = IndexArray();
                                        IndexArray& path = (*p_DualConn)[mmP] ;
                                        Global::mesh->m_BasicOp.GetShortestPath((VertexID)maxVid, (VertexID)minVid, path);
                                        //FindShortestPath(*p_vtx, maxVid, minVid, path);
                                }
                        }
                }
        }

        for(map<MaxMinPair, IndexArray>::iterator im = p_DualConn->begin(); im!=p_DualConn->end(); ++im)
        {
                const MaxMinPair& mmP = im->first;
                if(mmPSet.find(mmP) == mmPSet.end()) p_DualConn->erase(im);
        }
        
}

size_t NoiseRemoval::FindCenterInCluster(NoiseCluster& cluster, Global::POINTTYPE type)
{
        /* FindCenterInCluster : from a bad vertex cluster, find its center point, whose point-type 
         *      is type and in the center of the cluster
         */
        for(size_t k=0; k<cluster.noisePoints.size(); ++k)
        {
                if(cluster.noiseType[k] == type) return cluster.noisePoints[k];
        }

        return UNCERTAIN;
}

size_t NoiseRemoval::SelectMeasureOnNormal(size_t sadP, set<size_t>& pointSet)
{
        /* SelectMeasure : select a kind of measure on the basis of normal, 
         * if normal difference between saddle and other points is small,
         * return 0(length), else return 1(angle)
         * sadP : the saddle point
         * pointSet : points set
         */
        return 0;
        NormalArray& vNormal = (Global::mesh)->m_Kernel.GetVertexInfo().GetNormal();
        Coord& sadNormal = vNormal[sadP];
        for(set<size_t>::iterator is=pointSet.begin(); is!=pointSet.end(); ++is)
        {
                Coord& pointNormal = vNormal[*is];
                double agl = Angle(sadNormal, pointNormal);
                if(agl>PI/4.0) return 1;
        }
        return 0;
}

void NoiseRemoval::FindBestMMPairOnLength(size_t sadVid, set<size_t>& pointSet, vector<size_t>& ret)
{
        ret.clear(); ret.resize(2);
/*      double minLen1 = INFINITE_DISTANCE-1.0;
        double minLen2 = INFINITE_DISTANCE;
        for(set<size_t>::iterator is=pointSet.begin(); is!=pointSet.end(); ++is)
        {
                double len=((*(Global::p_coord))[*is] - (*(Global::p_coord))[sadP]).abs();
                if(len<minLen1){
                        minLen2 = minLen1;      ret[1] = ret[0];
                        minLen1 = len;          ret[0] = *is;
                }else if(len<minLen2){
                        minLen2 = len;          ret[1] = *is;
                }
        }*/
        size_t minLen1 = INT_MAX, minLen2 = INT_MAX-1;
        for(set<size_t>::iterator is=pointSet.begin(); is!=pointSet.end(); ++is)
        {
                IntArray path;
                Global::mesh->m_BasicOp.GetShortestPath((VertexID)sadVid,(VertexID)(*is),path);
                size_t len = path.size();
                if(len<minLen1){
                        minLen2 = minLen1;      ret[1] = ret[0];
                        minLen1 = len;          ret[0] = *is;
                }else if(len<minLen2){
                        minLen2 = len;          ret[1] = *is;
                }
        }
        
}

void NoiseRemoval::FindBestMMPairOnAngle(size_t sadP, set<size_t>& pointSet, vector<size_t>& ret)
{
        ret.clear(); ret.resize(2);
        double maxAgle = -1.0;
        for(set<size_t>::iterator is1=pointSet.begin(); is1!=pointSet.end(); ++is1)
        {
                for(set<size_t>::iterator is2=is1; is2!=pointSet.end(); ++is2)
                {
                        double agl = Angle(sadP, *is1, *is2);
                        if(agl>maxAgle){
                                maxAgle = agl;
                                ret[0] = *is1; ret[1] = *is2;
                        }
                }
        }
}

void NoiseRemoval::CheckBadConnLine(){}

void NoiseRemoval::CheckDualBadConnLine()
{
        m_EdgeAdjFace.clear();
        CalcEdgeAdjFace();
        SetEdgePathMapping();
        m_FaceVisitFlag.clear(); m_FaceVisitFlag.resize(Global::faceNum, false);
        m_BadPathID.clear();

        for(size_t fID=0; fID<Global::faceNum; ++fID)
        {
                if(m_FaceVisitFlag[fID]) continue;
                FloodFillAFace(fID);
        }
        printf("There are %d bad Charts!\n", m_BadPathID.size());
}

void NoiseRemoval::FloodFillAFace(size_t fID)
{
        /* FloodFillAFace : flood fill all faces in an area from a face
         */

        PolyIndexArray& fIndex = Global::mesh->m_Kernel.GetFaceInfo().GetIndex();

        set<MaxMinPair> pathSet;

        queue <size_t> q;
        q.push(fID); m_FaceVisitFlag[fID] = true;

        size_t vid1, vid2, nxtF;
        while(!q.empty())
        {
                fID = q.front(); q.pop();
                for(size_t k=0; k<3; ++k)
                {
                        vid1 = fIndex[fID][k];
                        vid2 = fIndex[fID][(k+1)%3];
                        pair<size_t,size_t> e= (vid1>vid2)?make_pair(vid2,vid1):make_pair(vid1,vid2);
                        map< pair<size_t,size_t>, vector<MaxMinPair> > ::iterator im = m_EdgePathMapping.find(e);
                        if(im!=m_EdgePathMapping.end()){
                                vector<MaxMinPair>& pathID=im->second;
                                if(pathID.size()==1) pathSet.insert(pathID[0]);
                                else{
                                        
                                }
                        }else{
                                vector<size_t>& adjFaces=m_EdgeAdjFace[e];
                                ASSERT(adjFaces.size()==2);
                                nxtF = (fID==adjFaces[0])?adjFaces[1]:adjFaces[0];
                                if(m_FaceVisitFlag[nxtF]==false){
                                        q.push(nxtF);
                                        m_FaceVisitFlag[nxtF] = true;
                                }
                        }
                }

        }
        if(pathSet.size()!=4)
        {
                m_BadPathID.push_back(pathSet);
        }
}

void NoiseRemoval::SetEdgePathMapping()
{
        /* SetEdgePathMapping : for each edge in path, set its corresponding path id
         * there may be an edge corresponding more than one paths
         */
        m_EdgePathMapping.clear();
        pair<size_t,size_t> e;
        size_t vid1, vid2;
        for(map<MaxMinPair, IndexArray>::iterator im = p_DualConn->begin(); im!=p_DualConn->end(); ++im)
        {
                IntArray& path = im->second;
                for(size_t k=1; k<path.size(); ++k)
                {
                        vid1 = (size_t)path[k-1];
                        vid2 = (size_t)path[k];
                        if(vid1>vid2) e=make_pair(vid2,vid1);
                        else e=make_pair(vid1,vid2);
                        m_EdgePathMapping[e].push_back(im->first);
                }
        }
}

void NoiseRemoval::CalcEdgeAdjFace()
{
        PolyIndexArray& fIndex = Global::mesh->m_Kernel.GetFaceInfo().GetIndex();
        size_t vid1, vid2;
        pair<size_t,size_t> edgePair;
        for(size_t fID=0; fID<Global::faceNum; ++fID)
        {
                for(size_t k=0;k<3; ++k)
                {
                        vid1 = fIndex[fID][k]; 
                        vid2 = fIndex[fID][(k+1)%3];
                        
                        if(vid1>vid2) edgePair = make_pair(vid2,vid1);
                        else edgePair = make_pair(vid1,vid2);

                        m_EdgeAdjFace[edgePair].push_back(fID);
                }
        }
}

void NoiseRemoval::TestConnection()
{
        for(set<size_t>::iterator is=p_SadPointID->begin(); is!=p_SadPointID->end(); ++is)
        {
                M_Point* mp = (*p_vtx)[*is];
                if(mp->getType() != Global::SADDLEPOINT){
                        printf("Error Connection! -- %d is not Saddle Point!\n", *is);
                }
                SadPoint& sadPoint = dynamic_cast<SadPoint&>(*mp);
                for(size_t k=0;k<2;++k)
                {
                        size_t maxVid = sadPoint.m_MaxPoints[k];
                        M_Point* pMax = (*p_vtx)[maxVid];
                        if(pMax->getType() != Global::MAXPOINT) {
                                printf("Error Connection! -- %d is not Max Point\n", maxVid);
                        }
                        size_t minVid = sadPoint.m_MinPoints[k];
                        M_Point* pMin = (*p_vtx)[minVid];
                        if(pMin->getType() != Global::MINPOINT){
                                printf("Error Connection! -- %d is not Min Point!\n", minVid);
                        }
                }
        }
}
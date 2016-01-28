#ifndef NOISEREMOVAL_H_
#define NOISEREMOVAL_H_

#include "..\..\MeshUI\Common\watchtime.h"
#include "..\Global\Global.h"
#include "..\Global\CommonData.h"
#include "..\DisjointSetUnion.h"
#include "M_Point.h"
#include <cmath>

struct Persistence{
        size_t sad, maxmin;
        double pValue;
};


struct NoiseCluster
{
        size_t centerPoint;
        vector<size_t> noisePoints;     // the points set in this cluster
        vector<Global::POINTTYPE> noiseType; // each point's type
        Global::POINTTYPE type;

        NoiseCluster():centerPoint(UNCERTAIN), type(Global::BADPOINT){}

};

class NoiseRemoval
{
        typedef std::pair<int,int> VtxPair;
        typedef std::pair<double,double> DoublePair;

public:
        NoiseRemoval();
        NoiseRemoval(CommonData& cd);
        ~NoiseRemoval();

        void CalPersistence();
        void SetThreshold(double lowBound, double upBound) 
        {
                m_threshold.first = lowBound; m_threshold.second = upBound;
        }

        DoublePair GetThreshold() const { return m_threshold;}
        vector<set<MaxMinPair>>* GetBadPaths(){ return &m_BadPathID;}

        void RemoveNoise();

private:

        void RemoveBadCriticalPoints();

        // method for modifying connection 
        void ModifyConnLine();
        void ModifySadConnLine();
        void ModifySadConnLine(SadPoint& sadPoint);
        void ModifyDualConnLine();
        size_t FindCenterInCluster(NoiseCluster& cluster, Global::POINTTYPE type);

        size_t SelectMeasureOnNormal(size_t sadP, set<size_t>& pointSet);
        void FindBestMMPairOnLength(size_t sadP, set<size_t>& pointSet, vector<size_t>& ret);
        void FindBestMMPairOnAngle (size_t sadP, set<size_t>& pointSet, vector<size_t>& ret);

        // method for finding bad connection
        void SetEdgePathMapping();
        void CalcEdgeAdjFace();
        void CheckBadConnLine();
        void CheckDualBadConnLine();
        void FloodFillAFace(size_t fID);
        

        // some help method
        void FindAdjFaces(size_t vid1,size_t vid2, size_t& fid1, size_t& fid2);
        inline double Angle(size_t p0,size_t p1, size_t p2 );
        inline double Angle(Coord& v1, Coord& v2);
        void TestConnection();

private:
        // common data
        vector <M_Point*>* p_vtx;
        set <size_t>* p_SadPointID;
        set <size_t>* p_MaxPointID;
        set <size_t>* p_MinPointID;
        vector <double>* p_EigenValue;
        map<MaxMinPair, IndexArray>* p_DualConn;

        map < pair<size_t,size_t>, double> m_persistence;
        DoublePair m_threshold;

        DisjointSet disjoinSet;
        set <size_t> m_badPoints;
        vector <NoiseCluster> m_NoiseCluster;
        map<size_t,size_t> m_center_cluster_mapping;
        map<size_t,size_t> m_badvtx_cluster_mapping;
        

        map <size_t, vector <size_t>> m_badPointCluster;
        map <size_t, size_t> m_bad_vtx_mapping;
        map <size_t, size_t> m_center_vtx_mapping;
        vector <size_t> m_newAddSad;

        vector<bool> m_FaceVisitFlag;
        map<pair<size_t,size_t>, vector<MaxMinPair> > m_EdgePathMapping;//the mapping between edge and its path
        vector< set<MaxMinPair> > m_BadPathID;
        map<pair<size_t,size_t>, vector<size_t> > m_EdgeAdjFace;// the adj-faces of one edge;

        SystemWatchTime m_time;
};

inline double NoiseRemoval::Angle(size_t p0,size_t p1, size_t p2 )
{
        // calculate the angle between (p0,p1) and (p0,p2)
        return Angle((*Global::p_coord)[p1]-(*Global::p_coord)[p0], (*Global::p_coord)[p2]-(*Global::p_coord)[p0]);
}

inline double NoiseRemoval::Angle(Coord& v1, Coord& v2)
{
        if(v1 == v2) return 0.0;
        return acos(dot(v1,v2)/(v1.abs()*v2.abs()));
}
#endif
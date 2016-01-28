#ifndef CRITICALPOINT_H_
#define CRITICALPOINT_H_


#include "..\Global\Global.h"
#include "..\Global\CommonData.h"
#include "M_Point.h"

#include <map>

class CriticalPoint
{
public:
        CriticalPoint(vector<M_Point*>& vtx);
        CriticalPoint(CommonData& cd);
        ~CriticalPoint();

        bool LoadEigenField(string filename);
        void FindCriticalPoints();
        void TraceLine();

        void clear();           

private:
        Global::POINTTYPE CheckVertexType(vector <bool>& adjFlag);

        // methods for find the corresponding max-min points of a saddle point
        void TraceLineFromSaddle(SadPoint& sadP);
        bool FindMaxMinFromSadNeigh(SadPoint& sadP);
        size_t FindMaxPInRange(size_t vid, const pair<size_t,size_t>& range);
        size_t FindMinPInRange(size_t vid, const pair<size_t,size_t>& range);
        void TraceMaxLineFromSaddle(SadPoint& sadP,size_t& maxP, IntArray& pathArray);
        void TraceMinLineFromSaddle(SadPoint& sadP,size_t& minP, IntArray& pathArray);

        size_t FindMaxEigenValueFromVertexNeigh(size_t vid);
        size_t FindMinEigenValueFromVertexNeigh(size_t vid);

        void FindMaxMinMapping2Sad();

        void TraceLine4MaxMin(SadPoint& sadP);
        void VtxValue2VtxColor(vector<double>& vertexValue);

private:

        vector < M_Point*>& m_vtx;
        set <size_t>* p_SadPointID;
        set <size_t>* p_MaxPointID;
        set <size_t>* p_MinPointID;

        map<MaxMinPair, IndexArray>* p_DualConn;

        vector < double >* p_EigenValue;

        

};

#endif
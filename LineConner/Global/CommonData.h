#ifndef COMMONDATA_H_
#define COMMONDATA_H_

#include "Global.h"
#include "M_Point.h"
#include <set>

struct CommonData
{
        vector <M_Point* > m_vtx;
        set <size_t> m_SadPointID;
        set <size_t> m_MaxPointID;
        set <size_t> m_MinPointID;

        map <MaxMinPair, IndexArray> m_DualConn;
        
        vector <double> m_EigenValue;
        
        void clear();
        
};

void FindShortestPath(vector<M_Point*>& m_vtx, size_t u, size_t v, IntArray& path);
#endif 
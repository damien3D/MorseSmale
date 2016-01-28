#ifndef _M_DISJOINTSETUNION_H_
#define _M_DISJOINTSETUNION_H_
#include <algorithm>

class DisjointSet
{
        int m_size;
        vector <int> m_set;
public:

        void reSize(size_t size)
        {
                m_size=(int)size;
                m_set.resize(size);
        }

        void Init(int v)
        {
                std::fill(m_set.begin(),m_set.end(),v);
        }
        
        int Find(int x)
        {
                ASSERT(x>=0&&x<m_size);
                if(m_set[x]<=0) return x;
                return m_set[x] = Find(m_set[x]);
        }

        void Union(int r1,int r2)
        {
                ASSERT(r1>=0&&r1<m_size);
                ASSERT(r2>=0&&r2<m_size);
                r1=Find(r1); r2=Find(r2);
                if(r1!=r2) m_set[r2]=r1;
        }

        ~DisjointSet(){};

};

#endif
#ifndef M_POINT_H_
#define M_POINT_H_

#include "Global.h"

class M_Point
{

public:
        size_t m_vid;
        double m_eigenvalue;
        Global::POINTTYPE m_type;
        bool m_isPathVtx;

public:

        M_Point() : m_vid(0),m_type(Global::REGULARPOINT), m_isPathVtx(false){}
        M_Point(size_t vid,Global::POINTTYPE type=(Global::REGULARPOINT), bool isPathVtx=false): 
        m_vid(vid),m_type(type), m_isPathVtx(isPathVtx) {}

        bool operator<(const M_Point& p) const { return m_vid<p.m_vid;}

        void setId(size_t id) { m_vid = id;}
        size_t getId() { return m_vid;}
        void setType(Global::POINTTYPE type){ m_type = type;}
        virtual Global::POINTTYPE getType() const { return m_type;}
        Coord& getCoord() const { return (*(Global::p_coord))[m_vid]; }

};

class SadPoint : public M_Point
{
public:
        typedef vector<size_t> Path;

public:
        vector<size_t> m_MaxPoints;
        vector<size_t> m_MinPoints;
        vector< IntArray > m_MaxPath;
        vector< IntArray > m_MinPath;

public:

        SadPoint();
        ~SadPoint();
        SadPoint(size_t id):M_Point(id,Global::SADDLEPOINT){}
        SadPoint(const SadPoint& sad);
        
        int getType() { return m_type = Global::SADDLEPOINT;}
        void clear() {
                m_MaxPoints.clear(); 
                m_MinPoints.clear();
                m_MaxPath.clear();
                m_MinPath.clear();
        }
};

class MinPoint : public M_Point
{
public:
        vector<size_t> m_SadPoints; // the corresponding saddle points 

public:
        MinPoint();
        ~MinPoint();
        MinPoint(size_t id):M_Point(id,Global::MINPOINT){}
        MinPoint(const MinPoint& minP);

        int getType() { return m_type = Global::MINPOINT;}
};

class MaxPoint : public M_Point
{
public:
        vector<size_t> m_SadPoints; // the corresponding saddle points

public:
        MaxPoint();
        ~MaxPoint();
        MaxPoint(size_t id):M_Point(id,Global::MAXPOINT){}
        MaxPoint(const MaxPoint& maxP);

        int getType() { return m_type = Global::MAXPOINT;}
};

class BadPoint : public M_Point
{
public:
        int getType() { return m_type = Global::BADPOINT;}
};


#endif
#include "stdafx.h"
#include "M_Point.h"


SadPoint::SadPoint(const SadPoint& sad)
{
        m_vid = sad.m_vid;
        m_MaxPoints.assign(sad.m_MaxPoints.begin(), sad.m_MaxPoints.end());
        m_MinPoints.assign(sad.m_MinPoints.begin(), sad.m_MinPoints.end());
        m_MaxPath.assign(sad.m_MaxPath.begin(), sad.m_MaxPath.end());
        m_MinPath.assign(sad.m_MinPath.begin(), sad.m_MinPath.end());
}
SadPoint::~SadPoint() 
{ 
        m_MaxPoints.clear();
        m_MinPoints.clear();
        m_MaxPath.clear();
        m_MinPath.clear();
}

MaxPoint::MaxPoint(const MaxPoint& maxP)
{
        m_vid = maxP.m_vid;
        m_SadPoints.assign(maxP.m_SadPoints.begin(), maxP.m_SadPoints.end());
}
MaxPoint::~MaxPoint()
{
        m_SadPoints.clear();
}

MinPoint::MinPoint(const MinPoint& minP)
{
        m_vid = minP.m_vid;
        m_SadPoints.assign(minP.m_SadPoints.begin(), minP.m_SadPoints.end());
}
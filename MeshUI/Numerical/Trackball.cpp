/*_____________________________________________________
 |
 |                                          
 |  ÎÄ¼þ£ºTrackball.cpp
 |
*/

#include "stdafx.h"             
#include <math.h>               
#include <assert.h>
#include "Trackball.h"


Trackball::Trackball(void)
{
        m_iWidth=m_iHeight=400; 
        m_iCenterX=m_iCenterY=200;
        m_iRadius=200;
        m_iMouseX=m_iMouseY=1;

        // ÁãÊ¸Á¿
        m_start[0]=0;
        m_start[1]=0;
        m_start[2]=0;
}

Trackball::~Trackball(void)
{

}

// ¸ú×ÙÇòµÄÖÐÐÄÎª´°¿ÚµÄÖÐÐÄ
void Trackball::SetTrackWindow(int iWidth, int iHeight)
{
        m_iWidth   = iWidth;
        m_iHeight  = iHeight;
        m_iCenterX = m_iWidth/2;
        m_iCenterY = m_iHeight/2;
        m_iRadius  = (m_iWidth > m_iHeight) ? m_iCenterY : m_iCenterX ;
}

// ¸ú×ÙÇòµÄÖÐÐÄÎª´°¿Ú×ø±ê(centerX, centerY) 
void Trackball::SetTrackWindow(int iWidth, int iHeight, int iCenterX, int iCenterY)
{
        m_iWidth   = iWidth;
        m_iHeight  = iHeight;
        m_iCenterX = iCenterX;
        m_iCenterY = iCenterY;
        m_iRadius  = (m_iWidth > m_iHeight) ? m_iCenterY : m_iCenterX ;
}

// ½«Êó±ê(mouseX, mouseY)Í¶Ó°µ½ÇòÃæÒ»µã P
// º¯ÊýÖµ¼´ÎªÇòÐÄµ½PµÄÊ¸Á¿
Coord Trackball::MouseToCoord(int iMouseX, int iMouseY)
{
        Coord V(0,0,0);

        if(m_iCenterX >= m_iWidth/2)  
                V[0]=float(iMouseX-m_iCenterX)/float(m_iCenterX);
        else
                V[0]=float(iMouseX-m_iCenterX)/float(m_iWidth-m_iCenterX);

        if(m_iCenterY >= m_iHeight/2)  
                V[1]=float(m_iCenterY-iMouseY)/float(m_iCenterY);
        else
                V[1]=float(m_iCenterY-iMouseY)/float(m_iHeight-m_iCenterY);

        float d=float(sqrt(V[0] * V[0] + V[1] * V[1]));
        V[2]=float( cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0)) );
        V.normalize();
        return V;
}

// ÊäÈë¸Õ°´ÏÂÊó±êÊ±µÄ×ø±ê (mouseX, mouseY)
void Trackball::Start(int iMouseX, int iMouseY)
{
        m_iMouseX=iMouseX;
        m_iMouseY=iMouseY;
        m_start=MouseToCoord(m_iMouseX, m_iMouseY);
}

// Êó±êÒÆ¶¯ (dx,dy), ¼ÆËãÐý×ªÖá axis Óë½Ç¶È angle 
void Trackball::Tracking(int iDx, int iDy, Coord *axis, float *fAngle)
{
        m_iMouseX+=iDx;
        m_iMouseY+=iDy;
        m_stop=MouseToCoord(m_iMouseX, m_iMouseY);
        *axis=cross(m_start, m_stop); // cross product

        const float ANGLE_SCALE=2.0f;   // ÈÎÒâ¸ø¶¨Ò»¸ö½Ç¶È·Å´óÏµÊý
        *fAngle=float(ANGLE_SCALE*acos(dot(m_start, m_stop))/3.14159*180.0);
        
        // ¿ØÖÆÊä³öµÄ×îÐ¡½Ç¶È£¬·ñÔò Rotation ½«³ö´íÎó
        const float     DELTA_ANGLE=0.00001f;
        if(*fAngle < DELTA_ANGLE)       *fAngle=DELTA_ANGLE;

        m_start=m_stop; // ²»ÒªÍü¼Ç
}

/*-------------------------------------------------------
        ¸´ÔÓµÄ¸ú×ÙÇòËã·¨¿ÉÒÔ²Î¿¼ 
        ²ÌÎÄÁ¢£¬¡¶¿ÆÑ§¼ÆËã¿ÉÊÓ»¯Ëã·¨ÓëÏµÍ³¡· P232 - P233
  -------------------------------------------------------*/
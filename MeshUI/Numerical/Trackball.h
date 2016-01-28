/*_____________________________________________________
 |
 |  ÎÄ¼þ£ºTrackball.h
 |
 |_____________________________________________________*/




#if !defined(TRACKBALL_H)
#define          TRACKBALL_H

#include "..\MeshLib\BasicElement.h"

class Trackball
{
public:
        Trackball(void);
   ~Trackball(void);

public:
        // ¸ú×ÙÇòµÄÖÐÐÄÎª´°¿ÚµÄÖÐÐÄ
        void SetTrackWindow(int iWidth, int iHeight);

        // ¸ú×ÙÇòµÄÖÐÐÄÎª´°¿Ú×ø±ê(centerX, centerY) 
        void SetTrackWindow(int iWidth, int iHeight, int iCenterX, int iCenterY);

        // ÊäÈë¸Õ°´ÏÂÊó±êÊ±µÄ×ø±ê (mouseX, mouseY)
        void Start(int iMouseX, int iMouseY);

        // Êó±êÒÆ¶¯ (dx,dy), ¼ÆËãÐý×ªÖá axis Óë½Ç¶È angle 
        void Tracking(int iDx, int iDy, Coord *axis, float *fAngle);

protected:

        // ½«Êó±ê(mouseX, mouseY)Í¶Ó°µ½ÇòÃæÒ»µã P
        // º¯ÊýÖµ¼´ÎªÇòÐÄµ½PµÄÊ¸Á¿
        Coord MouseToCoord(int iMouseX, int iMouseY);

        int    m_iWidth,   m_iHeight;   // ´°¿ÚµÄ¿í¸ß
        int    m_iCenterX, m_iCenterY;  // ¸ú×ÙÇòÖÐÐÄ
        int    m_iRadius;                               // ¸ú×ÙÇò°ë¾¶
        int    m_iMouseX,  m_iMouseY;   // Êó±êÎ»ÖÃ

        Coord m_start;  // ¸Õ°´ÏÂÊ±µÄÊó±êÍ¶Ó°Ê¸Á¿
        Coord m_stop;   // ÒÆ¶¯(dx,dy)Ê±µÄÊó±êÍ¶Ó°Ê¸Á¿
};

#endif // TRACKBALL_H
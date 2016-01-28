#ifndef MESHDRAWER_H_
#define MESHDRAWER_H_

#include "..\Global\Global.h"
#include "..\Global\CommonData.h"
#include "M_Point.h"

struct Global;

class MeshDrawer
{

private:
        
        Global::DrawMode m_mode;

        vector <M_Point*>* p_vtx;
        set <size_t>* p_SadPointID;
        set <size_t>* p_MaxPointID;
        set <size_t>* p_MinPointID;
        map<MaxMinPair, IndexArray>* p_DualConn;

        vector <set<MaxMinPair>>* p_BadPaths;

public:
        MeshDrawer();
        MeshDrawer(CommonData& cd,Global::DrawMode mode = 
                Global::DrawMode(Global::DRAWMAXMINPOINTS|Global::DRAWSADDLEPOINTS));
        ~MeshDrawer();

        void DrawMesh() const;

        void setMode(Global::DrawMode mode){ m_mode = Global::DrawMode(m_mode^mode);}
        void setBadPaths(vector<set<MaxMinPair> >* badPaths) { p_BadPaths = badPaths;}


        void clear();

private:

        void DrawMaxMinPoints() const;
        void DrawSaddlePoints() const;
        void DrawConnectLines() const;
        void DrawBadConnLines() const;

        void DrawSMMConnLines() const;
        void DrawMMConnLines() const;

        void DrawStraighten() const;
        void DrawSMMStraighten() const;
        void DrawMMStraighten() const;
        

        void DrawSphere(Coord& center) const;   
};

#endif
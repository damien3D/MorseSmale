#include "stdafx.h"
#include "MeshDrawer.h"

MeshDrawer::MeshDrawer():
p_vtx(NULL),p_SadPointID(NULL),p_MaxPointID(NULL),p_MinPointID(NULL),p_DualConn(NULL)
{}

MeshDrawer::MeshDrawer(CommonData& cd ,Global::DrawMode mode):
p_vtx(&cd.m_vtx),
p_SadPointID(&cd.m_SadPointID),
p_MaxPointID(&cd.m_MaxPointID),
p_MinPointID(&cd.m_MinPointID),
p_DualConn(&cd.m_DualConn),
m_mode(mode)
{
        p_BadPaths=NULL;
}

MeshDrawer::~MeshDrawer()
{
        p_vtx = NULL;
        p_SadPointID = NULL;
        p_MaxPointID = NULL;
        p_MinPointID = NULL;
        p_DualConn = NULL;
}


void MeshDrawer::DrawMesh() const
{
        if(p_vtx == NULL) return;
        // Get previous material diffuse components
        float ambient[4], diffuse[4], specular[4], emission[4], shininess;
        glGetMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glGetMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glGetMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glGetMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glGetMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

        //glDisable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);

        glEnable(GL_POLYGON_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0, 2.0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

        if(m_mode & Global::DRAWMAXMINPOINTS) DrawMaxMinPoints();
        if(m_mode & Global::DRAWSADDLEPOINTS) DrawSaddlePoints();
        
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_SMOOTH);
        glEnable(GL_LIGHTING);

        // Reset previous material diffuse components
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

        glDisable(GL_LIGHTING);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glLineWidth(2.0f);

        if(m_mode & Global::DRAWCONNECTLINES) DrawConnectLines();
        if(m_mode & Global::DRAWBADCONNLINES) DrawBadConnLines();
        if(m_mode & Global::DRAWSTRAIGHTENLINES) DrawStraighten();

        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glDisable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glEnable(GL_LIGHTING);
        glDepthFunc(GL_LESS);
        glEnable(GL_LIGHTING);


}


void MeshDrawer::DrawMaxMinPoints() const
{
        // Draw Max-Points
        glColor3f(1.0f,0.0f,0.0f);
        for(set<size_t>::iterator it=p_MaxPointID->begin(); it!=p_MaxPointID->end(); ++it)
        {
                DrawSphere((*p_vtx)[*it]->getCoord());
        }
        // Draw Min-Points
        glColor3f(0.0f,0.0f,1.0f);
        for(set<size_t>::iterator it=p_MinPointID->begin();it!=p_MinPointID->end(); ++it)
        {
                DrawSphere((*p_vtx)[*it]->getCoord());
        }
}

void MeshDrawer::DrawSaddlePoints() const
{
        // Draw Saddle-Points
        glColor3f(0.0f,1.0f,0.0f);
        for(set<size_t>::iterator it=p_SadPointID->begin(); it!=p_SadPointID->end(); ++it)
        {
                DrawSphere((*p_vtx)[*it]->getCoord());
        }
}

void MeshDrawer::DrawConnectLines() const
{
        if(m_mode & Global::DRAWSADDLEPOINTS)
        {
                DrawSMMConnLines();
        }else if(m_mode & Global::DRAWMAXMINPOINTS){
                DrawMMConnLines();
        }
}


void MeshDrawer::DrawSMMConnLines() const
{
        Color c = RED*0.970;
        glColor3d(c[0], c[1], c[2]);


        Coord sadCoord, maxCoord,minCoord;

        for(set<size_t>::iterator it=p_SadPointID->begin(); it!=p_SadPointID->end(); ++it)
        {
                SadPoint& sadP = dynamic_cast<SadPoint&>( *(*p_vtx)[*it]);
                sadCoord = (*(Global::p_coord))[*it];

                glColor3f(0.0,1.0,1.0);
                for(size_t k=0;k<sadP.m_MaxPath.size(); ++k)
                {
                        glBegin(GL_LINE_STRIP);
                        for(size_t j=0;j<sadP.m_MaxPath[k].size(); ++j)
                        {
                                maxCoord = (*(Global::p_coord))[sadP.m_MaxPath[k][j]];
                                glVertex3d(maxCoord[0],maxCoord[1],maxCoord[2]);
                        }
                        glEnd();
                }
        
                glColor3f(1.0,0.0,1.0);
                
                for(size_t k=0;k<sadP.m_MinPoints.size(); ++k)
                {
                        glBegin(GL_LINE_STRIP);
                        for(size_t j=0;j<sadP.m_MinPath[k].size(); ++j)
                        {
                                minCoord = (*(Global::p_coord))[sadP.m_MinPath[k][j]];
                                glVertex3d(minCoord[0],minCoord[1],minCoord[2]);
                        }
                        glEnd();
                }
        }

}

void MeshDrawer::DrawStraighten() const
{
        if(m_mode & Global::DRAWSADDLEPOINTS)
        {
                DrawSMMStraighten();
        }else if(m_mode & Global::DRAWMAXMINPOINTS){
                DrawMMStraighten();
        }
}

void MeshDrawer::DrawMMConnLines() const
{
        glColor3d(0.2, 0.8, 1.0);

        for(map<MaxMinPair,IndexArray>::iterator im=p_DualConn->begin(); im!=p_DualConn->end(); ++im)
        {
                IndexArray& path = im->second;
                glBegin(GL_LINE_STRIP);
                for(size_t k=0; k<path.size(); ++k)
                {
                        Coord& vtxCoord=(*Global::p_coord)[path[k]];
                        glVertex3d(vtxCoord[0],vtxCoord[1],vtxCoord[2]);                        
                }
                glEnd();
        }

}

void MeshDrawer::DrawSMMStraighten() const
{
        Coord sadCoord, maxCoord,minCoord;

        glBegin(GL_LINES);
        for(set<size_t>::iterator it=p_SadPointID->begin(); it!=p_SadPointID->end(); ++it)
        {
                SadPoint& sadP = dynamic_cast<SadPoint&>( *(*p_vtx)[*it]);
                sadCoord = (*(Global::p_coord))[*it];
                glColor3f(0.0,1.0,1.0);
                for(size_t k=0;k<sadP.m_MaxPoints.size(); ++k)
                {
                        maxCoord = (*(Global::p_coord))[sadP.m_MaxPoints[k]];
                        glVertex3d(sadCoord[0],sadCoord[1],sadCoord[2]); 
                        glVertex3d(maxCoord[0],maxCoord[1],maxCoord[2]);
                }
                glColor3f(1.0,1.0,0.0);
                for(size_t k=0;k<sadP.m_MinPoints.size(); ++k)
                {
                        minCoord = (*(Global::p_coord))[sadP.m_MinPoints[k]];
                        glVertex3d(sadCoord[0],sadCoord[1],sadCoord[2]); 
                        glVertex3d(minCoord[0],minCoord[1],minCoord[2]);
                }

        }
        glEnd();
}

void MeshDrawer::DrawMMStraighten() const
{
        size_t maxVid, minVid;
        Coord maxCoord, minCoord;
        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_LINES);
        for(map<MaxMinPair,IndexArray>::iterator im=p_DualConn->begin(); im!=p_DualConn->end(); ++im)
        {
                maxVid = (im->first).first;             maxCoord = (*(Global::p_coord))[maxVid];
                minVid = (im->first).second;    minCoord = (*(Global::p_coord))[minVid];
                glVertex3d(maxCoord[0],maxCoord[1],maxCoord[2]);
                glVertex3d(minCoord[0],minCoord[1],minCoord[2]);
        }
        glEnd();
}



void MeshDrawer::DrawBadConnLines() const
{
        if(p_BadPaths==NULL) return;

        Color c = GREEN*0.470;
        glColor3d(c[0], c[1], c[2]);

        for(vector<set<MaxMinPair>>::iterator it=p_BadPaths->begin(); it!=p_BadPaths->end(); ++it)
        {
                set<MaxMinPair>& pathID = *it;
                for(set<MaxMinPair>::iterator is=pathID.begin(); is!=pathID.end(); ++is)
                {
                        IntArray& path = (*p_DualConn)[*is];
                        glBegin(GL_LINE_STRIP);
                        for(size_t k=0; k<path.size(); ++k)
                        {
                                Coord& vtxCoord=(*Global::p_coord)[path[k]];
                                glVertex3d(vtxCoord[0],vtxCoord[1],vtxCoord[2]);                        
                        }
                        glEnd();
                }
        }

}

void MeshDrawer::DrawSphere(Coord& center) const
{
        glPushMatrix();
        glTranslatef(center[0], center[1], center[2]);
        glutSolidSphere(0.005 * (Global::bRadius),10, 10);
        glPopMatrix();
}


void MeshDrawer::clear()
{
        p_vtx                   = NULL;
        p_SadPointID    = NULL;
        p_MaxPointID    = NULL;
        p_MinPointID    = NULL;
}
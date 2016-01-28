#include "StdAfx.h"
#include "MeshVectorUI.h"

//////////////////////////////////////////////////////////////////////
// CMeshVectorUI Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMeshVectorUI::CMeshVectorUI()
{
        this->m_CircleNum = 20;
}
CMeshVectorUI::CMeshVectorUI(const CMeshVectorUI& vec)
{
        this->m_CircleNum = vec.m_CircleNum;
        this->m_Start = vec.m_Start;
        this->m_End = vec.m_End;
        this->m_RealEnd = vec.m_RealEnd;
        this->m_Normal = vec.m_Normal;
        this->m_Radius = vec.m_Radius;
        this->m_DefaultMaterial = vec.m_DefaultMaterial;


        //
        this->m_ColumnS.assign(vec.m_ColumnS.begin(), vec.m_ColumnS.end());
        this->m_ColumnE.assign(vec.m_ColumnE.begin(), vec.m_ColumnE.end());
        this->m_TaperE.assign(vec.m_TaperE.begin(), vec.m_TaperE.end());

        this->m_ColumnNormal.assign(vec.m_ColumnNormal.begin(), vec.m_ColumnNormal.end());
        this->m_TaperNormal.assign(vec.m_TaperNormal.begin(), vec.m_TaperNormal.end());
}
CMeshVectorUI::~CMeshVectorUI()
{
        ClearData();
}

//////////////////////////////////////////////////////////////////////
// CMeshVectorUI public methods
//////////////////////////////////////////////////////////////////////

void CMeshVectorUI::SetVectorUIInfo(Coord& start, Coord& end, Coord& normal, double radius)
{
        this->m_Start = start;
    this->m_End = end;
        this->m_Normal = normal;
        this->m_Radius = radius;

        Coord dir = end - start;
        double length = dir.abs();
        dir.normalize();

        this->m_RealEnd = dir * (length - 2 * radius) + m_Start;

        //
        CalCoordinate4Column();
        CalCoordinate4Taper();
}
void CMeshVectorUI::Draw()
{
        SetDefaultMaterial();

        DrawColumn();
        DrawTaper();
}

//////////////////////////////////////////////////////////////////////
// CMeshVectorUI private methods
//////////////////////////////////////////////////////////////////////
void CMeshVectorUI::ClearData()
{
        util.FreeVector(m_ColumnS);
        util.FreeVector(m_ColumnE);
        util.FreeVector(m_TaperE);
        util.FreeVector(m_ColumnNormal);
        util.FreeVector(m_TaperNormal);
}
void CMeshVectorUI::SetDefaultMaterial()
{
        m_DefaultMaterial.ambient[0] = 0.2f;
        m_DefaultMaterial.ambient[1] = 0.2f;
        m_DefaultMaterial.ambient[2] = 0.2f;
        m_DefaultMaterial.ambient[3] = 1.0f;
        
        m_DefaultMaterial.diffuse[0] = 0.8125f;
        m_DefaultMaterial.diffuse[1] = 0.4687f;
        m_DefaultMaterial.diffuse[2] = 0.09f;
        m_DefaultMaterial.diffuse[3] = 1.0f;
        
        m_DefaultMaterial.specular[0] = 0.05f;
        m_DefaultMaterial.specular[1] = 0.0f;
        m_DefaultMaterial.specular[2] = 0.0f;
        m_DefaultMaterial.specular[3] = 1.0f;
        
        m_DefaultMaterial.emission[0] = 0.0f;
        m_DefaultMaterial.emission[1] = 0.0f;
        m_DefaultMaterial.emission[2] = 0.0f;
        m_DefaultMaterial.emission[3] = 0.0f;
        
        m_DefaultMaterial.shininess = 10.0;
        
        m_DefaultMaterial.SetMaterial();
}

void CMeshVectorUI::DrawColumn()
{
        glEnable(GL_POLYGON_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0, 2.0);
        
        int i, j;
        glBegin(GL_QUADS);
        
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord& normal = m_ColumnNormal[i];

                j = (i  + 1) % m_CircleNum;
                
                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnE[j][0], m_ColumnE[j][1], m_ColumnE[j][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnE[i][0], m_ColumnE[i][1], m_ColumnE[i][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnS[i][0], m_ColumnS[i][1], m_ColumnS[i][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnS[j][0], m_ColumnS[j][1], m_ColumnS[j][2]);
        }

        glEnd();


        glBegin(GL_TRIANGLES);
        
        int index = m_CircleNum;
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord normal = m_ColumnNormal[i + index];
                
                j = (i  + 1) % m_CircleNum;
                
                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_Start[0], m_Start[1], m_Start[2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnS[i][0], m_ColumnS[i][1], m_ColumnS[i][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnS[j][0], m_ColumnS[j][1], m_ColumnS[j][2]);
        }
        
        glEnd();

        glBegin(GL_TRIANGLES);
        
        index = m_CircleNum * 2;
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord normal = m_ColumnNormal[i + index];
                
                j = (i  + 1) % m_CircleNum;
                
                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnE[i][0], m_ColumnE[i][1], m_ColumnE[i][2]);
                
                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_ColumnE[j][0], m_ColumnE[j][1], m_ColumnE[j][2]);
                
                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_RealEnd[0], m_RealEnd[1], m_RealEnd[2]);
        }
        

        glEnd();

        glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_SMOOTH);
}
void CMeshVectorUI::DrawTaper()
{
        glEnable(GL_POLYGON_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0, 2.0);
        
        int i, j;

        glBegin(GL_TRIANGLES);
        
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord& normal = m_TaperNormal[i];
                
                j = (i + 1) % m_CircleNum;

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_End[0], m_End[1], m_End[2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_TaperE[j][0], m_TaperE[j][1], m_TaperE[j][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_TaperE[i][0], m_TaperE[i][1], m_TaperE[i][2]);
        }
        
        glEnd();
        
        glBegin(GL_TRIANGLES);
        
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord& normal = -m_Normal;
                
                j = (i  + 1) % m_CircleNum;
                
                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_TaperE[i][0], m_TaperE[i][1], m_TaperE[i][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_TaperE[j][0], m_TaperE[j][1], m_TaperE[j][2]);

                glNormal3d(normal[0], normal[1], normal[2]);
                glVertex3d(m_RealEnd[0], m_RealEnd[1], m_RealEnd[2]);
        }
        
        glEnd();

        glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_SMOOTH);
}


//////////////////////////////////////////////////////////////////////
// CMeshVectorUI private methods
//////////////////////////////////////////////////////////////////////
void CMeshVectorUI::CalCoordinate4Column()
{
        CalCircleCoord4Normal(m_ColumnS, m_Start, m_Normal, m_Radius, m_CircleNum);
        CalCircleCoord4Normal(m_ColumnE, m_RealEnd, m_Normal, m_Radius, m_CircleNum);
        CalColumnNormal();
}
void CMeshVectorUI::CalCoordinate4Taper()
{
        CalCircleCoord4Normal(m_TaperE, m_RealEnd, m_Normal, 1.5 * m_Radius, m_CircleNum);
        CalTaperNormal();
}
void CMeshVectorUI::CalCircleCoord4Normal(vector<Coord>& circleArray, Coord& p, Coord& normal, double radius, int circleNum)
{
        int i;
        Coord u, v;
        CalUV4Normal(normal, u, v);

        // 
        u.normalize();

        circleArray.clear();
        circleArray.reserve(circleNum);

        double pdegree =  PI * 2.0 / circleNum;
    double degree = 0;
        for (i = 0; i < circleNum; i++)
        {
                Coord tmp = u.Spin(normal, p, degree);
                tmp.normalize();
                tmp = (tmp * radius) + p;

                circleArray.push_back(tmp);
                degree += pdegree;
        }
}
void CMeshVectorUI::CalUV4Normal(Coord& normal, Coord& u, Coord& v)
{
        if (fabs(normal[0]) >= fabs(normal[1]))
        {
                u[0] = normal[2];
                u[1] = 0;
                u[2] = -normal[0];
        }
        else
        {
                u[0] = 0;
                u[1] = normal[2];
                u[2] = -normal[1];
        }

        v = cross(normal, u);
}

void CMeshVectorUI::CalColumnNormal()
{
        int i;
        int index;

        m_ColumnNormal.clear();
        m_ColumnNormal.resize(m_CircleNum * 3);

        for (i = 0; i < m_CircleNum; i++)
        {
                Coord v1 = m_ColumnE[(i + 1) % m_CircleNum];
                Coord v2 = m_ColumnE[i];
                Coord v3 = m_ColumnS[i];

                m_ColumnNormal[i] = cross(v1 - v2, v2 - v3).unit();
        }

        index = m_CircleNum;
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord v1 = m_ColumnS[i];
                Coord v2 = m_ColumnS[(i + 1) % m_CircleNum];
                Coord v3 = m_Start;
                
                m_ColumnNormal[i + index] = cross(v1 - v2, v2 - v3).unit();
        }

        index = m_CircleNum * 2;
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord v1 = m_ColumnE[i];
                Coord v2 = m_ColumnE[(i + 1) % m_CircleNum];
                Coord v3 = m_RealEnd;
                
                m_ColumnNormal[i + index] = cross(v1 - v2, v2 - v3).unit();
        }
}
void CMeshVectorUI::CalTaperNormal()
{
        int i;
        
        m_TaperNormal.clear();
        m_TaperNormal.resize(m_CircleNum);
        
        for (i = 0; i < m_CircleNum; i++)
        {
                Coord v1 = m_TaperE[i];
                v1.normalize();

                Coord v2 = m_TaperE[(i + 1) % m_CircleNum];
                v2.normalize();
                
                m_TaperNormal[i] = (v1 + v2) / 2.0;
        }
}
#ifndef MESH_VECTOR_UI_H
#define MESH_VECTOR_UI_H


#include "Macro.h"      // Macro definitions
#include "Utility.h"
#include "BasicDataType.h"      // Basic data types -- including Coord, Color, Coord2D, etc
#include "..\OpenGL\GLElement.h"


class CMeshVectorUI
{
public:
        CMeshVectorUI();
        CMeshVectorUI(const CMeshVectorUI& vec);
        ~CMeshVectorUI();

public:
        void SetVectorUIInfo(Coord& start, Coord& end, Coord& normal, double radius);
    void Draw();

private:
        //
        void ClearData();

    // methods for set the material here.
        void SetDefaultMaterial();

        // methods for drawing the column and taper.
        void DrawColumn();
        void DrawTaper();

        // methods fro computing the column and taper's coordinate.
        void CalCoordinate4Column();
        void CalCoordinate4Taper();

        
        void CalCircleCoord4Normal(vector<Coord>& circleArray, Coord& p, Coord& normal, double radius, int circleNum);
    void CalUV4Normal(Coord& normal, Coord& u, Coord& v);
        void CalColumnNormal();
        void CalTaperNormal();

private:
        int    m_CircleNum;
        double m_Radius;
        double m_Length;
        Coord  m_Start;
        Coord  m_End;
        Coord  m_RealEnd;
        Coord  m_Normal;

private:
        Utility     util;
        GLMaterial m_DefaultMaterial;

private:
        vector<Coord> m_ColumnS;
        vector<Coord> m_ColumnE;
        vector<Coord> m_TaperE;

        vector<Coord> m_ColumnNormal;
        vector<Coord> m_TaperNormal;
};

#endif
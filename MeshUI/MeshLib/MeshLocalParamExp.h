#ifndef MESH_LOCAL_PARAM_EXP_H
#define MESH_LOCAL_PARAM_EXP_H

#include "MeshModel.h"

class CNormalCoordinate
{
public:
        CNormalCoordinate();
        ~CNormalCoordinate();

public:
        void SetNC(int cvid, Coord& cvpos, Coord& cvnormal);
        void AddNeighVertex(int vid, Coord& vpos);
        void Rotate(Coord& othernormal);

public:
        Coord m_X_Coord;
        Coord m_Y_Coord;
        Coord m_N_Coord;
        Coord m_Center_Vtx_Pos;
        int m_Center_Vtx_Id;
        vector<pair<int, Coord> > m_NeighVtx_On3D;
};

class CMeshLocalParamExp
{
public:
        CMeshLocalParamExp();
        ~CMeshLocalParamExp();

public:
        void Attach(MeshModel* pModel);
        void LocalParam(int cvid, vector<int>& tri_index, vector<int>& vtx_index, vector<double>& param_uv);

private:
        void Project(Coord& ncnormal, vector<pair<bool, Coord> >& dijvectors, vector<double>& param_uv);

private:
        MeshModel*        mesh;                   // Attached domain mesh pointer
};

#endif
#include "stdafx.h"
#include "Global.h"

MeshModel* Global::mesh = NULL;
size_t  Global::vtxNum=0;
size_t   Global::faceNum=0;
CoordArray* Global::p_coord=NULL;
PolyIndexArray* Global::p_adjVtxArray=NULL;
Coord Global::bCenter=Coord(0,0,0); 
double Global::bRadius=0.0;
double Global::maxLineLength=0.0;

double Global::MAXVALUE = 1e20;
double Global::MINVALUE = -1e20;
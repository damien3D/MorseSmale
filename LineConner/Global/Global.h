#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "..\..\MeshUI\MeshLib\MeshModel.h"
#include "..\..\MeshUI\MeshLib\Utility.h"
#include "..\..\MeshUI\MeshLib\MeshLocalParamExp.h"
#include "..\..\MeshUI\MeshLib\HSVColor.h"

#include <windows.h>
#include <gl/glut.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>

typedef pair<size_t,size_t> MaxMinPair;
typedef vector<size_t> PATH;

#define UNCERTAIN       INT_MAX

struct  Global{
        /* some global data*/

        static MeshModel* mesh;                   
        static size_t   vtxNum;
        static size_t   faceNum;
        static CoordArray* p_coord;
        static PolyIndexArray* p_adjVtxArray;

        static Coord bCenter; 
        static double bRadius;
        static double maxLineLength;
        
        static double MAXVALUE;
        static double MINVALUE;

        enum POINTTYPE{ 
                REGULARPOINT,
                SADDLEPOINT,
                MINPOINT,
                MAXPOINT,
                BADPOINT
        };

        enum DrawMode {
                        DRAWMAXMINPOINTS        = 0x00000001
                ,       DRAWSADDLEPOINTS        = 0x00000002
                ,       DRAWCONNECTLINES        = 0x00000004
                ,       DRAWSTRAIGHTENLINES     = 0x00000008
                ,       DRAWBADCONNLINES        = 0x00000010

                ,       DRAWPOINTS
                ,       DRAWSTRAIGHTEN  
                ,       DRAWPATH
                ,       DRAWDUAL
                ,       DRAWBADPATH
        };

};



#endif
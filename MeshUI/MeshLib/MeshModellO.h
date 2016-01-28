/* ================== Library Information ================== */
// [Name] 
// MeshLib Library
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-05
//
// [Goal]
// A general, flexible, versatile and easy-to-use mesh library for research purpose.
// Supporting arbitrary polygonal meshes as input, but with a 
// primary focus on triangle meshes.

/* ================== File Information ================== */
// [Name]
// MeshModelIO.h
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-05
//
// [Goal]
// Defining the input/output (I/O) functions of the kernel components of the mesh model
// Supporting various 3D model files, including .tm, .obj, .off, etc



#include "MeshModelKernel.h"
#include "Utility.h"
#pragma once

using namespace std;


class MeshModelIO
{
private:
    MeshModelKernel* kernel;
    Utility util;

public:
    // Constructor
    MeshModelIO();

    // Destructor
    ~MeshModelIO();

    // Initializer
    void ClearData();
    void AttachKernel(MeshModelKernel* pKernel = NULL);

public:
    // General I/O functions
    bool LoadModel(string filename);
    bool StoreModel(string filename);

    // .tm file I/O functions
    bool OpenTmFile(string filename);
    bool SaveTmFile(string filename);

    // .ply2 file I/O functions
    bool OpenPly2File(string filename);
    bool SavePly2File(string filename);

        // .off file I/O functions
        bool OpenOffFile(string filename);
        bool SaveOffFile(string filename);
        
        //
        bool OpenObjFile(string filename);
        bool SaveObjFile(string filename);
};
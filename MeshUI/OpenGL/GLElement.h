/* ================== Library Information ================== */
// [Name] 
// OpenGL Library
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-09
//
// [Goal]
// A general, flexible, versatile and easy-to-use OpenGL library for research purpose.

/* ================== File Information ================== */
// [Name]
// OpenGL.h
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-09
//
// [Goal]
// Defining the various elements in OpenGL



#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "GLContext.h"

#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib")
#pragma comment( lib, "glaux.lib")

#pragma once



/* ================== Macros ================== */

// Material macros
#define MATERIAL_AMBIENT    0X00000001
#define MATERIAL_DIFFUSE    0X00000002
#define MATERIAL_SPECULAR   0X00000003
#define MATERIAL_EMISSION   0X00000004
#define MATERIAL_SHININESS  0X00000005

// Light macros
#define LIGHT_TYPE_DIRECTIONAL      0X0000001
#define LIGHT_TYPE_SPOT             0X0000001
#define LIGHT_TYPE_POINT            0X0000002

#define LIGHT_AMBIENT               0X00000001
#define LIGHT_DIFFUSE               0X00000002
#define LIGHT_SPECULAR              0X00000003
#define LIGHT_POSITION              0X00000004
#define LIGHT_SPOT_DIRECTION        0X00000005
#define LIGHT_SPOT_EXPONENT         0X00000006
#define LIGHT_SPOT_CUTOFF           0X00000007
#define LIGHT_CONSTANT_ATTENUATION  0X00000008
#define LIGHT_LINEAR_ATTENUATION    0X00000009
#define LIGHT_QUADRATIC_ATTENUATION 0X0000000A

// Projection macros
#define PROJECTION_TYPE_ORTHOGONAL  0X00000001
#define PROJECTION_TYPE_PERSPECTIVE 0X00000002



/* ================== Material Information ================== */

class GLMaterial
{
public:
    float   ambient[4];
    float   diffuse[4];
    float   specular[4];
    float   emission[4];
    float   shininess;
    
    GLenum  face;

public:
    // Constructor
    GLMaterial();
        GLMaterial(const GLMaterial& material);

    // Destructor
    ~GLMaterial();

    // Initializer
    void ClearData();

    // Set material value
    void SetValue(int pname, float* param);

    // Set material
    void SetMaterial();

};



/* ================== Light Information ================== */

class GLLight
{
public:
    float   type;
    float   ambient[4];
    float   diffuse[4];
    float   specular[4];
    float   position[4];
    float   direction[3];
    float   exponent;
    float   cutoff;
    float   constant_attenuation;
    float   linear_attenuation;
    float   quadratic_attenuation;

    GLenum  lightID;

public:
    // Constructor
    GLLight();

    // Destructor
    ~GLLight();

    // Initializer
    void ClearData();

    // Set light value
    void SetValue(int pname, float* param);

    // Set light
    void SetLight();
};



/* ================== Projection Information ================== */

class GLProjection
{
private:
    double vv_left; // Viewing volume (vv) left
    double vv_right;
    double vv_top;
    double vv_bottom;
    double vv_near;
    double vv_far;

    int type;

    double fov;
    double aspect;

    int vp_left;    // Viewport (vp) left
    int vp_right;
    int vp_top;
    int vp_bottom;

    double object_center[3];
    double object_radius;
    double object_ratio;

public:
    // Constructor
    GLProjection();

    // Destructor
    ~GLProjection();

    // Initializer
    void ClearData();

    // Set object information
    void SetObjectInfo(double cx, double cy, double cz, double r);

    // Set projection
    void SetProjection(int width, int height);
    void SetProjection(int left, int right, int top, int bottom);

    // Zoom
    void Zoom(double scale);
};



/* ================== OpenGL Element Information ================== */

class COpenGL
{
public:
    double m_BkColor[4];        // Background color (rgba)
    
    GLMaterial   m_Material;    // OpenGL material structure
    GLLight      m_Light;       // OpenGL lighting structure
    GLProjection m_Projection;  // OpenGL projection structure
    CGLContext   m_Context;     // OpenGL context wraper

public:
    // Constructor
        COpenGL();

    // Destructor
        ~COpenGL();
        
    // Initializer
    void ClearData();

    // Debug
    bool DetectOpenGLError();

    // Message handlers
    void OnCreate(CWnd* pWnd);
    void OnResize(int width, int height);
    void OnReset(int width, int height);
    void OnInit();
    void OnBeginPaint();
    void OnEndPaint();
    void OnDestroy();
    
     // Set object information
    void SetObjectInfo(double cx, double cy, double cz, double r);
};
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
// OpenGL.cpp
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



#include "stdafx.h"
#include "GLElement.h"
#include <math.h>



/* ================== Material Information ================== */

// Constructor
GLMaterial::GLMaterial()
{
    ClearData();
}

GLMaterial::GLMaterial(const GLMaterial& material)
{
        int i;
        
        for(i = 0; i < 4; ++ i)
        {
        this->ambient[i] = material.ambient[i];
        }
        
        for(i = 0; i < 4; ++ i)
        {
        this->diffuse[i] = material.diffuse[i];
        }
        
        for(i = 0; i < 4; ++ i)
        {
        this->specular[i] = material.specular[i];
        }
        
        for(i = 0; i < 4; ++ i)
        {
        this->emission[i] = material.emission[i];
        }
        
        this->shininess = material.shininess;
        this->face = material.face;
}

// Destructor
GLMaterial::~GLMaterial()
{
}


// Initializer
void GLMaterial::ClearData()
{
    ambient[0] = 0.2f;
    ambient[1] = 0.2f;
    ambient[2] = 0.2f;
    ambient[3] = 1.0f;

    diffuse[0] = 0.8f;
    diffuse[1] = 0.738f;
    diffuse[2] = 0.738f;
    diffuse[3] = 1.0f;

    specular[0] = 0.05f;
    specular[1] = 0.0f;
    specular[2] = 0.0f;
    specular[3] = 1.0f;

    emission[0] = 0.0f;
    emission[1] = 0.0f;
    emission[2] = 0.0f;
    emission[3] = 1.0f;

    shininess = 10.0f;

    face = GL_FRONT_AND_BACK;
}

// Set material value
void GLMaterial::SetValue(int pname, float* param)
{
    int i;
    switch(pname) {
    case MATERIAL_AMBIENT:
        for(i = 0; i < 4; ++ i)
        ambient[i] = param[i];
        break;
    case MATERIAL_DIFFUSE:
        for(i = 0; i < 4; ++ i)
        diffuse[i] = param[i];
        break;
    case MATERIAL_SPECULAR:
        for(i = 0; i < 4; ++ i)
        specular[i] = param[i];
        break;
    case MATERIAL_EMISSION:
        for(i = 0; i < 4; ++ i)
        emission[i] = param[i];
        break;
    case MATERIAL_SHININESS:
        shininess = *param;
        break;
    }
}

// Set material
void GLMaterial::SetMaterial()
{
    glMaterialfv(face, GL_AMBIENT, ambient);
    glMaterialfv(face, GL_DIFFUSE, diffuse);
    glMaterialfv(face, GL_SPECULAR, specular);
    glMaterialfv(face, GL_EMISSION, emission);
    glMaterialf( face, GL_SHININESS, shininess);
}



/* ================== Light Information ================== */

// Constructor
GLLight::GLLight()
{
    ClearData();
}


// Destructor
GLLight::~GLLight()
{
}


// Initializer
void GLLight::ClearData()
{
    type = LIGHT_TYPE_DIRECTIONAL;
    
    ambient[0] = 0.05f;
    ambient[1] = 0.0f;
    ambient[2] = 0.0f;
    ambient[3] = 1.0f;

    diffuse[0] = 1.0f;
    diffuse[1] = 1.0f;
    diffuse[2] = 1.0f;
    diffuse[3] = 1.0f;

    specular[0] = 1.0f;
    specular[1] = 1.0f;
    specular[2] = 1.0f;
    specular[3] = 1.0f;

    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 1.0f;
    position[3] = 0.0f;

    direction[0] = 0.0f;
    direction[1] = 0.0f;
    direction[2] = -1.0f;

    exponent = 0.0f;

    cutoff = 180.0f;
    
    constant_attenuation = 1.0f;
    linear_attenuation = 0.0f;
    quadratic_attenuation = 0.0f;

    lightID = GL_LIGHT0;
}

// Set material value
void GLLight::SetValue(int pname, float* param)
{
    int i;
    switch(pname) {
    case LIGHT_AMBIENT:
        for(i = 0; i < 4; ++ i)
        ambient[i] = param[i];
        break;
    case LIGHT_DIFFUSE:
        for(i = 0; i < 4; ++ i)
        diffuse[i] = param[i];
        break;
    case LIGHT_SPECULAR:
        for(i = 0; i < 4; ++ i)
        specular[i] = param[i];
        break;
    case LIGHT_POSITION:
        for(i = 0; i < 4; ++ i)
        position[i] = param[i];
        break;
    case LIGHT_SPOT_DIRECTION:
        for(i = 0; i < 3; ++ i)
        direction[i] = param[i];
        break;
    case LIGHT_SPOT_EXPONENT:
        exponent = *param;
        break;
    case LIGHT_SPOT_CUTOFF:
        cutoff = *param;
        break;
    case LIGHT_CONSTANT_ATTENUATION:
        constant_attenuation = *param;
        break;
    case LIGHT_LINEAR_ATTENUATION:        
        linear_attenuation = *param;
        break;
    case LIGHT_QUADRATIC_ATTENUATION:
        quadratic_attenuation = *param;
        break;
    }
}

// Set material
void GLLight::SetLight()
{
    glLightfv(lightID, GL_AMBIENT, ambient);
    glLightfv(lightID, GL_DIFFUSE, diffuse);
    glLightfv(lightID, GL_SPECULAR, specular);
    glLightfv(lightID, GL_POSITION, position);

    if(position[3] == 0.0f)  // Directional light
    {
        type = LIGHT_TYPE_DIRECTIONAL;

        glLightf(lightID, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(lightID, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(lightID, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
    else 
    {
        if(cutoff == 180.0f)
        {
            type = LIGHT_TYPE_POINT;
        }
        else
        {
            type = LIGHT_TYPE_SPOT;
            glLightfv(lightID, GL_SPOT_DIRECTION, direction);
            glLightf(lightID, GL_SPOT_EXPONENT, exponent);
            glLightf(lightID, GL_SPOT_CUTOFF, cutoff);
        }
        glLightf(lightID, GL_CONSTANT_ATTENUATION, constant_attenuation);
        glLightf(lightID, GL_LINEAR_ATTENUATION, linear_attenuation);
        glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);
    }

    glEnable(lightID);  // Enable lightID
}
                 
              
              
/* ================== Projection Information ================== */

// Constructor
GLProjection::GLProjection()
{
    ClearData();
}

// Destructor
GLProjection::~GLProjection()
{
}

// Initializer
void GLProjection::ClearData()
{
    vv_left = vv_right = vv_top = vv_bottom = 0.0;
    fov = 40.0f;
    aspect = 1.0f;

    vv_near = 1.0;
    vv_far = 20.0;

    type = PROJECTION_TYPE_ORTHOGONAL;

    vp_left = vp_right = vp_top = vp_bottom = 0;

    object_center[0] = 0.0;
    object_center[1] = 0.0;
    object_center[2] = 0.0;
    object_radius = 1.0;
    
    object_ratio = 1.3;
}

// Set object information
void GLProjection::SetObjectInfo(double cx, double cy, double cz, double r)
{
    object_center[0] = cx;
    object_center[1] = cy;
    object_center[2] = cz;
    object_radius    = r;
}

// Set projection
void GLProjection::SetProjection(int width, int height)
{
    if(height == 0)
        height = width+1;

    aspect = (double)width/(double)height;

    glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
    if(type == PROJECTION_TYPE_ORTHOGONAL)
    {
        vv_top = object_radius*object_ratio;
        vv_bottom = -vv_top;
        vv_right = aspect*vv_top;
        vv_left = -vv_right;
        glOrtho(vv_left, vv_right, vv_bottom, vv_top, -vv_far, vv_far);
    }
    else
    {
        vv_top = vv_near*tan(fov/2.0);
        vv_bottom = -vv_top;
        vv_right = vv_top*aspect;
        vv_left = -vv_right;
        glFrustum(vv_left, vv_right, vv_bottom, vv_top, vv_near, vv_far);
    }
    
//    double z_dist = (object_radius*object_ratio)/tan(fov/2.0);
//    gluLookAt(object_center[0], object_center[1], object_center[2]+z_dist, object_center[0], object_center[1], object_center[2], 0, 1, 0);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
}

void GLProjection::SetProjection(int left, int right, int top, int bottom)
{
}

void GLProjection::Zoom(double scale)
{
    if(type == PROJECTION_TYPE_ORTHOGONAL)
        object_ratio *= scale;
    else
        fov *= scale;
}

/* ================== OpenGL Element Information ================== */

// Constructor
COpenGL::COpenGL()
{
    ClearData();
}

// Destructor
COpenGL::~COpenGL()
{
}

// Initializer
void COpenGL::ClearData()
{
        /*
        m_BkColor[0] = 1.0;
    m_BkColor[1] = 1.0;
    m_BkColor[2] = 1.0;
    m_BkColor[3] = 1.0;*/

        
    m_BkColor[0] = 0.50;
    m_BkColor[1] = 0.50;
    m_BkColor[2] = 0.50;
    m_BkColor[3] = 1.0;

    m_Light.ClearData();
    m_Projection.ClearData();
}

// Debug
bool COpenGL::DetectOpenGLError()
{
        GLenum errCode;
        const GLubyte *errString;

        if((errCode = glGetError()) != GL_NO_ERROR)
        {
                errString = gluErrorString(errCode);
                fprintf(stdout,"OpenGL Error: %s\n",errString);
        ASSERT(0);
        }
        
    return (errCode == GL_NO_ERROR);
}

// Message handlers
void COpenGL::OnCreate(CWnd* pWnd)
{
        m_Context.OnCreate(pWnd);
}

void COpenGL::SetObjectInfo(double cx, double cy, double cz, double r)
{
        m_Context.glEnter();
        m_Projection.SetObjectInfo(cx, cy, cz, r);
        m_Context.glLeave();
}

void COpenGL::OnResize(int width, int height)
{
    if ((width <= 0) || (height <= 0))
        return;

        m_Context.glEnter();

        DetectOpenGLError();

    // m_Projection.SetObjectInfo(0, 0, 0, 1);
    m_Projection.SetProjection(width, height);

    DetectOpenGLError();
    
        m_Context.glLeave();
}

void COpenGL::OnReset(int width, int height)
{
    if ((width <= 0) || (height <= 0))
        return;

        m_Context.glEnter();

        DetectOpenGLError();

    m_Projection.ClearData();
    m_Projection.SetProjection(width, height);

    DetectOpenGLError();
    
        m_Context.glLeave();
}

void COpenGL::OnInit()
{
        m_Context.glEnter();

    // Set background color
        glClearColor((float)m_BkColor[0], (float)m_BkColor[1], (float)m_BkColor[2], (float)m_BkColor[3]);
        
    DetectOpenGLError();

        // Set lights
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    m_Light.SetLight();
        glEnable(GL_LIGHTING);

    DetectOpenGLError();

    // Set material
    m_Material.SetMaterial();

    DetectOpenGLError();

    // Enable depth test
        glEnable(GL_DEPTH_TEST);

        m_Context.glLeave();
}

void COpenGL::OnBeginPaint()
{
    m_Context.glEnter();

        glDrawBuffer(GL_BACK);      // Set back buffer
        glClearColor((float)m_BkColor[0], (float)m_BkColor[1], (float)m_BkColor[2], (float)m_BkColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear contents
}

void COpenGL::OnEndPaint()
{
        glFinish();
    m_Context.SwapBuffers();    // Swap back buffer
    m_Context.glLeave();
}

void COpenGL::OnDestroy()
{
    m_Context.OnDestroy();
}
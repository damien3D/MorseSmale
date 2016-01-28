// GLContext.cpp: implementation of the CGLContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GLContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLContext::CGLContext()
:m_pCDC(NULL),
m_hGLContext(NULL),
m_GLPixelIndex(NULL),
m_nEnterCounter(0)
{

}

CGLContext::~CGLContext()
{

}


// Help function
BOOL CGLContext::CreateViewGLContext(HDC hDC)
{
        m_hGLContext = wglCreateContext(hDC);
        
        if(m_hGLContext==NULL)
                return FALSE;
        
        if(wglMakeCurrent(hDC, m_hGLContext)==FALSE)
                return FALSE;

        return TRUE;
}

BOOL CGLContext::SetWindowPixelFormat(HDC hDC)
{
        static PIXELFORMATDESCRIPTOR pixelDesc = 
        {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                                                              // version number
        PFD_DRAW_TO_WINDOW |                    // support window
        PFD_SUPPORT_OPENGL |                    // support OpenGL
        PFD_DOUBLEBUFFER ,                              // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 16-bit z-buffer
        16,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };

        int pixelformat;
    if((pixelformat = ChoosePixelFormat(hDC, &pixelDesc)) == 0)
    {
        AfxMessageBox(_T("ChoosePixelFormat failed"));
        return FALSE;
    }

    if(SetPixelFormat(hDC, pixelformat, &pixelDesc) == FALSE)
    {
        AfxMessageBox(_T("SetPixelFormat failed"));
        return FALSE;
    }

//    CreateRGBPalette(m_hDc);

    return TRUE;
//      m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
//      if(m_GLPixelIndex==0) // Choose default
//      {
//              m_GLPixelIndex = 1;
//              if(!DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc))
//                      return FALSE;
//      }
//      
//      if(!SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc))
//              return FALSE;
//      
//      return TRUE;
}

BOOL CGLContext::OnCreate(CWnd *pWnd)
{
        m_pCDC= new CClientDC(pWnd);
        if(SetWindowPixelFormat(m_pCDC->GetSafeHdc()) == -1)
                return FALSE;
        
        if(CreateViewGLContext(m_pCDC->GetSafeHdc()) == -1)
                return FALSE;
        return TRUE;
}

void CGLContext::OnDestroy()
{
        if(wglGetCurrentContext() != NULL)
                wglMakeCurrent(NULL, NULL);
        
        if(m_hGLContext != NULL)
        {
                wglDeleteContext(m_hGLContext);
                m_hGLContext = NULL;
        }

        if(m_pCDC) 
                delete m_pCDC;
}

void CGLContext::glEnter()
{
        wglMakeCurrent(m_pCDC->GetSafeHdc(), m_hGLContext);
        ++m_nEnterCounter;
}

void CGLContext::glLeave()
{
        --m_nEnterCounter;
        if(!m_nEnterCounter)
                wglMakeCurrent(NULL,NULL);
}

void CGLContext::SwapBuffers()
{
        ::SwapBuffers(m_pCDC->GetSafeHdc());
}
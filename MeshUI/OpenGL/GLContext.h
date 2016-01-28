// GLContext.h: interface for the CGLContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLCONTEXT_H__D688FE0C_0999_4E8F_A458_68F624C4A046__INCLUDED_)
#define AFX_GLCONTEXT_H__D688FE0C_0999_4E8F_A458_68F624C4A046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib")
#pragma comment( lib, "glaux.lib")


class CGLContext  
{
public:
        CGLContext();
        virtual ~CGLContext();
        
public:
        BOOL OnCreate(CWnd *pWnd);
        void OnDestroy();
        
        void glLeave();
        void glEnter();
        
        void SwapBuffers();
        HGLRC           m_hGLContext;

private:
        CClientDC  *m_pCDC;
        int                     m_GLPixelIndex;
        int                     m_nEnterCounter;
        
        BOOL SetWindowPixelFormat(HDC hDC);
        BOOL CreateViewGLContext(HDC hDC);
};

#endif // !defined(AFX_GLCONTEXT_H__D688FE0C_0999_4E8F_A458_68F624C4A046__INCLUDED_)

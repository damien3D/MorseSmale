// MeshModelFrameWork.h : main header file for the MeshModelFrameWork application
//
#pragma once

#ifndef __AFXWIN_H__
        #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMeshModelFrameWorkApp:
// See MeshModelFrameWork.cpp for the implementation of this class
//

class CMeshModelFrameWorkApp : public CWinApp
{
public:
        CMeshModelFrameWorkApp();

        void CreateConsole();

// Overrides
public:
        virtual BOOL InitInstance();

// Implementation
        afx_msg void OnAppAbout();
        DECLARE_MESSAGE_MAP()
};

extern CMeshModelFrameWorkApp theApp;
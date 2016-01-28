// MeshModelFrameWorkView.h : interface of the CMeshModelFrameWorkView class
//


#pragma once

#include ".\Source Codes\MeshUI\MeshLib\MeshModel.h"
#include ".\Source Codes\MeshUI\OpenGL\GLContext.h"
#include ".\Source Codes\MeshUI\OpenGL\GLElement.h"
#include ".\Source Codes\MeshUI\MeshUI.h"

#include ".\Source Codes\LineConner\Main\MeshEigenConner.h"

#include "MeshModelFrameWorkDoc.h"

#include ".\Source Codes\mfc\DSetThreshold.h"

class CMeshModelFrameWorkView : public CView
{
protected: // create from serialization only
        CMeshModelFrameWorkView();
        DECLARE_DYNCREATE(CMeshModelFrameWorkView)

// Attributes
public:
        CMeshModelFrameWorkDoc* GetDocument() const;

        // Variables
public:
        MeshModel               mesh;
        COpenGL                 m_OpenGL;
        CMeshUI                 m_MeshUI;
        Utility                 util;
        CMeshConner                             m_MEConner;


        // Methods
public:
        void DrawScene();
        BOOL SaveAsBMP(char *bmpFileName);

private:
        void SetupBmpHeader(BITMAPINFOHEADER *pbmih, int sx, int sy, int bpp);
        unsigned int WidthByte32(unsigned int width, unsigned int depth);
        void ClearModel();

// Overrides
public:
        virtual void OnDraw(CDC* pDC);  // overridden to draw this view
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
        virtual ~CMeshModelFrameWorkView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        DECLARE_MESSAGE_MAP()
public:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnDestroy();
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        afx_msg void OnFileOpenModel();
        afx_msg void OnFileSaveModel();
        afx_msg void OnManipulatorMove();
        afx_msg void OnUpdateManipulatorMove(CCmdUI* pCmdUI);
        afx_msg void OnManipulatorSpin();
        afx_msg void OnUpdateManipulatorSpin(CCmdUI* pCmdUI);
        afx_msg void OnManipulatorViewall();
        afx_msg void OnLoadViewmatrix();
        afx_msg void OnSaveViewmatrix();
        afx_msg void OnRenderingSolidFlat();
        afx_msg void OnUpdateRenderingSolidFlat(CCmdUI* pCmdUI);
        afx_msg void OnRenderingSolidSmooth();
        afx_msg void OnUpdateRenderingSolidSmooth(CCmdUI* pCmdUI);
        afx_msg void OnRenderingSolidWireframe();
        afx_msg void OnUpdateRenderingSolidWireframe(CCmdUI* pCmdUI);
        afx_msg void OnRenderingVertices();
        afx_msg void OnUpdateRenderingVertices(CCmdUI* pCmdUI);
        afx_msg void OnRenderingWireframe();
        afx_msg void OnUpdateRenderingWireframe(CCmdUI* pCmdUI);
        afx_msg void OnRenderingTransparent();
        afx_msg void OnUpdateRenderingTransparent(CCmdUI* pCmdUI);
        afx_msg void OnRenderingVertexColor();
        afx_msg void OnUpdateRenderingVertexColor(CCmdUI* pCmdUI);
        afx_msg void OnRenderingBoudingbox();
        afx_msg void OnUpdateRenderingBoudingbox(CCmdUI* pCmdUI);
        afx_msg void OnFileSaveAsBmp();
        afx_msg void OnLoadMeshEigenField();
        afx_msg void OnSaveMeshConnLine();
        afx_msg void OnConnMeshLine();
        afx_msg void OnRemoveChainedCpNoise();
        afx_msg void OnUpdateRemoveChainedCpNoise(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in MeshModelFrameWorkView.cpp
inline CMeshModelFrameWorkDoc* CMeshModelFrameWorkView::GetDocument() const
   { return reinterpret_cast<CMeshModelFrameWorkDoc*>(m_pDocument); }
#endif
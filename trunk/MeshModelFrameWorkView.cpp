// MeshModelFrameWorkView.cpp : implementation of the CMeshModelFrameWorkView class
//

#include "stdafx.h"
#include "MeshModelFrameWork.h"

#include "MeshModelFrameWorkDoc.h"
#include "MeshModelFrameWorkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMeshModelFrameWorkView

IMPLEMENT_DYNCREATE(CMeshModelFrameWorkView, CView)

BEGIN_MESSAGE_MAP(CMeshModelFrameWorkView, CView)
        ON_WM_CREATE()
        ON_WM_DESTROY()
        ON_WM_ERASEBKGND()
        ON_WM_SETCURSOR()
        ON_WM_SIZE()
        ON_WM_CONTEXTMENU()
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_MOUSEWHEEL()
        ON_WM_MOUSEMOVE()
        ON_WM_KEYDOWN()
        ON_COMMAND(ID_FILE_OPEN_MODEL, &CMeshModelFrameWorkView::OnFileOpenModel)
        ON_COMMAND(ID_FILE_SAVE_MODEL, &CMeshModelFrameWorkView::OnFileSaveModel)
        ON_COMMAND(ID_MANIPULATOR_MOVE, &CMeshModelFrameWorkView::OnManipulatorMove)
        ON_UPDATE_COMMAND_UI(ID_MANIPULATOR_MOVE, &CMeshModelFrameWorkView::OnUpdateManipulatorMove)
        ON_COMMAND(ID_MANIPULATOR_SPIN, &CMeshModelFrameWorkView::OnManipulatorSpin)
        ON_UPDATE_COMMAND_UI(ID_MANIPULATOR_SPIN, &CMeshModelFrameWorkView::OnUpdateManipulatorSpin)
        ON_COMMAND(ID_MANIPULATOR_VIEWALL, &CMeshModelFrameWorkView::OnManipulatorViewall)
        ON_COMMAND(ID_LOAD_VIEWMATRIX, &CMeshModelFrameWorkView::OnLoadViewmatrix)
        ON_COMMAND(ID_SAVE_VIEWMATRIX, &CMeshModelFrameWorkView::OnSaveViewmatrix)
        ON_COMMAND(ID_RENDERING_SOLID_FLAT, &CMeshModelFrameWorkView::OnRenderingSolidFlat)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_SOLID_FLAT, &CMeshModelFrameWorkView::OnUpdateRenderingSolidFlat)
        ON_COMMAND(ID_RENDERING_SOLID_SMOOTH, &CMeshModelFrameWorkView::OnRenderingSolidSmooth)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_SOLID_SMOOTH, &CMeshModelFrameWorkView::OnUpdateRenderingSolidSmooth)
        ON_COMMAND(ID_RENDERING_SOLID_WIREFRAME, &CMeshModelFrameWorkView::OnRenderingSolidWireframe)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_SOLID_WIREFRAME, &CMeshModelFrameWorkView::OnUpdateRenderingSolidWireframe)
        ON_COMMAND(ID_RENDERING_VERTICES, &CMeshModelFrameWorkView::OnRenderingVertices)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_VERTICES, &CMeshModelFrameWorkView::OnUpdateRenderingVertices)
        ON_COMMAND(ID_RENDERING_WIREFRAME, &CMeshModelFrameWorkView::OnRenderingWireframe)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_WIREFRAME, &CMeshModelFrameWorkView::OnUpdateRenderingWireframe)
        ON_COMMAND(ID_RENDERING_TRANSPARENT, &CMeshModelFrameWorkView::OnRenderingTransparent)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_TRANSPARENT, &CMeshModelFrameWorkView::OnUpdateRenderingTransparent)
        ON_COMMAND(ID_RENDERING_VERTEX_COLOR, &CMeshModelFrameWorkView::OnRenderingVertexColor)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_VERTEX_COLOR, &CMeshModelFrameWorkView::OnUpdateRenderingVertexColor)
        ON_COMMAND(ID_RENDERING_BOUDINGBOX, &CMeshModelFrameWorkView::OnRenderingBoudingbox)
        ON_UPDATE_COMMAND_UI(ID_RENDERING_BOUDINGBOX, &CMeshModelFrameWorkView::OnUpdateRenderingBoudingbox)
        ON_COMMAND(ID_FILE_SAVE_AS_BMP, &CMeshModelFrameWorkView::OnFileSaveAsBmp)
        ON_COMMAND(ID_LOAD_MESH_EIGENFIELD, &CMeshModelFrameWorkView::OnLoadMeshEigenField)
        ON_COMMAND(ID_SAVE_MESH_CONNLINE, &CMeshModelFrameWorkView::OnSaveMeshConnLine)
        ON_COMMAND(ID_CONN_MESH_LINE, &CMeshModelFrameWorkView::OnConnMeshLine)
        ON_COMMAND(ID_REMOVE_CHAINED_CP_NOISE, &CMeshModelFrameWorkView::OnRemoveChainedCpNoise)
        ON_UPDATE_COMMAND_UI(ID_REMOVE_CHAINED_CP_NOISE, &CMeshModelFrameWorkView::OnUpdateRemoveChainedCpNoise)

END_MESSAGE_MAP()

// CMeshModelFrameWorkView construction/destruction

CMeshModelFrameWorkView::CMeshModelFrameWorkView()
{
        // TODO: add construction code here

}

CMeshModelFrameWorkView::~CMeshModelFrameWorkView()
{
}

BOOL CMeshModelFrameWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        if (!CWnd::PreCreateWindow(cs))
                return FALSE;

        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        cs.dwExStyle |= WS_EX_CLIENTEDGE;
        cs.style &= ~WS_BORDER;
        cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
                ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

        return TRUE;

        //return CView::PreCreateWindow(cs);
}

// CMeshModelFrameWorkView drawing

void CMeshModelFrameWorkView::OnDraw(CDC* /*pDC*/)
{
        CMeshModelFrameWorkDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        if (!pDoc)
                return;

        // TODO: add draw code for native data here
        DrawScene();
}


// CMeshModelFrameWorkView diagnostics

#ifdef _DEBUG
void CMeshModelFrameWorkView::AssertValid() const
{
        CView::AssertValid();
}

void CMeshModelFrameWorkView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CMeshModelFrameWorkDoc* CMeshModelFrameWorkView::GetDocument() const // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshModelFrameWorkDoc)));
        return (CMeshModelFrameWorkDoc*)m_pDocument;
}
#endif //_DEBUG


// CMeshModelFrameWorkView message handlers

int CMeshModelFrameWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
        if (CView::OnCreate(lpCreateStruct) == -1)
                return -1;

        // TODO:  Add your specialized creation code here
        m_OpenGL.OnCreate(this);
        m_OpenGL.OnInit();

        return 0;
}

void CMeshModelFrameWorkView::OnDestroy()
{
        CView::OnDestroy();

        // TODO: Add your message handler code here
        m_OpenGL.OnDestroy();
}

BOOL CMeshModelFrameWorkView::OnEraseBkgnd(CDC* pDC)
{
        // TODO: Add your message handler code here and/or call default

        return TRUE;
}

BOOL CMeshModelFrameWorkView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
        // TODO: Add your message handler code here and/or call default

        HCURSOR hCursor;

        switch (m_MeshUI.MouseAct())
        {
        case MOUSE_TYPE_MOVE:
        case MOUSE_TYPE_TRANSLATE:
        case MOUSE_TYPE_RIGID_EDIT:
                hCursor = AfxGetApp()->LoadCursor( IDC_MOVE );
                SetCursor( hCursor );
                break;

        case MOUSE_TYPE_SPIN:
        case MOUSE_TYPE_ROTATE:
                hCursor = AfxGetApp()->LoadCursor( IDC_SPIN );
                SetCursor( hCursor );
                break;

        case MOUSE_TYPE_ZOOM:
        case MOUSE_TYPE_SCALE:
                hCursor = AfxGetApp()->LoadCursor( IDC_ZOOM );
                SetCursor( hCursor );
                break;

        case MOUSE_TYPE_SELECT_LASSO:
        case MOUSE_TYPE_SELECT_FILL:
        case MOUSE_TYPE_SELECT_PAINT:
        case MOUSE_TYPE_SET_AXIS:
                break;

                //++++++Added by Zmy++++++//
        case MOUSE_TYPE_SET_MESH_VECTOR:
                //hCursor = AfxGetApp()->LoadCursor( IDC_SELECTION );
                //SetCursor( hCursor );
                break;

        default:
                hCursor = AfxGetApp()->LoadStandardCursor( IDC_ARROW );
                SetCursor( hCursor );
                break;
        }

        return TRUE;
        // return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CMeshModelFrameWorkView::OnSize(UINT nType, int cx, int cy)
{
        CView::OnSize(nType, cx, cy);

        // TODO: Add your message handler code here
        m_OpenGL.OnResize(cx, cy);
}

void CMeshModelFrameWorkView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
        // TODO: Add your message handler code here
        if (m_MeshUI.MouseAct() != MOUSE_TYPE_BEND_SRCCRV && m_MeshUI.MouseAct() != MOUSE_TYPE_BEND_TARCRV && 
                m_MeshUI.MouseAct() != MOUSE_TYPE_SELECT_LASSO && m_MeshUI.MouseAct() != MOUSE_TYPE_SET_MESH_VECTOR)
        {
                CMenu menu;
                menu.LoadMenu(IDR_MAIN_CONTEXT_MENU);

                CMenu* pContextMenu = menu.GetSubMenu(0);
                pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON |
                        TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

                Invalidate();
        }
}

void CMeshModelFrameWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
        // TODO: Add your message handler code here and/or call default
        SetCapture();

        if(m_MeshUI.OnLButtonDown(nFlags, point))
        {
                if(m_MeshUI.MouseAct() == MOUSE_TYPE_SET_MESH_VECTOR)
                {
                        Coord tmpCoord;

                        m_MeshUI.GetWorldCoord(point.x, point.y, tmpCoord);
                }

                Invalidate();
        }

        CView::OnLButtonDown(nFlags, point);
}

void CMeshModelFrameWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
        // TODO: Add your message handler code here and/or call default

        if(m_MeshUI.OnLButtonUp(nFlags, point))
        {
                if(m_MeshUI.MouseAct() == MOUSE_TYPE_SET_MESH_VECTOR)
                {
                        Coord tmpCoord;

                        m_MeshUI.GetWorldCoord(point.x, point.y, tmpCoord);
                }

                Invalidate();
        }

        ReleaseCapture();

        CView::OnLButtonUp(nFlags, point);
}

BOOL CMeshModelFrameWorkView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
        // TODO: Add your message handler code here and/or call default

        if(m_MeshUI.OnMouseWheel(nFlags, zDelta, pt))
        {
                // Reset view frustum
                CRect rect;
                GetClientRect(&rect);
                m_OpenGL.OnResize(rect.Width(), rect.Height());

                Invalidate();
        }

        return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMeshModelFrameWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
        // TODO: Add your message handler code here and/or call default

        if(m_MeshUI.OnMouseMove(nFlags, point))
        {
                if(m_MeshUI.MouseAct() == MOUSE_TYPE_ZOOM)
                {
                        CRect rect;
                        GetClientRect(&rect);
                        m_OpenGL.OnResize(rect.Width(), rect.Height());
                }

                if(m_MeshUI.MouseAct() == MOUSE_TYPE_SET_MESH_VECTOR)
                {
                        Coord tmpCoord;

                        m_MeshUI.GetWorldCoord(point.x, point.y, tmpCoord);
                }

                Invalidate();
        }

        CView::OnMouseMove(nFlags, point);
}
void CMeshModelFrameWorkView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
        switch(nChar)
        {
        case 27:
                exit(0); break;
        case 'd':
        case 'D':
                m_MEConner.setDrawMode(Global::DRAWSADDLEPOINTS);
                break;
        case 'p':
        case 'P':
                m_MEConner.setDrawMode(Global::DRAWSADDLEPOINTS);
                break;
        case 'b':
        case 'B':
                m_MEConner.setDrawMode(Global::DRAWCONNECTLINES);
                m_MEConner.setDrawMode(Global::DRAWBADCONNLINES);
                break;
        case 's':
        case 'S':
                m_MEConner.setDrawMode(Global::DRAWCONNECTLINES);
                m_MEConner.setDrawMode(Global::DRAWSTRAIGHTENLINES);
                break;
        case 'm':
        case 'M':
                m_MEConner.setDrawMode(Global::DRAWMAXMINPOINTS);
                break;
        default:
                break;
        }

        Invalidate();
}
/////////////////////////////////////////////////////////////////////////////
// CMeshModelFrameWorkView public methods.
/////////////////////////////////////////////////////////////////////////////

void CMeshModelFrameWorkView::DrawScene()
{
        m_OpenGL.OnBeginPaint();

        // Set Model/View Matrix
        m_MeshUI.SetViewMatrix();

        m_OpenGL.DetectOpenGLError();
        mesh.DrawModel();
        m_OpenGL.DetectOpenGLError();    

        m_MEConner.Draw();
        // Reset (Pop) Model/View Matrix
        m_MeshUI.ResetViewMatrix();

        m_OpenGL.OnEndPaint();

        m_MeshUI.SelectionRender();
}

void CMeshModelFrameWorkView::OnFileOpenModel()
{
        // TODO: Add your command handler code here
        CFileDialog dlg(TRUE, _T("tm"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                _T("Wavefront Files (*.obj)|*.obj|TM Files (*.tm)|*.tm|Ply2 Files (*.ply2)|*.ply2|OFF Files (*.off)|*.off||"));
        dlg.m_ofn.lpstrTitle = _T("Load Model");

        if(dlg.DoModal() == IDOK)
        {
                ClearModel();

                CString filename = dlg.GetPathName();
                string fn((const char*) filename.GetBuffer(1024));
                mesh.AttachModel(fn);

                m_MeshUI.Attach(&mesh, &m_OpenGL);
                
                m_MEConner.Attach(&mesh);

                printf("Attach over!\n");
                
                Invalidate();
        }
}

void CMeshModelFrameWorkView::OnFileSaveModel()
{
        // TODO: Add your command handler code here
        CFileDialog dlg(FALSE, _T("obj"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                _T("Wavefront Files (*.obj)|*.obj|TM Files (*.tm)|*.tm|Ply2 Files (*.ply2)|*.ply2|OFF Files (*.off)|*.off||"));
        dlg.m_ofn.lpstrTitle = _T("Store Model");

        if(dlg.DoModal() == IDOK)
        {
                CString filename = dlg.GetPathName();
                string fn((const char*) filename.GetBuffer(1024));
                mesh.StoreModel(fn);
        }
}
void CMeshModelFrameWorkView::OnFileSaveAsBmp()
{
        // TODO: Add your command handler code here
        CFileDialog dlg(FALSE, _T(".bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("BMP files(*.bmp)|*.bmp||")); 
        dlg.m_ofn.lpstrTitle = _T("Save as BMP");

        if(dlg.DoModal() == IDOK)
        {
                DrawScene();

                CString filename;

                filename = dlg.GetPathName();
                SaveAsBMP(filename.GetBuffer(1024));
        }
}
void CMeshModelFrameWorkView::OnLoadMeshEigenField()
{
        CFileDialog dlg(TRUE, _T("seigen"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                _T("SEIGEN Files (*.seigen)|*.seigen"));
        dlg.m_ofn.lpstrTitle = _T("Load Mesh eigenvector");

        if(dlg.DoModal() == IDOK)
        {
                CString filename = dlg.GetPathName();
                string fn((const char*) filename.GetBuffer(1024));
                m_MEConner.LoadEigenField(fn);
                Invalidate();
        //      UpdateWindow();
        }
}
void CMeshModelFrameWorkView::OnSaveMeshConnLine()
{
        CFileDialog dlg(FALSE, _T("quad"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                _T("QUAD Files (*.quad)"));
        dlg.m_ofn.lpstrTitle = _T("Store Mesh Quad");

        if(dlg.DoModal() == IDOK)
        {
                CString filename = dlg.GetPathName();
                string fn((const char*) filename.GetBuffer(1024));

                m_MEConner.SaveMEConnLine(fn);
        }
}
void CMeshModelFrameWorkView::OnConnMeshLine()
{
        m_MEConner.setDrawMode(Global::DRAWCONNECTLINES);
//      m_MEConner.Draw();
        Invalidate(TRUE);
        UpdateWindow();
}
//////////////////////////////////////////////////////////////////////////
// Manipulate functions

void CMeshModelFrameWorkView::OnManipulatorMove() 
{
        // TODO: Add your command handler code here
        m_MeshUI.MouseAct() = MOUSE_TYPE_MOVE;
}

void CMeshModelFrameWorkView::OnUpdateManipulatorMove(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(m_MeshUI.MouseAct() == MOUSE_TYPE_MOVE);
}

void CMeshModelFrameWorkView::OnManipulatorSpin() 
{
        // TODO: Add your command handler code here
        m_MeshUI.MouseAct() = MOUSE_TYPE_SPIN;
}

void CMeshModelFrameWorkView::OnUpdateManipulatorSpin(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(m_MeshUI.MouseAct() == MOUSE_TYPE_SPIN);
}

void CMeshModelFrameWorkView::OnManipulatorViewall() 
{
        // TODO: Add your command handler code here
        Coord objectCenter;
        double radius;

        mesh.m_Kernel.GetModelInfo().GetBoundingSphere(objectCenter, radius);
        m_OpenGL.SetObjectInfo(objectCenter[0], objectCenter[1], objectCenter[2], radius);

        CRect rect;
        GetClientRect(&rect);
        m_OpenGL.OnResize(rect.Width(), rect.Height());

        Invalidate();
}

void CMeshModelFrameWorkView::OnLoadViewmatrix() 
{
        // TODO: Add your command handler code here
        CFileDialog dlg(TRUE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "TXTÎÄ¼þ(*.txt)|*.txt||"); 
        dlg.m_ofn.lpstrTitle = "Load View Matrix" ;

        if(dlg.DoModal() == IDOK)
        {
                CString filename;

                filename = dlg.GetPathName();
                string fn((const char*) filename.GetBuffer(1024));
                m_MeshUI.LoadViewMatrix(fn);

                Invalidate();
        }
}

void CMeshModelFrameWorkView::OnSaveViewmatrix() 
{
        // TODO: Add your command handler code here
        CFileDialog dlg(FALSE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "TXTÎÄ¼þ(*.txt)|*.txt||"); 
        dlg.m_ofn.lpstrTitle = "Save View Matrix" ;

        if(dlg.DoModal() == IDOK)
        {
                CString filename;

                filename = dlg.GetPathName();
                string fn((const char*) filename.GetBuffer(1024));
                m_MeshUI.SaveViewMatrix(fn);
        }
}
void CMeshModelFrameWorkView::OnRemoveChainedCpNoise()
{

        double lowB, upB;
        LPCSTR stLowB, stUpB;
        DSetThreshold *pDiaSetThreshold = new DSetThreshold;
        pair <double,double> curThreshold = m_MEConner.getThreshold();

        if(pDiaSetThreshold->DoModal()==IDOK)
        {
                stLowB = pDiaSetThreshold->GetLowBound();
                lowB = (double)atof(stLowB);
                stUpB =pDiaSetThreshold->GetUpBound();
                upB = (double)atof(stUpB);
                m_MEConner.setThreshold(lowB,upB);
        }
        m_MEConner.RemoveNoise();
        
        Invalidate(TRUE);
        UpdateWindow();
}
void CMeshModelFrameWorkView::OnUpdateRemoveChainedCpNoise(CCmdUI* pCmdUI)
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(m_MEConner.IsRemoveNoise());
}
//////////////////////////////////////////////////////////////////////////
// Rendering functions

void CMeshModelFrameWorkView::OnRenderingSolidFlat() 
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_SOLID_FLAT;

        Invalidate();
}

void CMeshModelFrameWorkView::OnUpdateRenderingSolidFlat(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_SOLID_FLAT);
}

void CMeshModelFrameWorkView::OnRenderingSolidSmooth() 
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_SOLID_SMOOTH;

        Invalidate();
}

void CMeshModelFrameWorkView::OnUpdateRenderingSolidSmooth(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_SOLID_SMOOTH);
}

void CMeshModelFrameWorkView::OnRenderingSolidWireframe() 
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_SOLID_AND_WIREFRAME;
        Invalidate();
}

void CMeshModelFrameWorkView::OnUpdateRenderingSolidWireframe(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_SOLID_AND_WIREFRAME);
}

void CMeshModelFrameWorkView::OnRenderingVertices() 
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_VERTICES;

        Invalidate();
}

void CMeshModelFrameWorkView::OnUpdateRenderingVertices(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_VERTICES);
}

void CMeshModelFrameWorkView::OnRenderingWireframe() 
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_WIREFRAME;

        Invalidate();
}

void CMeshModelFrameWorkView::OnUpdateRenderingWireframe(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_WIREFRAME);
}

void CMeshModelFrameWorkView::OnRenderingTransparent() 
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_TRANSPARENT;

        Invalidate();
}
void CMeshModelFrameWorkView::OnUpdateRenderingTransparent(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_TRANSPARENT);
}
void CMeshModelFrameWorkView::OnRenderingVertexColor()
{
        // TODO: Add your command handler code here
        mesh.m_Render.Mode() = RENDER_MODEL_VERTEX_COLOR;

        Invalidate();
}
void CMeshModelFrameWorkView::OnUpdateRenderingVertexColor(CCmdUI *pCmdUI)
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(mesh.m_Render.Mode() == RENDER_MODEL_VERTEX_COLOR);
}
void CMeshModelFrameWorkView::OnRenderingBoudingbox() 
{
        // TODO: Add your command handler code here
        util.ToggleFlag(mesh.m_Render.State(), RENDER_MODEL_BOUNDING_BOX);

        Invalidate();
}

void CMeshModelFrameWorkView::OnUpdateRenderingBoudingbox(CCmdUI* pCmdUI) 
{
        // TODO: Add your command update UI handler code here
        pCmdUI->SetCheck(util.IsSetFlag(mesh.m_Render.State(), RENDER_MODEL_BOUNDING_BOX));
}

/////////////////////////////////////////////////////////////////////////////
// CMeshModelFrameWorkView help methods

BOOL CMeshModelFrameWorkView::SaveAsBMP(char *bmpFileName)
{
        GLubyte  *lpBits;
        RECT  rect;

        GetClientRect(&rect);

        CDC *pDC = GetDC();
        ASSERT(pDC->m_hDC);

        // Creating Compatible Memory Device Context
        CDC *pMemDC = new CDC;
        if (!pMemDC->CreateCompatibleDC(pDC))
        {
                MessageBox("CompatibleDC Error");
                delete pMemDC;
                ReleaseDC(pDC);
                return 0;
        }

        // Preparing bitmap header for DIB section
        BITMAPINFO bi;
        int bpp, sx, sy;
        ZeroMemory(&bi, sizeof(BITMAPINFO));
        sx = (rect.right-rect.left);
        sy = (rect.bottom-rect.top);
        bpp = 24;
        unsigned int widthByte32 = WidthByte32(sx,bpp);
        long dwSizeImage = widthByte32 * sy; // 8 means 8 bit/byte
        SetupBmpHeader(&(bi.bmiHeader), sx, sy, bpp);

        // Creating a DIB surface
        HBITMAP hBm, hBmOld;
        hBm = CreateDIBSection(pDC->GetSafeHdc(), &bi, DIB_RGB_COLORS, 
                (void **)&lpBits, NULL, (DWORD)0);
        if (!hBm)
        {
                MessageBox("CreateDIBSection Error");
                delete pMemDC;
                ReleaseDC(pDC);
                return 0;
        }

        // Selecting the DIB Surface
        hBmOld = (HBITMAP)::SelectObject(pMemDC->GetSafeHdc(), hBm);
        if (!hBmOld)
        {
                MessageBox("Select Object Error");
                delete pMemDC;
                ReleaseDC(pDC);
                return 0;
        }

        m_OpenGL.m_Context.glEnter();

        glReadBuffer(GL_BACK);
        glReadPixels(rect.left, rect.top, sx, sy, GL_BGR_EXT, GL_UNSIGNED_BYTE, lpBits);

        m_OpenGL.m_Context.glLeave();

        // Preparing BMP file header information
        BITMAPFILEHEADER bmfh;
        bmfh.bfType = 0x4d42;  // 'BM'
        long nSize =  sizeof(BITMAPINFOHEADER) +  dwSizeImage;
        bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER);
        bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
        bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);   

        CFile *pbmpFile;
        CFileException fileException;

        pbmpFile = new CFile;
        if (!pbmpFile->Open(bmpFileName , 
                CFile::modeCreate | CFile::modeWrite ) )
        {
                TRACE( "Unable to open file: %s\n, error = %u\n",
                        "" ,fileException.m_cause);
        }

        // Wrinting the DIB image
        TRY 
        {
                pbmpFile->Write(&bmfh, sizeof(BITMAPFILEHEADER));
                pbmpFile->Write(&(bi.bmiHeader), sizeof(BITMAPINFOHEADER));
                pbmpFile->Write(lpBits,dwSizeImage);
                pbmpFile->Flush();
        }
        CATCH (CException, e) 
        {
                AfxMessageBox("write error");
                delete pbmpFile;
                delete pMemDC;
                ReleaseDC(pDC);
                return 0;
        }
        END_CATCH

                // Cleaning
                pbmpFile->Close();
        delete pbmpFile;

        hBm = (HBITMAP)::SelectObject(pMemDC->GetSafeHdc(), hBmOld);
        //      ::SelectObject(pMemDC->GetSafeHdc(), hBmOld);
        DeleteObject(hBm);

        delete pMemDC;
        ReleaseDC(pDC);

        return 1;
}

void CMeshModelFrameWorkView::SetupBmpHeader(BITMAPINFOHEADER *pbmih, int sx, int sy, int bpp)
{
        pbmih->biSize = sizeof(BITMAPINFOHEADER);
        pbmih->biWidth = sx;
        pbmih->biHeight = sy;
        pbmih->biPlanes = 1;
        pbmih->biBitCount = bpp;
        pbmih->biCompression = BI_RGB;
        pbmih->biSizeImage = sx * sy * (bpp/8);
        pbmih->biXPelsPerMeter = 2925;
        pbmih->biYPelsPerMeter = 2925;
        pbmih->biClrUsed = 0;
        pbmih->biClrImportant = 0;
}

unsigned int CMeshModelFrameWorkView::WidthByte32(unsigned int width,
                                                                                                  unsigned int depth)
{
        // 32 bits alignment (4 bytes)
        int rest=(width*depth/8)%4;
        if(rest != 0)
                return (width*depth/8 + 4-rest);
        else
                return (width*depth/8);
}
void CMeshModelFrameWorkView::ClearModel()
{
        mesh.ClearData();
        m_MeshUI.ClearData();
}

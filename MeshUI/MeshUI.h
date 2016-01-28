// MeshUI.h: interface for the CMeshUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHUI_H__224690EF_7F9E_4686_9E0A_E60A026F1BD9__INCLUDED_)
#define AFX_MESHUI_H__224690EF_7F9E_4686_9E0A_E60A026F1BD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "Numerical\Matrix.h"
#include "Numerical\Trackball.h"
#include "MeshLib\MeshModel.h"
#include "MeshLib\Utility.h"
#include "OpenGL\GLContext.h"
#include "OpenGL\GLElement.h"



/* ================== Macros ================== */

// Selection macros
#define GIZMO_NUMBER            256
#define SELECTION_TYPE_BACKGROUND   0X00000001
#define SELECTION_TYPE_GIZMO        0X00000002
#define SELECTION_TYPE_FOREGROUND   0X00000003

// Mouse macros
#define MOUSE_TYPE_MOVE             0X00000001
#define MOUSE_TYPE_SPIN             0X00000002
#define MOUSE_TYPE_ZOOM             0X00000003

#define MOUSE_ZOOM_RATIO_LARGE      0.1
#define MOUSE_ZOOM_RATIO_MIDDLE     0.01
#define MOUSE_ZOOM_RATIO_SMALL      0.001

#define MOUSE_TYPE_TRANSLATE        0X00000010
#define MOUSE_TYPE_ROTATE           0X00000020
#define MOUSE_TYPE_SCALE            0X00000030
#define MOUSE_TYPE_SET_AXIS         0X00000040
#define MOUSE_TYPE_RIGID_EDIT       0X00000050

#define MOUSE_TYPE_SELECT_LASSO     0X00000100
#define MOUSE_TYPE_SELECT_PAINT     0X00000200
#define MOUSE_TYPE_SELECT_FILL      0X00000300

#define SELECT_MODE_VERTEX          0X00000000
#define SELECT_MODE_FACE            0X00000001
#define SELECT_MODE_OBJECT          0X00000002
#define SELECT_MODE_BOUNDARY        0X00000003
#define SELECT_MODE_COMPONENT       0X00000004

//++++++Added by Wangjun++++++//
#define MOUSE_TYPE_BEND_SRCCRV          0X00001000
#define MOUSE_TYPE_BEND_TARCRV          0X00002000
//============================//

//++++++Added by Zmy++++++//
#define MOUSE_TYPE_SET_MESH_VECTOR              0X00001001
//============================//


class CMeshUI  
{
private:
    MeshModel*   mesh;
    COpenGL*     opengl;
    Utility      util;
    
    CMatrix      m_ViewMtx;     // Model/View matrix
    int          m_MouseAct;    // Mouse action (Move, Spin, ...)
    int          m_SelectMode;  // Selection mode (Vertex, Face, ...)

    CPoint       m_OldPoint;    // Previous cursor position
    CPoint       m_StartPoint;  // LButton down cursor position
    Trackball    m_Trackball;   // Track ball structure

    // Global selection buffers
    IndexArray      m_SelectedVtx;      // Total selected vertices
    IndexArray      m_SelectedFace;     // Total selected faces
    PolyIndexArray  m_SelectedVtxArr;   // Every step selected vertices
    PolyIndexArray  m_SelectedFaceArr;  // Every step selected faces
    
    // Lasso buffers
    IndexArray      m_LassoKeyVtx;      // Lasso key vertices
    PolyIndexArray  m_LassoPath;        // Lasso paths
    IndexArray      m_LassoVtxLoop;     // Lasso loop

    // Fill buffers
    IndexArray      m_FillVtx;          // Fill selected vertices
    IndexArray      m_FillFace;         // Fill selected faces

    // Paint buffers
    IndexArray      m_PaintVtx;         // Paint selected vertices
    IndexArray      m_PaintFace;        // Paint selected faces

    // Sketch buffer
    CoordArray      m_SrcKeyPt;
    CoordArray      m_TarKeyPt;

public:
    bool    m_bOnElement;

    double          m_PaintRadius;      // Paint brush radius

    // Transform parameters
    Coord           m_Translation;
    Coord           m_RotCenter;
    Coord           m_RotAxis;
    double          m_RotAngle;
    Coord           m_Scale;

public:
        CMeshUI();
        virtual ~CMeshUI();
    
    void ClearData();
    void Attach(MeshModel* pModel, COpenGL* pOpenGL); // Attach pointers of environment support

    int& MouseAct() { return m_MouseAct; }
    int& SelectMode() { return m_SelectMode; }

    // View matrix manipulators
    void SetViewMatrix();   // Set current OpenGL model/view matrix (glMultMatrixd, glPushMatrix)
    void ResetViewMatrix(); // Reset current OpenGL model/view matrix (glPopMatrix)
    void ResetModel();      // Reset current OpenGL model/view matrix
    
        //
        void SaveViewMatrix(string filename);
        void LoadViewMatrix(string filename);

    // Utility
    void Project(Coord pos, Coord& eye);    // From world coord to eye coord
    void UnProject(Coord eye, Coord& pos);  // From eye coord to world coord

    void GetDepth(int x, int y, float& depth);      // Get the depth at the cursor position (x, y)
    void GetWorldCoord(int x, int y, Coord& pos);   // Get the world coord at the cursor position (x, y)
    void GetSelectInfo(int x, int y, int& index, int& type);  // Get the selection information at the cursor position (x, y)
    
    void Color2Index(GLubyte color[], int& index, int& type);   // Translate color into index & type
    void Index2Color(int index, int type, GLubyte color[]);     // Translate index & type into color

    // Message handlers
    bool OnLButtonDown(UINT nFlags, CPoint point);
    bool OnLButtonUp(UINT nFlags, CPoint point);
    bool OnRButtonDown(UINT nFlags, CPoint point);
    bool OnRButtonUp(UINT nFlags, CPoint point);
    bool OnMouseMove(UINT nFlags, CPoint point);
    bool OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    // Rendering for selection
    void SelectionRender();
    void SelectionRenderObject();
    void SelectionRenderVertex();
    void SelectionRenderFace();

    // Rendering for clipping
    void RenderObjectDepth();

    // Selection adjustment functions
    void ExpandSelected();
    void ShrinkSelected();
    void UndoSelected();
    void ClearSelected();
    bool CanUndo();
    bool CanClear();

    void ClearLassoData();
    void ClearFillData();
    void ClearPaintData();
    void AddSelected(IndexArray& Selected);

    //++++++Added by Wangjun++++++//
    void ClearSketchData();
    //============================//

    void JudgeSelection(int x, int y);
    void GetNearestVertex(FaceID fID, int x, int y, VertexID& vID);

    // Debug
    void Test();

    // Visualization
    void AddPoint(VertexID vID, Color color);
    void AddPath(IndexArray& Path, Color color);
    void DrawSelected();

    // Selection output
    bool IsVtxSelected();
    bool IsFaceSelected();
    IndexArray GetSelectedVtx();
    IndexArray GetSelectedFace();
    IndexArray GetLastSelectedVtx();

    // Transform functions
    bool IsTransform();

    // I/O functions
    bool StoreSelection(string filename, bool bVtx = true);
    bool LoadSelection(string filename);

};

#endif // !defined(AFX_MESHUI_H__224690EF_7F9E_4686_9E0A_E60A026F1BD9__INCLUDED_)
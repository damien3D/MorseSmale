// MeshUI.cpp: implementation of the CMeshUI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshUI.h"
#include <iostream>
#include <fstream>
#include ".\Numerical\Heap.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshUI::CMeshUI()
{
    ClearData();
}

CMeshUI::~CMeshUI()
{

}

void CMeshUI::ClearData()
{
    ResetModel();

    mesh = NULL;
    opengl = NULL;

    m_MouseAct = MOUSE_TYPE_SPIN;    // MOUSE_TYPE_SELECT_PAINT;
    m_SelectMode = SELECT_MODE_VERTEX;

    m_PaintRadius = 3.0;
    m_bOnElement = false;

    Utility util;
    util.FreeVector(m_SelectedVtx);
    util.FreeVector(m_SelectedVtxArr);
    util.FreeVector(m_SelectedFace);
    util.FreeVector(m_SelectedFaceArr);

    util.FreeVector(m_LassoKeyVtx);
    util.FreeVector(m_LassoPath);
    util.FreeVector(m_LassoVtxLoop);

    util.FreeVector(m_FillVtx);
    util.FreeVector(m_FillFace);

    util.FreeVector(m_PaintVtx);
    util.FreeVector(m_PaintFace);

    //++++++Added by Wangjun++++++//
    util.FreeVector(m_SrcKeyPt);
    util.FreeVector(m_TarKeyPt);
    //============================//
}

void CMeshUI::Attach(MeshModel* pModel, COpenGL* pOpenGL)
{
    ASSERT(pModel != NULL && pOpenGL != NULL);
    mesh = pModel;
    opengl = pOpenGL;
}

/* ================== View Matrix Manipulator ================== */
void CMeshUI::SetViewMatrix()
{
        glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
        glLoadIdentity();
    
    double TempMatrix[16];
        m_ViewMtx.Matrix2Array(TempMatrix);
        glMultMatrixd(TempMatrix);
}

void CMeshUI::ResetViewMatrix()
{
        glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
}

void CMeshUI::ResetModel()
{
    m_ViewMtx.MakeUnitMatrix(4);
    m_ViewMtx.SetTrans2Matrix(Coord(0, 0, -3.0));
}
void CMeshUI::SaveViewMatrix(string filename)
{
        m_ViewMtx.Write(filename);
}
void CMeshUI::LoadViewMatrix(string filename)
{
        m_ViewMtx.Read(filename);
        SetViewMatrix();
}
// Input: World Position
// Output: Corresponding Eye Coordinate
void CMeshUI::Project(Coord pos, Coord& eye)
{       
    opengl->m_Context.glEnter();

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport

        GLdouble modelview[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //Retrieve the matrix

        GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        gluProject(pos[0], pos[1], pos[2], modelview, projection, viewport, &(eye[0]), &(eye[1]), &(eye[2]));
        
        opengl->m_Context.glLeave();
}

// Input: Eye Coordinate
// Output: Corresponding World Coordinate
void CMeshUI::UnProject(Coord eye, Coord& pos)
{
        opengl->m_Context.glEnter();

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport

        GLdouble modelview[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //Retrieve the matrix

        GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        gluUnProject(eye[0], eye[1], eye[2], modelview, projection, viewport, &(pos[0]), &(pos[1]), &(pos[2]));
        
        opengl->m_Context.glLeave();
}

// Get the depth at the cursor position (x, y)
void CMeshUI::GetDepth(int x, int y, float& depth)
{
        opengl->m_Context.glEnter();

        GLint viewport[4]; 
        glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport

        glReadPixels(x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        opengl->m_Context.glLeave();
}

// Get the world coord at the cursor position (x, y)
void CMeshUI::GetWorldCoord(int x, int y, Coord& pos)
{
        opengl->m_Context.glEnter();

        GLint viewport[4]; 
        glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport

        GLdouble modelview[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //Retrieve the matrix

        GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        float winX, winY, winZ; //Window Coordinate     
        winX = (float)x; //Store the x coord
        winY = (float)(viewport[3] - y); //From Windows Coord System to OpenGL Coord System

        glReadPixels((GLint) winX, (GLint) winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        int  ret_val = gluUnProject(winX, winY, winZ, modelview, projection, viewport, &(pos[0]), &(pos[1]), &(pos[2]));
    ASSERT(ret_val == GL_TRUE);

        opengl->m_Context.glLeave();
}

// Get the selection information at the cursor position (x, y)
void CMeshUI::GetSelectInfo(int x, int y, int& index, int& type)
{
        opengl->m_Context.glEnter();
        
        GLint viewport[4]; //Where the values will be stored
        glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport
    
    if(x <= viewport[0] || y <= viewport[1] || x >= viewport[2] || y >= viewport[3])    // Invalid region
    {
        type = SELECTION_TYPE_BACKGROUND;
        return;
    }

        int winX = x;
        int winY = viewport[3] - y; //From Windows Coord System to OpenGL Coord System

        GLubyte color[4];
        glReadBuffer(GL_BACK);
        glReadPixels(winX, winY, 1, 1, GL_RGBA , GL_UNSIGNED_BYTE, &color);

        Color2Index(color, index, type);

    // Overload the original implementation
    if(SelectMode() == SELECT_MODE_VERTEX && type == SELECTION_TYPE_FOREGROUND)   // Select a face
    {
        FaceID fID = index;
        ASSERT(mesh->m_BasicOp.IsValidFaceIndex(fID));
        Coord pos;
        GetWorldCoord(x, y, pos);
        VertexID vID;
        mesh->m_BasicOp.GetNearestVertex(fID, pos, vID);
        index = vID;
//        VertexID vID;
//        GetNearestVertex(fID, winX, winY, vID);
//        index = vID;    // Assign the nearest vertex index
    }
        
        opengl->m_Context.glLeave();
//      fprintf(stdout, "Point(%d,%d), fID=%d, Color(%d,%d,%d,%d)\n",
//              point.x, point.y, FaceIndex, color[0], color[1], color[2], color[3]);
}

// Translate color into index & type
void CMeshUI::Color2Index(GLubyte color[], int& index, int& type)
{
    unsigned int idx = RGB(color[0],color[1],color[2]);
    if(idx == 0)    // Background
    {
        index = -1;
        type = SELECTION_TYPE_BACKGROUND;
    }
    else if(idx < GIZMO_NUMBER)  // Gizmo
    {
        index = idx;
        type = SELECTION_TYPE_GIZMO;
    }
    else    // Foreground
    {
        index = idx-GIZMO_NUMBER;
        type = SELECTION_TYPE_FOREGROUND;
    }
}

// Translate index & type into color
void CMeshUI::Index2Color(int index, int type, GLubyte color[])
{
    switch(type)
    {
    case SELECTION_TYPE_BACKGROUND:
        color[0] = color[1] = color[2] = color[3] = 0;
        break;
    case SELECTION_TYPE_GIZMO:
                color[0] = (GLubyte)index;
                color[1] = (GLubyte)(index >> 8);
                color[2] = (GLubyte)(index >> 16);
                color[3] = 0;
        break;
    case SELECTION_TYPE_FOREGROUND:
        index += GIZMO_NUMBER;
                color[0] = (GLubyte)index;
                color[1] = (GLubyte)(index >> 8);
                color[2] = (GLubyte)(index >> 16);
                color[3] = 0;
        break;
    default:
        break;
    }
}

// Rendering for selection
void CMeshUI::SelectionRender()
{
    if(mesh == NULL)
        return;

    if (MouseAct() != MOUSE_TYPE_SELECT_LASSO && MouseAct() != MOUSE_TYPE_SELECT_FILL && MouseAct() != MOUSE_TYPE_SELECT_PAINT
        && MouseAct() != MOUSE_TYPE_BEND_SRCCRV && MouseAct() != MOUSE_TYPE_BEND_TARCRV) //++++++Added by Wangjun++++++//
        return;
    
    // Enter OpenGL Rendering Context
    opengl->m_Context.glEnter();

    glDrawBuffer(GL_BACK);      // Set back buffer

    // Clear contents
    GLubyte color[4];
    Index2Color(0, SELECTION_TYPE_BACKGROUND, color);
    glClearColor(color[0], color[1], color[2], color[3]);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set view/model transformation
    SetViewMatrix();

        // Get Information
        GLboolean bEnableBlending = glIsEnabled(GL_BLEND);
        GLboolean bEnableLighting = glIsEnabled(GL_LIGHTING);
        GLboolean bEnableTexturing = glIsEnabled(GL_TEXTURE_2D);
        GLboolean bEnablePointSmooth = glIsEnabled(GL_POINT_SMOOTH);

        // Disable if needed
        if(bEnableBlending)
                glDisable(GL_BLEND);
        if(bEnableLighting)
                glDisable(GL_LIGHTING);
        if(bEnableTexturing)
                glDisable(GL_TEXTURE_2D);
        if(bEnablePointSmooth)
                glDisable(GL_POINT_SMOOTH);

        // Draw Model
    switch(m_SelectMode) {
    case SELECT_MODE_VERTEX:    // Overload the original implementation for better user interaction
//        RenderObjectDepth();        // Clipping back faces
//        SelectionRenderVertex();    // Render vertex index
//      break;
    case SELECT_MODE_FACE:
        RenderObjectDepth();        // Clipping back faces
        SelectionRenderFace();    // Render vertex index
        break;
    case SELECT_MODE_OBJECT:
        break;
    case SELECT_MODE_BOUNDARY:
        break;
    case SELECT_MODE_COMPONENT:
        break;
    default:
        break;
    }

        // Enable if needed
        if(bEnableBlending)
                glEnable(GL_BLEND);
        if(bEnableLighting)
                glEnable(GL_LIGHTING);
        if(bEnableTexturing)
                glEnable(GL_TEXTURE_2D);
        if(bEnablePointSmooth)
                glEnable(GL_POINT_SMOOTH);

    ResetViewMatrix();

    glFinish();
    
    // Leave OpenGL Rendering Context
    opengl->m_Context.glLeave();
}

void CMeshUI::SelectionRenderVertex()
{
    glDepthFunc(GL_LEQUAL);

    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    int nVertex = (int) vCoord.size();

        GLubyte color[4];
    glPointSize(6.0f);
    
    glBegin(GL_POINTS);
    for(int i = 0; i < nVertex; ++ i)
    {
        Coord& v = vCoord[i];
        Index2Color(i, SELECTION_TYPE_FOREGROUND, color);
            glColor4ubv(color);
        glVertex3d(v[0], v[1], v[2]);
    }
        glEnd();

    glDepthFunc(GL_LESS);
}

// Rendering object depth for clipping
void CMeshUI::RenderObjectDepth()
{
    glColorMask(false, false, false, false);

    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    NormalArray& vNormal = mesh->m_Kernel.GetVertexInfo().GetNormal();
    ColorArray& vColor = mesh->m_Kernel.GetVertexInfo().GetColor();
    PolyIndexArray& fIndex = mesh->m_Kernel.GetFaceInfo().GetIndex();

    size_t nFace = fIndex.size();
    size_t i, j, n;

    if(!mesh->m_Kernel.GetModelInfo().IsGeneralMesh())
    {
        if(mesh->m_Kernel.GetModelInfo().IsTriMesh())  // Triangle mesh
            glBegin(GL_TRIANGLES);
        else if(mesh->m_Kernel.GetModelInfo().IsQuadMesh())    // Quadangle mesh
            glBegin(GL_QUADS);
        else
            return;

        for(i = 0; i < nFace; ++ i)
        {
            IndexArray& face = fIndex[i];
            n = face.size();
            for(j = 0; j < n; ++ j)
            {
                VertexID& vID = face[j];
                Coord& v = vCoord[vID];
                glVertex3d(v[0], v[1], v[2]);
            }
        }
        glEnd();
    }

    else    // General polygonal mesh
    {
        for(i = 0; i < nFace; ++ i)
        {
            IndexArray& face = fIndex[i];
            n = face.size();
            glBegin(GL_POLYGON);
            for(j = 0; j < n; ++ j)
            {
                VertexID vID = face[j];
                Coord& v = vCoord[vID];
                glVertex3d(v[0], v[1], v[2]);
            }
            glEnd();
        }
    }

    glColorMask(true, true, true, true);
}

void CMeshUI::SelectionRenderFace()
{
    glDepthFunc(GL_LEQUAL);

    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    NormalArray& vNormal = mesh->m_Kernel.GetVertexInfo().GetNormal();
    ColorArray& vColor = mesh->m_Kernel.GetVertexInfo().GetColor();
    PolyIndexArray& fIndex = mesh->m_Kernel.GetFaceInfo().GetIndex();

    int nFace = (int) fIndex.size();
    int i, j, n;

        GLubyte color[4];
    if(!mesh->m_Kernel.GetModelInfo().IsGeneralMesh())
    {
        if(mesh->m_Kernel.GetModelInfo().IsTriMesh())  // Triangle mesh
            glBegin(GL_TRIANGLES);
        else if(mesh->m_Kernel.GetModelInfo().IsQuadMesh())    // Quadangle mesh
            glBegin(GL_QUADS);
        else
            return;

        for(i = 0; i < nFace; ++ i)
        {
            IndexArray& face = fIndex[i];
            n = (int) face.size();
            Index2Color(i, SELECTION_TYPE_FOREGROUND, color);
                glColor4ubv(color);

            for(j = 0; j < n; ++ j)
            {
                VertexID& vID = face[j];
                Coord& v = vCoord[vID];
                glVertex3d(v[0], v[1], v[2]);
            }
        }
        glEnd();
    }

    else    // General polygonal mesh
    {
        for(i = 0; i < nFace; ++ i)
        {
            IndexArray& face = fIndex[i];
            n = (int) face.size();
            Index2Color(i, SELECTION_TYPE_FOREGROUND, color);
                glColor4ubv(color);

            glBegin(GL_POLYGON);
            for(j = 0; j < n; ++ j)
            {
                VertexID vID = face[j];
                Coord& v = vCoord[vID];
                glVertex3d(v[0], v[1], v[2]);
            }
            glEnd();
        }
    }

    glDepthFunc(GL_LESS);
}

// Message handlers
bool CMeshUI::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(mesh == NULL)
        return false;

    bool ret_val = false;

    // Get model center and radius
    Coord Center;
    double radius;
    mesh->m_Kernel.GetModelInfo().GetBoundingSphere(Center, radius);

    switch(m_MouseAct) {
    case MOUSE_TYPE_MOVE:
        break;

    case MOUSE_TYPE_SPIN:
        {
            opengl->m_Context.glEnter();
            GLint viewport[4]; 
            glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport
            opengl->m_Context.glLeave();
            m_Trackball.SetTrackWindow(viewport[2], viewport[3]);
                        m_Trackball.Start(point.x, point.y);
        }
        break;

    case MOUSE_TYPE_ZOOM:
        break;

    case MOUSE_TYPE_SELECT_LASSO:
        {
            int index, type;
            GetSelectInfo(point.x, point.y, index, type);
            if(type != SELECTION_TYPE_FOREGROUND)
                break;

            if(m_LassoKeyVtx.size() == 0)   // First vertex
            {
                ClearLassoData();
                m_LassoKeyVtx.push_back(index);
            }
            else
            {
                m_LassoKeyVtx.push_back(index);
                IndexArray LassoPath;
                Index vID1 = m_LassoKeyVtx[m_LassoKeyVtx.size()-2];
                Index vID2 = index;
                mesh->m_BasicOp.GetShortestPath(vID1, vID2, LassoPath);
                m_LassoPath.push_back(LassoPath);
                
                // Highlight Lasso Path
                AddPath(LassoPath, GREEN);
            }

            // Highlight Lasso KeyVtx
            AddPoint(index, YELLOW);
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_SELECT_FILL:
        {
            int index, type;
            GetSelectInfo(point.x, point.y, index, type);
            if(type != SELECTION_TYPE_FOREGROUND)
                break;

            ClearFillData();
            if(SelectMode() == SELECT_MODE_VERTEX)
            {
                mesh->m_BasicOp.SurfaceFloodFillVertex(index, m_FillVtx, m_FillFace, m_SelectedVtx);
                AddSelected(m_FillVtx);     // Add selected
            }
            else if(SelectMode() == SELECT_MODE_FACE)    // Find the nearest vertex
            {
//                GetNearestVertex(index, point.x, point.y, index);
                mesh->m_BasicOp.SurfaceFloodFillFace(index, m_FillVtx, m_FillFace, m_SelectedFace);
                AddSelected(m_FillFace);    // Add selected
            }
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_SELECT_PAINT:
        {
            int index, type;
            GetSelectInfo(point.x, point.y, index, type);
            if(type != SELECTION_TYPE_FOREGROUND)
                break;

            if(m_PaintVtx.size() == 0)  // First paint
                ClearPaintData();
            if(SelectMode() == SELECT_MODE_FACE)    // Find the nearest vertex
                GetNearestVertex(index, point.x, point.y, index);

            mesh->m_BasicOp.GetNeighborhood(index, m_PaintVtx, m_PaintFace, m_PaintRadius);
        }
        ret_val = true;
        break;
        
    case MOUSE_TYPE_SET_AXIS:
        {
            Coord CenterAfter, CenterDir;
            Project(Center, CenterAfter);

            opengl->m_Context.glEnter();
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport); //Retrieves the viewport
            opengl->m_Context.glLeave();

            CenterAfter[0] = point.x;
            CenterAfter[1] = viewport[3] - point.y;
            UnProject(CenterAfter, m_RotCenter);

            CenterDir = CenterAfter+Coord(0.0, 0.0, 1.0);
            UnProject(CenterDir, m_RotAxis);
            m_RotAxis = (m_RotAxis-m_RotCenter).unit();
        }
        ret_val = true;
        break;

    //++++++Added by Wangjun++++++//
        case MOUSE_TYPE_BEND_SRCCRV:
        case MOUSE_TYPE_BEND_TARCRV:
        {
            CoordArray& KeyPt = (MouseAct() == MOUSE_TYPE_BEND_SRCCRV) ? m_SrcKeyPt : m_TarKeyPt;
            KeyPt.push_back(Coord(point.x, point.y, 0.0));
        }
                ret_val = true;
                break;
    //============================//

        //============================//
        case MOUSE_TYPE_SET_MESH_VECTOR:
                ret_val = true;
                break;
    }

    m_StartPoint = point;
    m_OldPoint = point;

    return ret_val;
}

bool CMeshUI::OnMouseMove(UINT nFlags, CPoint point)
{
    if(mesh == NULL)
        return false;

    if((nFlags&MK_LBUTTON) != MK_LBUTTON)   // LButton is not pressed (Not drag)
    {
        switch(m_MouseAct) {
        case MOUSE_TYPE_SELECT_LASSO:
        case MOUSE_TYPE_SELECT_FILL:
        case MOUSE_TYPE_SELECT_PAINT:
        case MOUSE_TYPE_BEND_SRCCRV:        // Source curve key points must be on the surface
            JudgeSelection(point.x, point.y);
            return true;
        default:
            return false;
        }
    }

    bool ret_val = false;

    // Get model center and radius
    Coord Center;
    double radius;
    mesh->m_Kernel.GetModelInfo().GetBoundingSphere(Center, radius);

    switch(m_MouseAct) {
    case MOUSE_TYPE_MOVE:
        {
                        double dx,dy,dz;
                        // Method thinking in Eye Coordinate System
                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
            
                        CenterAfter[0] += point.x-m_OldPoint.x;
                        CenterAfter[1] += m_OldPoint.y-point.y;
                        UnProject(CenterAfter, NewCenter);

                        Coord CenterView = Center;
                        Coord NewCenterView = NewCenter;
                        m_ViewMtx.TransformVector(CenterView);
                        m_ViewMtx.TransformVector(NewCenterView);

                        dx = NewCenterView[0] - CenterView[0];
                        dy = NewCenterView[1] - CenterView[1];
                        dz = NewCenterView[2] - CenterView[2];
//                      fprintf(stdout, "Translate(%f, %f, %f)\n", dx, dy, dz);

                        CMatrix TempMatrix(4);
                        TempMatrix.SetTrans2Matrix(Coord(dx, dy, dz));
                        m_ViewMtx = TempMatrix*m_ViewMtx;

            
                        // Method thinking in World Coordinate System
//                      Coord CenterAfter, NewCenter;
//                      Project(Center, CenterAfter);
//                      CenterAfter[0] += point.x-m_OldPoint.x;
//                      CenterAfter[1] += m_OldPoint.y-point.y;
//                      Unproject(CenterAfter, NewCenter);
//
//                      dx = NewCenter[0] - Center[0];
//                      dy = NewCenter[1] - Center[1];
//                      dz = NewCenter[2] - Center[2];
//                      fprintf(stdout, "Translate(%f, %f, %f)\n", dx, dy, dz);
//
//                      CMatrix TempMatrix(4);
//                      TempMatrix.SetTrans2Matrix(Coord(dx, dy, dz));
//                      m_ViewMtx = m_ViewMatrix*TempMtx;
            ret_val = true;
        }
        break;

    case MOUSE_TYPE_SPIN:
        {               
                        Coord pAxis;
                        float pAngle=0;
                        m_Trackball.Tracking((point.x-m_OldPoint.x), (point.y-m_OldPoint.y), &pAxis, &pAngle);

                        CMatrix PreTransMtx(4), PostTransMtx(4), RotMtx(4);
                                
            Coord CenterAfter = Center;
                        m_ViewMtx.TransformVector(CenterAfter);
                                
            PreTransMtx.SetTrans2Matrix(-CenterAfter);
                        PostTransMtx.SetTrans2Matrix(CenterAfter);

            // Constrain Rotation Axis
            if((nFlags&MK_CONTROL) && (nFlags&MK_SHIFT))        // Around Z Axis
            {
                if(pAxis[0] > 0.0)
                    pAxis.setCoords(0.0,0.0,1.0);
                else
                    pAxis.setCoords(0.0,0.0,-1.0);
            }
            else 
            {
                if(nFlags&MK_CONTROL)   // Around X Axis
                {
                    if(pAxis[0] > 0.0)
                        pAxis.setCoords(1.0,0.0,0.0);
                    else
                        pAxis.setCoords(-1.0,0.0,0.0);
                }
                else if(nFlags&MK_SHIFT)// Around Y Axis
                {
                    if(pAxis[0] > 0.0)
                        pAxis.setCoords(0.0,1.0,0.0);
                    else
                        pAxis.setCoords(0.0,-1.0,0.0);
                }
            }
                                
            RotMtx.SetRotation2Matrix(pAngle, pAxis);
                        m_ViewMtx = PostTransMtx*RotMtx*PreTransMtx*m_ViewMtx;
            ret_val = true;
        }        
        break;

    case MOUSE_TYPE_ZOOM:
        {
            double ratio;
            if(nFlags&MK_CONTROL)
                ratio = MOUSE_ZOOM_RATIO_LARGE;
            else if(nFlags&MK_SHIFT)
                ratio = MOUSE_ZOOM_RATIO_SMALL;
            else
                ratio = MOUSE_ZOOM_RATIO_MIDDLE;

            double scale = (point.y > m_OldPoint.y) ? (1.0+ratio) : (1.0-ratio);
            opengl->m_Projection.Zoom(scale);

//                      CMatrix PreTransMtx(4), PostTransMtx(4), ScaleMtx(4);
//
//            Coord CenterAfter = Center;
//                      m_ViewMtx.TransformVector(CenterAfter);
//
//            PreTransMtx.SetTrans2Matrix(-CenterAfter);
//                      PostTransMtx.SetTrans2Matrix(CenterAfter);
//            double scale = (point.y > m_OldPoint.y) ? (1.0+ratio) : (1.0-ratio);
//            
//            ScaleMtx.SetScale2Matrix(Coord(scale, scale, scale));
//            m_ViewMtx = PostTransMtx*ScaleMtx*PreTransMtx*m_ViewMtx;
            ret_val = true;
        }
        break;

    case MOUSE_TYPE_SELECT_PAINT:
        {
            int index, type;
            GetSelectInfo(point.x, point.y, index, type);
            if(type != SELECTION_TYPE_FOREGROUND)
                break;

            if(SelectMode() == SELECT_MODE_FACE)    // Find the nearest vertex
                GetNearestVertex(index, point.x, point.y, index);
            mesh->m_BasicOp.GetNeighborhood(index, m_PaintVtx, m_PaintFace, m_PaintRadius);
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_TRANSLATE:
    case MOUSE_TYPE_RIGID_EDIT:
        {
            // Method thinking in World Coordinate System
                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
                        CenterAfter[0] += point.x-m_OldPoint.x;
                        CenterAfter[1] += m_OldPoint.y-point.y;
                        UnProject(CenterAfter, NewCenter);

            m_Translation = NewCenter-Center;
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_ROTATE:
        {
                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
                        CenterAfter[0] += point.x-m_OldPoint.x;
                        CenterAfter[1] += m_OldPoint.y-point.y;
                        UnProject(CenterAfter, NewCenter);

            double a = RAD2DEG((NewCenter-Center).abs());
            if(point.y < m_OldPoint.y)
                a *= -1;

            m_RotAngle = a;
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_SCALE:
        {
            double ratio = 1.0;

                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
                        CenterAfter[0] += point.x-m_OldPoint.x;
                        CenterAfter[1] += m_OldPoint.y-point.y;
                        UnProject(CenterAfter, NewCenter);

            if(point.y < m_OldPoint.y)
                ratio *= -1;

            double s = (NewCenter-Center).abs()*ratio;
            if(s < -0.999999)
                s = -0.999999;
            m_Scale = Coord(s, s, s);
        }
        ret_val = true;
        break;

        case MOUSE_TYPE_SET_MESH_VECTOR:
                ret_val = true;
                break;
    }
    
    m_OldPoint = point;
    
    return ret_val;
}

bool CMeshUI::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(mesh == NULL)
        return false;

    // Get model center and radius
    Coord Center;
    double radius;
    mesh->m_Kernel.GetModelInfo().GetBoundingSphere(Center, radius);

    bool ret_val = false;

    switch(m_MouseAct) {
    case MOUSE_TYPE_SELECT_PAINT:
        {
            // Add selected
            if(SelectMode() == SELECT_MODE_VERTEX)
                AddSelected(m_PaintVtx);
            else if(SelectMode() == SELECT_MODE_FACE)
                AddSelected(m_PaintFace);
            
            ClearPaintData();
        }
        break;

    // ------ Added for Lazy Update Begin ------ //
    case MOUSE_TYPE_TRANSLATE:
    case MOUSE_TYPE_RIGID_EDIT:
        {
            // Method thinking in World Coordinate System
                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
                        CenterAfter[0] += point.x-m_StartPoint.x;   // Start point 
                        CenterAfter[1] += m_StartPoint.y-point.y;   // Start point 
                        UnProject(CenterAfter, NewCenter);

            m_Translation = NewCenter-Center;
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_ROTATE:
        {
                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
                        CenterAfter[0] += point.x-m_StartPoint.x;
                        CenterAfter[1] += m_StartPoint.y-point.y;
                        UnProject(CenterAfter, NewCenter);

            double a = RAD2DEG((NewCenter-Center).abs());
            if(point.y < m_StartPoint.y)
                a *= -1;

            m_RotAngle = a;
        }
        ret_val = true;
        break;

    case MOUSE_TYPE_SCALE:
        {
            double ratio = 1.0;

                        Coord CenterAfter, NewCenter;
                        Project(Center, CenterAfter);
                        CenterAfter[0] += point.x-m_StartPoint.x;
                        CenterAfter[1] += m_StartPoint.y-point.y;
                        UnProject(CenterAfter, NewCenter);

            if(point.y < m_StartPoint.y)
                ratio *= -1;

            double s = (NewCenter-Center).abs()*ratio;
            if(s < -0.999999)
                s = -0.999999;
            m_Scale = Coord(s, s, s);
        }
        ret_val = true;
        break;

    // ------ Added for Lazy Update End ------ //
        case MOUSE_TYPE_SET_MESH_VECTOR:
                ret_val = true;
                break;
    }

    return ret_val;
}

bool CMeshUI::OnRButtonDown(UINT nFlags, CPoint point)
{
    if(mesh == NULL)
        return false;

    bool ret_val = false;

    switch(m_MouseAct) {
    case MOUSE_TYPE_SELECT_LASSO:
        {
            if(m_LassoKeyVtx.size() > 1)   // At least two vertices, not empty
            {
                IndexArray LassoPath;
                VertexID vID1 = m_LassoKeyVtx[m_LassoKeyVtx.size()-1];
                VertexID vID2 = m_LassoKeyVtx[0];
                mesh->m_BasicOp.GetShortestPath(vID1, vID2, LassoPath);
                m_LassoPath.push_back(LassoPath);

                // Highlight Lasso Path
                AddPath(LassoPath, GREEN);

                // Create lasso loop
                size_t nPath = m_LassoPath.size();
                size_t i, j, n;
                for(i = 0; i < nPath; ++ i)
                {
                    n = m_LassoPath[i].size();
                    for(j = 0; j < n-1; ++ j)
                        m_LassoVtxLoop.push_back(m_LassoPath[i][j]);
                }

                // Add selected
                AddSelected(m_LassoVtxLoop);
            }
            
            ClearLassoData();
        }
        break;
        //++++++Added by Wangjun++++++//
        case MOUSE_TYPE_BEND_SRCCRV:
        case MOUSE_TYPE_BEND_TARCRV:
        ret_val = true;
                break;
        //============================//
    }

    return ret_val;
}

bool CMeshUI::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if(mesh == NULL)
        return false;

    bool ret_val = true;

    // Get model center and radius
    Coord Center;
    double radius;
    mesh->m_Kernel.GetModelInfo().GetBoundingSphere(Center, radius);

    double ratio;
    if(nFlags&MK_CONTROL)
        ratio = MOUSE_ZOOM_RATIO_LARGE;
    else if(nFlags&MK_SHIFT)
        ratio = MOUSE_ZOOM_RATIO_SMALL;
    else
        ratio = MOUSE_ZOOM_RATIO_MIDDLE;

    double scale = (zDelta > 0) ? (1.0+ratio) : (1.0-ratio);
    opengl->m_Projection.Zoom(scale);

//    CMatrix PreTransMtx(4), PostTransMtx(4), ScaleMtx(4);
//    
//    Coord CenterAfter = Center;
//    m_ViewMtx.TransformVector(CenterAfter);
//    
//    PreTransMtx.SetTrans2Matrix(-CenterAfter);
//    PostTransMtx.SetTrans2Matrix(CenterAfter);
//    double scale = (zDelta > 0) ? (1.0+ratio) : (1.0-ratio);
//    
//    ScaleMtx.SetScale2Matrix(Coord(scale, scale, scale));
//    m_ViewMtx = PostTransMtx*ScaleMtx*PreTransMtx*m_ViewMtx;

    return ret_val;
}

// Selection adjustment functions
void CMeshUI::ExpandSelected()
{
    // Using the paint radius as the expansion distance
    double distance = mesh->m_BasicOp.GetDistanceFactor() * m_PaintRadius;

    CoordArray vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    PolyIndexArray vAdjVertices = mesh->m_Kernel.GetVertexInfo().GetAdjVertices();
    size_t nVertex = vCoord.size();

    size_t i, n;
    if(SelectMode() == SELECT_MODE_VERTEX)
    {
        CHeap heap;
        BoolArray VtxVisited;
        DoubleArray m_VtxDist;
        VtxVisited.resize(nVertex, false);
        m_VtxDist.resize(nVertex, INFINITE_DISTANCE);

        n = m_SelectedVtx.size();
        for(i = 0; i < n; ++ i)
        {
            VertexID vID = m_SelectedVtx[i];
            VtxVisited[vID] = true;
            m_VtxDist[vID] = 0.0;
            Node* pNode = new Node;
            pNode->v = 0.0;
            pNode->type = vID;
            heap.insert(pNode);
        }

        // Gather all neighboring vertices
        IndexArray NeiVtx;
        while(!heap.heapEmpty())
        {
            Node* pNode = heap.Remove();
            VertexID vID = pNode->type;
            double v_dist = m_VtxDist[vID];
            Coord v = vCoord[vID];
            IndexArray& adjVertices = vAdjVertices[vID];
            n = adjVertices.size();
            for(i = 0; i < n; ++ i)
            {
                VertexID vtxID = adjVertices[i];
                double vtx_dist = m_VtxDist[vtxID];
                Coord vtx = vCoord[vtxID];
                double edge_length = (vtx-v).abs();
                if(v_dist+edge_length < vtx_dist)               // Update
                {
                    m_VtxDist[vtxID] = v_dist+edge_length;
                    int index = heap.heapFind(vtxID);
                    if(index)   // Already in heap
                    {
                        heap.a[index]->v = m_VtxDist[vtxID];
                        heap.upheap(index);
                    }
                    else if(m_VtxDist[vtxID] < distance)
                    {
                        Node* pNewNode = new Node;
                        pNewNode->v = m_VtxDist[vtxID];
                        pNewNode->type = vtxID;
                        heap.insert(pNewNode);
                    }
                }
            }
            delete pNode;
            
            // Add to neighboring vertex array
            if(!VtxVisited[vID])
            {
                NeiVtx.push_back(vID);
                VtxVisited[vID] = true;
            }
        }

        // Add to selected array
        if(NeiVtx.size())
            AddSelected(NeiVtx);
    }
    else if(SelectMode() == SELECT_MODE_FACE)
    {
    }
}

void CMeshUI::ShrinkSelected()
{
/*
    // Using the paint radius as the expansion distance
    double distance = mesh->m_BasicOp.GetDistanceFactor() * m_PaintRadius;

    CoordArray vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    PolyIndexArray vAdjVertices = mesh->m_Kernel.GetVertexInfo().GetAdjVertices();
    int nVertex = vCoord.size();

    int i, j, n, m;
    if(SelectMode() == SELECT_MODE_VERTEX)
    {
        BoolArray VtxVisited;
        VtxVisited.resize(nVertex, false);

        // Gathering the boundary vertices
        IndexArray BdyVtx;
        n = m_SelectedVtx.size();
        for(i = 0; i < n; ++ i)
        {
            VertexID vID = m_SelectedVtx[i];
            IndexArray& adjVertices = vAdjVertices[vID];
            VtxVisited[vID] = true;
            m = adjVertices.size();
            for(j = 0; j < m; ++ j)
            {
                VertexID vtxID = adjVertices[j];
                if(!VtxVisited[vtxID])
                {
                    BdyVtx.push_back(vtxID);
                    VtxVisited[vtxID] = true;
                }
            }
        }
    }
    else if(SelectMode() == SELECT_MODE_FACE)
    {
    }
*/
}

void CMeshUI::UndoSelected()
{
    if(MouseAct() == MOUSE_TYPE_SELECT_LASSO && m_LassoKeyVtx.size() > 0)   // Not finish a Lasso selection session
    {
        int i, n = (int) m_LassoKeyVtx.size();
        if(n == 1)  // Only the first key vertex
            ClearLassoData();
        else
        {
            // Clear highlighted Lasso path
            VertexID vID1 = m_LassoKeyVtx[m_LassoKeyVtx.size()-2];
            VertexID vID2 = m_LassoKeyVtx[m_LassoKeyVtx.size()-1];
            IndexArray LassoPath;
            mesh->m_BasicOp.GetShortestPath(vID1, vID2, LassoPath);
            n = (int) LassoPath.size();
            int nLine = (int) mesh->m_AuxData.GetLineInfo().GetfCoord().size();
            for(i = 0; i < n; ++ i)
                mesh->m_AuxData.GetLineInfo().Remove(nLine- 1- i);

            // Clear highlighted Lasso key vertex
            int nPoint = (int) mesh->m_AuxData.GetPointInfo().GetCoord().size();
            mesh->m_AuxData.GetPointInfo().Remove(nPoint - 1);

            // Clear Lasso data
            m_LassoKeyVtx.erase(m_LassoKeyVtx.begin()+(m_LassoKeyVtx.size()-1));
            m_LassoPath.erase(m_LassoPath.begin()+(m_LassoPath.size()-1));
        }
    }
    //++++++Added by Wangjun++++++//
    else if(MouseAct() == MOUSE_TYPE_BEND_SRCCRV || MouseAct() == MOUSE_TYPE_BEND_TARCRV)
    {
        CoordArray& KeyPt = (MouseAct() == MOUSE_TYPE_BEND_SRCCRV) ? m_SrcKeyPt : m_TarKeyPt;
        int n = (int) KeyPt.size();
        if(n > 0)
            KeyPt.erase(KeyPt.begin()+(n-1));
    }
    //============================//
    else
    {
        // Clear selected data
        PolyIndexArray& SelectedArr = (SelectMode() == SELECT_MODE_VERTEX) ? m_SelectedVtxArr : m_SelectedFaceArr;
        IndexArray& LastSelected = SelectedArr[SelectedArr.size()-1];
        IndexArray& Selected = (SelectMode() == SELECT_MODE_VERTEX) ? m_SelectedVtx : m_SelectedFace;
        size_t i, n = LastSelected.size();
        for(i = 0; i < n; ++ i)
        {
            IndexArray::iterator iter = find(Selected.begin(), Selected.end(), LastSelected[i]);
            if(iter != Selected.end())
                Selected.erase(iter);
        }
        SelectedArr.erase(SelectedArr.begin()+(SelectedArr.size()-1));
    }
}

void CMeshUI::ClearSelected()
{
    switch(SelectMode()) {
    case SELECT_MODE_VERTEX:
        m_SelectedVtx.clear();
        m_SelectedVtxArr.clear();
        break;
    case SELECT_MODE_FACE:
        //++++++Added by Wangjun++++++//
        if(MouseAct() == MOUSE_TYPE_BEND_SRCCRV || MouseAct() == MOUSE_TYPE_BEND_TARCRV)
        {
            CoordArray& KeyPt = (MouseAct() == MOUSE_TYPE_BEND_SRCCRV) ? m_SrcKeyPt : m_TarKeyPt;
            KeyPt.clear();
        }
        //============================//
        else
        {
            m_SelectedFace.clear();
            m_SelectedFaceArr.clear();
        }
        break;
    }
}

bool CMeshUI::CanUndo()
{
    if(SelectMode() == SELECT_MODE_VERTEX)
    {
        if(MouseAct() == MOUSE_TYPE_SELECT_LASSO && m_LassoKeyVtx.size() > 0)
            return true;
        else
            return m_SelectedVtxArr.size() > 0;
    }
    else if(SelectMode() == SELECT_MODE_FACE)
    {
        //++++++Added by Wangjun++++++//
        if(MouseAct() == MOUSE_TYPE_BEND_SRCCRV || MouseAct() == MOUSE_TYPE_BEND_TARCRV)
        {
            CoordArray& KeyPt = (MouseAct() == MOUSE_TYPE_BEND_SRCCRV) ? m_SrcKeyPt : m_TarKeyPt;
            return KeyPt.size() > 0;
        }
        else if(MouseAct() == MOUSE_TYPE_BEND_TARCRV)
            return m_TarKeyPt.size() > 0;
        //============================//
        else
            return m_SelectedFaceArr.size() > 0;
    }
    else
        return false;
}

bool CMeshUI::CanClear()
{
    if(SelectMode() == SELECT_MODE_VERTEX)
        return m_SelectedVtx.size() > 0;
    else if(SelectMode() == SELECT_MODE_FACE)
    {
        //++++++Added by Wangjun++++++//
        if(MouseAct() == MOUSE_TYPE_BEND_SRCCRV || MouseAct() == MOUSE_TYPE_BEND_TARCRV)
        {
            CoordArray& KeyPt = (MouseAct() == MOUSE_TYPE_BEND_SRCCRV) ? m_SrcKeyPt : m_TarKeyPt;
            return KeyPt.size() > 0;
        }
        else if(MouseAct() == MOUSE_TYPE_BEND_TARCRV)
            return m_TarKeyPt.size() > 0;
        //============================//
        else
            return m_SelectedFace.size() > 0;
    }
    else
        return false;
}

void CMeshUI::ClearLassoData()
{
    m_LassoKeyVtx.clear();
    m_LassoPath.clear();
    m_LassoVtxLoop.clear();

    mesh->m_AuxData.ClearData();
}

void CMeshUI::ClearFillData()
{
    m_FillVtx.clear();
    m_FillFace.clear();

    mesh->m_AuxData.ClearData();
}

void CMeshUI::ClearPaintData()
{
    m_PaintVtx.clear();
    m_PaintFace.clear();

    mesh->m_AuxData.ClearData();
}

//++++++Added by Wangjun++++++//
void CMeshUI::ClearSketchData()
{
    m_SrcKeyPt.clear();
    m_TarKeyPt.clear();
}
//============================//

void CMeshUI::AddSelected(IndexArray& Selected)
{
    // Make sure no duplicate vertex exists in the selected array
    util.Unique(Selected);

    size_t i, n = Selected.size();

    size_t index = m_SelectedVtx.size();
    if(SelectMode() == SELECT_MODE_VERTEX)
    {
        IndexArray NewSelected; // Remove those have been selected previously
        for(i = 0; i < n; ++ i)
        {
            VertexID vID = Selected[i];
            if(find(m_SelectedVtx.begin(), m_SelectedVtx.end(), vID) == m_SelectedVtx.end())    // New
                NewSelected.push_back(vID);
        }
        if(NewSelected.size())
        {
            m_SelectedVtxArr.push_back(NewSelected);
            n = NewSelected.size();
            for(i = 0; i < n; ++ i)
                m_SelectedVtx.push_back(NewSelected[i]);
        }
    }
    else if(SelectMode() == SELECT_MODE_FACE)
    {
        IndexArray NewSelected; // Remove those have been selected previously
        for(i = 0; i < n; ++ i)
        {
            FaceID fID = Selected[i];
            if(find(m_SelectedFace.begin(), m_SelectedFace.end(), fID) == m_SelectedFace.end())    // New
                NewSelected.push_back(fID);
        }
        if(NewSelected.size())
        {
            m_SelectedFaceArr.push_back(NewSelected);
            n = NewSelected.size();
            for(i = 0; i < n; ++ i)
                m_SelectedFace.push_back(NewSelected[i]);
        }
    }

//    // Visualization
//    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
//    n = m_SelectedVtx.size();
//    for(i = index; i < n; ++ i)
//    {
//        VertexID vID = m_SelectedVtx[i];
//        mesh->m_AuxData.AddPoint(vCoord[vID], RED);
//    }
}

void CMeshUI::JudgeSelection(int x, int y)
{
    int index, type;
    GetSelectInfo(x, y, index, type);
    if(type == SELECTION_TYPE_FOREGROUND)
        m_bOnElement = true;
    else
        m_bOnElement = false;
}

void CMeshUI::Test()
{
    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    size_t i, n = vCoord.size();
    for(i = 0; i < n; ++ i)
    {
        Coord pos, eye;
        Project(vCoord[i], eye);
        
        float depth;
        GetDepth((int) eye[0], (int) eye[1], depth);
        ASSERT(ALMOST_EQUAL_LARGE(depth, eye[2]));
        UnProject(eye, pos);
        mesh->m_AuxData.AddPoint(pos, RED);
    }
}

void CMeshUI::GetNearestVertex(FaceID fID, int x, int y, VertexID& vID)
{
    IndexArray& face = mesh->m_Kernel.GetFaceInfo().GetIndex()[fID];
    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    size_t i, n = face.size();

    DoubleArray Dist;
    Dist.resize(n);
    Coord center(x, y, 0.0);
    for(i = 0; i < n; ++ i)
    {
        Coord& v = vCoord[face[i]];
        Coord eye;
        Project(v, eye);
        eye[2] = 0.0;
        Dist[i] = (center - eye).abs();
    }
    
    size_t index = distance(Dist.begin(), min_element(Dist.begin(), Dist.end()));
    ASSERT(index >= 0 && index < n);
    vID = face[index];
}

// Visualization
void CMeshUI::AddPoint(VertexID vID, Color color)
{
    ASSERT(mesh->m_BasicOp.IsValidVertexIndex(vID));

    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    mesh->m_AuxData.AddPoint(vCoord[vID], color);
}

void CMeshUI::AddPath(IndexArray& Path, Color color)
{
    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    
    size_t i, n = Path.size();
    for(i = 0; i < n-1; ++ i)
    {
        ASSERT(mesh->m_BasicOp.IsValidVertexIndex(Path[i]));
        ASSERT(mesh->m_BasicOp.IsValidVertexIndex(Path[i+1]));
        mesh->m_AuxData.AddLine(vCoord[Path[i]], vCoord[Path[i+1]], color);
    }
}

void CMeshUI::DrawSelected()
{
    if(opengl == NULL)
        return;
    
    CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
    PolyIndexArray& fIndex = mesh->m_Kernel.GetFaceInfo().GetIndex();

    glDisable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);

    switch(SelectMode()) {
    case SELECT_MODE_VERTEX:
        {
            glEnable(GL_POINT_SMOOTH);            
            glPointSize(3.0f);
            
            Color c = RED;
            glColor3d(c[0], c[1], c[2]);

            glBegin(GL_POINTS);
            // Draw selected vertices
            size_t i, n = m_SelectedVtx.size();
            for(i = 0; i < n; ++ i)
            {
                Coord p = vCoord[m_SelectedVtx[i]];
                glVertex3d(p[0], p[1], p[2]);
            }
            // Draw intermediate painted vertices
            if(MouseAct() == MOUSE_TYPE_SELECT_PAINT && m_PaintVtx.size())
            {
                n = m_PaintVtx.size();
                for(i = 0; i < n; ++ i)
                {
                    Coord p = vCoord[m_PaintVtx[i]];
                    glVertex3d(p[0], p[1], p[2]);
                }
            }
            glEnd();
            
            glDisable(GL_POINT_SMOOTH);
        }
        break;
    case SELECT_MODE_FACE:
        {
            glDisable(GL_LIGHTING);
            glDepthFunc(GL_LEQUAL);
            
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glLineWidth(mesh->m_Render.DftEdgeWidth());

            CoordArray& vCoord = mesh->m_Kernel.GetVertexInfo().GetCoord();
            ColorArray& vColor = mesh->m_Kernel.GetVertexInfo().GetColor();
            PolyIndexArray& fIndex = mesh->m_Kernel.GetFaceInfo().GetIndex();
            NormalArray& fNormal = mesh->m_Kernel.GetFaceInfo().GetNormal();

            Color c = BLUE*0.70;
            glColor3d(c[0], c[1], c[2]);
            
            glBegin(GL_LINES);
            // Draw selected faces
            size_t i, j, n = m_SelectedFace.size();
            for(i = 0; i < n; ++ i)
            {
                IndexArray& f = fIndex[m_SelectedFace[i]];
                size_t m = f.size();
                for(j = 0; j < m; ++ j)
                {
                    Coord& v1 = vCoord[f[j]];
                    Coord& v2 = vCoord[f[(j+1)%m]];
                    glVertex3d(v1[0], v1[1], v1[2]);
                    glVertex3d(v2[0], v2[1], v2[2]);
                }
            }
            // Draw intermediate painted faces
            if(MouseAct() == MOUSE_TYPE_SELECT_PAINT && m_PaintFace.size())
            {
                n = m_PaintFace.size();
                for(i = 0; i < n; ++ i)
                {
                    IndexArray& f = fIndex[m_PaintFace[i]];
                    size_t m = f.size();
                    for(j = 0; j < m; ++ j)
                    {
                        Coord& v1 = vCoord[f[j]];
                        Coord& v2 = vCoord[f[(j+1)%m]];
                        glVertex3d(v1[0], v1[1], v1[2]);
                        glVertex3d(v2[0], v2[1], v2[2]);
                    }
                }
            }
            glEnd();
            
            glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }
        break;
    }

    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
}

// Selection output
bool CMeshUI::IsVtxSelected()
{
    return (m_SelectedVtx.size() > 0);
}

bool CMeshUI::IsFaceSelected()
{
    return (m_SelectedFace.size() > 0);
}

IndexArray CMeshUI::GetSelectedVtx()
{
    return m_SelectedVtx;
}

IndexArray CMeshUI::GetSelectedFace()
{
    return m_SelectedFace;
}

IndexArray CMeshUI::GetLastSelectedVtx()
{
    ASSERT(m_SelectedVtxArr.size());
    return m_SelectedVtxArr[m_SelectedVtxArr.size()-1];
}

// Transform functions
bool CMeshUI::IsTransform()
{
    return (m_MouseAct == MOUSE_TYPE_TRANSLATE || m_MouseAct == MOUSE_TYPE_ROTATE || m_MouseAct == MOUSE_TYPE_SCALE);
}

// I/O functions
bool CMeshUI::StoreSelection(string filename, bool bVtx /* = true */)
{
        // Read data from file
    ofstream file(filename.c_str());
    if(!file)
        return false;

    // Store vertex/face flag
    file << bVtx << '\n';

    IndexArray& Selection = (bVtx) ? m_SelectedVtx : m_SelectedFace;
    // Store selection information
    size_t i, n = Selection.size();
    file << n << '\n';
    for(i = 0; i < n; ++ i)
    {
        Index& idx = Selection[i];
        file << idx << ((i<n-1) ? ' ' : '\n');
    }

    file.close();

        return true;
}

bool CMeshUI::LoadSelection(string filename)
{
    // Read data from file
    ifstream file(filename.c_str());
    if(! file)
        return false;

    // Load vertex/face flag
    bool bVtx;
    file >> bVtx;

    IndexArray& Selection = (bVtx) ? m_SelectedVtx : m_SelectedFace;
    PolyIndexArray& SelArr = (bVtx) ? m_SelectedVtxArr : m_SelectedFaceArr;
    Selection.clear();
    SelArr.clear();

    // Load selection information
    int i, n;
    file >> n;
    Selection.resize(n);

    for(i = 0; i < n; ++ i)
    {
        Index& idx = Selection[i];
        file >> idx;
    }
    
    // Add to selection buffer
    if(n)
        SelArr.push_back(Selection);

    file.close();

        return true;
}
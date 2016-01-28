#include "stdafx.h"
#include "MeshEigenConner.h"
#include "../Common/watchtime.h"

CMeshConner::CMeshConner()
{
        p_comData = new CommonData;
        p_criPoint = new CriticalPoint(*p_comData);
        p_drawer = new MeshDrawer(*p_comData);
        p_removal = new NoiseRemoval(*p_comData);
}

CMeshConner::~CMeshConner()
{
        if(p_removal) delete p_removal;
        if(p_drawer) delete p_drawer;
        if(p_criPoint)  delete p_criPoint;
        if(p_comData) delete p_comData;
}

void CMeshConner::Attach(MeshModel* pModel)
{
        ASSERT(pModel != NULL);

        Global::mesh = pModel;
        Global::p_coord = &(Global::mesh->m_Kernel.GetVertexInfo().GetCoord());
        Global::p_adjVtxArray = &(Global::mesh->m_Kernel.GetVertexInfo().GetAdjVertices());
        Global::vtxNum = Global::p_coord->size();
        Global::faceNum = Global::mesh->m_Kernel.GetFaceInfo().GetIndex().size();

        Global::mesh->m_Kernel.GetModelInfo().GetBoundingSphere(Global::bCenter,Global::bRadius);
        Global::maxLineLength = 2*Global::bRadius;

        p_comData->clear();
}

bool CMeshConner::LoadEigenField(string filename)
{
        SystemWatchTime t;
        bool rst=p_criPoint->LoadEigenField(filename);  
        
        t.print_elapsed_time();
        return rst;
}

void CMeshConner::setDrawMode(Global::DrawMode mode)
{
        p_drawer->setMode(mode);
}

void CMeshConner::Draw()
{
        p_drawer->DrawMesh();
}

void CMeshConner::SaveMEConnLine(string filename)
{

}

bool CMeshConner::IsRemoveNoise()
{
        return false;
}

void CMeshConner::RemoveNoise()
{
        SystemWatchTime t;
        printf("begin to remove noise...\n");
        p_removal->CalPersistence();
        p_removal->RemoveNoise();
        t.print_elapsed_time();
        p_drawer->setBadPaths(p_removal->GetBadPaths());
}

void CMeshConner::clear()
{       
        if(p_drawer)    p_drawer->clear();
        if(p_criPoint)  p_criPoint->clear();
        if(p_comData)   p_comData->clear();
}
Hide details
Change log
r2 by weihongyu1987 on Mar 4, 2010   Diff
The First Commit
Go to: 	
Older revisions
All revisions of this file
File info
Size: 1824 bytes, 81 lines
View raw file

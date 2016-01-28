// MeshModelFrameWorkDoc.cpp : implementation of the CMeshModelFrameWorkDoc class
//

#include "stdafx.h"
#include "MeshModelFrameWork.h"

#include "MeshModelFrameWorkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMeshModelFrameWorkDoc

IMPLEMENT_DYNCREATE(CMeshModelFrameWorkDoc, CDocument)

BEGIN_MESSAGE_MAP(CMeshModelFrameWorkDoc, CDocument)
END_MESSAGE_MAP()


// CMeshModelFrameWorkDoc construction/destruction

CMeshModelFrameWorkDoc::CMeshModelFrameWorkDoc()
{
        // TODO: add one-time construction code here

}

CMeshModelFrameWorkDoc::~CMeshModelFrameWorkDoc()
{
}

BOOL CMeshModelFrameWorkDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}




// CMeshModelFrameWorkDoc serialization

void CMeshModelFrameWorkDoc::Serialize(CArchive& ar)
{
        if (ar.IsStoring())
        {
                // TODO: add storing code here
        }
        else
        {
                // TODO: add loading code here
        }
}


// CMeshModelFrameWorkDoc diagnostics

#ifdef _DEBUG
void CMeshModelFrameWorkDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CMeshModelFrameWorkDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG


// CMeshModelFrameWorkDoc commands

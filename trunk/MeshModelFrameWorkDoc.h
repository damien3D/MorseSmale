// MeshModelFrameWorkDoc.h : interface of the CMeshModelFrameWorkDoc class
//


#pragma once


class CMeshModelFrameWorkDoc : public CDocument
{
protected: // create from serialization only
        CMeshModelFrameWorkDoc();
        DECLARE_DYNCREATE(CMeshModelFrameWorkDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
        virtual BOOL OnNewDocument();
        virtual void Serialize(CArchive& ar);

// Implementation
public:
        virtual ~CMeshModelFrameWorkDoc();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        DECLARE_MESSAGE_MAP()
};
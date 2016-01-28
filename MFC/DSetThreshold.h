#pragma once
#include "Resource.h"

// DSetThreshold dialog

class DSetThreshold : public CDialog
{
        DECLARE_DYNAMIC(DSetThreshold)

public:
        DSetThreshold(CWnd* pParent = NULL);   // standard constructor
        virtual ~DSetThreshold();

// Dialog Data
        enum { IDD = IDD_SETTHRESHOLD };

protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

        DECLARE_MESSAGE_MAP()
        CString m_lowBound;
        CString m_upBound;

public:
        CString GetLowBound() { return m_lowBound;}
        CString GetUpBound() { return m_upBound;}
        void SetLowBound(CString lowBound) { m_lowBound = lowBound; }
        void SetUpBound(CString upBound) { m_upBound = upBound; }
        afx_msg void OnEnChangeEdit1();
        afx_msg void OnBnClickedOk();
        afx_msg void OnEnChangeLowbound();
};
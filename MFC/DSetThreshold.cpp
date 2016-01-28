// DSetThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "DSetThreshold.h"

// DSetThreshold dialog

IMPLEMENT_DYNAMIC(DSetThreshold, CDialog)

DSetThreshold::DSetThreshold(CWnd* pParent /*=NULL*/)
        : CDialog(DSetThreshold::IDD, pParent)
        , m_lowBound(_T("0.00"))
        , m_upBound(_T("1.00"))
{

}


DSetThreshold::~DSetThreshold()
{
}

void DSetThreshold::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        DDX_Text(pDX, IDC_LOWBOUND, m_lowBound);
        DDX_Text(pDX, IDC_UPBOUND, m_upBound);
}


BEGIN_MESSAGE_MAP(DSetThreshold, CDialog)
        ON_BN_CLICKED(IDOK, &DSetThreshold::OnBnClickedOk)
END_MESSAGE_MAP()

void DSetThreshold::OnBnClickedOk()
{
        // TODO: Add your control notification handler code here
        UpdateData(TRUE);
        USES_CONVERSION;
        OnOK();
}
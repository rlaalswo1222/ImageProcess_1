// DlgContrastStretchOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgContrastStretchOption.h"


// DlgContrastStretchOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgContrastStretchOption, CDialog)

DlgContrastStretchOption::DlgContrastStretchOption(CWnd* pParent /*=NULL*/)
	: CDialog(DlgContrastStretchOption::IDD, pParent)
	, m_nStretchType(0)
	, m_fLow(0)
	, m_fHigh(0)
{

}

DlgContrastStretchOption::~DlgContrastStretchOption()
{
}

void DlgContrastStretchOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_CS_AUTO, m_nStretchType);
	DDX_Text(pDX, IDC_CS_LOW, m_fLow);
	DDX_Text(pDX, IDC_CS_HIGH, m_fHigh);
}


BEGIN_MESSAGE_MAP(DlgContrastStretchOption, CDialog)
END_MESSAGE_MAP()


// DlgContrastStretchOption 메시지 처리기입니다.

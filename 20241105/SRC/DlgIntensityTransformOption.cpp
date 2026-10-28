// DlgIntensityTransformOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgIntensityTransformOption.h"


// DlgIntensityTransformOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgIntensityTransformOption, CDialog)

DlgIntensityTransformOption::DlgIntensityTransformOption(CWnd* pParent /*=NULL*/)
	: CDialog(DlgIntensityTransformOption::IDD, pParent)
	, m_nTransformType(0)
	, m_nLinearTransfromCoefficient_a(1)
	, m_nLinearTransfromCoefficient_b(0)
	, m_byThresholdValue(127)
	, m_byContrastStretching_Start(0)
	, m_byContrastStretching_End(255)
{

}

DlgIntensityTransformOption::~DlgIntensityTransformOption()
{
}

void DlgIntensityTransformOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_IT_LINEAR, m_nTransformType);
	DDX_Text(pDX, IDC_IT_LINEAR_A, m_nLinearTransfromCoefficient_a);
	DDX_Text(pDX, IDC_IT_LINEAR_B, m_nLinearTransfromCoefficient_b);
	DDX_Text(pDX, IDC_IT_THRESHOLD_VALUE, m_byThresholdValue);
	DDX_Text(pDX, IDC_IT_CS_START, m_byContrastStretching_Start);
	DDX_Text(pDX, IDC_IT_CS_END, m_byContrastStretching_End);
}


BEGIN_MESSAGE_MAP(DlgIntensityTransformOption, CDialog)
END_MESSAGE_MAP()


// DlgIntensityTransformOption 메시지 처리기입니다.

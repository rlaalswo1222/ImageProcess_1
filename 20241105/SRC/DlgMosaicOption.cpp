// DlgMosaicOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgMosaicOption.h"


// DlgMosaicOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgMosaicOption, CDialog)

DlgMosaicOption::DlgMosaicOption(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMosaicOption::IDD, pParent)
	, m_dwWindowSize(0)
{

}

DlgMosaicOption::~DlgMosaicOption()
{
}

void DlgMosaicOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_MO_WIN_SIZE, m_dwWindowSize);
}


BEGIN_MESSAGE_MAP(DlgMosaicOption, CDialog)
END_MESSAGE_MAP()


// DlgMosaicOption 메시지 처리기입니다.

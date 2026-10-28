#pragma once


// DlgMosaicOption 대화 상자입니다.

class DlgMosaicOption : public CDialog
{
	DECLARE_DYNAMIC(DlgMosaicOption)

public:
	DlgMosaicOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgMosaicOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOSAIC_OPTION };

public:
	DWORD m_dwWindowSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

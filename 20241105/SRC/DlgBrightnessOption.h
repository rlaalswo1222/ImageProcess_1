#pragma once


// DlgBrightnessOption 대화 상자입니다.

class DlgBrightnessOption : public CDialog
{
	DECLARE_DYNAMIC(DlgBrightnessOption)

public:
	DlgBrightnessOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgBrightnessOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BRIGHTNESS_OPTION };

public:
	int m_nPlusMinus;
	BYTE m_byModifyValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

};

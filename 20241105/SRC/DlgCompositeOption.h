#pragma once
#include "afxwin.h"


// DlgCompositeOption 대화 상자입니다.

class DlgCompositeOption : public CDialog
{
	DECLARE_DYNAMIC(DlgCompositeOption)

public:
	DlgCompositeOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgCompositeOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COMPOSITE_OPTION };

// 특성입니다.
public:
	inline int GetCompositeOperatorID() { return m_nOperatorID; }
	inline CxImage * GetSecondImage() { return m_pSecondImage; }

private:
	CxImage * m_pFirstImage;
	CxImage * m_pSecondImage;
	int m_nOperatorID;

	CComboBox m_Combo_Operator;
	CComboBox m_Combo_SecondImage;

// 재정의입니다.
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeCoOperator();
	afx_msg void OnCbnSelchangeCoSecondImage();

	DECLARE_MESSAGE_MAP()
};

#pragma once


// DlgIntensityTransformOption 대화 상자입니다.

class DlgIntensityTransformOption : public CDialog
{
	DECLARE_DYNAMIC(DlgIntensityTransformOption)

public:
	DlgIntensityTransformOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgIntensityTransformOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INTENSITY_TRANSFORM_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nTransformType;
	double m_nLinearTransfromCoefficient_a;
	double m_nLinearTransfromCoefficient_b;
	BYTE m_byThresholdValue;
	BYTE m_byContrastStretching_Start;
	BYTE m_byContrastStretching_End;
};

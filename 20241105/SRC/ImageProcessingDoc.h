
// ImageProcessingDoc.h : CImageProcessingDoc 클래스의 인터페이스
//


#pragma once


class CImageProcessingDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImageProcessingDoc();
	DECLARE_DYNCREATE(CImageProcessingDoc)

// 특성입니다.
public:
	inline CxImage *GetImage() { return m_pImage; }

	inline long GetHistogramRed(int i) { return m_histogramRed[i]; }
	inline long GetHistogramGreen(int i) { return m_histogramGreen[i]; }
	inline long GetHistogramBlue(int i) { return m_histogramBlue[i]; }
	inline long GetHistogramGray(int i) { return m_histogramGray[i]; }
	inline long GetHistogramMax() { return m_histogramMax; }

private:
	CxImage * m_pImage;

	long m_histogramRed[256];
	long m_histogramGreen[256];
	long m_histogramBlue[256];
	long m_histogramGray[256];
	long m_histogramMax;

// 작업입니다.
public:
	CString FindExtension(const CString& name);
	CString RemoveExtension(const CString& name);
	int FindType(const CString& ext);
	void CalculateHistogram();

// 재정의입니다.
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CImageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnProcessBrightness();
	afx_msg void OnProcessMosaic();
	afx_msg void OnProcessComposite();
	afx_msg void OnProcessIntensityTransform();

	DECLARE_MESSAGE_MAP()	
	
public:
	afx_msg void OnProcessContrastStretch();
	afx_msg void OnProcessEqualization();
	afx_msg void OnHistogramSpecification();
};



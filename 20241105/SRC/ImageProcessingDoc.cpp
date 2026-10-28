
// ImageProcessingDoc.cpp : Implement a CImageProcessingDoc Class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "DlgBrightnessOption.h"
#include "DlgMosaicOption.h"
#include "DlgCompositeOption.h"
#include "ImageMask.h"
#include "ObjectDetector.h"
#include "CompositeProcessor.h"
#include "DlgIntensityTransformOption.h"
#include "DlgContrastStretchOption.h"

#include "ImageProcessingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingDoc

IMPLEMENT_DYNCREATE(CImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessingDoc, CDocument)
	ON_COMMAND(ID_PROCESS_MOSAIC, &CImageProcessingDoc::OnProcessMosaic)
	ON_COMMAND(ID_PROCESS_BRIGHTNESS, &CImageProcessingDoc::OnProcessBrightness)
	ON_COMMAND(ID_PROCESS_COMPOSITE, &CImageProcessingDoc::OnProcessComposite)
	ON_COMMAND(ID_PROCESS_INTENSITY_TRANSFORM, &CImageProcessingDoc::OnProcessIntensityTransform)
	ON_COMMAND(ID_PROCESS_CONTRAST_STRETCH, &CImageProcessingDoc::OnProcessContrastStretch)
	ON_COMMAND(ID_PROCESS_EQUALIZATION, &CImageProcessingDoc::OnProcessEqualization)
	ON_COMMAND(ID_32783, &CImageProcessingDoc::OnHistogramSpecification)
END_MESSAGE_MAP()


// CImageProcessingDoc Contruction/Destuction

CImageProcessingDoc::CImageProcessingDoc()
{
	//// TODO: Add an one-time generating code here
	m_pImage = NULL;
}

CImageProcessingDoc::~CImageProcessingDoc()
{
	if (NULL != m_pImage)
		delete m_pImage;
}

BOOL CImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: load imagefile // DONE
	m_pImage = new CxImage;
	m_pImage->Load(lpszPathName, FindType(lpszPathName));

	CalculateHistogram();

	return TRUE;
}

BOOL CImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//// TODO: Add a re-initialization code here
	//// SDI documents will reuse this article

	return TRUE;
}




// CImageProcessingDoc serialization

void CImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//// TODO: Add a saving code here
	}
	else
	{
		//// TODO: Add a loading code here
	}
}


// CImageProcessingDoc diagnosis

#ifdef _DEBUG
void CImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProcessingDoc command

CString CImageProcessingDoc::FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString(_T(""));
}

CString CImageProcessingDoc::RemoveExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(0,i);
		}
	}
	return name;
}

int CImageProcessingDoc::FindType(const CString& ext)
{
	return CxImage::GetTypeIdFromName(ext);
}

void CImageProcessingDoc::CalculateHistogram()
{
	// TODO: Add a calculating histogram code here
	if (m_pImage) {
		// Histogram function, which is implemented in Cximage
		 //m_histogramMax = m_pImage->Histogram(m_histogramRed, m_histogramGreen, m_histogramBlue, m_histogramGray);

		 
		// 가짜코드
		for (int i = 0; i < 256; i++){
			m_histogramRed[i]   = rand() % 200;
			m_histogramGreen[i] = rand() % 200;
			m_histogramBlue[i]  = rand() % 200;
			m_histogramGray[i]  = rand() % 200;
		}

		m_histogramMax = 0;
		for (int i = 0; i < 256; i++){
			m_histogramMax = max(m_histogramMax, m_histogramRed[i]  );
			m_histogramMax = max(m_histogramMax, m_histogramGreen[i]);
			m_histogramMax = max(m_histogramMax, m_histogramBlue[i] );
			m_histogramMax = max(m_histogramMax, m_histogramGray[i] );
		}
		//////////////////////////////////////////////////////////////
	}
}

// clamp value to 0~255 (prevent overflow/underflow)
static BYTE ClampByte(int v)
{
	if (v > 255) return 255;
	if (v < 0)   return 0;
	return (BYTE)v;
}

void CImageProcessingDoc::OnProcessBrightness()
{
	// TODO: Add a changing the brightness histogram code here
	if (m_pImage) {
		DlgBrightnessOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nPlusMinus = dlg.m_nPlusMinus;		// 0 : plus(+), 1 : minus(-)
			BYTE byModifyValue = dlg.m_byModifyValue;

			int nModify = (nPlusMinus == 0) ? (int)byModifyValue : -(int)byModifyValue;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;

			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					color = m_pImage->GetPixelColor(x, y);

					// compute in int, then clamp to 0~255
					color.rgbRed   = ClampByte((int)color.rgbRed   + nModify);
					color.rgbGreen = ClampByte((int)color.rgbGreen + nModify);
					color.rgbBlue  = ClampByte((int)color.rgbBlue  + nModify);

					m_pImage->SetPixelColor(x, y, color);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessMosaic()
{
	// TODO: Add a mosaic code here
	if (m_pImage) {
		DlgMosaicOption dlg;

		if (dlg.DoModal() == IDOK) {
			DWORD dwWindowSize = dlg.m_dwWindowSize;
			if (dwWindowSize < 1) dwWindowSize = 1;		// prevent divide by zero

			// 1. Get image size
			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();

			// 2. Find steps for window shifting (round up to include the edge of the image)
			DWORD xStep = (width  + dwWindowSize - 1) / dwWindowSize;
			DWORD yStep = (height + dwWindowSize - 1) / dwWindowSize;

			RGBQUAD color;
			RGBQUAD newcolor;

			// 5, 6. Shift the window by window size (not by 1 pixel : Mosaic != Blur)
			for (DWORD sy = 0; sy < yStep; sy++) {
				for (DWORD sx = 0; sx < xStep; sx++) {
					DWORD x0 = sx * dwWindowSize;
					DWORD y0 = sy * dwWindowSize;

					// Edge of the image : the last window can be smaller than window size
					DWORD x1 = min(x0 + dwWindowSize, width);
					DWORD y1 = min(y0 + dwWindowSize, height);

					// 3. Calculate average of R, G, B individually in the window
					DWORD sumR = 0, sumG = 0, sumB = 0;
					DWORD count = 0;

					for (DWORD y = y0; y < y1; y++) {
						for (DWORD x = x0; x < x1; x++) {
							color = m_pImage->GetPixelColor(x, y);
							sumR += color.rgbRed;
							sumG += color.rgbGreen;
							sumB += color.rgbBlue;
							count++;
						}
					}

					if (count == 0) continue;

					// Loss of data : round off instead of truncating the decimal point
					newcolor.rgbRed      = (BYTE)((sumR + count / 2) / count);
					newcolor.rgbGreen    = (BYTE)((sumG + count / 2) / count);
					newcolor.rgbBlue     = (BYTE)((sumB + count / 2) / count);
					newcolor.rgbReserved = 0;

					// 4. Put the average value to whole pixels in the window
					for (DWORD y = y0; y < y1; y++) {
						for (DWORD x = x0; x < x1; x++) {
							m_pImage->SetPixelColor(x, y, newcolor);
						}
					}
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

// ---------------------------------------------------------------------------
// Composite (Team Project 01):  squid_head + squid_body  ->  - squid_points
// Helpers below only use CxImage's GetPixelColor / SetPixelColor / Create /
// Load / Save (allowed by the assignment). All image analysis and composition
// is done in ObjectDetector / CompositeProcessor.
// ---------------------------------------------------------------------------

static CString CompositeFolderOf(const CString& path)
{
	int i = max(path.ReverseFind(_T('\\')), path.ReverseFind(_T('/')));
	return (i >= 0) ? path.Left(i + 1) : CString(_T(""));
}

static void CompositeAddFolder(CStringArray& list, CString dir)
{
	if (dir.IsEmpty())
		return;
	if (dir.Right(1) != _T("\\") && dir.Right(1) != _T("/"))
		dir += _T("\\");
	for (int i = 0; i < list.GetSize(); i++)
		if (list[i].CompareNoCase(dir) == 0)
			return;
	list.Add(dir);
}

static bool CompositeLoadFile(const CString& path, CxImage& img)
{
	int dot = path.ReverseFind(_T('.'));
	if (dot < 0)
		return false;
	DWORD type = CxImage::GetTypeIdFromName(path.Mid(dot + 1));
	if (type == CXIMAGE_FORMAT_UNKNOWN)
		return false;
	return img.Load(path, type) && img.IsValid();
}

// look for <folder>\<baseName>.(png|jpg|jpeg|bmp) in every candidate folder
static bool CompositeFindAndLoad(const CStringArray& folders, const TCHAR* baseName, CxImage& img, CString& foundPath)
{
	static const TCHAR* exts[] = { _T("png"), _T("jpg"), _T("jpeg"), _T("bmp") };
	for (int f = 0; f < folders.GetSize(); f++) {
		for (int e = 0; e < 4; e++) {
			CString path = folders[f] + baseName + _T(".") + exts[e];
			if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES)
				continue;
			if (CompositeLoadFile(path, img)) {
				foundPath = path;
				return true;
			}
		}
	}
	return false;
}

// fall back: let the user pick the file
static bool CompositeAskAndLoad(const TCHAR* baseName, CxImage& img, CString& foundPath)
{
	CString title;
	title.Format(_T("Select %s image"), baseName);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Image Files (*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp|All Files (*.*)|*.*||"),
		AfxGetMainWnd());
	dlg.m_ofn.lpstrTitle = title;
	if (dlg.DoModal() != IDOK)
		return false;
	foundPath = dlg.GetPathName();
	return CompositeLoadFile(foundPath, img);
}

static bool CompositeCxToRaw(CxImage* pImg, RawImage& out)
{
	if (!pImg || !pImg->IsValid())
		return false;
	const int w = (int)pImg->GetWidth();
	const int h = (int)pImg->GetHeight();
	out.Create(w, h);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			RGBQUAD c = pImg->GetPixelColor(x, y);
			out.Set(x, y, c.rgbRed, c.rgbGreen, c.rgbBlue);
		}
	}
	return true;
}

static void CompositeRawToCx(const RawImage& src, CxImage* dst)
{
	dst->Create(src.width, src.height, 24);
	RGBQUAD c;
	c.rgbReserved = 0;
	for (int y = 0; y < src.height; y++) {
		for (int x = 0; x < src.width; x++) {
			unsigned char r, g, b;
			src.Get(x, y, r, g, b);
			c.rgbRed = r;
			c.rgbGreen = g;
			c.rgbBlue = b;
			dst->SetPixelColor(x, y, c);
		}
	}
}

static void CompositeConvertMask(const ObjectMask& src, ImageMask& dst)
{
	dst.Create((int)src.width, (int)src.height);
	for (size_t i = 0; i < src.pixels.size(); i++)
		dst.data[i] = src.pixels[i] ? 1 : 0;
	dst.bgR = src.backgroundColor.rgbRed;
	dst.bgG = src.backgroundColor.rgbGreen;
	dst.bgB = src.backgroundColor.rgbBlue;
	dst.left = src.bounds.left;
	dst.top = src.bounds.top;
	dst.right = src.bounds.right - 1;      // ObjectMask bounds are exclusive (RECT style)
	dst.bottom = src.bounds.bottom - 1;
}

void CImageProcessingDoc::OnProcessComposite()
{
	if (!m_pImage)
		return;

	// ---- 1. load squid_head / squid_body / squid_points ----
	// search order: folder of the opened image, folder of the .exe, current dir.
	CStringArray folders;
	CompositeAddFolder(folders, CompositeFolderOf(GetPathName()));
	TCHAR exePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, exePath, MAX_PATH);
	CompositeAddFolder(folders, CompositeFolderOf(exePath));
	TCHAR cwd[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, cwd);
	CompositeAddFolder(folders, cwd);

	const TCHAR* names[3] = { _T("squid_head"), _T("squid_body"), _T("squid_points") };
	CxImage head, body, points;
	CxImage* imgs[3] = { &head, &body, &points };
	CString outFolder;

	for (int i = 0; i < 3; i++) {
		CString found;
		if (!CompositeFindAndLoad(folders, names[i], *imgs[i], found) &&
		    !CompositeAskAndLoad(names[i], *imgs[i], found)) {
			CString msg;
			msg.Format(_T("Could not load %s. Composite was cancelled."), names[i]);
			AfxMessageBox(msg);
			return;
		}
		if (i == 0)
			outFolder = CompositeFolderOf(found);
	}

	// ---- 2. CxImage -> RawImage ----
	RawImage rawHead, rawBody, rawPoints;
	if (!CompositeCxToRaw(&head, rawHead) ||
	    !CompositeCxToRaw(&body, rawBody) ||
	    !CompositeCxToRaw(&points, rawPoints)) {
		AfxMessageBox(_T("Invalid input image."));
		return;
	}

	// ---- 3. shape masks (ObjectDetector by member A: AnalyzeImage) ----
	ObjectMask objHead, objBody, objPoints;
	if (!AnalyzeImage(&head, 15, objHead) ||
	    !AnalyzeImage(&body, 15, objBody) ||
	    !AnalyzeImage(&points, 15, objPoints)) {
		AfxMessageBox(_T("Could not find a shape in one of the input images."));
		return;
	}
	ImageMask maskHead, maskBody, maskPoints;
	CompositeConvertMask(objHead, maskHead);
	CompositeConvertMask(objBody, maskBody);
	CompositeConvertMask(objPoints, maskPoints);

	// ---- 4. head + body, then - points ----
	CompositeOptions opt;            // defaults: additive overlap color, RGB subtract
	RawImage intermediate, result;
	if (!CompositeProcessor::Compose(rawHead, maskHead, rawBody, maskBody,
	                                 rawPoints, maskPoints, opt, intermediate, result)) {
		AfxMessageBox(_T("Composite failed."));
		return;
	}

	// ---- 5. show result in the current document and save both steps ----
	CompositeRawToCx(result, m_pImage);

	CxImage stepImage;
	CompositeRawToCx(intermediate, &stepImage);
	if (!outFolder.IsEmpty()) {
		stepImage.Save(outFolder + _T("squid_step1_head_body.png"), CXIMAGE_FORMAT_PNG);
		m_pImage->Save(outFolder + _T("squid_result.png"), CXIMAGE_FORMAT_PNG);
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessIntensityTransform()
{
	// TODO: Add a Contrast conversion code here
	if (m_pImage) {
		DlgIntensityTransformOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nTransformType = dlg.m_nTransformType;
			double nLinearTransfromCoefficient_a = dlg.m_nLinearTransfromCoefficient_a;
			double nLinearTransfromCoefficient_b = dlg.m_nLinearTransfromCoefficient_b;

			int byThresholdValue = dlg.m_byThresholdValue;

			BYTE byContrastStretching_Start = dlg.m_byContrastStretching_Start;
			BYTE byContrastStretching_End = dlg.m_byContrastStretching_End;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessContrastStretch()
{
	// TODO: Add a Contrast stretching code here
	if (m_pImage) {
		DlgContrastStretchOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nStretchType = dlg.m_nStretchType;
			float fLow = dlg.m_fLow;
			float fHigh = dlg.m_fHigh;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;

			BYTE LUT[256];

			// initialize thresholds
			int nLowTherhs = 0;
			int nHighThresh = 255;


			// compute thresholds
			if (nStretchType == 0) { // Auto

			} else if (nStretchType == 1) { // Ends-In

			}


			// compute LUT


			// trnasfer image
			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					color = m_pImage->GetPixelColor(x, y);

					// using LUT

					m_pImage->SetPixelColor(x, y, newcolor);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessEqualization()
{
	// TODO: Add a Histogram equalization code here
	if (m_pImage) 
	{

		DWORD width = m_pImage->GetWidth();
		DWORD height = m_pImage->GetHeight();
		RGBQUAD color;	// Save the current color value
		RGBQUAD newcolor;	// After the conversion, save the color value

		//(1) Histogram has already been generated histogram(Omission) 
		//(2) Create a cumulative histogram
		DWORD sum = 0;
		float scale_factor = 255.0 / (width*height);
		DWORD sum_hist[256]; // a cumulative histogram

		for(int i=0; i<256 ; i++)
		{
		// (Coding)

		}


		//(3) Calculating look-up table
		BYTE LUT[256]; // look-up table

		for(int i=0; i<256 ; i++)
		{
		// (Coding)  


		}

		// (4) Image Conversion
		for (DWORD y = 0; y < height; y++) 
		{
			for (DWORD x = 0; x < width; x++) 
			{
				color = m_pImage->GetPixelColor(x, y);
				// (Coding)



				m_pImage->SetPixelColor(x, y, newcolor);
			}
		}		
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}



void CImageProcessingDoc::OnHistogramSpecification()
{
	// TODO: Add a Histogram specifications code here
	if (m_pImage) 
	{

		DWORD width = m_pImage->GetWidth();
		DWORD height = m_pImage->GetHeight();
		RGBQUAD color;	// Save the current color value
		RGBQUAD newcolor;	// After the conversion, save the color value

		//(1) Histogram has already been generated histogram(Omission) 
		//(2) Create a cumulative histogram
		DWORD sum = 0;
		float scale_factor = 255.0 / (width*height);
		DWORD sum_hist[256]; // a cumulative histogram

		for(int i=0; i<256 ; i++)
		{
		// (coding)

		}


		//(3) Calculating look-up table
		BYTE LUT[256]; // look-up table

		for(int i=0; i<256 ; i++)
		{
		// (coding)  


		}


		// Same as histogram equalization code from here.
		// However, After a cumulative histogram convert to float sum_hist[256] and
		// modifing to be entered in the real number on calculating sum_hist


		//(4) 
		DWORD desired_histogram[256];
		// Making desired_histogram
/*
		//1.
		for (int i=0; i<=255; i++)
		{
			desired_histogram[i]=i;
		}
*/
/*
		//2.
		for (int i=0; i<=255; i++)
		{
			desired_histogram[i]=255-i;
		}
*/
/*
		//3.		
		for (int i=0; i<=127; i++)
		{
			desired_histogram[i]=127-i;
		}
		for (int i=128; i<=255; i++)
		{
			desired_histogram[i]=i-127;
		}
*/

		// redefinition
		sum = 0;
		scale_factor = 255.0 / (width*height);

		//(5) Create the cumulative histogram of histogram you want
		for(int i=0; i<256; i++)
		{
			//(coding)



		}

		//(6) Is the inverse.
		float difference;	// calculate the difference
		int min;	// Calculate the minimum of the difference
		DWORD inv_hist[256];	// reverse-histogram

		//(Hint) will use dual-route.
		// float fabs() Use library functions.
		for(int i=0; i<256; i++)
		{
		//(coding)




		}

		//(7) Create look-up table of reverse-histogram
		for(int i=0; i<256 ; i++)
		{
		//(coding)

		}
		
		

		// (8) Image Conversion
		for (DWORD y = 0; y < height; y++) 
		{
			for (DWORD x = 0; x < width; x++) 
			{
				color = m_pImage->GetPixelColor(x, y);
				// (coding)



				m_pImage->SetPixelColor(x, y, newcolor);
			}
		}		
	} // if(m_pImage)

	CalculateHistogram();
	UpdateAllViews(NULL);

	

}

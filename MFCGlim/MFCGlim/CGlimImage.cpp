// CGlimImage.cpp: 구현 파일
//

#include "pch.h"
#include "MFCGlim.h"
#include "afxdialogex.h"
#include "CGlimImage.h"


#include "MFCGlimDlg.h"
#include "iostream"	
#include <cmath>

using namespace std;

#pragma comment(linker, "/entry:mainCRTStartup /subsystem:console")
// CGlimImage 대화 상자

IMPLEMENT_DYNAMIC(CGlimImage, CDialogEx)

CGlimImage::CGlimImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGlimImage, pParent)
{
	m_pParent = pParent;
}

CGlimImage::~CGlimImage()
{
}

void CGlimImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGlimImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CGlimImage 메시지 처리기

void CGlimImage::OnBnClickedParent()
{
	static int n = 0;
	CMFCGlimDlg* pDlg = (CMFCGlimDlg*)m_pParent;
	pDlg->callfunc(n++);
}

BOOL CGlimImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MoveWindow(0, 0, 640, 480);
	InitImage();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGlimImage::OnPaint()
{
	CPaintDC dc(this);
	Graphics graphics(dc.m_hDC);

	if (m_GlimImage) {
		m_GlimImage.Draw(dc, 0, 0);
	}
	//cout << "OnPaint called" << endl;
	drawData(&graphics);
	if (circles.size() >= 3) {
		DrawCircumscribedCircle(&graphics);  
	}
}

void CGlimImage::InitImage() {
	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	m_GlimImage.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_GlimImage.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_GlimImage.GetPitch();
	unsigned char* fm = (unsigned char*)m_GlimImage.GetBits();

	memset(fm, 0xff, nHeight * nWidth);
}


void CGlimImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptMouse = point;
	CDialogEx::OnLButtonDown(nFlags, point);
	CircleData newCircle = { point.x, point.y, m_radius };
	if (circles.size() >= 3) {
		for (size_t i = 0; i < circles.size(); ++i) {
			int dx = point.x - circles[i].x;
			int dy = point.y - circles[i].y;
			if (sqrt(dx * dx + dy * dy) <= circles[i].radius) {
				m_bDragging = true;
				m_nDragIndex = i;
				SetCapture(); // 마우스 캡처 (창 밖으로 나가도 추적)
				break;
			}
		}
		return;
	}
	circles.push_back(newCircle);
	strTmp.Format(_T("%03d,%03d"), m_ptMouse.x, m_ptMouse.y);
	Invalidate(); // 화면 다시 그리기 요청
	CDialogEx::OnLButtonDown(nFlags, point);
	//cout << "Mouse: " << m_ptMouse.x << ", " << m_ptMouse.y << endl;
}


void CGlimImage::SetRadius(int radius)
{
	m_radius = radius;
	Invalidate(); 
}

void CGlimImage::SetThickness(int thickness)
{
	m_thickness = thickness;
	Invalidate(); 
}


void CGlimImage::drawData(Graphics* pGraphics)
{
	for (const auto& circle : circles) {
		SolidBrush brush(Color(0xff, 0x00, 0x00, 0x00));
		Pen pen(Color(0xff, 0x00, 0x00, 0x00), 2);
		int diameter = circle.radius * 2;
		pGraphics->FillEllipse(&brush, circle.x - circle.radius, circle.y - circle.radius, diameter, diameter);
		pGraphics->DrawEllipse(&pen, circle.x - circle.radius, circle.y - circle.radius, diameter, diameter);
	}
}

void CGlimImage::DrawCircumscribedCircle(Graphics* pGraphics) {
	// 세 점을 (x1, y1), (x2, y2), (x3, y3)로 정의

	int thickness = m_thickness;
	int x1 = circles[0].x, y1 = circles[0].y;
	int x2 = circles[1].x, y2 = circles[1].y;
	int x3 = circles[2].x, y3 = circles[2].y;

	// 외접원의 중심 (Xc, Yc) 구하기
	double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

	double Xc = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
	double Yc = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;

	// 반지름 R 구하기 (세 점 중 하나로 거리 계산)
	double R = sqrt((x1 - Xc) * (x1 - Xc) + (y1 - Yc) * (y1 - Yc));

	// 원 그리기
	Pen pen(Color(0xff, 0x00, 0x00, 0x00), m_thickness);
	pGraphics->DrawEllipse(&pen, (float)(Xc - R), (float)(Yc - R), (float)(2 * R), (float)(2 * R));
}
void CGlimImage::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && m_nDragIndex >= 0 && m_nDragIndex < circles.size()) {
		circles[m_nDragIndex].x = point.x;
		circles[m_nDragIndex].y = point.y;
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CGlimImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging) {
		m_bDragging = false;
		m_nDragIndex = -1;
		ReleaseCapture(); 
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


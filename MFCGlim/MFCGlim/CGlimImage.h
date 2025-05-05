#pragma once
#include "afxdialogex.h"

#include <vector>
#include <thread>
#include <chrono>
#include <cmath>

// CGlimImage 대화 상자

class CGlimImage : public CDialogEx
{
	DECLARE_DYNAMIC(CGlimImage)

public:
	CGlimImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGlimImage();

	CImage m_GlimImage;
	CWnd* m_pParent;
	CPoint m_ptMouse;
	CString strTmp;
	int m_nradius = 1;
	int m_nthickness = 1;
	int m_nDataCount = 0;	
	struct CircleData {
		int x, y;
		int radius;
	};
	std::vector<CircleData> circles;
	bool m_bDragging = false;
	int m_nDragIndex = -1;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGlimImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void InitImage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetRadius(int radius);
	void SetThickness(int thickness);
	void drawData(Graphics* pGraphics);
	void DrawCircumscribedCircle(Graphics* pGraphics);
	void ResetAll();
	void RandomMovePoints();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

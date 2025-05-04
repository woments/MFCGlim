
// MFCGlimDlg.h: 헤더 파일
//
#include "CGlimImage.h"
#pragma once


// CMFCGlimDlg 대화 상자
class CMFCGlimDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCGlimDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CGlimImage *m_pCGlimImage;
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCGLIM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();

	void callfunc(int n);
	afx_msg void OnBnClickedRadiusenter();
	int Radius_value;
	afx_msg void OnBnClickedThicknessenter();
	int Thickness_Value;
	afx_msg void OnBnClickedBtnRST();
	afx_msg void OnBnClickedBtnRandom();
	void UpdateCoord(int index, CPoint pt);
};

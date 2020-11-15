
// CSocket_TCPDlg.h : header file
//

#pragma once
#include "tchart1.h"
#include "CSeries.h"
#include <opencv2/opencv.hpp>
#include "afxcmn.h"
using namespace cv;

// CCSocket_TCPDlg dialog
class CCSocket_TCPDlg : public CDialogEx
{
// Construction
public:
	CCSocket_TCPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CSOCKET_TCP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	static DWORD WINAPI SRVProc(LPVOID lpParameter);
	static const int m_recvport=0;
	//static CString m_recvIP;
public:
	CSocket m_sockClient;
	CTchart1 m_chart;
	CString m_recvstr;
	CStringArray m_tempar;
	CSeries m_lineSeries;
	VideoCapture m_camera;
	Mat m_img;
	CWnd* m_pichwnd;
	string m_strImgSavePath;
	CListCtrl m_list;
	int m_num;
	string m_path;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	void showMatImgToWnd(CWnd* pWnd, const cv::Mat& img);
	void saveImage(cv::Mat image, string &outPutPath, int index);
	
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
/*const CString CCSocket_TCPDlg::m_recvIP="";*/
//CString CCSocket_TCPDlg::m_recvIP="";
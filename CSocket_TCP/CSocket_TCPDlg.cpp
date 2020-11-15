
// CSocket_TCPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSocket_TCP.h"
#include "CSocket_TCPDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCSocket_TCPDlg dialog




CCSocket_TCPDlg::CCSocket_TCPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCSocket_TCPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CCSocket_TCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CCSocket_TCPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCSocket_TCPDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CCSocket_TCPDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCSocket_TCPDlg::OnDblclkList)
END_MESSAGE_MAP()


// CCSocket_TCPDlg message handlers

BOOL CCSocket_TCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rect;
	m_list.GetClientRect(&rect);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rect.Width() / 8, 0);
	m_list.InsertColumn(1, _T("时间"), LVCFMT_CENTER, rect.Width() / 2, 1);
	m_list.InsertColumn(2, _T("温度"), LVCFMT_CENTER, rect.Width() / 8, 2);
	m_list.InsertColumn(3, _T("图片存储位置"), LVCFMT_CENTER, rect.Width() /4 , 3);
	
	SetTimer(2,1000,NULL);
	m_camera.open(1);
	m_lineSeries = (CSeries)m_chart.Series(0);
	m_pichwnd=GetDlgItem(IDC_STATIC_PIC);
	m_path="C:\\Users\\16973\\Desktop\\img";
	SetTimer(3,200,NULL);
	m_num=0;
	HANDLE hSRVThread=CreateThread(NULL,0,SRVProc,NULL,0,NULL);
	CloseHandle(hSRVThread);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSocket_TCPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCSocket_TCPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCSocket_TCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI CCSocket_TCPDlg::SRVProc(LPVOID lpParameter)
{
	CSocket SockSrv;
	SockSrv.Create(5200);
	SockSrv.Listen(5); 

	CSocket SockCon;
	char recvBuf[100];

	SOCKADDR_IN addr;
	int len=sizeof(SOCKADDR);

	SockSrv.Accept(SockCon,(SOCKADDR*)&addr,&len);

	CString recvIP="";
	//m_recvIP=inet_ntoa(addr.sin_addr);
	recvIP=inet_ntoa(addr.sin_addr);
	SockCon.Send("L1234W!",strlen("L1234W!")+1);
	SockCon.Receive(recvBuf,100);
	return 0;
}

void CCSocket_TCPDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_sockClient.Create();
	m_sockClient.Connect(L"192.168.4.1",5000); 
	m_sockClient.Send("This is Client!",strlen("This is Client!")+1);
	SetTimer(1,500,NULL);
}


void CCSocket_TCPDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==1)
	{
		char Recvbuf[100];
		m_sockClient.Receive(Recvbuf,100);
		m_recvstr=(CString)Recvbuf;
		int i=m_recvstr.Find('L');
		if (m_recvstr.Find('L')>=0&&m_recvstr.Find('W')>=0)
		{
			m_recvstr=m_recvstr.Right(m_recvstr.GetLength()-m_recvstr.Find('L')-1);
			m_recvstr=m_recvstr.Left(m_recvstr.Find('W'));
			if(_ttof(m_recvstr)>=32)
			{
				SetDlgItemText(IDC_EDIT_NORMAL,L"温度过高 需要隔离");
				CString temp;
				temp.Format(L"%d",m_num);
				m_list.InsertItem(m_num,temp);
				GetDlgItemText(IDC_EDIT_TIME,temp);
				m_list.SetItemText(m_num,1,temp);
				m_list.SetItemText(m_num,2,m_recvstr);
				saveImage(m_img,m_path,m_num);
				m_num++;
			}
			else
			{
				SetDlgItemText(IDC_EDIT_NORMAL,L"正常 可通过");
			}
			m_tempar.Add(m_recvstr);
			m_lineSeries.Clear();
			for (int i = 0; i < m_tempar.GetSize(); i++)
			{
				m_lineSeries.AddXY((double)i,_ttof(m_tempar[i]),NULL,NULL);
			}
			SetDlgItemText(IDC_EDIT_TEMP,m_recvstr);
		}
	}
	if (nIDEvent==2)
	{
		CString str;
		CTime tm;
		tm=CTime::GetCurrentTime();
		str=tm.Format("%Y.%m.%d %X");
		SetDlgItemText(IDC_EDIT_TIME,str);
	}
	if (nIDEvent==3)
	{
		if(m_camera.isOpened())
		{
			m_camera>>m_img;
		}
		showMatImgToWnd(m_pichwnd,m_img);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CCSocket_TCPDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

}

void CCSocket_TCPDlg::showMatImgToWnd(CWnd* pWnd, const cv::Mat& img)
{ 
	if(img.empty()) 
		return; 
	static BITMAPINFO *bitMapinfo = NULL;
	static bool First=TRUE;
	if(First)
	{  
		BYTE *bitBuffer = new BYTE[40+4*256];
		if(bitBuffer == NULL)
		{ 
			return;
		}
		First=FALSE;
		memset(bitBuffer, 0, 40+4*256);
		bitMapinfo = (BITMAPINFO *)bitBuffer;
		bitMapinfo->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
		bitMapinfo->bmiHeader.biPlanes   = 1;   
		for(int i=0; i<256; i++)
		{ 
			bitMapinfo->bmiColors[i].rgbBlue  =bitMapinfo->bmiColors[i].rgbGreen =bitMapinfo->bmiColors[i].rgbRed   =(BYTE) i;
		} 
	}
	bitMapinfo->bmiHeader.biHeight = -img.rows;   
	bitMapinfo->bmiHeader.biWidth = img.cols;
	bitMapinfo->bmiHeader.biBitCount= img.channels() *8;    

	CRect drect;       
	pWnd->GetClientRect(drect);    
	CClientDC dc(pWnd);
	HDC hDC =dc.GetSafeHdc();                  
	SetStretchBltMode(hDC, COLORONCOLOR);    
	StretchDIBits(hDC,
		0,
		0,
		drect.right,  
		drect.bottom,  
		0,
		0,
		img.cols,     
		img.rows,    
		img.data,   
		bitMapinfo,   
		DIB_RGB_COLORS, 
		SRCCOPY);
}

void CCSocket_TCPDlg::saveImage(cv::Mat image, string &outPutPath, int index)
{
	string strSaveName;
	char buffer[256];
	sprintf(buffer, "D%04d", index);
	strSaveName = buffer;
	m_strImgSavePath = outPutPath + "\\" + strSaveName;
	m_strImgSavePath += ".jpeg";
	m_list.SetItemText(m_num,3, CA2T(m_strImgSavePath.c_str()));
	imwrite(m_strImgSavePath.c_str(), image);
}

void CCSocket_TCPDlg::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CString strLangName;    // 选择语言的名称字符串   
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		strLangName = m_list.GetItemText(pNMListView->iItem,3);
		string temp= CT2A(strLangName.GetString());
		Mat image = imread(temp);        //读取一张图片
		imshow(temp,image);                //通过img窗体显示image图片
		waitKey(0);                
	}

}


// mfcwithcef3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mfcwithcef3.h"
#include "mfcwithcef3Dlg.h"
#include "afxdialogex.h"
#include "simple_app.h"
#include "simple_handler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CefRefPtr<SimpleHandler> g_handler;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// Cmfcwithcef3Dlg 对话框




Cmfcwithcef3Dlg::Cmfcwithcef3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmfcwithcef3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmfcwithcef3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cmfcwithcef3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &Cmfcwithcef3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cmfcwithcef3Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cmfcwithcef3Dlg 消息处理程序

BOOL Cmfcwithcef3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CreateBrowserAsChildWnd();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmfcwithcef3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmfcwithcef3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmfcwithcef3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cmfcwithcef3Dlg::CreateBrowser()
{
	CefWindowInfo window_info;

#if defined(OS_WIN)
	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	window_info.SetAsPopup(NULL, "cefsimple");
#endif

	// SimpleHandler implements browser-level callbacks.
	CefRefPtr<SimpleHandler> handler(new SimpleHandler());

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	std::string url;

	// Check if a "--url=" value was provided via the command-line. If so, use
	// that instead of the default URL.
	CefRefPtr<CefCommandLine> command_line =
		CefCommandLine::GetGlobalCommandLine();
	url = command_line->GetSwitchValue("url");
	if (url.empty())
		url = "http://www.baidu.com";

	// Create the first browser window.
	CefBrowserHost::CreateBrowser(window_info, handler.get(), url,
		browser_settings, NULL);
}

void Cmfcwithcef3Dlg::CreateBrowserAsChildWnd()
{
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.DeflateRect(1,1,1,1);

	RECT rcBrowser = *(LPRECT)rcClient;

	CefWindowInfo window_info;

#if defined(OS_WIN)
	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	window_info.SetAsChild(GetSafeHwnd(), rcBrowser);
#endif

	// SimpleHandler implements browser-level callbacks.
	g_handler = new SimpleHandler();

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	std::string url;

	// Check if a "--url=" value was provided via the command-line. If so, use
	// that instead of the default URL.
	CefRefPtr<CefCommandLine> command_line =
		CefCommandLine::GetGlobalCommandLine();
	url = command_line->GetSwitchValue("url");
	if (url.empty())
		url = "http://www.baidu.com";

	// Create the first browser window.
	CefBrowserHost::CreateBrowser(window_info, g_handler.get(), url,
		browser_settings, NULL);
}

VOID Cmfcwithcef3Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (g_handler.get())
	{
		ReposBrowserPos();
	}
}

void Cmfcwithcef3Dlg::ReposBrowserPos()
{
	g_handler->OnContainerWndSizeChanged(GetSafeHwnd());
}



BOOL Cmfcwithcef3Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	return CDialogEx::OnEraseBkgnd(pDC);
}


void Cmfcwithcef3Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	
}


void Cmfcwithcef3Dlg::OnClose()
{
	if (g_handler->IsDoCloseCalled())
	{
		CDialogEx::OnClose();
	}
	else
	{
		g_handler->CloseAllBrowsers(false);
		return;
	}
}


void Cmfcwithcef3Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true);
	if(IDOK == dlg.DoModal() )
	{
		CString strFilePath = dlg.GetPathName();
		
		g_handler->OpenURL((LPCTSTR)strFilePath);
	}

}


void Cmfcwithcef3Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_handler->Refresh();
}

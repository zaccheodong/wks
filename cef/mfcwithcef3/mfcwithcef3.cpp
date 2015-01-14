
// mfcwithcef3.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "mfcwithcef3.h"
#include "mfcwithcef3Dlg.h"

#include "simple_app.h"
#include "simple_handler.h" 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define CEF_ENABLE_SANDBOX 0
// Cmfcwithcef3App

BEGIN_MESSAGE_MAP(Cmfcwithcef3App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cmfcwithcef3App 构造
#include <string>
Cmfcwithcef3App::Cmfcwithcef3App()
{
	// TODO: 在此处添加构造代码，
}


// 唯一的一个 Cmfcwithcef3App 对象

Cmfcwithcef3App theApp;


// Cmfcwithcef3App 初始化

BOOL Cmfcwithcef3App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
	void* sandbox_info = NULL;

#if CEF_ENABLE_SANDBOX
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(this->m_hInstance);

	// SimpleApp implements application-level callbacks. It will create the first
	// browser instance in OnContextInitialized() after CEF has initialized.
	cefapp_ = new SimpleApp();

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, cefapp_.get(), sandbox_info);
	if (exit_code >= 0) {
		// render 和 gpu进程的返回值>=0,必须直接返回false导致，CWinApp的ExitInstance被调用		
		return FALSE;		
	}
	

	

	// Specify CEF global settings here.
	CefSettings settings;

#if !CEF_ENABLE_SANDBOX
	settings.no_sandbox = true;
#endif
	
	// Initialize CEF.
	CefInitialize(main_args, settings, cefapp_.get(), sandbox_info);

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	Cmfcwithcef3Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}


	//CefShutdown();
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



BOOL Cmfcwithcef3App::PumpMessage()
{
	
	CefDoMessageLoopWork();
	return CWinApp::PumpMessage();
}


int Cmfcwithcef3App::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if( cefapp_->IsBrowserProcess())
	{
		CefShutdown();
	}
	cefapp_ = NULL;
	return CWinApp::ExitInstance();
}

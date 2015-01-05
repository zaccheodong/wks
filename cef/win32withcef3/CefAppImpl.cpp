
#include "CefAppImpl.h"
#include "ClientHandler.h"


CCefAppImpl::CCefAppImpl(void)
{
	
}


CCefAppImpl::~CCefAppImpl(void)
{
}

// bool CCefAppImpl::Init(HINSTANCE hInstance)
// {
// 	if (_inited)
// 	{
// 		assert(false);
// 		return false;
// 	}
// 
// 
// 	void* sandbox_info = NULL;
// 	CefMainArgs main_args(hInstance);
// 	CefSettings cef_setttings;
// 	cef_setttings.single_process = true;
// 	cef_setttings.no_sandbox = true;
// 
// 
// 	int exit_code = CefExecuteProcess(main_args, dynamic_cast<CefApp*>(this), NULL);
// 	if (exit_code >= 0) {
// 		// The sub-process has completed so return here.
// 		return exit_code;
// 	}
// 
// 	_inited= CefInitialize(main_args,cef_setttings,dynamic_cast<CefApp*>(this),NULL);
// 
// 	return _inited;
// }

// bool CCefAppImpl::IsInited() const
// {
// 	return _inited;
// }

// void CCefAppImpl::Shutdown()
// {
// 	 CefShutdown();
// 	 _inited = false;
// }

void CCefAppImpl::OnContextInitialized()
{
	// Information used when creating the native window.
	CefWindowInfo window_info;

#if defined(OS_WIN)
	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	window_info.SetAsPopup(NULL, "dzc");
#endif

	// SimpleHandler implements browser-level callbacks.
	CefRefPtr<CClientHandler> handler(new CClientHandler());

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
	//CefBrowserHost::CreateBrowserSync(window_info, handler.get(), url,
		//browser_settings, NULL);
}



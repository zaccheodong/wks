// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cefsimple/simple_app.h"

#include <string>

#include "cefsimple/simple_handler.h"
#include "cefsimple/util.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include <iostream>
#include <fstream>
#include "CefV8HandlerImpl.h"
SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
	return;
  REQUIRE_UI_THREAD();

  // Information used when creating the native window.
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

void SimpleApp::OnWebKitInitialized()
{
	static const CefString jsfilepath(L"extension.js");
	static const CefString extension_name(L"dzc");
	static CefString js_code ;

	std::ifstream f(jsfilepath.ToString());
	std::string strJsCode ;
	char c;
	while( (c= f.get()) != EOF )
	{
		strJsCode.append(1,c);
	};


	CefRegisterExtension(extension_name, strJsCode, new CefV8HandlerImpl);

}

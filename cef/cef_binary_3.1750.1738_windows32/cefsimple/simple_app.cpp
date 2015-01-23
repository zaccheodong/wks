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

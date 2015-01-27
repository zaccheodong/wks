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
#include "utilities.h"
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


	CefRegisterExtension(extension_name, strJsCode, new CefV8HandlerImpl(this));

}

//处理命令行参数
void SimpleApp::OnBeforeCommandLineProcessing( const CefString& process_type, 
	CefRefPtr<CefCommandLine> command_line )
{
	if (process_type.length() == 0)//主进程
	{
		if( command_line->HasSwitch(L"bkimage"))
		{
			CefString value = command_line->GetSwitchValue(L"bkimage");
		}
	}
	else
	{
		std::string type = process_type.ToString();
	}
}

bool SimpleApp::AddMessageInfo( int32 id ,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Value> callbackFunction )
{
	callbackmap_[id] = std::make_pair(context,callbackFunction);
	return true;
}

//render进程中被调用
bool SimpleApp::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, 
	CefRefPtr<CefProcessMessage> message )
{
	if (message->GetName() == "invokeCallback")
	{
		CefRefPtr<CefListValue> messageArgs = message->GetArgumentList();
		if( messageArgs->GetSize() <2 )
		{
			fprintf(stderr,"SimpleApp::OnProcessMessageReceived receive invokeCallback param must more than 2\n");
			return true;
		}

		int32 callbackid = messageArgs->GetInt(0);
		const int32 error_no   = messageArgs->GetInt(1);

		
		if( callbackmap_.end() == callbackmap_.find(callbackid))
		{
			fprintf(stderr,"SimpleApp::OnProcessMessageReceived can not find msgid:%d\n",callbackid);
			return true;
		}

		CefRefPtr<CefV8Context> context = callbackmap_[callbackid].first;
		CefRefPtr<CefV8Value> callbackFunction = callbackmap_[callbackid].second;

		 context->Enter();
		 if (context->GetBrowser())
		 {
			 CefV8ValueList arguments;
			 for (size_t i = 1; i < messageArgs->GetSize(); i++)
			 {
				arguments.push_back(ListValueToV8Value(messageArgs,i));
			 }
			 callbackFunction->ExecuteFunction(NULL,arguments);
			 context->Exit();
			 callbackmap_.erase(callbackid);
		 }

	}
	
	return true;

}

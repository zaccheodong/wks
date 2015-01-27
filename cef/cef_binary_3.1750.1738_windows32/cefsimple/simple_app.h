// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "include/cef_render_process_handler.h"

class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler,
				  public CefRenderProcessHandler{
 public:
  SimpleApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()OVERRIDE
  {
	  return this; 
  }

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()OVERRIDE 
  {
	  return this;
  }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;
  //CefRenderProcessHandler
  virtual void OnWebKitInitialized() OVERRIDE;


  virtual void OnBeforeCommandLineProcessing( const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;


  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
										CefProcessId source_process,
										CefRefPtr<CefProcessMessage> message) OVERRIDE;
public:
	bool AddMessageInfo(int32 id ,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Value> callbackFunction);
private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);

private:
	typedef std::map<int32 ,std::pair<CefRefPtr<CefV8Context>,CefRefPtr<CefV8Value>>> CallbackMap;
	CallbackMap callbackmap_;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

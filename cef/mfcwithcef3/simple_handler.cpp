// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "stdafx.h"
#include "simple_handler.h"

#include <sstream>
#include <string>

#include "cefsimple/util.h"
#include "include/cef_app.h"
#include "include/cef_runnable.h"

namespace {

	SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler()
	: is_closing_(false) {
		ASSERT(!g_instance);
		g_instance = this;
		is_doclose_called = false;
}

SimpleHandler::~SimpleHandler() {
	g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
	return g_instance;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (browser_list_.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	is_doclose_called = true;
	return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}

	if (browser_list_.empty()) {
		// All browser windows have closed. Quit the application message loop.
		
		//CefQuitMessageLoop();
	}
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl) {
		REQUIRE_UI_THREAD();

		// Don't display an error for downloaded files.
		if (errorCode == ERR_ABORTED)
			return;

		// Display a load error message.
		std::stringstream ss;
		ss << "<html><body bgcolor=\"white\">"
			"<h2>Failed to load URL " << std::string(failedUrl) <<
			" with error " << std::string(errorText) << " (" << errorCode <<
			").</h2></body></html>";
		frame->LoadString(ss.str(), failedUrl);
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			NewCefRunnableMethod(this, &SimpleHandler::CloseAllBrowsers,
			force_close));
		return;
	}

	if (browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}

void SimpleHandler::OnContainerWndSizeChanged(HWND hParent)
{
	RECT rcClient;
	::GetClientRect(hParent,&rcClient);

	

	CRect rcBrowser(rcClient);
	rcBrowser.DeflateRect(0,20,0,0);

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
	{
		HWND hWnd = (*it)->GetHost()->GetWindowHandle();
		::MoveWindow(hWnd,rcBrowser.left,rcBrowser.top,rcBrowser.Width(),rcBrowser.Height(),true);	
	}

}

void SimpleHandler::OpenURL( CefString url )
{

	if (0 !=browser_list_.size() )
	{
		CefRefPtr<CefBrowser> browser = *browser_list_.begin();
		browser->GetMainFrame()->LoadURL(url);
	}
}

void SimpleHandler::Refresh()
{
	if (0 !=browser_list_.size() )
	{
		CefRefPtr<CefBrowser> browser = *browser_list_.begin();
		browser->Reload();
	}
}

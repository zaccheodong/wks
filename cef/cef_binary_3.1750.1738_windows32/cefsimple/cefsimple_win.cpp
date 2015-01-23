// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <windows.h>

#include "cefsimple/simple_app.h"
#include "include/cef_sandbox_win.h"
#include "simple_handler.h"


// Set to 0 to disable sandbox support.
#define CEF_ENABLE_SANDBOX 0

#if CEF_ENABLE_SANDBOX
// The cef_sandbox.lib static library is currently built with VS2010. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

const wchar_t* _szClassName =L"CLS_CEF_IMG_VIEWER";
HWND _mainWnd;

CefRefPtr<SimpleHandler> g_handler;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam);

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CEFCLIENT));
	wcex.hCursor       = NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName  = NULL;//MAKEINTRESOURCE(IDC_CEFCLIENT);
	wcex.lpszClassName = _szClassName;
	wcex.hIconSm       = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// Entry point function for all processes.
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  void* sandbox_info = NULL;

#if CEF_ENABLE_SANDBOX
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif

  // Provide CEF with command-line arguments.
  CefMainArgs main_args(hInstance);

  // SimpleApp implements application-level callbacks. It will create the first
  // browser instance in OnContextInitialized() after CEF has initialized.
  CefRefPtr<SimpleApp> app(new SimpleApp);

  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  
  MessageBox(NULL,L"begin",NULL,1);
  int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Specify CEF global settings here.
  CefSettings settings;

#if !CEF_ENABLE_SANDBOX
  settings.no_sandbox = true;
#endif


  // Initialize CEF.
  CefInitialize(main_args, settings, app.get(), sandbox_info);


  MyRegisterClass(hInstance);

  const int nWidth  = GetSystemMetrics(SM_CXSCREEN);
  const int nHeight = GetSystemMetrics(SM_CYSCREEN);
  _mainWnd = CreateWindow(_szClassName,
	  L"dzc",
	  WS_POPUP|WS_EX_TOPMOST,
	  0,0,nWidth, nHeight,NULL,NULL,hInstance,NULL);
  ShowWindow(_mainWnd,nCmdShow);

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();

  // Shut down CEF.
  CefShutdown();

  return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam) 
{
		PAINTSTRUCT ps;
		HDC hdc;
		// Callback for the main window
		switch (message) 
		{
		case WM_CREATE:
			{
				// Create the single static handler class instance
				g_handler = new SimpleHandler();

				CefWindowInfo info;
				CefBrowserSettings settings;		

				RECT rect;
				::GetClientRect(hWnd,&rect);
				// Initialize window info to the defaults for a child window.
				info.SetAsChild(hWnd, rect);

				// Creat the new child browser window
				CefBrowserHost::CreateBrowser(info, g_handler.get(),
					L"http://127.0.0.1:37397/index.html", settings, NULL);
				break;
			}
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
				return 0;
			}
			

		case WM_SETFOCUS:
			{
				if (g_handler.get() && g_handler->GetBrowser()) 
				{
					g_handler->GetBrowser()->GetHost()->SetFocus(true);
				}
				return 0;
			}
			

		case WM_SIZE:
			// Minimizing resizes the window to 0x0 which causes our layout to go all
			// screwy, so we just ignore it.
			if (wParam != SIZE_MINIMIZED && g_handler.get() &&
				g_handler->GetBrowser()) {
					CefWindowHandle hwnd =
						g_handler->GetBrowser()->GetHost()->GetWindowHandle();
					if (hwnd) {
						// Resize the browser window and address bar to match the new frame
						// window size
						RECT rect;
						GetClientRect(hWnd, &rect);
						

						HDWP hdwp = BeginDeferWindowPos(1);
						hdwp = DeferWindowPos(hdwp, hwnd, NULL,
							rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
							SWP_NOZORDER);
						EndDeferWindowPos(hdwp);
					}
			}
			break;

		case WM_ERASEBKGND:
			if (g_handler.get() && g_handler->GetBrowser()) 
			{
				CefWindowHandle hwnd =
					g_handler->GetBrowser()->GetHost()->GetWindowHandle();
				if (hwnd) {
					// Dont erase the background if the browser window has been loaded
					// (this avoids flashing)
					return 0;
				}
			}
			break;

		case WM_CLOSE:
			if (g_handler.get() && !g_handler->IsClosing()) 
			{
				CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
				if (browser.get()) {
					// Notify the browser window that we would like to close it. This
					// will result in a call to ClientHandler::DoClose() if the
					// JavaScript 'onbeforeunload' event handler allows it.
					browser->GetHost()->CloseBrowser(false);

					// Cancel the close.
					return 0;
				}
			}

			// Allow the close.
			break;
		}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
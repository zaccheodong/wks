#pragma once
#include "include\cef_render_process_handler.h"

//Will be called on the render process  main thread 
class CefRenderProcessHandlerImpl :
	public CefRenderProcessHandler
{
public:
	CefRenderProcessHandlerImpl(void);
	~CefRenderProcessHandlerImpl(void);
private:
	IMPLEMENT_REFCOUNTING(CefRenderProcessHandlerImpl);
};


#pragma once
#include "include\cef_render_process_handler.h"
#include "CefV8HandlerImpl.h"
//Will be called on the render process  main thread 
class CefRenderProcessHandlerImpl :
	public CefRenderProcessHandler
{
public:
	CefRenderProcessHandlerImpl(void);
	~CefRenderProcessHandlerImpl(void);
private:
	IMPLEMENT_REFCOUNTING(CefRenderProcessHandlerImpl);
	virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
	{
		OutputDebugString(L"OnRenderThreadCreated");
	}

	virtual void OnWebKitInitialized()
	{
		static const CefString jsfilepath(L"extension.js");
		static const CefString extension_name(L"dzc");
		static CefString js_code ;
		ReadJsFile(jsfilepath,js_code);
		std::string filecode = js_code.ToString();
		
		CefRegisterExtension("v8/test", filecode, new CefV8HandlerImpl);
	}
	bool ReadJsFile( const CefString& jsfilepath, CefString& js_code );
	void ReadJsInmemory( CefString& js_code );
};


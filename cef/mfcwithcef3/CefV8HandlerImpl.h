#pragma once
#include "include\cef_v8.h"
class CefV8HandlerImpl :
	public CefV8Handler
{
public:
	CefV8HandlerImpl(void);
	~CefV8HandlerImpl(void);

	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception);

private:
	int GenMsgID();
private:
	IMPLEMENT_REFCOUNTING(CefV8HandlerImpl);
};


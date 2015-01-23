#include "CefV8HandlerImpl.h"


CefV8HandlerImpl::CefV8HandlerImpl(void)
{
}


CefV8HandlerImpl::~CefV8HandlerImpl(void)
{
}

bool CefV8HandlerImpl::Execute( const CefString& name, CefRefPtr<CefV8Value> object,
	const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception )
{
	if (name == "getBKImage" )
	{
	//	MessageBox(NULL,L"Execute getBKImage",NULL,0);
		retval = CefV8Value::CreateString(L"file:///E:/code/github/wks/cef/cef_binary_3.1750.1738_windows32/image_viewer/Desert.jpg");
		return true;
	}
	return false;
}

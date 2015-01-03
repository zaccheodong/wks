#include "StdAfx.h"
#include "CefAppImpl.h"

CefRefPtr<CCefAppImpl> CCefAppImpl::_app;

CCefAppImpl::CCefAppImpl(void)
{
	_inited = false;
}


CCefAppImpl::~CCefAppImpl(void)
{
}

bool CCefAppImpl::Init(HINSTANCE hInstance)
{
	if (_inited)
	{
		assert(false);
		return false;
	}

	void* sandbox_info = NULL;
	CefMainArgs main_args(hInstance);
	CefSettings cef_setttings;
	cef_setttings.single_process = true;
	cef_setttings.no_sandbox = true;

	_inited= CefInitialize(main_args,cef_setttings,dynamic_cast<CefApp*>(this),sandbox_info);

	return _inited;
}

bool CCefAppImpl::IsInited() const
{
	return _inited;
}

void CCefAppImpl::Shutdown()
{
	 CefShutdown();
	 _inited = false;
}



#pragma once


#include "include/cef_app.h"
class CCefAppImpl :public CefApp,
	public CefBrowserProcessHandler
{
public: 
	CCefAppImpl(void);
	virtual ~CCefAppImpl(void);

public:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()OVERRIDE{return this;}

	virtual void OnContextInitialized() OVERRIDE;

private:
	IMPLEMENT_REFCOUNTING(CCefAppImpl);
};


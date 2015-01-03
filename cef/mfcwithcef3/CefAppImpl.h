#pragma once

#include "include/cef_base.h"
#include "include/cef_app.h"
class CCefAppImpl /*:public CefApp*/
{
private: //singleton
	CCefAppImpl(void);
	virtual ~CCefAppImpl(void);
public:
	static CefRefPtr<CCefAppImpl> Instance()
	{
		if (!_app.get())
		{
			_app = new CCefAppImpl();			
		}
		return _app;
		
	}
public:
	bool IsInited() const;
	bool Init(HINSTANCE hInstance);
	void Shutdown();
private:
	IMPLEMENT_REFCOUNTING(CCefAppImpl);
private:
	static CefRefPtr<CCefAppImpl> _app;

private:
	bool _inited;
};


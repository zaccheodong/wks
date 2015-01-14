#include "StdAfx.h"
#include "CefRenderProcessHandlerImpl.h"
#include <fstream>
#include <string>

CefRenderProcessHandlerImpl::CefRenderProcessHandlerImpl(void)
{
}


CefRenderProcessHandlerImpl::~CefRenderProcessHandlerImpl(void)
{
}

bool CefRenderProcessHandlerImpl::ReadJsFile( const CefString& jsfilepath, CefString& js_code )
{
	
    std::ifstream f(jsfilepath.ToString());
	std::string strJsCode ;
	char c;
	while( (c= f.get()) != EOF )
	{
		strJsCode.append(1,c);
	}

js_code.FromString(strJsCode);
	return false;
}

void CefRenderProcessHandlerImpl::ReadJsInmemory( CefString& js_code )
{
	
}

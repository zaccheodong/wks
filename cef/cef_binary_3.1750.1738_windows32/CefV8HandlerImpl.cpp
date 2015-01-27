#include "CefV8HandlerImpl.h"
#include "./cefsimple/simple_app.h"
#include "utilities.h"

CefV8HandlerImpl::CefV8HandlerImpl(CefRefPtr<SimpleApp> cefApp)
{
	cef_app_ = cefApp;
}


CefV8HandlerImpl::~CefV8HandlerImpl(void)
{
}

bool CefV8HandlerImpl::Execute( const CefString& name, CefRefPtr<CefV8Value> object,
	const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception )
{
	
	
	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
	CefRefPtr<CefListValue> messageArgs = message->GetArgumentList();
	if(arguments.size() >0 && !arguments[0]->IsFunction())
	{
		std::string functionName = name;
		fprintf(stderr,"Function callled with no callback as first param :%s\n",functionName.c_str());
		return false;
	}

	static int msgid = 0;
	msgid++;
	if (arguments.size()>0)
	{
		cef_app_->AddMessageInfo(msgid,CefV8Context::GetCurrentContext(),arguments[0]);
		messageArgs->SetInt(0,msgid);
	}

	for (int i = 1; i < (int)arguments.size() ;i++)
	{
		SetListValue(messageArgs,i,arguments[i]);
	}

	CefRefPtr<CefBrowser> browser = 
		CefV8Context::GetCurrentContext()->GetBrowser();
	browser->SendProcessMessage(PID_BROWSER,message);

	return true;
}

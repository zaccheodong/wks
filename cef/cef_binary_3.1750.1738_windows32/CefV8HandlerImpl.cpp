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

	if(arguments.size() >0)//有参数
	{
		if(!arguments[0]->IsFunction())//第一个参数不是回调函数
		{
			for (size_t i = 0; i < arguments.size(); i++)
			{
				SetListValue(messageArgs,i,arguments[i]);
			}
		}
		else//第一个参数是回调函数
		{
			static int msgid = 0;
			msgid++;
			cef_app_->AddMessageInfo(msgid,CefV8Context::GetCurrentContext(),arguments[0]);
			messageArgs->SetInt(0,msgid);
			

			//从第二个开始
			for (size_t i = 1; i < arguments.size() ;i++)
			{
				SetListValue(messageArgs,i,arguments[i]);
			}
		}


	}
	

	CefRefPtr<CefBrowser> browser = 
		CefV8Context::GetCurrentContext()->GetBrowser();
	browser->SendProcessMessage(PID_BROWSER,message);

	return true;
}

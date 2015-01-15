#include "StdAfx.h"
#include "CefV8HandlerImpl.h"
#include <sstream>

CefV8HandlerImpl::CefV8HandlerImpl(void)
{
}


CefV8HandlerImpl::~CefV8HandlerImpl(void)
{
}

bool CefV8HandlerImpl::Execute( const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception )
{	

	CefRefPtr<CefV8Context> curr_v8_context = CefV8Context::GetCurrentContext();

	if (name=="getcurrentdir")
	{
		const int arc = arguments.size();
		for (int i = 0; i < arc ; i++)
		{
			CefString prefix ;
			CefRefPtr<CefV8Value> v8_value = arguments.at(i);
			if (v8_value->IsString())
			{
				prefix = v8_value->GetStringValue();
			}
			if (v8_value->IsFunction())
			{
				CefString fun_name = v8_value->GetFunctionName();

				CefV8ValueList cb_arg_list;
				std::stringstream ss;
				ss<<prefix.ToString()<<"ddddddddd";

				cb_arg_list.push_back(CefV8Value::CreateString(ss.str()));
				v8_value->ExecuteFunction(object,cb_arg_list);
			}
		}

		
	}

	
	

	return true;
}

int CefV8HandlerImpl::GenMsgID()
{
	static int msgID =0;
	msgID++;
	if (0 == msgID)
	{
		msgID++;
	}
	return msgID;
}

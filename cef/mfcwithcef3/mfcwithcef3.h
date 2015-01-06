
// mfcwithcef3.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// Cmfcwithcef3App:
// 有关此类的实现，请参阅 mfcwithcef3.cpp
//
#include "simple_app.h"
class Cmfcwithcef3App : public CWinApp
{
public:
	Cmfcwithcef3App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual BOOL PumpMessage();
	virtual int ExitInstance();

private:
	CefRefPtr<SimpleApp> cefapp_;
};

extern Cmfcwithcef3App theApp;
#pragma once 
#include "include/cef_base.h"
#include "include/cef_values.h"
#include "include/cef_v8.h"
void SetListValue(CefRefPtr<CefListValue> list, int index,	CefRefPtr<CefV8Value> value) ;
void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
CefRefPtr<CefV8Value> ListValueToV8Value(CefRefPtr<CefListValue> value, int index);
void SetListValue(CefRefPtr<CefV8Value> list, int index,CefRefPtr<CefListValue> value);
void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);
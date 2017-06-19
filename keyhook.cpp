#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#define DEF_PROCESS_NAME "notepad.exe"
HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpreserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hinstDll;
		break;
	}
	return TRUE;
}
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MessageBox(NULL, L"GetMessage!", L"Message", 0);

	//若非notepad.exe 则调用CallNextHookEx()函数，将消息传递给应用程序或者下一个钩子
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}
#ifdef __cplusplus
extern "C"{
#endif 
	__declspec(dllexport) void HookStart(DWORD threadId)
	{
		//关键函数这一个，安装消息钩子
		g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, threadId);

	}
	__declspec(dllexport) void HookStop()
	{
		if (g_hHook)
		{
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}
#ifdef __cplusplus
}
#endif

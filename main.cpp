#include "stdafx.h"
#include "windows.h"
#define DEF_DLL_NAME "keyhook.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP "HookStop"
typedef void(*PFN_HOOKSTART)(DWORD);
typedef void(*PFN_HOOKSTOP)();

HMODULE hDll;
PFN_HOOKSTART HookStart;
PFN_HOOKSTOP HookStop;

DWORD WINAPI mouseLLHookThreadProc(LPVOID lParam)
{
	HWND hwnd = FindWindow(L"Notepad", NULL);
	DWORD notpadId = GetWindowThreadProcessId(hwnd, NULL);

	HMODULE hDll = NULL;
	PFN_HOOKSTART HookStart = NULL;
	PFN_HOOKSTOP  HookStop = NULL;
	hDll = LoadLibraryA(DEF_DLL_NAME);
	//获取导出函数地址
	HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
	HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);
	//开始勾取
	HookStart(notpadId);

	MSG msg;
	
	while (GetMessage(&msg, NULL, 0, 0) != FALSE)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//终止勾取
	HookStop();

	//卸载KeyHook.dll
	FreeLibrary(hDll);

	return 0;
}

void _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hthread;
	
	hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mouseLLHookThreadProc, NULL, 0, NULL);//获得句柄
	
	DWORD re = WaitForSingleObject(hthread, 100000);

	if (hDll != NULL)
	{
		//终止勾取
		HookStop();

		//卸载KeyHook.dll
		FreeLibrary(hDll);
	}


}

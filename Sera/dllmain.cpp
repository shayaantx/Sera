// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "util.h"

typedef int(__cdecl *check_modified_dll)(void *Src, int a2);
check_modified_dll check_modified_dll_method;

int __cdecl checkModifiedDll(void* src, int a2) {
	return 1;
}

typedef bool(__stdcall *check_for_startup_exe)(DWORD* thisx);
check_for_startup_exe check_for_startup_exe_method;

bool __stdcall checkForStartupExe(DWORD* thisx) {
	return true;
}

typedef void(__cdecl *assert)(LPCSTR a1, LPCSTR lpString, int a3, char a4);
assert assert_method;

void __cdecl assertHook(LPCSTR a1, LPCSTR lpString, int a3, char a4) {
	log("Assert called, str1=%s, str2=%s", a1, lpString);
	assert_method(a1, lpString, a3, a4);
}

typedef int(__cdecl *debug_log)(wchar_t *a1, size_t a2, size_t a3, const wchar_t **a4, va_list a5);
debug_log debug_log_method;

int __cdecl debugLog(wchar_t *a1, size_t a2, size_t a3, const wchar_t **a4, va_list a5) {
	int result = debug_log_method(a1, a2, a3, a4, a5);
	logW(a1);
	return result;
}

void gameModifications() {
	log("Sera Mod");
	DWORD base = (DWORD)GetModuleHandle(NULL);
	DWORD dwBack;

	BYTE skipIsDebuggerPresent[] = { 0xb8, 0x00, 0x00, 0x00, 0x00, 0x90};
	WriteBytes(base + 0x1808, skipIsDebuggerPresent, 6);
	WriteBytes(base + 0x9731B, skipIsDebuggerPresent, 6);
	WriteBytes(base + 0x981B9, skipIsDebuggerPresent, 6);
	WriteBytes(base + 0xD507F1, skipIsDebuggerPresent, 6);

	check_modified_dll_method = (check_modified_dll)DetourFunc((BYTE*)base + 0xB707E0, (BYTE*)checkModifiedDll, 7);
	VirtualProtect(check_modified_dll_method, 4, PAGE_EXECUTE_READWRITE, &dwBack);

	check_for_startup_exe_method = (check_for_startup_exe)DetourClassFunc((BYTE*)base + 0xD1AF40, (BYTE*)checkForStartupExe, 9);
	VirtualProtect(check_for_startup_exe_method, 4, PAGE_EXECUTE_READWRITE, &dwBack);

	assert_method = (assert)DetourFunc((BYTE*)base + 0xB5C160, (BYTE*)assertHook, 7);
	VirtualProtect(assert_method, 4, PAGE_EXECUTE_READWRITE, &dwBack);

	debug_log_method = (debug_log)DetourFunc((BYTE*)base + 0xB6C9C0, (BYTE*)debugLog, 6);
	VirtualProtect(debug_log_method, 4, PAGE_EXECUTE_READWRITE, &dwBack);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		gameModifications();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


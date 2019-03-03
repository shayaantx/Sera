#include "util.h"

FILE* gowLogFile = nullptr;

void write_time(FILE* logHandle)
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	fwprintf(logHandle, L"%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

void log(char* format, ...)
{
	if (!gowLogFile) {
		gowLogFile = fopen("log.txt", "wt");
	}

	va_list	arg;
	va_start(arg, format);

	write_time(gowLogFile);

	vfprintf(gowLogFile, format, arg);
	fprintf(gowLogFile, "\n");

	va_end(arg);

	fflush(gowLogFile);
}

void logW(wchar_t* format, ...) {
	if (!gowLogFile) {
		gowLogFile = fopen("log.txt", "wt");
	}

	va_list	arg;
	va_start(arg, format);

	write_time(gowLogFile);

	vfwprintf(gowLogFile, format, arg);
	fwprintf(gowLogFile, L"\n");

	va_end(arg);

	fflush(gowLogFile);
}

void WriteBytes(DWORD destAddress, LPVOID bytesToWrite, int numBytes)
{
	DWORD OldProtection;
	DWORD temp;

	VirtualProtect((LPVOID)destAddress, numBytes, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy((LPVOID)destAddress, bytesToWrite, numBytes);
	VirtualProtect((LPVOID)destAddress, numBytes, OldProtection, &temp); //quick fix for exception that happens here
}

template<int len>inline void NopFill(DWORD address)
{
	BYTE bytesArray[len];
	memset(bytesArray, 0x90, len);
	WriteBytes(address, bytesArray, len);
}

void *DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwBack;

	VirtualProtect(src, len, PAGE_READWRITE, &dwBack);

	memcpy(jmp, src, len);
	jmp += len;

	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;

	for (int i = 5; i < len; i++)
		src[i] = 0x90;

	VirtualProtect(src, len, dwBack, &dwBack);

	return (jmp - len);
}

void *DetourClassFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 8);

	DWORD dwBack;
	VirtualProtect(src, len, PAGE_READWRITE, &dwBack);
	memcpy(jmp + 3, src, len);

	// calculate callback function call
	jmp[0] = 0x58;							// pop eax
	jmp[1] = 0x59;							// pop ecx
	jmp[2] = 0x50;							// push eax
	jmp[len + 3] = 0xE9;						// jmp
	*(DWORD*)(jmp + len + 4) = (DWORD)((src + len) - (jmp + len + 3)) - 5;

	// detour source function call
	src[0] = 0x58;							// pop eax;
	src[1] = 0x51;							// push ecx
	src[2] = 0x50;							// push eax
	src[3] = 0xE9;							// jmp
	*(DWORD*)(src + 4) = (DWORD)(dst - (src + 3)) - 5;

	for (int i = 8; i < len; i++)
		src[i] = 0x90;

	VirtualProtect(src, len, dwBack, &dwBack);

	return jmp;
}
#pragma once
#include "stdafx.h"
#include <io.h>
#include <iostream>
#include <stdio.h>
#include <mutex>
#include <fcntl.h>

void WriteBytes(DWORD destAddress, LPVOID bytesToWrite, int numBytes);
template<int len>inline void NopFill(DWORD address);
void *DetourFunc(BYTE *src, const BYTE *dst, const int len);
void *DetourClassFunc(BYTE *src, const BYTE *dst, const int len); 
void log(char* format, ...);
void logW(wchar_t* format, ...);
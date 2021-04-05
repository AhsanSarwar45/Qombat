#pragma once

#include "Core/Aliases.hpp"
// Platform detection using predefined macros

//==============================================
// WINDOWS
//==============================================
#ifdef _WIN32

#include <stdlib.h>
#include <windows.h>
constexpr size_t GetCacheLineSize()
{
	size_t lineSize = 0;
	DWORD bufferSize = 0;
	DWORD i = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = 0;

	GetLogicalProcessorInformation(0, &bufferSize);
	buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferSize);
	GetLogicalProcessorInformation(&buffer[0], &bufferSize);

	for (i = 0; i != bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i)
	{
		if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
		{
			lineSize = buffer[i].Cache.LineSize;
			break;
		}
	}

	free(buffer);
	return lineSize;
}
/* Windows x64/x86 */
#ifdef _WIN64
/* Windows x64  */
#define QMBT_PLATFORM_WINDOWS

constexpr const char* GetOSName()
{
	return "Windows x64";
}

#else
/* Windows x86 */
#error "x86 Builds are not supported!"

constexpr const char* GetOSName()
{
	return "Windows x86";
}
#endif

//==============================================
// MACOS and IOS
//==============================================
#elif defined(__APPLE__) || defined(__MACH__)
#include <sys/sysctl.h>
inline size_t GetCacheLineSize()
{
	size_t lineSize = 0;
	size_t sizeOfLineSize = sizeof(lineSize);
	sysctlbyname("hw.cachelinesize", &lineSize, &sizeOfLineSize, 0, 0);
	return lineSize;
}
#include <TargetConditionals.h>
/* 
TARGET_OS_MAC exists on all the apple platforms
so we must check all of them (in this order)
to ensure that we're running on MAC
and not some other Apple platform 
*/
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"

#elif TARGET_OS_IPHONE == 1
#define QMBT_PLATFORM_IOS
#error "IOS is not supported!"

#elif TARGET_OS_MAC == 1
#define QMBT_PLATFORM_MACOS
#error "MacOS is not supported!"

#else
#error "Unknown Apple platform!"
#endif

//==============================================
// ANDROID
//==============================================
/* 
We also have to check __ANDROID__ before __linux__
since android is based on the linux kernel
it has __linux__ defined 
 */
#elif defined(__ANDROID__)
#define QMBT_PLATFORM_ANDROID
#error "Android is not supported!"

//==============================================
// LINUX
//==============================================
#elif defined(__linux__)
#define QMBT_PLATFORM_LINUX
constexpr const char* GetOSName()
{
	return "Linux";
}

#include <stdio.h>
inline size_t GetCacheLineSize()
{
	FILE* p = 0;
	p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
	unsigned int lineSize = 0;
	if (p)
	{
		fscanf(p, "%d", &lineSize);
		fclose(p);
	}
	return lineSize;
}
#else

/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection
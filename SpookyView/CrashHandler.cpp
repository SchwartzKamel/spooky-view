#include "stdafx.h"
#include "CrashHandler.h"
#include "Logger.h"
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

namespace
{
	LPTOP_LEVEL_EXCEPTION_FILTER s_previousFilter = NULL;

	LONG WINAPI UnhandledFilter(EXCEPTION_POINTERS* info)
	{
		if (info && info->ExceptionRecord)
		{
			LOG_ERROR("Unhandled exception: code=0x%08lX flags=0x%08lX addr=%p",
				info->ExceptionRecord->ExceptionCode,
				info->ExceptionRecord->ExceptionFlags,
				info->ExceptionRecord->ExceptionAddress);
		}
		else
		{
			LOG_ERROR("Unhandled exception (no record)");
		}

		// Write a minidump next to the log file.
		wchar_t dumpPath[MAX_PATH];
		const wchar_t* dir = Logger::Instance().LogDirectory();
		if (dir && dir[0])
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			// %ls is portable in MSVC; under MinGW UCRT _snwprintf_s with %ls
			// is reliable for wchar_t* substitution.
			_snwprintf(dumpPath, MAX_PATH,
				L"%ls\\spookyview-crash-%04u%02u%02u-%02u%02u%02u-%lu.dmp",
				dir,
				st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond,
				GetCurrentProcessId());
			dumpPath[MAX_PATH - 1] = L'\0';

			HANDLE hDump = CreateFileW(dumpPath, GENERIC_WRITE, 0, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hDump != INVALID_HANDLE_VALUE)
			{
				MINIDUMP_EXCEPTION_INFORMATION mei;
				mei.ThreadId = GetCurrentThreadId();
				mei.ExceptionPointers = info;
				mei.ClientPointers = FALSE;

				BOOL ok = MiniDumpWriteDump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					hDump,
					MiniDumpNormal,
					info ? &mei : NULL,
					NULL,
					NULL);
				CloseHandle(hDump);
				if (ok)
				{
					LOG_ERROR("Minidump written: %ls", dumpPath);
				}
				else
				{
					LOG_ERROR("MiniDumpWriteDump failed: lastError=%lu", GetLastError());
				}
			}
		}

		if (s_previousFilter)
		{
			return s_previousFilter(info);
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}
}

void CrashHandler::Install()
{
	static bool installed = false;
	if (installed) return;
	s_previousFilter = SetUnhandledExceptionFilter(UnhandledFilter);
	installed = true;
	LOG_INFO("Crash handler installed");
}

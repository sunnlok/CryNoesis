#pragma once	
#include "CryString/CryString.h"

namespace Cry
{
namespace Ns
{
namespace Detail
{
	static void LogHandler(const char* filename, uint32_t line, uint32_t level, const char* channel, const char* message)
	{
		if (strcmp(channel, "") != 0)
			return;

		// [TRACE][DEBUG][INFO][WARNING][ERROR]
		const char* prefixes[] = { "T", "D", "I", "W", "E" };
		const static IMiniLog::ELogType types[] = { IMiniLog::ELogType::eAssert
			,IMiniLog::ELogType::eMessage
			,IMiniLog::ELogType::eComment
			,IMiniLog::ELogType::eWarningAlways
			,IMiniLog::ELogType::eErrorAlways
		};
		auto levelAdjusted = level < 5 ? level : 2;

		string format;

		format.Format("[NOESIS | %s] %s", prefixes[levelAdjusted], message);
		gEnv->pLog->LogWithType(types[levelAdjusted], format.c_str());
		if (strcmp(filename, "") != 0)
		{
			format.Format("[File: %s | Line %d]", filename, line);
			gEnv->pLog->LogWithType(types[levelAdjusted], format.c_str());
		}
			
	}

	static void ErrorHandler(const char* file, uint32_t line, const char* message, bool fatal)
	{
		string format;
		if (fatal)
		{
			
			if (strcmp(file, "") != 0)
				format.Format("[Noesis Error] %s (File: %s | Line: %s)", message, file, line);
			else
				format.Format("[Noesis Error] %s", message);

			CryFatalError(format.c_str());
		}
		else
		{
			format.Format("[NOESIS | E] %s", message);
			gEnv->pLog->LogWithType(IMiniLog::ELogType::eErrorAlways, format.c_str());
			if (strcmp(file, "") != 0)
			{
				format.Format("[File: %s | Line %d]", file, line);
				gEnv->pLog->LogWithType(IMiniLog::ELogType::eErrorAlways, format.c_str());
			}
				

		}
	}

	static bool AssertHandler(const char* file, uint32_t line, const char* expr)
	{

		bool bAssert = false;
		if (Cry::Assert::Detail::CryAssertIsEnabled())
		{

			Cry::Assert::Detail::SAssertData const assertData =
			{
				"[Noesis Assert]",
				"Noesis Assert Handler",
				file,
				line
			};

			static Cry::Assert::Detail::SAssertCond assertCond =
			{ false, true };

			bAssert = Cry::Assert::Detail::CryAssertHandler(assertData, assertCond, expr);

			if (Cry::Assert::IsAssertLevel(Cry::Assert::ELevel::DebugBreakOnAssert))
				return true;
		}

		return bAssert;
	}

	struct Allocator
	{
		static void* Alloc(void* user, size_t size) {
			return CryModuleCRTMalloc(size);
		}

		static void DeAlloc(void* user, void* ptr) {
			CryModuleCRTFree(ptr);
		}

		static void* ReAlloc(void* user, void* ptr, size_t size) {
			return CryModuleCRTRealloc(ptr, size);
		}

		static size_t AllocSize(void* user, void* ptr) {
			return CryModuleMemSize(ptr);
		}
	};
}
}
}
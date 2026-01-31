#include "RoutineManager.h"
#include "stdarg.h"
#include <stdio.h>


#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <time.h>
#include <string.h>

void Log(const char* msg, ...)
{
	auto msgPtr = POOL_NEW(Message_log);

	va_list argptr;

	va_start(argptr, msg);
	vsnprintf(msgPtr->szLog, sizeof(msgPtr->szLog), msg, argptr);
	va_end(argptr);
	g_RoutineManager.SendMsg2RoutineType(RoutineType::LOG, msgPtr);

}


#ifdef __LINUX__
#include <sys/time.h>
#endif
long long GetCurrencyTime()
{
	auto now = std::chrono::system_clock::now();
    
    // 转换为time_t（秒）
    auto now_seconds = std::chrono::system_clock::to_time_t(now);
    
    // 获取毫秒部分
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;
	return now_seconds * 1000 + milliseconds.count();
}
std::string GetTimeStr(long long now)
{
	time_t seconds = now / 1000;
	struct tm* local_time = localtime(&seconds);

	char buffer[32];
    // 格式化输出
    strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", local_time);
    
    // 追加毫秒部分
    char ms_part[5];
    snprintf(ms_part, sizeof(ms_part), "-%03d", (int)(now % 1000));
    strncat(buffer, ms_part, sizeof(buffer) - strlen(buffer) - 1);
	return std::string(buffer);
}


std::string getCurrentTimeStr() {
	char buffer[32];

    return GetTimeStr(GetCurrencyTime());
}


#include <fstream>
#include <string>
void __show__(const char* szTemp)
{

}
void __assertex__(const char* filename, int line, const char* func, const char* expr, const char* msg)
{
	char szTemp[1024] = { 0 };

#ifdef __LINUX__
	sprintf(szTemp, "[T=%s][%s][%d][%s][%s]\n[%s]\n", getCurrentTimestamp().c_str(), filename, line, func, expr, msg);
#else
	sprintf(szTemp, "[T=%s][%s][%d][%s][%s]\n[%s]", getCurrentTimeStr().c_str(), filename, line, func, expr, msg);
#endif
	//__show__(szTemp);
	static std::ofstream file(std::string("./assert.log"), std::ios::app | std::ios::binary);

	if (!file.is_open()) {
		return;
	}

	file << szTemp;
	//file.close();

	Log(szTemp);

	throw(1);
}

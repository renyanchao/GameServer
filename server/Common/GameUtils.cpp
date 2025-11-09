#include "RoutineManager.h"
#include "stdarg.h"
#include <stdio.h>


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
uint64_t GetCurrencyTime()
{
// #ifdef _WIN32
// 	return GetTickCount();
// #else
// 	struct timeval _tstart, _tend;
// 	struct timezone tz;
// 	gettimeofday(&_tend, &tz);
// 	double t1, t2;
// 	t1 = (double)_tstart.tv_sec * 1000 + (double)_tstart.tv_usec / 1000;
// 	t2 = (double)_tend.tv_sec * 1000 + (double)_tend.tv_usec / 1000;
// 	return uint64_t(t2 - t1);
// #endif
 return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}


#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <time.h>
#include <string.h>
std::string getCurrentTimestamp() {
	char buffer[32];

	// // 获取秒级时间和毫秒
    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    // time_t now = tv.tv_sec;
    time_t now =  std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count() / 1000;
    // 转换为本地时间
    struct tm* local_time = localtime(&now);
    
    // 格式化输出
    strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", local_time);
    
    // 追加毫秒部分
    char ms_part[5];
    snprintf(ms_part, sizeof(ms_part), "-%03d", (int)(now % 1000));
    strncat(buffer, ms_part, sizeof(buffer) - strlen(buffer) - 1);
	return std::string(buffer);
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
	sprintf(szTemp, "[T=%s][%s][%d][%s][%s]\n[%s]", getCurrentTimestamp().c_str(), filename, line, func, expr, msg);
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
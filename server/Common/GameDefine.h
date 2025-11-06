#pragma once

#define MAX_THREAD_NUM 24
#define MAX_THREAD_LOCAL_POOL_SIZE 1024
#define MAX_THREAD_TOTAL_POOL_SIZE (1024 * 64)
#define THREAD_SLEEP_TIME_MS 10
#define INVALID_ID (-1)

#define RoutineID unsigned long long
#define RoldID unsigned long long

enum RoutineType
{
	LOG = 0,
	LOGIN,
	MANAGER,//Manager
	SCNE,
	MAIL,
	DB,
	Net,
	Redis,
	Max,
};

enum MsgID
{
	MsgID_Message_Invalid = -1,
	MsgID_Message_1 = 0,
	MsgID_Message_2,
	MsgID_Message_3,
	MsgID_Message_log,
	MsgID_Message_createscene,
};

void Log(const char* msg, ...);

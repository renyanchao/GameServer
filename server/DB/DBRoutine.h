#pragma once

#include "Routine.h"
#include<mysql/mysql.h>
class DBRoutine :public Routine
{
public:
	DBRoutine(){ConnectMysql();}
	virtual int32_t GetRoutineType()
	{
		return RoutineType::DB;
	}
	void HeartBeat(int32_t nMillisecond);

	void ConnectMysql();
	void CloseMysql();

private:
	MYSQL* mysql;
};
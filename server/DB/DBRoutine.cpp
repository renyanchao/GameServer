#include "DBRoutine.h"

const std::string ip = "192.168.40.129";
const std::string port = "3306";
const std::string user = "root";
const std::string password = "993366";

void DBRoutine::HeartBeat(int32_t nMillisecond)
{
	Log("DBRoutine Tick() %d", std::this_thread::get_id());
}

void DBRoutine::ConnectMysql()
{
	mysql = mysql_init(nullptr);
	if(mysql == nullptr)
	{
		Log("DBRoutine ConnectMysql fail");
	}
	Log("DBRoutine ConnectMysql success");
}

void DBRoutine::CloseMysql()
{

}
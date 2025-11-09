

#include "RoutineManager.h"
#include "ThreadPool.h"


#include "Log/LogRoutine.h"
#include "Scene/SceneRoutine.h"
#include "Login/LoginRoutine.h"
#include "Mail/MailRoutine.h"
#include "DB/DBRoutine.h"
#include "Redis/RedisRoutine.h"


int main()
{
	g_threadPool.Start();


	g_RoutineManager.RegisterRoutine(std::make_shared<LogRoutine>());

	g_RoutineManager.RegisterRoutine(std::make_shared<LoginRoutine>());
	g_RoutineManager.RegisterRoutine(std::make_shared<SceneRoutine>());
	g_RoutineManager.RegisterRoutine(std::make_shared<MailRoutine>());
	g_RoutineManager.RegisterRoutine(std::make_shared<DBRoutine>());
	g_RoutineManager.RegisterRoutine(std::make_shared<RedisRoutine>());
	getchar();
}
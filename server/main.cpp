

#include "RoutineManager.h"
#include "ThreadPool.h"


#include "Log/LogRoutine.h"
#include "Scene/SceneRoutine.h"
#include "Login/LoginRoutine.h"
#include "Mail/MailRoutine.h"
#include "DB/DBRoutine.h"
#include "Redis/RedisRoutine.h"

#include <fstream>
#include <string>

//Routine注册服务
//类似Mail、DB、Redis、等等全局数据管理，都有一个单独的进程作为Server，负责处理业务
//启服时，所有进程，都创建这些Routine。但它们是以客户端模式运行。
//客户端Routine与服务器Routine,通过gRPC通信
//要负责服务注册、地址管理
//https://cloud.tencent.com/developer/article/2494197

std::map<std::string, std::shared_ptr<ObjPoolBase>> g_All_ObjPool;


int main()
{
	g_threadPool.Start();


	g_RoutineManager.RegisterRoutine(std::make_shared<LogRoutine>());
	//g_RoutineManager.RegisterRoutine(std::make_shared<LoginRoutine>());
	g_RoutineManager.RegisterRoutine(std::make_shared<SceneRoutine>());

	//g_RoutineManager.RegisterRoutine(std::make_shared<MailRoutine>());
	//g_RoutineManager.RegisterRoutine(std::make_shared<DBRoutine>());
	//g_RoutineManager.RegisterRoutine(std::make_shared<RedisRoutine>());

	//__show__(szTemp);
	// static std::ofstream file(std::string("./assert.log"), std::ios::app | std::ios::binary);

	// if (!file.is_open()) {
	// 	return 0;
	// }
	// while(1)
	// {
	// 	file << "hello";
	// 	sleep_ms(1000);
	// }

	getchar();
}


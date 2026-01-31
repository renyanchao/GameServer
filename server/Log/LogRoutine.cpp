#include "LogRoutine.h"
#include <iostream>

#include <sstream>
#include <fstream>
#include <string>
#include "GameDefine.h"
LogRoutine::LogRoutine()
{
	RegisterHandler(MsgID::MsgID_Message_log, std::bind(&LogRoutine::HandleMsg, this, std::placeholders::_1));

	static std::ofstream file(std::string("../log/assert.log"), std::ios::out | std::ios::trunc);
	if(file.is_open())
	{
		file<<"GameServer Start" <<std::endl;
		file<< getCurrentTimeStr();
		file.close();
	}
}


void LogRoutine::HandleMsg(const MessagePtr& msgPtr)
{
	const Message_log& logMsg = dynamic_cast<const Message_log&>(*msgPtr);
	std::string str("["+GetTimeStr(logMsg.time)+"]"+logMsg.szLog);
	std::cout<<str<<std::endl;
	static std::ofstream file(std::string("../log/assert.log"), std::ios::app | std::ios::binary);
	if (!file.is_open()) {
		return;
	}
	uint64_t time = logMsg.time;
	file << str <<"\n";
}
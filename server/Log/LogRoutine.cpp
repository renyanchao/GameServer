#include "LogRoutine.h"
#include <iostream>

#include <sstream>
#include <fstream>
#include <string>
void LogRoutine::HandleMsg(const MessagePtr& msgPtr)
{
	const Message_log& logMsg = dynamic_cast<const Message_log&>(*msgPtr);

	static std::ofstream file(std::string("./assert.log"), std::ios::app | std::ios::binary);
	if (!file.is_open()) {
		return;
	}

	file << logMsg.szLog<<"\n";
}
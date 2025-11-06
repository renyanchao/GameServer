#pragma once

#include "Routine.h"


class LogRoutine : public Routine
{
public:

	LogRoutine()
	{
		RegisterHandlerInit();
	}
	virtual int32_t GetRoutineType()
	{
		return RoutineType::LOG;
	}

	void HandleMsg(const MessagePtr& msgPtr);

	void RegisterHandlerInit()
	{
		RegisterHandler(MsgID::MsgID_Message_log, std::bind(&LogRoutine::HandleMsg, this, std::placeholders::_1));
	}

};
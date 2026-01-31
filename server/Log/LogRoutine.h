#pragma once

#include "Routine.h"


class LogRoutine : public Routine
{
public:

	LogRoutine();
	virtual int32_t GetRoutineType()
	{
		return RoutineType::LOG;
	}

	void HandleMsg(const MessagePtr& msgPtr);


};
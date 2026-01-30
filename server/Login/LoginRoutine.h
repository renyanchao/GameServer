#pragma once

#include "Routine.h"

#include"unordered_map"
class LoginRoutine : public Routine
{
public:
	virtual int32_t GetRoutineType()
	{
		return RoutineType::LOGIN;
	}
	void OnInit()
	{
		m_NetModuel.Listen();
	}
	virtual void HeartBeat(const TimeElpaseInfo& info);

private:
	//所有角色所在Routine
	std::unordered_map<RoldID, RoutineID> m_AllPlayerInfo;
};
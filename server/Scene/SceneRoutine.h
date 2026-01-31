#pragma once

#include "Routine.h"

class SceneRoutine : public Routine
{
public:
	SceneRoutine()
	{
		RegisterHandler(MsgID::MsgID_Message_Transfer, std::bind(&SceneRoutine::HandleMsg_Message_Transfer, this, std::placeholders::_1));
	}

	void HandleMsg_Message_Transfer(const MessagePtr& msgPtr);

	virtual int32_t GetRoutineType()
	{
		return RoutineType::SCNE;
	}
	void HeartBeat(const TimeElpaseInfo& info);
	void CreateMonster();
	void CreatePlayer();
	void CreateScene();
	uint32_t GetLeftTime()const { return m_LeftLifeTime; }
private:
	int32_t m_LeftLifeTime = 60 * 1000;
private:
	ScenePlayerManager m_PlayerManager;
	SceneMonsterManager m_MonsterManager;
	//ScenePtr m_pScene;

	std::vector<uint64_t> m_SonRoutine;
};
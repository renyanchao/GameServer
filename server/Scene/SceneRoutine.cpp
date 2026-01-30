#include "SceneRoutine.h"
#include "RoutineManager.h"

void SceneRoutine::HandleMsg_Message_Transfer(const MessagePtr& msgPtr)
{
	const Message_Transfer& rMsg = dynamic_cast<const Message_Transfer&>(*msgPtr);
	Log("RoutineID(%d) SceneRoutine Receive newObj. Now let it do something ", GetRoutineID());;
	rMsg.m_Ptr->DoSomething();
}

void SceneRoutine::HeartBeat(const TimeElpaseInfo& info)
{
	__ENTER_FUNCTION
	if (m_LeftLifeTime <= 0)return;

	//Log("RoutineID(%d) SceneRoutine Tick() m_LeftLifeTime(%d), nCurrencyTime(%d) ", GetRoutineID(), m_LeftLifeTime, info.m_nCurrencyTime);
	m_LeftLifeTime =  m_LeftLifeTime - info.m_nElpaseTime;
	if (m_LeftLifeTime <= 0)
	{
		//Scene die
		m_PlayerManager.Clear();
		m_MonsterManager.Clear();
		//Log("RoutineID(%d) SceneRoutine Die() ", GetRoutineID());;
		return;

	}


	if (std::rand() % 100 <= 1)
	{
		CreateScene();
	}
	if (std::rand() % 100 <= 100)
	{
		CreatePlayer();
	}
	if (std::rand() % 100 <= 100)
	{
		CreateMonster();
	}

	m_PlayerManager.HeartBeat();
	m_MonsterManager.HeartBeat();
	__LEAVE_FUNCTION
}

void SceneRoutine::CreateMonster()
{
	ObjMonsterPtr ptr = POOL_NEW(ObjMonster);
	if (std::rand() % 100 <= 50 && m_SonRoutine.size() != 0){
		auto nRoutineID = m_SonRoutine[std::rand() % m_SonRoutine.size()];
		Message_TransferPtr msgPtr = POOL_NEW(Message_Transfer);
		msgPtr->m_Ptr = ptr;
		g_RoutineManager.SendMsg2Routine(nRoutineID, msgPtr);
	}
	else{
		m_MonsterManager.AddMonster(ptr);
		Log("RoutineID(%d) SceneRoutine CreateMonster() ", GetRoutineID());;
	}
}

void SceneRoutine::CreatePlayer()
{
	ObjPlayerPtr ptr = POOL_NEW(ObjPlayer);
	if (std::rand() % 100 <= 50 && m_SonRoutine.size() != 0){
		auto nRoutineID = m_SonRoutine[std::rand() % m_SonRoutine.size()];
		Message_TransferPtr msgPtr = POOL_NEW(Message_Transfer);
		msgPtr->m_Ptr = ptr;
		g_RoutineManager.SendMsg2Routine(nRoutineID, msgPtr);
	}
	else{
		m_PlayerManager.AddPlayer(ptr);	
		Log("RoutineID(%d) SceneRoutine CreatePlayer() ", GetRoutineID());
	}
}
void SceneRoutine::CreateScene()
{
	__ENTER_FUNCTION
	//Log("RoutineID(%d) SceneRoutine CreateScene() Try Begin", GetRoutineID());
	auto nID = g_RoutineManager.RegisterRoutine(std::make_shared<SceneRoutine>());
	Log("RoutineID(%d) SceneRoutine CreateScene(%d) ", GetRoutineID(), nID);
	m_SonRoutine.push_back(nID);
	//AssertEx(false, "CreateScene fail");
	__LEAVE_FUNCTION
}
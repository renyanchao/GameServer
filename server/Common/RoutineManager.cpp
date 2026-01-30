#include "RoutineManager.h"
#include "ThreadPool.h"
#include "Routine.h"
RoutineManager g_RoutineManager;


bool RoutineManager::SendMsg2Routine(RoutineID nID, const MessagePtr& message)
{
	lock_read(m_Lock);
	for (auto it = m_AllRoutine.begin(); it != m_AllRoutine.end(); it++)
	{
		if ((*it)->GetRoutineID() == nID)
		{
			(*it)->Push(message);
		}
	}
	return false;
}
bool RoutineManager::SendMsg2RoutineType(RoutineType nType, const MessagePtr& message)
{
	lock_read(m_Lock);
	for (auto it = m_AllRoutine.begin(); it != m_AllRoutine.end(); it++)
	{
		if ((*it)->GetRoutineType() == nType)
		{
			(*it)->Push(message);
		}
	}
	return false;
}
uint64_t RoutineManager::RegisterRoutine(std::shared_ptr<Routine> r)
{
	//Log("SceneRoutine RegisterRoutine() try lock");
	uint64_t nRoutineID = -1;
	r->OnInit();
	{
		lock_write(m_Lock);
		nRoutineID = m_GenerateRoutineID++;
		//if(startserver == false)sleep_ms(10 * 1000);
		r->SetRoutineID(nRoutineID);
		m_AllRoutine.push_back(r);
	}
	g_threadPool.AddTask(r);
	return nRoutineID;
	//Log("SceneRoutine RegisterRoutine() try unlock");
}
//void RoutineManager::UnRegisterRoutine(std::shared_ptr<Routine> r)
//{
//	lock_write(m_Lock);
//	for (auto it = m_AllRoutine.begin(); it != m_AllRoutine.end(); it++)
//	{
//		if ((*it)->GetRoutineID() == r->GetRoutineID())
//		{
//			m_AllRoutine.erase(it);
//			break;
//		}
//	}
//}
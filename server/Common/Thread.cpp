#include "Thread.h"
#include "Routine.h"

void Thread::Start()
{
	std::thread mt([this] {while (true)this->Tick(); });
	t = std::move(mt);
	t.detach();
}
void Thread::AddTask(std::shared_ptr<Routine> r)
{
	std::lock_guard<std::mutex> lock(m_lock);
	m_AddRoutineList.push_back(r);
}
void Thread::RemoveTask(RoutineID nID)
{

	for (auto it = m_TickTaskList.begin(); it != m_TickTaskList.end(); it++)
	{
		if (nID == (*it)->GetRoutineID())
		{
			m_TickTaskList.erase(it);
			return;
		}
	}
}
void Thread::Tick()
{
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_TickTaskList.insert(m_TickTaskList.end(), std::make_move_iterator(m_AddRoutineList.begin()), std::make_move_iterator(m_AddRoutineList.end()));
		m_AddRoutineList.clear();
	}
	

	TimeElpaseInfo elpasetimeInfo;
	elpasetimeInfo.m_nCurrencyTime = GetCurrencyTime();
	elpasetimeInfo.m_nElpaseTime = elpasetimeInfo.m_nCurrencyTime - m_nCurrencyTime;

	m_nCurrencyTime = elpasetimeInfo.m_nCurrencyTime;

	std::list<std::shared_ptr<Routine>> dielist;
	for (auto& r : m_TickTaskList)
	{
		try
		{
			r->Tick(elpasetimeInfo);
		}
		catch (...)
		{
		}
		if (r->GetLeftTime() <= 0)
		{
			dielist.push_back(r);
		}
	}
	for (auto& r : dielist)
	{
		RemoveTask(r->GetRoutineID());
	}
	if (THREAD_SLEEP_TIME_MS > 0)
		sleep_ms(THREAD_SLEEP_TIME_MS);
}

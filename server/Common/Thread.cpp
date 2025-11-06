#include "Thread.h"
#include "Routine.h"

void Thread::Start()
{
//	std::thread mt(TaskFunc, std::ref(*this));
	std::thread mt([this] {while (true)this->Tick(); });
	t = std::move(mt);
	t.detach();
}
void Thread::AddTask(std::shared_ptr<Routine> r)
{
	std::lock_guard<std::mutex> lock(m_lock);
	m_TaskList.push_back(r);
}
void Thread::DelTask(RoutineID nID)
{
	std::lock_guard<std::mutex> lock(m_lock);
	for (auto it = m_TaskList.begin(); it != m_TaskList.end(); it++)
	{
		if (nID == (*it)->GetRoutineID())
		{
			m_TaskList.erase(it);
			return;
		}
	}
}
void Thread::Tick()
{

	std::lock_guard<std::mutex> lock(m_lock);

	//auto nowTime = std::chrono::system_clock::now();
	//auto elpasetime = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_threadTimer).count();
	auto elpasetime = 10;
	//m_threadTimer = nowTime;
	std::list<std::shared_ptr<Routine>> dielist;
	for (auto& r : m_TaskList)
	{
		r->Tick(elpasetime);
		if (r->GetLeftTime() <= 0)
		{
			dielist.push_back(r);
		}
	}
	for (auto& r : dielist)
	{
		for (auto it = m_TaskList.begin(); it != m_TaskList.end(); it++)
		{
			if ((*it)->GetRoutineID() == r->GetRoutineID())
			{
				m_TaskList.erase(it);
				break;
			}
		}
	}
	if(THREAD_SLEEP_TIME_MS > 0)
	std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_TIME_MS));
}

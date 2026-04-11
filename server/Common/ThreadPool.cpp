#include"ThreadPool.h"
#include"ObjType.h"
#include "Thread.h"
#include "Routine.h"

ThreadPool g_threadPool;




ThreadPool::ThreadPool()
{
	for (int i = 0; i < MAX_THREAD_NUM; i++)
	{
		m_allThread.push_back(std::make_shared<Thread>());
	}
}
ThreadPool::~ThreadPool() 
{
	m_allThread.clear();
}


void ThreadPool::Start()
{
	for (auto it = m_allThread.begin(); it != m_allThread.end(); it++)
	{
		(*it)->Start();
	}
}
void ThreadPool::Tick() {};
void ThreadPool::AddTask(std::shared_ptr<Routine>& r)
{
	int nIndex = r->GetRoutineID() % m_allThread.size();
	m_allThread[nIndex]->AddTask(r);
}
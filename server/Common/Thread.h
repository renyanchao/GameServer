#pragma once
#include<thread>
#include<vector>
#include<iostream>
#include<map>
#include<mutex>
#include<list>
#include "GameDefine.h"

class Routine;
class Thread
{
public:
	void Start();
	void AddTask(std::shared_ptr<Routine> r);
	void Tick();
	void RemoveTask(RoutineID nID);
private:
	//std::chrono::time_point<std::chrono::system_clock> m_threadTimer = std::chrono::system_clock::now();
	int64_t m_nCurrencyTime = GetCurrencyTime();
	std::mutex m_lock;
	std::thread t;
	std::vector<std::shared_ptr<Routine>> m_TickTaskList;
	std::vector<std::shared_ptr<Routine>> m_AddRoutineList;
};
#pragma once
#include<thread>
#include<vector>
#include<iostream>
#include<map>
#include<mutex>
#include<list>
#include<memory>
#include "GameDefine.h"

class Thread;
class Routine;

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();


	void Start();
	void Tick();
	void AddTask(std::shared_ptr<Routine>& r);

public:

private:
	std::vector<std::shared_ptr<Thread>> m_allThread;

};
extern ThreadPool g_threadPool;
#include"Routine.h"
#include"ObjType.h"
#include<iostream>
#include"ThreadPool.h"
#include "RoutineManager.h"

Routine::Routine()
{
	RegisterHandlerInit();
}
Routine:: ~Routine()
{}



void Routine::Tick(int32_t nMillisecond)
{
	m_elapsetime += nMillisecond;
	if (m_elapsetime < TickInteral())return;
	m_elapsetime = 0;
	for (int i = 0; i < 128; i++)
	{
		auto msgPtr = Pop();
		if (msgPtr == nullptr)break;
		
		if (m_HandlerList[msgPtr->GetID()])
		{
			m_HandlerList[msgPtr->GetID()](msgPtr);
		}
		
	}
	HeartBeat(nMillisecond);
}

//Local Other Routine Push to me
void Routine::Push(MessagePtr ptr)
{
	std::lock_guard<std::mutex> pushlock(m_PushLock);
	m_PushList.push_back(ptr);
}
//Self Pop
MessagePtr Routine::Pop()
{
	if (m_PopList.empty() == false)
	{
		auto p = m_PopList.front();
		m_PopList.pop_front();
		return p;
	}

	std::lock_guard<std::mutex> pushlock(m_PushLock);
	std::lock_guard<std::mutex> poplock(m_PopLock);

	m_PopList.swap(m_PushList);
	if (m_PopList.empty() == false)
	{
		auto p = m_PopList.front();
		m_PopList.pop_front();
		return p;
	}
	return nullptr;
}

void Routine::RegisterHandler(MsgID nMsgID, std::function<void(const MessagePtr)> handler)
{
	m_HandlerList[nMsgID] = handler;
}

void Routine::MsgDefaultHandler(const MessagePtr& rMsgPtr)
{
	std::cout << "MsgDefaultHandler " << std::endl;
}

void Routine::RegisterHandlerInit()
{
	RegisterHandler(MsgID::MsgID_Message_Invalid, std::bind(&Routine::MsgDefaultHandler, this, std::placeholders::_1));
}

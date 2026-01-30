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



void Routine::Tick(const TimeElpaseInfo& info)
{
	__ENTER_FUNCTION
	m_elapsetime += info.m_nElpaseTime;
	if (m_elapsetime < TickInteral())return;
	/*for (int i = 0; i < 128; i++)*/
	while(true)
	{
		auto msgPtr = Pop();
		if (msgPtr == nullptr)break;
		
		if (m_HandlerList[msgPtr->GetID()])
		{
			m_HandlerList[msgPtr->GetID()](msgPtr);
		}
		
	}
	
	HeartBeat(info);
	m_elapsetime = 0;
	__LEAVE_FUNCTION
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

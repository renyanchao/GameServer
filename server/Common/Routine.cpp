#include"Routine.h"

Routine::Routine()
{
	RegisterHandler(MsgID::MsgID_Message_Invalid, std::bind(&Routine::MsgDefaultHandler, this, std::placeholders::_1));
}
Routine:: ~Routine()
{}
int32_t Routine::TickInteral()
{
	return 50; //50ms
}


void Routine::Tick(const TimeElpaseInfo& info)
{
	__ENTER_FUNCTION
	m_elapsetime += info.m_nElpaseTime;
	if (m_elapsetime < TickInteral())return;
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

void Routine::Push(MessagePtr ptr)
{
	std::lock_guard<std::mutex> pushlock(m_Lock);
	m_PushList.push_back(ptr);
}
MessagePtr Routine::Pop()
{
	if (m_PopList.empty() == false)
		{
			auto p = m_PopList.front();
			m_PopList.pop_front();
			return p;
		}
		{
			std::lock_guard<std::mutex> pushlock(m_Lock);
			m_PopList.swap(m_PushList);
		}
		if (m_PopList.empty() == false)
		{
			auto p = m_PopList.front();
			m_PopList.pop_front();
			return p;
		}
		return nullptr;
}


void Routine::MsgDefaultHandler(const MessagePtr& rMsgPtr)
{
	std::cout << "MsgDefaultHandler " << std::endl;
}
void Routine::RegisterHandler(MsgID nMsgID, std::function<void(const MessagePtr)> handler)
{
	m_HandlerList[nMsgID] = handler;
}
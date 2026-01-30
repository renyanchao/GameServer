#pragma once

#include<list>
#include<mutex>
#include<functional>
#include<unordered_map>
#include"GameDefine.h"
#include"ObjType.h"
#include"Net.h"

class Routine
{
public:
	Routine();
	virtual ~Routine();

	virtual int32_t TickInteral() //100ms Default
	{
		return 50;
	}
	virtual int32_t GetRoutineType()
	{
		return INVALID_ID;
	}
	virtual void OnInit(){}
	virtual void HeartBeat(const TimeElpaseInfo& info)
	{

	}
	virtual void Tick(const TimeElpaseInfo& info);
	virtual uint32_t GetLeftTime()const { return 0xFFFFFFFF; }
	//Other Routine Push to me
	void Push(MessagePtr ptr)
	{
		std::lock_guard<std::mutex> pushlock(m_Lock);
		m_PushList.push_back(ptr);
	}
	//Self Pop
	MessagePtr Pop()
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

	//void RemotePush(MessagePtr ptr)
	//{

	//}
	//MessagePtr RemotePop()
	//{

	//}
	void RegisterHandler(MsgID nMsgID, std::function<void(const MessagePtr)> handler);

	void MsgDefaultHandler(const MessagePtr& rMsgPtr);

	virtual void RegisterHandlerInit();

	uint64_t GetRoutineID()const { return m_routineUID; }
	void	 SetRoutineID(uint64_t routineID) { m_routineUID = routineID; }

	

protected:
	NetManager m_NetModuel;
private:

	std::list<MessagePtr> m_PushList;
	std::mutex m_Lock;
	std::list<MessagePtr> m_PopList;

	uint64_t m_elapsetime = 0;
	uint64_t m_routineUID = 0;

	std::unordered_map<int, std::function<void(const MessagePtr)>> m_HandlerList;

	//NetManager m_NetManager;//every routine support net
	bool m_IsServer = false;//
};



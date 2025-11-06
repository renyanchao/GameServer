#pragma once

#include<list>
#include<mutex>
#include<functional>
#include<unordered_map>
#include"GameDefine.h"
#include"ObjType.h"


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
	virtual void HeartBeat(int32_t nMillisecond)
	{

	}
	virtual void Tick(int32_t nMillisecond);
	virtual uint32_t GetLeftTime()const { return 0xFFFFFFFF; }
	//Local Other Routine Push to me
	void Push(MessagePtr ptr);
	//Self Pop
	MessagePtr Pop();

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
private:

	std::mutex m_PushLock;
	std::list<MessagePtr> m_PushList;

	std::mutex m_PopLock;
	std::list<MessagePtr> m_PopList;

	uint64_t m_elapsetime = 0;
	uint64_t m_routineUID = 0;

	std::unordered_map<int, std::function<void(const MessagePtr)>> m_HandlerList;

	//NetManager m_NetManager;//every routine support net
	bool m_IsServer = false;//
};



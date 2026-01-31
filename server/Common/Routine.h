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

	virtual int32_t TickInteral();
	virtual int32_t GetRoutineType()
	{
		return INVALID_ID;
	}
	virtual void OnInit() {};
	virtual void HeartBeat(const TimeElpaseInfo& info){};
	virtual void Tick(const TimeElpaseInfo& info);
	virtual uint32_t GetLeftTime()const { return 0xFFFFFFFF; }
	//Other Routine Push to me
	void Push(MessagePtr ptr);
	//Self Pop
	MessagePtr Pop();

	void RegisterHandler(MsgID nMsgID, std::function<void(const MessagePtr)> handler);

	void MsgDefaultHandler(const MessagePtr& rMsgPtr);

	uint64_t GetRoutineID()const { return m_routineUID; }
	void	 SetRoutineID(uint64_t routineID) { m_routineUID = routineID; }

	

protected:
	//NetManager m_NetModuel;
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



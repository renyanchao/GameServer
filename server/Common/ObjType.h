#pragma once


#include"ObjectPool.h"
#include<list>
class Obj
{
public:
	Obj() {}
	virtual ~Obj() {}

	virtual void Tick() {}
	virtual void DoSomething() {}
};POOL_DEFINE(Obj);


class ObjPlayer : public Obj
{
public:
	virtual void Tick() {}
	virtual void DoSomething() 
	{
		Log("I am a ObjPlayer.Now Do something");
	}
};POOL_DEFINE(ObjPlayer);

class ObjMonster: public Obj
{
public:
	virtual void Tick() {}
	virtual void DoSomething() 
	{
		Log("I am a ObjMonster.Now Do something");
	}
};POOL_DEFINE(ObjMonster);


class ScenePlayerManager
{
public:
	void HeartBeat() 
	{
		for (auto& r : m_AllPlayer)
		{
			r->Tick();
		}
	}
	void Clear() { m_AllPlayer.clear(); }
	void AddPlayer(const ObjPlayerPtr& r){}
	void DelPlayer(const ObjPlayerPtr& r){}
private:
	std::list<ObjPlayerPtr> m_AllPlayer;
};
class SceneMonsterManager
{
public:
	void HeartBeat()
	{
		for (auto& r : m_AllMonster)
		{
			r->Tick();
		}
	}
	void Clear() { m_AllMonster.clear(); }
	void AddMonster(const ObjMonsterPtr& r){}
	void DelMonster(const ObjMonsterPtr& r){}
private:
	std::list<ObjMonsterPtr> m_AllMonster;
};
class Scene
{
public:
	virtual void Tick() {}
	virtual void DoSomething() {}
};
POOL_DEFINE(Scene);


struct Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_Invalid; }
};
POOL_DEFINE(Message);


struct Message_1 : public Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_1; }
	
};
POOL_DEFINE(Message_1);

struct Message_2 : public Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_2; }
};POOL_DEFINE(Message_2);

struct Message_3 : public Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_3; }
};POOL_DEFINE(Message_3);

struct Message_log : public Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_log; }
	char szLog[1024];
	long long time = GetCurrencyTime();
};POOL_DEFINE(Message_log);

struct Message_CreateScene :public Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_createscene; }
};POOL_DEFINE(Message_CreateScene);


struct Message_Transfer : public Message
{
	virtual MsgID GetID() { return MsgID::MsgID_Message_Transfer; }
	ObjPtr m_Ptr = nullptr;
};POOL_DEFINE(Message_Transfer);

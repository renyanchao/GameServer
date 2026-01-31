#pragma once

#include<vector>
#include<deque>
#include<array>
#include<memory>
#include <algorithm>
#include"GameDefine.h"
#include<thread>
#include<atomic>
#include<iostream>
#include<string>


class ObjPoolBase
{
public:
	virtual void printStats(){std::cout<<"ObjPoolBase::printStats()"<<std::endl;};
	size_t getAvailableCount() const{return 0;};
	size_t getTotalCount() const{return 0;};
};


template<typename T, int32_t nMaxPoolSize = MAX_THREAD_LOCAL_POOL_SIZE, int32_t nInitPoolSize = MAX_THREAD_LOCAL_POOL_SIZE>
class ObjPool_v2 : public ObjPoolBase
{
private:
	using Ptr = std::shared_ptr<T>;
	std::vector<std::vector<Ptr>> pool_array_;
	std::atomic<int> next_thread_id{0};
	struct _BusyFlag
	{
		std::atomic<bool> m_Flag;
		char m_Placeholder[63];
	};
	std::array<_BusyFlag, MAX_THREAD_NUM> m_busy_flag_;	
public:
	ObjPool_v2(){
		pool_array_.resize(MAX_THREAD_NUM);
	}
	~ObjPool_v2() {
		pool_array_.clear();
	}
	int getThreadIndex(){
		static thread_local int _thread_index = -1;
		if (_thread_index == -1)
		{
			_thread_index = next_thread_id.fetch_add(1);
			m_busy_flag_[_thread_index].m_Flag.store(false, std::memory_order_relaxed);
		}
		return _thread_index;
	}
	Ptr New(){
		int nThreadIndex = getThreadIndex();
		if (0 <= nThreadIndex && nThreadIndex < pool_array_.size())
		{
			for (int i = 0; i < pool_array_[nThreadIndex].size(); i++)
			{
				if (pool_array_[nThreadIndex][i].use_count() == 1)
				{
					return pool_array_[nThreadIndex][i];
				}
			}
			bool expect = false;
			auto & flag_ = m_busy_flag_[nThreadIndex].m_Flag;
			while (!flag_.compare_exchange_weak(expect, true, std::memory_order_acquire, std::memory_order_relaxed)) {}
			//extend
			Ptr newObj = std::make_shared<T>();
			pool_array_[nThreadIndex].push_back(newObj);
			flag_.store(false, std::memory_order_release);
			return newObj;
		}
		return nullptr;
	}


	virtual void printStats() {
		std::string infoStr;
        for (size_t i = 0; i < pool_array_.size(); ++i) {
            const auto& pool = pool_array_[i];
            if (!pool.empty()) {
                size_t total = pool.size();
				//while(!m_busy_flag_[nThreadIndex].compare_exchange_strong(false, true, std::memory_order_acquire, std::memory_order_release)){}
                bool expect = false;
				auto & flag_ = m_busy_flag_[i].m_Flag;
				while (!flag_.compare_exchange_weak(expect, true, 
                                              std::memory_order_acquire,
                                              std::memory_order_relaxed)) {

				}
				size_t in_use = std::count_if(pool.cbegin(), pool.cend(),
                    [](const Ptr& ptr) { return ptr.use_count() > 1; });
				flag_.store(false, std::memory_order_release);
                
				size_t available = total - in_use;
				char buff[1024];
				sprintf(buff, "Thread(%d) total(%d) in_use(%d) available(%d) SceneRoutine CreateMonster() \n", total, i, in_use, available);
				infoStr = infoStr + std::string(buff);
            }
			//Log(infoStr.c_str());
		
        }
    }
	// 获取当前线程的空闲对象数量
    size_t getAvailableCount() const {
        size_t idx = getThreadIndex();
        if (idx < pool_array_.size()) {
            const auto& pool = pool_array_[idx];
            return std::count_if(pool.begin(), pool.end(),
                [](const Ptr& ptr) { return ptr.use_count() == 1; });
        }
        return 0;
    }
    
    // 获取当前线程的总对象数量
    size_t getTotalCount() const {
        size_t idx = getThreadIndex();
        if (idx < pool_array_.size()) {
            return pool_array_[idx].size();
        }
        return 0;
    }

};

template<typename T, int32_t nMaxPoolSize = MAX_THREAD_LOCAL_POOL_SIZE, int32_t nInitPoolSize = MAX_THREAD_LOCAL_POOL_SIZE>
class ObjPool_v3
{
	using Ptr = std::shared_ptr<T>;
	using PtrList = std::vector<Ptr>;



public:
	ObjPool_v3()
	{
		m_thread_objlist.reserve(nMaxPoolSize);
		for (int32_t size = 0; size < nMaxPoolSize; size++)
		{
			m_thread_objlist.push_back(std::make_shared<T>());
		}
	}
	~ObjPool_v3()
	{

	}
	Ptr New()
	{

		PtrList& thread_local_list = m_thread_objlist;
		for (int i = 0; i < thread_local_list.size(); i++)
		{
			if (thread_local_list[i].use_count() == 1)
			{
				return thread_local_list[i];
			}

		}
		return std::make_shared<T>();
	}

	void OutLog()
	{
		int nTotalNum = 0;
		int nFreeNum = 0;
		PtrList& list = m_thread_objlist;
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i].use_count() == 1)
			{
				nFreeNum += 1;
			}
			nTotalNum += 1;
		}
		{

			printf("ThreadID = %d , nTotalNum = %d, nFreeNum = %d\n", std::this_thread::get_id(), nTotalNum, nFreeNum);
		}
	}

private:
	PtrList m_thread_objlist;
};


#include<map>
#include<string>
class ObjPoolMonitorer
{
public:
	std::map<std::string, std::shared_ptr<ObjPoolBase>> g_All_ObjPool;
	void Init();
};
extern ObjPoolMonitorer g_ObjPoolMonitorer;

#define POOL_DEFINE(T) extern std::shared_ptr<ObjPool_v2<T>> g_ObjPool_##T;\
	using T##Ptr = std::shared_ptr<T>;
#define POOL_NEW(T) g_ObjPool_##T->New();
#define POOL_INSTANCE(T) g_ObjPool_##T
#define POOL_INST(T) \
    std::shared_ptr<ObjPool_v2<T>> g_ObjPool_##T = std::make_shared<ObjPool_v2<T>>();
#define POOL_MONITOR(T)\
	g_ObjPoolMonitorer.g_All_ObjPool.insert(std::make_pair(#T, g_ObjPool_##T));
	// do{\
	// 	g_All_ObjPool.insert(std::make_pair(#T, g_ObjPool_##T));\
	// }while(0);

// #define POOL_DEFINE(T) extern thread_local ObjPool_v3<T> g_ObjPool_##T;\
// 	using T##Ptr = std::shared_ptr<T>;
// #define POOL_NEW(T) g_ObjPool_##T.New();
// #define POOL_INST(T) thread_local ObjPool_v3<T> g_ObjPool_##T;




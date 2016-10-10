#pragma once
#include <TData/IBase.h>
#include <RedisAdapter/RedisBuffer.h>
#include <unordered_set>
#include <RedisAdapter/RedisListen.h>
#include <TData/IDataAdapter.h>
namespace Data
{

	class RedisWrap;

	class RedisAdapter : public IDataAdapter
	{
	public:
		RedisAdapter();
		~RedisAdapter();

		template<typename T>
		void	Load(T& t);

		void	Init(uint32_t iActorId);

		void	SyncData();

		void	PubAndSet();

		void	SetRedisWrap(RedisWrap* pWrap);

		//===================================逻辑层不可直接调用的接口=============================
		virtual bool	Load(uint64_t iKey, ValueMap& mValue) override;

		virtual bool	Load(uint64_t iKey, uint64_t iIndex, ValueMap& mValue) override;

		virtual void	Save(uint64_t iKey, ValueMap& mValue) override;

		virtual void	Save(uint64_t iKey, uint64_t iIndex, ValueMap& mValue) override;

		virtual void	SetValue(uint64_t iKey, IValue* pValue) override;

		virtual void	SetValue(uint64_t iKey, uint64_t iIndex, IValue* pValue) override;

		virtual void	RegistStruct(IStruct* pIStruct) override;

		virtual void	UnRegistStruct(IStruct* pIStruct) override;

		virtual std::list<uint64_t> GetAllKey(uint64_t i64Key, uint32_t iRediskey) override;

		virtual void	AddRepeated(uint64_t i64Key, uint64_t i64Index, uint32_t iRedisKey) override;

		virtual void	RemoveRepeated(uint64_t i64Key, uint64_t i64Index, uint32_t iRedisKey) override;

		//====================================================================================
	private:
		RedisWrap*				m_pRedisWarp;
		RedisBuffer				m_Buffer;
		RedisListen				m_Listen;
		uint32_t				m_iActorId;
		std::unordered_map<uint64_t, std::unordered_set<IStruct*>> m_AllData;
	};

	template<typename T>
	void RedisAdapter::Load(T& t)
	{
		t.SetDataHandle(this);
		t.Load();
	}

}
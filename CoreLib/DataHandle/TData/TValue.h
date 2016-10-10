#pragma once
#include "IBase.h"
#include "StringValue.h"

namespace Data
{

	template<typename T, typename DataHandle, uint32_t _KeyField>
	class TBaseValue : public IValue
	{
	public:
		TBaseValue() :m_Value(), m_pDataHandle(nullptr)
		{}

		~TBaseValue() = default;

		operator const T&()const { return m_Value; }

		void	SetDataHandle(DataHandle* pHandle)
		{
			m_pDataHandle = pHandle;
			if (m_pDataHandle != nullptr)
				m_pDataHandle->RegistValue(this);
		}

		//只是设置在内存中，不会更新到redis
		void					Init(const T& value) { m_Value = value; }

	protected:

		virtual	void			Sync(const char* pStr) override
		{
			T OldValue = m_Value;
			StringValue::get(pStr, m_Value);
			if (OldValue != m_Value && IsConst())
			{
				m_pDataHandle->ValueChange(_KeyField);
			}
		}

		virtual std::string		ToString()const override { return StringValue::ToString(m_Value); }

		virtual uint32_t		KeyField()const override { return _KeyField; }

		virtual	bool			IsConst()const override { return true; }

	protected:
		T			m_Value;
		DataHandle*	m_pDataHandle;
	};

	template<typename T, typename DataHandle, uint32_t _KeyField>
	class TValue : public TBaseValue<T, DataHandle, _KeyField>
	{
	public:
		TValue() = default;
		~TValue() = default;

		const T& operator =(const T& value)
		{
			if (m_Value != value)
			{
				m_Value = value;
				m_pDataHandle->SetValue(this);
			}
			return m_Value;
		}

	protected:
		virtual	bool			IsConst()const override { return false; }

		void					Init(const T& value) = delete;
	};

}
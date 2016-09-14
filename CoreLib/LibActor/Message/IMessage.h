#pragma  once

#include <memory>
#include <LibActor/Address.h>

namespace ActorLite
{
	//消息父类，所有的消息不要继承这个类,单独一个结构体或者类就可以了

	class IMessage : public std::enable_shared_from_this<IMessage>
	{
	public:
		virtual ~IMessage() = default;
		explicit						IMessage(const Detail::Address& from) :m_From(from) {}

		virtual			uint32_t		TypeID()const = 0;
		inline  const Detail::Address&	From() const { return m_From; }
	private:
		Detail::Address					m_From;
	private:
		IMessage(const IMessage &other) = delete;
		IMessage &operator=(const IMessage &other) = delete;
	};

	using IMessagePtr = std::shared_ptr<IMessage>;

	template <class ValueType>
	struct MessageTraits
	{
		/**
		\brief Indicates whether the message type has an explicit name.
		Message types for which have valid type names are identified
		using their names rather than with built-in C++ Runtime Type Information
		(RTTI) via dynamic_cast.
		*/
		static const bool HAS_TYPE_NAME = false;

		/**
		\brief The unique name of the type.
		*/
		static const char *const TYPE_NAME;
		static	uint32_t	TYPE_ID;
	};

	template <class ValueType>
	uint32_t ActorLite::MessageTraits<ValueType>::TYPE_ID = 0;
	template <class ValueType>
	const char *const MessageTraits<ValueType>::TYPE_NAME = 0;

}


//下面的宏要在命名空间外面使用，切记！！！！！
//如果类有命名空间，MessageType也一定要带上命名空间！！！
#ifndef ACTOR_REGISTER_MESSAGE

#define ACTOR_REGISTER_MESSAGE(MessageType)													\
namespace ActorLite																			\
{																							\
template <>																					\
struct MessageTraits<MessageType>															\
{																							\
    static const bool HAS_TYPE_NAME = true;													\
    static const char *const TYPE_NAME;														\
	static uint32_t	TYPE_ID;													\
};																							\
	const char *const MessageTraits<MessageType>::TYPE_NAME = #MessageType;					\
	uint32_t	MessageTraits<MessageType>::TYPE_ID = IMessage::Hash(#MessageType);	\
}

#endif // ACTOR_REGISTER_MESSAGE

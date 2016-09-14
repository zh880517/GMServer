#pragma once
#include <functional>
#include <thread>
#include <LibActor/BoostNet/BoostHead.h>

namespace BoostNet
{

	class IOServicePool
	{
	public:
		explicit IOServicePool(std::size_t pool_size);

		~IOServicePool();

		void				Run();

		void				Stop();

		void				RegistStopSignalHandle(std::function<void(boost::system::error_code, int)>& handle);

		Asio::io_service&	GetIOService();
	private:
		using io_service_ptr = std::shared_ptr<Asio::io_service>;
		using work_ptr = std::shared_ptr<Asio::io_service::work>;
		using signal_set_ptr = std::shared_ptr<Asio::signal_set>;

		std::vector<io_service_ptr>			m_vService;
		std::vector<work_ptr>				m_vWork;

		signal_set_ptr						m_pSignals;
		std::size_t							m_iNextIoServiceIndex;


		std::vector<boost::shared_ptr<std::thread> > m_vThreads;
		bool								m_bStop;
	};

}


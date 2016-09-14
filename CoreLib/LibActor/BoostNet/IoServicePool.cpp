#include <LibActor/BoostNet/IOServicePool.h>

namespace BoostNet
{

	IOServicePool::IOServicePool(std::size_t pool_size)
		: m_iNextIoServiceIndex(0)
		, m_bStop(true)
	{
		if (pool_size == 0)
			throw std::runtime_error("io_service_pool size is 0");

		// Give all the io_services work to do so that their run() functions will not
		// exit until they are explicitly stopped.
		for (std::size_t i = 0; i < pool_size; ++i)
		{
			io_service_ptr io_service(new boost::asio::io_service);
			work_ptr work(new boost::asio::io_service::work(*io_service));
			m_vService.push_back(io_service);
			m_vWork.push_back(work);
		}

	}


	IOServicePool::~IOServicePool()
	{
		if (!m_bStop)
		{
			Stop();
		}
	}

	void IOServicePool::Run()
	{
		for (std::size_t i = 0; i < m_vService.size(); ++i)
		{
			boost::shared_ptr<std::thread> pThread(
				new std::thread([](io_service_ptr pIOService)
			{
				boost::system::error_code ec;
				pIOService->run(ec);
			}
			, m_vService[i]));

			m_vThreads.push_back(pThread);
		}
		m_pSignals = std::make_shared<Asio::signal_set>(GetIOService());
		m_bStop = false;
	}

	void IOServicePool::Stop()
	{
		for (std::size_t i = 0; i < m_vService.size(); ++i)
			m_vService[i]->stop();

		for (auto it : m_vThreads)
		{
			it->join();
		}
		m_bStop = true;
	}

	Asio::io_service & IOServicePool::GetIOService()
	{
		Asio::io_service& io_service = *m_vService[m_iNextIoServiceIndex];
		++m_iNextIoServiceIndex;
		if (m_iNextIoServiceIndex == m_vService.size())
			m_iNextIoServiceIndex = 0;
		return io_service;
	}

	void IOServicePool::RegistStopSignalHandle(std::function<void(boost::system::error_code, int)>& handle)
	{
		
		boost::system::error_code ec;
		m_pSignals->add(SIGTERM, ec); 
		m_pSignals->async_wait(handle);
	}

}

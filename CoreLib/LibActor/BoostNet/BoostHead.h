#pragma once

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif

#include <boost/asio.hpp>

namespace Asio = boost::asio;
using AsioTCP = boost::asio::ip::tcp;
using BoostErrorCode = boost::system::error_code;
#pragma once

// #define BOOST_DATE_TIME_SOURCE
// #define BOOST_ASIO_NO_LIB
// #define BOOST_SYSTEM_NO_LIB
// #define BOOST_REGEX_NO_LIB
// #define BOOST_ERROR_CODE_HEADER_ONLY
// #define BOOST_SYSTEM_NO_DEPRECATED

#ifndef SAFE_DELETE
#define  SAFE_DELETE(ptr) do{delete ptr; ptr=nullptr;} while(false) 
#endif
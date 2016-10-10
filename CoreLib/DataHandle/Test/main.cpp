#include <iostream>
#include <RedisAdapter/RedisSubMgr.h>
#include <RedisAdapter/RedisAdapter.h>
#include "Actor.h"
#include <DataTransfer/DataTransfer.h>
int main()
{
// 	Data::RedisWrap stWarp;
// 	Data::RedisSubMgr::Instance().Init("127.0.0.1", 6379, 150000);
// 	stWarp.Init("127.0.0.1", 6379, true,150000);
// 	
// 
// 	Data::RedisAdapter stRedis;
// 	stRedis.Init(10);
// 	stRedis.SetRedisWrap(&stWarp);
// 
// 	PlayerBaseData stData;
// 	stData.SetKey(10000);
// 	stRedis.Load(stData);
// 	stData.Level = 100;
// 
// 	player_bagequip_data stBage;
// 	stBage.Level = 1;
// 	stBage.ConfigId = 10010;
// 	stData.bagequip.Add(stBage, 10);
// 
// 	stData.bagequip.Remove(10);
// 	stRedis.PubAndSet();
	Data::RedisConfig stRedis;
	CDBConf stDB;
	Data::DataTransfer stTrans(stRedis, stDB);
	//stTrans.LoadPackageToRedis("player_base", 12345);
	//stTrans.LoadRepeatedToRedis("player_bagequip", 12345);
	//stTrans.SavePackageToDB("player_base", 12345);
	//stTrans.SaveRepeatedToDB("player_bagequip", 12345);
	CDBConf stConfig;
	stConfig.sHost = "localhost";
	stConfig.sDatabase = "world";
	stConfig.sUser = "root";
	stConfig.sPassword = "123456";
	stConfig.sCharset = "utf8";
	CMysql mySql(stConfig);
	mySql.Connect();
	CMysqlResultPtr pRes = mySql.Query("SELECT * FROM test WHERE KeyFirst = 2");
	while (pRes->Next())
	{
		std::cout << pRes->Get<uint32_t>("value") << std::endl;
	}
	system("pause");
	return 0;
}
#pragma once
#include <string>
#include <list>

class FileUtils
{
public:
	static void ScanDir(const std::string& strPath, std::list<std::string>& listFile, const std::string strFileType = "");

	static void ScanDirWithFullFileName(const std::string& strPath, std::list<std::string>& listFile, const std::string strFileType = "");
private:

};

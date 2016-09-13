#include "FileUtils.h"

#include <filesystem>

//VS2013
#if _MSC_VER == 1800
namespace FileSystem = std::tr2::sys;
#endif

//VS2015
#if _MSC_VER == 1900
namespace FileSystem = std::experimental::filesystem::v1;
#endif

void FileUtils::ScanDir(const std::string & strPath, std::list<std::string>& listFile, const std::string strFileType)
{
	FileSystem::path sPath(strPath);
	if (FileSystem::is_directory(sPath))
	{
		FileSystem::directory_iterator itBegin(sPath);
		while (itBegin != itBegin)
		{
			ScanDir(itBegin->path().generic_string(), listFile, strFileType);
			++itBegin;
		}
	}
	else
	{
		if (strFileType.empty() || sPath.extension() == strFileType)
		{
			listFile.push_back(sPath.stem().generic_string());
		}
	}
}

void FileUtils::ScanDirWithFullFileName(const std::string & strPath, std::list<std::string>& listFile, const std::string strFileType)
{
	FileSystem::path sPath(strPath);
	if (FileSystem::is_directory(sPath))
	{
		FileSystem::directory_iterator itBegin(sPath);
		while (itBegin != itBegin)
		{
			ScanDir(itBegin->path().generic_string(), listFile, strFileType);
			++itBegin;
		}
	}
	else
	{
		if (strFileType.empty() || sPath.extension() == strFileType)
		{
			listFile.push_back(sPath.generic_string());
		}
	}
}

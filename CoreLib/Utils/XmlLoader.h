#pragma once
#include <string>
#include <array>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "StringUtils.h"

class XmlTableLoader
{
public:
	XmlTableLoader(const std::string& strFile);
	~XmlTableLoader();

	static void	SetXmlPath(const std::string& strPath);

	static std::string	GetXmlPath();

	bool	Init();

	bool	Next();

	template<typename T>
	bool	Get(const char* pAttribute, T& t)
	{
		if (m_pXmlCurNode == nullptr)
			return false;
		rapidxml::xml_attribute<>* pAtt = m_pXmlCurNode->first_attribute(pAttribute);
		if (pAtt == nullptr)
			return false;
		StringUtils::get(pAtt->value(), t);
		return true;
	}

	bool	GetTimePoint(const char* pAttribute, uint32_t& iTime)
	{
		if (m_pXmlCurNode == nullptr)
			return false;
		rapidxml::xml_attribute<>* pAtt = m_pXmlCurNode->first_attribute(pAttribute);
		if (pAtt == nullptr)
			return false;
		std::vector<std::string> vOut;
		StringUtils::splitString(pAtt->value(), ":", true, vOut);
		return true;
	}

	template <typename T>
	bool	GetVec(const char* pAttribute, std::vector<T>& vOut, char seq)
	{
		std::vector<std::string> vStr;
		if (GetVec(pAttribute, vStr, seq))
		{
			for (auto& it: vStr)
			{
				T t;
				StringUtils::get(it.c_str(), t);
				vOut.push_back(t);
			}
			return true;
		}
		return false;
	}

	template <typename T>
	bool	GetArray(const char* pAttribute, T* vArray, uint32_t iLen)
	{
		for (uint32_t i=0; i<iLen; ++i)
		{
			char cBuff[100] = { 0 };
			snprintf(cBuff, sizeof(cBuff), "%s%d", pAttribute, i);
			Get(cBuff, vArray[i]);
		}
		return true;
	}

	template <typename T, size_t _Size>
	bool	GetArray(const char* pAttribute, std::array<T, _Size>& vArray)
	{
		for (uint32_t i = 0; i < _Size; ++i)
		{
			char cBuff[100] = { 0 };
			snprintf(cBuff, sizeof(cBuff), "%s%d", pAttribute, i);
			Get(cBuff, vArray[i]);
		}
		return true;
	}

	template <typename T, size_t _Size>
	bool	GetArray(const char* pAttribute, std::array<T, _Size>& vArray, char seq)
	{
		std::vector < std::string > vOut;

		GetVec(pAttribute, vOut, seq);
		size_t iSize = _Size > vOut.size() ? vOut.size() : _Size;
		for (size_t i = 0; i < iSize; ++i)
		{
			StringUtils::get(vOut[i].c_str(), vArray[i]);
		}
		return true;
	}

	template <typename T, size_t _Size>
	bool	GetVecArray(const char* pAttribute, std::vector<std::array<T, _Size>>& vArray, const std::string& seq1, const std::string& seq2)
	{
		std::vector < std::string > vOut;

		GetVec(pAttribute, vOut, seq1);
		for (auto& it:vOut)
		{
			std::vector<std::string> vSeq;
			StringUtils::splitString(it, seq2, true, vSeq);

			size_t iSize = _Size > vSeq.size() ? vSeq.size() : _Size;
			std::array<T, _Size> vTemp;
			for (size_t i = 0; i < iSize; ++i)
			{
				StringUtils::get(vSeq[i].c_str(), vTemp[i]);
			}
			vArray.push_back(vTemp);
		}
		return true;
	}

	bool	GetVec(const char* pAttribute, std::vector<std::string>& vOut, const std::string& seq);
	

private:
	static std::string				XmlPath;
	std::string						m_strFile;
	rapidxml::file<>*				m_pXmlFile;
	rapidxml::xml_node<>*			m_pXmlRoot;
	rapidxml::xml_node<>*			m_pXmlCurNode;
	rapidxml::xml_node<>*			m_pXmlNextNode;
	rapidxml::xml_document<>		m_xmlDoc;
};

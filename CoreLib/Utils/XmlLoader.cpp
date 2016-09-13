#include "XmlLoader.h"
#include <iostream>
std::string XmlTableLoader::XmlPath;

XmlTableLoader::XmlTableLoader(const std::string & strFile)
	: m_pXmlFile(nullptr)
	, m_pXmlRoot(nullptr)
	, m_pXmlCurNode(nullptr)
{
	m_strFile = XmlPath + strFile + ".xml";
}

XmlTableLoader::~XmlTableLoader()
{
	if (m_pXmlFile != nullptr)
	{
		delete m_pXmlFile;
	}
}

void XmlTableLoader::SetXmlPath(const std::string & strPath)
{
	XmlPath = strPath;
}

std::string XmlTableLoader::GetXmlPath()
{
	return XmlPath;
}

bool XmlTableLoader::Init()
{
	if (m_pXmlFile == nullptr)
	{
		try
		{
			m_pXmlFile = new rapidxml::file<>(m_strFile.c_str());
		}
		catch (const std::exception& e)
		{
			std::cout << "Table Load Fail!!!" << std::endl;
			std::cout << e.what() << std::endl;
			return false;
		}
		m_xmlDoc.parse<0>(m_pXmlFile->data());

	}
	m_pXmlRoot = m_xmlDoc.first_node("root");
	if (m_pXmlRoot == nullptr)
	{
		std::cout << "Error: parse table failed, no root node. file name = " << m_strFile;
		return false;
	}

	return true;
}

bool XmlTableLoader::Next()
{
	if (m_pXmlRoot == nullptr)
		return false;
	if (m_pXmlCurNode == nullptr)
	{
		m_pXmlCurNode = m_pXmlRoot->first_node("item");
	}
	else
	{
		m_pXmlCurNode = m_pXmlCurNode->next_sibling("item");
	}
	if (m_pXmlCurNode == nullptr)
	{
		m_pXmlRoot = nullptr;
		return false;
	}
	return true;
}

bool XmlTableLoader::GetVec(const char* pAttribute, std::vector<std::string>& vOut, char seq)
{
	if (m_pXmlCurNode == nullptr)
		return false;
	rapidxml::xml_attribute<>* pAtt = m_pXmlCurNode->first_attribute(pAttribute);
	if (pAtt == nullptr)
		return false;

	StringHelper::Split(pAtt->value(), vOut, seq);
	return true;
}

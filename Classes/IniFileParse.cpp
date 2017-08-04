#include "IniFileParse.h"
#include "cocos2d.h"

USING_NS_CC;

CIniItem::CIniItem(const std::string &key, const std::string &value)
{
	_key = key;
	_value = value;
}

CIniItem::~CIniItem()
{
}

//////////////////////////////////////////////////////////////////////////
CIniSection::CIniSection()
{

}

CIniSection::~CIniSection()
{

}

CIniItem* CIniSection::getItem(int index)
{
	if (index < 0 || index >= (int)_iniItem.size())
	{
		return NULL;
	}

	return &(_iniItem[index]);
}

//////////////////////////////////////////////////////////////////////////
CIniFileParse::CIniFileParse()
{

}

CIniFileParse::~CIniFileParse()
{

}

bool CIniFileParse::LoadFromFile(const std::string &fileName)
{
	//获取文件在系统的绝对路径
	std::string filePath = CCFileUtils::getInstance()->fullPathForFilename(fileName);
	std::unordered_map<std::string, std::string> path = CCFileUtils::getInstance()->getFullPathCache();
	std::unordered_map<std::string, std::string>::iterator ite = path.begin();
	
	//读取的内容
	Data filedata;
	CCFileUtils::getInstance()->getContents(filePath, &filedata);
	//unsigned char* 转 sting 
	std::string my_std_string(reinterpret_cast<char*>(filedata.getBytes()), filedata.getSize());

	StringReplace(my_std_string, " ", "");
	StringReplace(my_std_string, "\r", "");
	StringReplace(my_std_string, ";", "");
	StringReplace(my_std_string, "；", "");

	parseSection(my_std_string);

	return true;
}

std::string& CIniFileParse::StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();
	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
	return strBig;
}

void CIniFileParse::parseSection(const std::string &data)
{
	std::string sectionTag;
	sectionTag.clear();
	std::vector<CIniItem> sectionValue;
	sectionValue.clear();
	
	std::string::const_iterator ite = data.begin();
	while (ite != data.end())
	{
		switch (*ite)
		{
		case '[':
		{
			if (!sectionValue.empty())
			{
				//Saving the sectionValue to _iniItem
				CIniSection section;
				section.setItemTag(sectionTag);
				std::vector<CIniItem>::const_iterator  valueIte = sectionValue.begin();
				while (valueIte != sectionValue.end())
				{
					section.addItem(*valueIte);
					++valueIte;
				}
				sectionValue.clear();
				_iniItem.push_back(section);
			}
			sectionTag.clear();
			++ite;
			char cText = *ite;
			std::string source;
			source = cText;
			while ( ite != data.end() && strcmp(source.c_str(),"]") != 0)
			{
				sectionTag += *ite;
				++ite;
				if (ite != data.end())
				{
					cText = *ite;
					source = cText;
				}
			}

			if (ite != data.end())
			{
				++ite;
			}
		}
		break;
		case ']':
		case '\n':
			++ite;
			break;
		default:
		{
			std::string itemData;
			char cText = *ite;
			std::string source;
			source = cText;
			while (ite != data.end() && strcmp(source.c_str(), "\n") != 0)
			{
				itemData += *ite;
				++ite;
				if (ite != data.end())
				{
					cText = *ite;
					source = cText;
				}
			}

			CIniItem item = parseItem(itemData);
			sectionValue.push_back(item);

			if (ite != data.end())
			{
				++ite;
			}
		}
		break;
		}
	}

	//Saving the last sectionValue to _iniItem
	CIniSection section;
	section.setItemTag(sectionTag);
	std::vector<CIniItem>::const_iterator  valueIte = sectionValue.begin();
	while (valueIte != sectionValue.end())
	{
		section.addItem(*valueIte);
		++valueIte;
	}
	sectionValue.clear();
	_iniItem.push_back(section);
}

CIniItem CIniFileParse::parseItem(const std::string &data)
{
	std::string::const_iterator  ite = data.begin();
	std::string itemTag, itemValue;
	std::string pData;
	pData.clear();
	while (ite != data.end())
	{
		char cText = *ite;
		std::string str;
		str = cText;
		if (strcmp(str.c_str(),"=") != 0)
		{
			pData += *ite;
		}
		else
		{
			itemTag = pData;
			pData.clear();
		}
		++ite;
	}
	itemValue = pData;
	return CIniItem(itemTag,itemValue);
}

CIniSection* CIniFileParse::getItem(int index)
{
	if (index < 0 || index >= (int)_iniItem.size())
	{
		return NULL;
	}

	return &(_iniItem[index]);
}
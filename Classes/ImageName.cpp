#include "ImageName.h"

CImageName::~CImageName(void)
{
}

void CImageName::InitImageName()
{
	auto filePath = cocos2d::FileUtils::getInstance()->getSearchPaths();

	char str[256];
	snprintf(str, sizeof(str), "%s%s",filePath[1].c_str(),"config/imageName.ini");
 	CIniFileParse IniFileParse;
	if (IniFileParse.LoadFromFile(std::string(str)))
 	{
		int n = IniFileParse.getItemNumber();
		for (int i = 0; i < n; i++)
		{
			CIniSection* IniSection = IniFileParse.getItem(i);
			std::string strtempname = IniSection->getTag();
			if (strcmp(strtempname.c_str(), "MapImageName") == 0)
			{
				add(IniSection, MAP_PARENT_TYPE);
			}
			else if (strcmp(strtempname.c_str(), "NormalGemImageName") == 0)
			{
				add(IniSection, NORMAL_GEM_PARENT_TYPE);
			}
			else if (strcmp(strtempname.c_str(), "SpecialGemImageName") == 0)
			{
				add(IniSection, SPECIAL_GEM_PARENT_TYPE);
			}
			else if (strcmp(strtempname.c_str(), "BarriersImageName") == 0)
			{
				add(IniSection, STUMBLING_BLOCK_PARENT_TYPE);
			}
			else
			{
				continue;
			}
		}
 	}
}

void CImageName::add(CIniSection* IniSection, ParentType pType)
{
	int m = IniSection->getItemNumber();
	for (int j = 0; j < m; j++)
	{
		CIniItem* IniItem = IniSection->getItem(j);
		std::string key = IniItem->getTag();
		std::string value = IniItem->getValue();
		if (value.empty() || value == "") continue;

		m_imageName.insert(std::make_pair(key, std::make_pair(pType, atoi(value.c_str()))));
	}
}

std::map<std::string,std::pair<int,int>>::iterator CImageName::FindImageName(const std::string &name)
{
	return m_imageName.find(name);
}

std::string  CImageName::getImageName(int parentType, int type)
{
	std::map<std::string, std::pair<int, int>>::iterator  begin = m_imageName.begin();
	for  (size_t i = 0; i < m_imageName.size(); ++i)
	{
		std::pair<std::string, std::pair<int, int>> data = *(begin++);
		if (data.second.first == parentType && data.second.second == type)
		{
			return data.first;
		}
	}

	return "";
}
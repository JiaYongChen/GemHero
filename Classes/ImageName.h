#pragma once
#ifndef _IMAGE_NAME_H_
#define _IMAGE_NAME_H_
#include <string>
#include <map>
#include "CMD_ThreeEliminate.h"
#include "IniFileParse.h"

class CImageName
{
public:
	static CImageName& getInstance()
	{
		static CImageName imageName;

		return imageName;
	}

public:
	std::map<std::string, std::pair<int, int>>& getImageNameV() { return m_imageName; }
	std::map<std::string, std::pair<int, int>>::iterator FindImageName(const std::string &name);
	std::string getImageName(int parentType, int type);

private:
	CImageName() {
		InitImageName();
	};

	CImageName(const CImageName&);

	CImageName& operator = (const CImageName&);

public:
	~CImageName(void);

private:
	void InitImageName();
	void add(CIniSection* IniSection, ParentType pType);

private:
	std::map<std::string,std::pair<int,int>> m_imageName;
};

#endif




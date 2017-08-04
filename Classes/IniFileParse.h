#pragma once
#include <string>
#include <vector>

class CIniItem
{
public:
	CIniItem(const std::string &key, const std::string &value);
	~CIniItem();

	std::string getTag() { return _key; }
	std::string getValue() { return _value; }

private:
	std::string _key;
	std::string _value;
};

class CIniSection
{
public:
	CIniSection();
	~CIniSection();

public:
	int getItemNumber() { return _iniItem.size(); }
	CIniItem* getItem(int index);
	std::string getTag() { return _tag; }
	void addItem(CIniItem item) { _iniItem.push_back(item); }
	void setItemTag(const std::string &tag) { _tag = tag; }
private:
	std::string _tag;
	std::vector<CIniItem> _iniItem;
};

class CIniFileParse
{
public:
	CIniFileParse();
	~CIniFileParse();

public:
	bool LoadFromFile(const std::string &fileName);
	int getItemNumber() { return _iniItem.size(); }
	CIniSection* getItem(int index);
	void addItem(CIniSection item) { _iniItem.push_back(item); }

private:
	/*Ìæ»»×Ö·û´®Êý¾Ý
	*@param strBig  String data
	*@param strsrc	  Source data
	*@param strdst  Replace data
	*/
	std::string& StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst);

	/* parse ini file
	*@param data Parse data
	*/
	void parseSection(const std::string &data);
	/*
	*@param data Section data
	*/
	CIniItem parseItem(const std::string &data);

private:
	std::vector<CIniSection> _iniItem;
};
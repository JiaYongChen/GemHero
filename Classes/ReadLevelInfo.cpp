#include "ReadLevelInfo.h"
#include <algorithm>
#include "ImageName.h"
#include <iostream>
#include <fstream>
#include "cocos2d.h"

using namespace std;

CReadLevelInfo::CReadLevelInfo(void)
{
}


CReadLevelInfo::~CReadLevelInfo(void)
{
}

void CReadLevelInfo::ClearData()
{
	SafeErase(m_targetList);
	SafeErase(m_levelsGemList);
	SafeErase(m_specialBricksList);
	SafeErase(m_gemList);
	SafeErase(m_mapList);

	m_levelNumber = 0;
	m_stepNum = 0;
	m_time = 0;
	m_maxScore = 0;
	m_score1 = 0;
	m_score2 = 0;
	m_score3 = 0;
	m_levelGameType = 0;
}

int CReadLevelInfo::readCharToInt(const char *file,long &offLength)
{
	char data[4] = {0};
	memcpy(data,file+offLength,sizeof(int));
// 	file.seekg(offLength,std::ios::beg);
// 	file.read(data,sizeof(int));
	offLength += sizeof(int);

	return *(int*)(data);
}

bool CReadLevelInfo::ReadDataFromeFileForIOS(const std::string &fileName)
{
	//关卡信息
	TileImageInfo _tileImageInfo;
	//关卡目标
	std::vector<ImageData> _targetData;
	//关卡宝石
	std::vector<std::string> _gemName;
	
	cocos2d::Data data;
	cocos2d::FileUtils::getInstance()->getContents(fileName, &data);

	char *inFile = reinterpret_cast<char*>(data.getBytes());

	if (data.getSize() > 0)
	{
		ClearData();
		long count = 0;

		m_levelNumber = readCharToInt(inFile,count);
		m_stepNum = readCharToInt(inFile,count);
		m_time = readCharToInt(inFile,count);
		m_maxScore = readCharToInt(inFile,count);
		m_score1 = readCharToInt(inFile,count);
		m_score2 = readCharToInt(inFile,count);
		m_score3 = readCharToInt(inFile,count);

		//读取地图、特殊宝石和障碍
		int number = readCharToInt(inFile,count);
		for (int i = 0; i < number;++i)
		{
			tileImageInfo info;
			info._posX = readCharToInt(inFile,count);
			info._posY = readCharToInt(inFile,count);

			int dataNumber = readCharToInt(inFile,count);
			for (int j = 0; j < dataNumber;++j)
			{
				ImageData pData = {-1};
				pData._type = readCharToInt(inFile,count);

				int length = readCharToInt(inFile,count);
				char name[128] = {0};
				memcpy(name, inFile + count, length);
// 				inFile.seekg(count,std::ios::beg);
// 				inFile.read(name,length);
				count += length;
				pData._imageName = name;

				info._imageInfo.push_back(pData);
			}
			_tileImageInfo.push_back(info);
		}

		//读取关卡目标
		int targetNumber = readCharToInt(inFile,count);
		for (int i = 0; i < targetNumber;++i)
		{
			ImageData info = {-1};
			info._type = readCharToInt(inFile,count);
			
			int length = readCharToInt(inFile,count);
			char name[128] = {0};
			memcpy(name, inFile + count, length);
// 			inFile.seekg(count,std::ios::beg);
// 			inFile.read(name,length);
			count += length;
			info._imageName = name;

			info._num = readCharToInt(inFile,count);
			_targetData.push_back(info);
		}

		//读取关卡出现的宝石
		int gemNumber = readCharToInt(inFile,count);;
		for (int i = 0; i < gemNumber;++i)
		{
			int length = readCharToInt(inFile,count);;
			char name[128]={0};
			memcpy(name, inFile + count, length);
// 			inFile.seekg(count,std::ios::beg);
// 			inFile.read(name,length);
			count += length;

			std::string fileName(name);
			_gemName.push_back(fileName);
		}

		//读取关卡模式
		m_levelGameType = readCharToInt(inFile,count);

		//读取地图元素属性
		int proNumber = readCharToInt(inFile,count);
		for (int i = 0; i < proNumber;++i)
		{
			tileImageInfo *info = &_tileImageInfo[i];

			int dataNum = readCharToInt(inFile,count);
			for (size_t j = 0; j < info->_imageInfo.size();++j)
			{
				ImageData* pData = &info->_imageInfo[j];
				pData->_attribute = readCharToInt(inFile,count);
			}
		}
	}

	ReadDataFromeData(_tileImageInfo,_targetData,_gemName);
	return true;
}

void CReadLevelInfo::ReadDataFromeData(const TileImageInfo &ImageInfo, const std::vector<ImageData> &targetData, const std::vector<std::string> &gemName)
{
	//关卡数据
	m_rulesData.num = m_levelNumber;
	m_rulesData.step = m_stepNum;
	m_rulesData.time = m_time;

	if (m_rulesData.step > 0 && m_rulesData.time > 0)
	{
		m_rulesData.type = LEVELSTART_SETPANDTIME;
	}
	else if (m_rulesData.step > 0 && m_rulesData.time <=0)
	{
		m_rulesData.type = LEVELSTART_STEP;
	}
	else if (m_rulesData.step <= 0 && m_rulesData.time > 0)
	{
		m_rulesData.type = LEVELSTART_TIME;
	}
	else
	{
		m_rulesData.type = LEVELSTART_NULL;
	}

	m_rulesData.gameType = LevelGameType(m_levelGameType);

	//关卡得分
	m_starScore.maxScore = m_maxScore;
	m_starScore.score_1 = m_score1;
	m_starScore.score_2 = m_score2;
	m_starScore.score_3 = m_score3;

	//关卡目标
	ReadTargetData(targetData);

	//宝石
	ReadGemData(gemName);

	//关卡地图数据
	ReadTileMapData(ImageInfo);
}

void CReadLevelInfo::ReadTargetData(const std::vector<ImageData> &targetData)
{
	for (size_t i = 0; i < targetData.size(); ++i)
	{
		ImageData data = targetData[i];
		tagTargetData targetData;
		targetData.ID = i;
		targetData.number = data._num;

		std::map<std::string,std::pair<int,int>>::iterator ite = CImageName::getInstance().FindImageName(data._imageName);
		if (ite != CImageName::getInstance().getImageNameV().end())
		{
			targetData.parentType = ParentType(ite->second.first);
			targetData.type = ite->second.second;

			m_targetList.push_back(targetData);
		}
	}
}

void CReadLevelInfo::ReadGemData(const std::vector<std::string> &gemName)
{
	for (size_t i = 0; i < gemName.size(); ++i)
	{
		std::string name = gemName[i];
		tagLevelsGem gemData;
		std::map<std::string,std::pair<int,int>>::iterator ite = CImageName::getInstance().FindImageName(name);
		if (ite != CImageName::getInstance().getImageNameV().end())
		{
			gemData.ID = i;
			gemData.color = GemColor(ite->second.second);

			m_levelsGemList.push_back(gemData);
		}
	}
}

void CReadLevelInfo::ReadTileMapData(const TileImageInfo &ImageInfo)
{
	for (size_t i = 0; i < ImageInfo.size(); ++i)
	{
		tileImageInfo imageImage = ImageInfo[i];
		for (size_t j = 0; j < imageImage._imageInfo.size(); ++j)
		{
			ImageData imageData = imageImage._imageInfo[j];
			switch(imageData._type)
			{
			case 0:
				{
					tagMapData *pData = new tagMapData();

					pData->id = m_mapList.size();
					pData->indexX = imageImage._posX;
					pData->indexY = imageImage._posY;
					pData->parentType = MAP_PARENT_TYPE;
					pData->isShow = false;
					pData->collect= false;

					if (strcmp(imageData._imageName.c_str(),"fkbg") == 0)
					{
						pData->isShow = true;
						pData->childType = 0;
					}
					else if (strcmp(imageData._imageName.c_str(),"selectGold") == 0)
					{
						pData->isShow = true;
						pData->collect = true;
						pData->childType = 1;
					}
					m_mapList.push_back(pData);
				}
				break;
			case 1:
				{
					tagGemData *pData = new tagGemData();

					pData->id = m_gemList.size();
					pData->indexX = imageImage._posX;
					pData->indexY = imageImage._posY;

					std::map<std::string,std::pair<int,int>>::iterator ite = CImageName::getInstance().FindImageName(imageData._imageName);
					if (ite != CImageName::getInstance().getImageNameV().end())
					{
						pData->parentType =ParentType( ite->second.first);
						if (ite->second.first == NORMAL_GEM_PARENT_TYPE)
						{
							pData->childType = GemColor(ite->second.second);
						}
						else if (ite->second.first == SPECIAL_GEM_PARENT_TYPE)
						{
							pData->childType = GEM_COLOR_RED;
							pData->gemStatus = GemStatus(ite->second.second);
						}
						pData->attribute = imageData._attribute;
					}
					
					m_gemList.push_back(pData);
				}
				break;
			case 2:
				{
					tagSpecialBrickData *pData = new tagSpecialBrickData();

					pData->id = m_specialBricksList.size();
					pData->indexX = imageImage._posX;
					pData->indexY = imageImage._posY;
					pData->parentType = STUMBLING_BLOCK_PARENT_TYPE;
					pData->childType = STUMBLING_BLOCK_NULL;
					pData->direction = 0;

					std::map<std::string,std::pair<int,int>>::iterator ite = CImageName::getInstance().FindImageName(imageData._imageName);
					if (ite != CImageName::getInstance().getImageNameV().end() )
					{
						pData->childType = StumblingBlock(ite->second.second);
						if (strcmp(imageData._imageName.c_str(),"Portal_top") == 0)
						{
							pData->direction = int(UP_DIRECTION);
						}
						else if (strcmp(imageData._imageName.c_str(),"Portal_bottom") == 0)
						{
							pData->direction = int(DOWN_DIRECTION);
						}
						else if (strcmp(imageData._imageName.c_str(),"geban_1") == 0)
						{
							pData->direction = int(LEFT_DIRECTION);
						}
						else if (strcmp(imageData._imageName.c_str(),"geban_2") == 0)
						{
							pData->direction = int(UP_DIRECTION);
						}
						else if (strcmp(imageData._imageName.c_str(),"geban_3") == 0)
						{
							pData->direction = int(RIGHT_DIRECTION);
						}
						else if (strcmp(imageData._imageName.c_str(),"geban_4") == 0)
						{
							pData->direction = int(DOWN_DIRECTION);
						}
					}

					pData->attribute = imageData._attribute;

					tagSpecialBrickData* spData = nullptr;
					for (auto item : m_specialBricksList)
					{
						if (*((tagSpecialBrickData*)item) == *pData)
						{
							spData = (tagSpecialBrickData*)item;
						}
					}
					if (spData != nullptr)
					{
						spData->number++;
					}
					else
					{	
						pData->number++;
						m_specialBricksList.push_back(pData);
					}
					
				}
				break;
			default:
				break;
			}
		}
	}
}

PBaseData CReadLevelInfo::getMapList()
{
	return m_mapList;
}
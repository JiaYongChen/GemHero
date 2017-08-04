#ifndef _READ_LEVEL_INFO_H_
#define _READ_LEVEL_INFO_H_
#include <string>
#include "CMD_ThreeEliminate.h"

class CReadLevelInfo
{
public:
	CReadLevelInfo(void);
	~CReadLevelInfo(void);

	bool ReadDataFromeFileForIOS(const std::string &fileName);
	void ClearData();

public:
	PBaseData getMapList();
	TargetData getTargetList(){return m_targetList;}		//����Ŀ��
	LevelsGem getLevelsGemList(){return m_levelsGemList;}		//�ؿ�����������ֵı�ʯ
	PBaseData getSpecialBricksList(){return m_specialBricksList;}	//�����ϰ�
	PBaseData getGemItemList(){return m_gemList;}

	tagRulesData  getRulesData(){return m_rulesData;}		//�ؿ�ʧ������
	tagStartScore  getStarScore(){return m_starScore;}	//�ǵķ�ֵ

private:
	void ReadDataFromeData(const TileImageInfo &ImageInfo, const std::vector<ImageData> &targetData, const std::vector<std::string> &gemName);
	void ReadTargetData(const std::vector<ImageData> &targetData);
	void ReadGemData(const std::vector<std::string> &gemName);
	void ReadTileMapData(const TileImageInfo &ImageInfo);

	int readCharToInt(const char *file,long &offLength);

public:
	PBaseData m_mapList;

	TargetData m_targetList;						//����Ŀ��
	LevelsGem m_levelsGemList;					//�ؿ�����������ֵı�ʯ
	PBaseData m_specialBricksList;		//�����ϰ�
	PBaseData m_gemList;							//�ؿ���ʼ��ʯ

	tagRulesData  m_rulesData;		//�ؿ�ʧ������
	tagStartScore  m_starScore;		//�ǵķ�ֵ

private:
	//�ؿ�
	int m_levelNumber;
	// ����
	int m_stepNum;
	// ʱ��
	int m_time;
	// ������
	int m_maxScore;
	// һ�Ƿ���
	int m_score1;
	// ���Ƿ���
	int m_score2;
	// ���Ƿ���
	int m_score3;
	//��Ϸ����
	int m_levelGameType;
};
#endif
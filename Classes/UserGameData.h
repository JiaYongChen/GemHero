#ifndef _USER_GAME_DATA_H_
#define _USER_GAME_DATA_H_
#include "CMD_ThreeEliminate.h"

//�û���Ϣ
struct tagUserGameData 
{
	unsigned char strength;		//����
	int countStar;	//�ܹ���õ�������
	int unlockLevel;	//�����Ĺؿ���
	int currentLevel;	//��ǰ�����ؿ�
	time_t oldTime;		//�ϴ���Ϸ����ʱ��
};

//�ؿ���Ϣ
struct tagLeveStarScorelInfo
{
	int levelID;				//�ؿ�ID
	unsigned char starNumber;		//������
	int	score;				//����
};
typedef std::vector<tagLeveStarScorelInfo> LevelData;

class CUserGameData
{
public:
	CUserGameData();
	~CUserGameData();

public:

	void clearUserGameData();
	//�����������
	void setUserGameData(const tagUserGameData& pGameData){m_userGameData = pGameData;}
	//��ȡ�������
	tagUserGameData& getUserGameData(){return m_userGameData;}
	//��ȡ����
	//CBag& getBag(){return m_bag;}
	//������ҹؿ�����
	void addLevelData(const tagLeveStarScorelInfo &pData);
	//��ȡ��ҹؿ���Ϣ
	LevelData& getLevelData(){return m_levelData;}
	//��ȡ��ҹؿ���Ϣ
	tagLeveStarScorelInfo* getLevelInfoByLevelID(int ID);
	//��ȡ��������
	int getCountStarNumber();

private:
	tagUserGameData m_userGameData;		//�����Ϸ��Ϣ
	LevelData  m_levelData;	//�ؿ���Ϣ
	//CBag m_bag;	//����

public:
	int m_bagNumber;
	int m_bagUnlockNumber;
};
#endif
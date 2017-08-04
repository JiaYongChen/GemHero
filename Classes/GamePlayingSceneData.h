#ifndef _GAME_PLAYING_SCENE_DATA_H_
#define _GAME_PLAYING_SCENE_DATA_H_
#include "CMD_ThreeEliminate.h"

class CGamePlayingSceneData
{
public:
	static CGamePlayingSceneData& getInstance()
	{
		static CGamePlayingSceneData _instance;
		return _instance;
	}

private:
	CGamePlayingSceneData();
	~CGamePlayingSceneData();

public:
	//�������
	void clear();
	//��ʼ���ؿ�����
	 bool  initialization(int levelID);
	 //���õ���
	 void setMapData(MapData& pData)
	 {
		 m_mapData.clear();
		 SafeErase(m_mapData);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_mapData.push_back(&pData[i]);
		 }
	 }
	 //��ȡ����
	 const PBaseData& getMapData(){return m_mapData;}
	 //���ñ�ʯ
	 void setGemItem(GemData& pData)
	 {
		 m_gemData.clear();
		 SafeErase(m_gemData);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_gemData.push_back(&pData[i]);
		 }
	 }
	 //��ȡ��ʯ����
	 const PBaseData& getGemItem(){return m_gemData;}
	 //�����ϰ���
	 void setSpecialBrickItem(SpecialBrickData& pData)
	 {
		 m_specialBrick.clear();
		 SafeErase(m_specialBrick);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_specialBrick.push_back(&pData[i]);
		 }
	 }
	 //��ȡ�ϰ�������
	 const PBaseData& getSpecialBrickItem(){return m_specialBrick;}
	 //���ùؿ�������ֵı�ʯ
	 void setLevelsGem(const LevelsGem& pData)
	 {
		 m_levelsGem.clear();
		 SafeErase(m_levelsGem);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_levelsGem.push_back(pData[i]);
		 }
	 }
	 //��ȡ�ؿ�������ֵı�ʯ
	 const LevelsGem& getLevelsGem(){return m_levelsGem;}
	 //���ù���Ŀ��
	 void setTargetData(const TargetData& pData)
	 {
		 m_targetData.clear();
		 SafeErase(m_targetData);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_targetData.push_back(pData[i]);
		 }
	 }
	 //��ȡ����Ŀ��
	 const TargetData& getTargetData(){return m_targetData;}
	 //���ó�ʼ����ʯ
	 void setInitGemData(GemData& pData)
	 {
		 m_initGemList.clear();
		 SafeErase(m_initGemList);
		 for (size_t i = 0; i < pData.size(); ++i)
		 {
			 m_initGemList.push_back(&pData[i]);
		 }
	 }
	 //��ȡ��ʼ����ʯ
	 const GemData& getInitGemData();
	 //���õ÷���Ϣ
	 void setStarScore(const tagStartScore& pData){m_starScore = pData;}
	 //��ȡ�÷���Ϣ
	 const tagStartScore& getStarScore(){return m_starScore;}
	 //����ʧ������
	 void setRulesData(const tagRulesData& pData){m_rulesData = pData;}
	 //��ȡʧ������
	 const tagRulesData& getRulesData(){return m_rulesData;}

private:
	//�ؿ���ʼ����ʯ
	void initializationGem();
	//�������
	void clearData(PBaseData& data);

private:
	PBaseData m_mapData;	//��ͼ��״
	PBaseData m_gemData;	//���б�ʯ����
	PBaseData m_specialBrick;	//�����ϰ�����
	TargetData m_targetData; //����Ŀ��;
	LevelsGem m_levelsGem;	//�ؿ����ֵı�ʯ;
	tagStartScore m_starScore; //�ؿ��÷���Ϣ;
	tagRulesData  m_rulesData;	//�ؿ�ʧ������;

	static PBaseData m_initGemList;		//��ʼ����ʯ����

	int m_gateNumber;	//�ؿ�����
};

#endif
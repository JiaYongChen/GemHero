#ifndef  _GAME_CONFIG_H_
#define _GAME_CONFIG_H_
#include "CMD_ThreeEliminate.h"

class CGameConfig
{
public:
	CGameConfig(void);
	~CGameConfig(void);

	//��ʼ������
	void initialise();
	//�������
	void cleanup();
	//��ȡ�����ļ�
	void load_script(const char* fileName);
	//��ȡ���ؿ���
	int getMaxLevelNumber(){return m_maxLevelNumber;}
	//��ȡ��������а���Ŀ
	int getMaxTotalNumber(){return m_maxTotalNumber;}
	//��ȡ���ؿ����а���Ŀ
	int getMaxLevelListNumber(){return m_maxLevelListNumber;}
	//��ȡ�������ֵ
	int getMaxStrength(){return m_maxStrength;}
	//��ȡ����������ʱ��
	int getAddStrengthTime(){return m_addStrengthTime;}
	//��ȡ��������ֵ
	int getLostStrength(){return m_lostStrength;}
	//��ȡ������������ֵ
	int getSPLostStrength(){return m_specielLostStrength;}
	//��ȡ������������
	long long getBuyStrengthMoney(){return m_buyStrengthMoney;}
	//��ȡһ�ν����Ĺؿ���Ŀ
	int getUnlockLevelNumber(){return m_UnlockLevelNumber;}
	//��ȡ���Ӳ���ʱ��
	int getAddStepTime(){return m_addStepTime;}
	//��ȡ���ӵĲ���
	int getAddStepNumber(){return m_addStepNum;}
	//��ȡ���Ӳ�����Ҫ�Ľ���
	long long getAddStepMoney(){return m_addStepMoney;}
	//��ȡ
	int getAddScorePrecent(){return m_addScorePrecent;}

	//�Ƿ񳬼��û�
	bool IsSuperUser(unsigned long userID);

private:
	//�ؿ�
	int m_maxLevelNumber;		//���ؿ���
	int m_maxTotalNumber;		//������а����
	int m_maxLevelListNumber;	//���ؿ����а����
	int m_UnlockLevelNumber;	//һ�ν����Ĺؿ���Ŀ

	//����
	int m_addStrengthTime;		//����������ʱ
	int m_maxStrength;	//�������ֵ
	int m_lostStrength;	//���ĵ�����ֵ
	int m_specielLostStrength;	//����ؿ����ĵ�����ֵ
	long long m_buyStrengthMoney; //������������
	int m_addStepTime;//���Ӳ�����ʱ��
	int m_addStepNum;	//���Ӳ���
	long long m_addStepMoney;//���Ӳ�����Ҫ�Ľ���
	int m_addScorePrecent;		//���ӷ����ı���


	//�����û�
	std::vector<unsigned long> m_superUser;
};

#endif
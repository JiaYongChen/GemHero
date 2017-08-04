#include "GameConfig.h"
#include <boost/lexical_cast.hpp>

CGameConfig::CGameConfig(void)
{
	initialise();
}


CGameConfig::~CGameConfig(void)
{
	cleanup();
}

void CGameConfig::initialise()
{
	m_addStrengthTime = ADD_STRENGTH_TIME;
	m_lostStrength = LOST_STRENGTH;
	m_specielLostStrength = SP_LOST_STRING;
	m_maxLevelListNumber = MAX_LEVELLISTNUM;
	m_maxLevelNumber = MAX_LEVELNUMBER;
	m_maxStrength = MAX_STRENGTH;
	m_maxTotalNumber = MAX_TOTALLISTNUM;
	m_buyStrengthMoney = 2000;
	m_UnlockLevelNumber = 7;
	m_addStepMoney = 2000;
	m_addStepNum = 5;
	m_addStepTime = 15;
	m_addScorePrecent = 25;
}

void CGameConfig::cleanup()
{
	m_addStrengthTime = ADD_STRENGTH_TIME;
	m_lostStrength = LOST_STRENGTH;
	m_specielLostStrength = SP_LOST_STRING;
	m_maxLevelListNumber = MAX_LEVELLISTNUM;
	m_maxLevelNumber = MAX_LEVELNUMBER;
	m_maxStrength = MAX_STRENGTH;
	m_maxTotalNumber = MAX_TOTALLISTNUM;
	m_buyStrengthMoney = 2000;
	m_UnlockLevelNumber = 7;
	m_addStepMoney = 2000;
	m_addStepNum = 5;
	m_addStepTime = 15;
	
	SafeErase(m_superUser);
	m_superUser.clear();
}

void CGameConfig::load_script(const char* fileName)
{
	//设置文件名
	std::string m_szIniFileName;
	m_szIniFileName = boost::lexical_cast<std::string>(fileName);

	//体力
// 	m_addStrengthTime=GetPrivateProfileInt(("step"),"addStrengthTime",m_addStrengthTime,m_szIniFileName);
// 	m_lostStrength =GetPrivateProfileInt(TEXT("step"),"lostStrength",m_lostStrength,m_szIniFileName);
// 	m_specielLostStrength = GetPrivateProfileInt(TEXT("step"),"specielLostStrength",m_specielLostStrength,m_szIniFileName);
// 	m_maxStrength =GetPrivateProfileInt(TEXT("step"),"maxStrength",m_maxStrength,m_szIniFileName);
// 	m_buyStrengthMoney =GetPrivateProfileInt(TEXT("step"),"buyMoney",m_buyStrengthMoney,m_szIniFileName);
// 	m_addStepTime =GetPrivateProfileInt(TEXT("step"),"addStepTime",m_addStepNum,m_szIniFileName);
// 	m_addStepNum =GetPrivateProfileInt(TEXT("step"),"addStep",m_addStepNum,m_szIniFileName);
// 	m_addStepMoney =GetPrivateProfileInt(TEXT("step"),"addStepMoney",m_addStepMoney,m_szIniFileName);
// 	m_addScorePrecent = GetPrivateProfileInt(TEXT("step"),"addScorePrecent",m_addScorePrecent,m_szIniFileName);

	//关卡
// 	m_maxLevelNumber = GetPrivateProfileInt(TEXT("level"),"maxLevel",m_maxLevelNumber,m_szIniFileName);
// 	m_maxTotalNumber = GetPrivateProfileInt(TEXT("level"),"maxTotalList",m_maxTotalNumber,m_szIniFileName);
// 	m_maxLevelListNumber = GetPrivateProfileInt(TEXT("level"),"maxLevleList",m_maxLevelListNumber,m_szIniFileName);
// 	m_UnlockLevelNumber =GetPrivateProfileInt(TEXT("level"),"unlockLevel",m_UnlockLevelNumber,m_szIniFileName);

	//读取超级用户
	m_superUser.clear();
	for (int i = 0;;++i)
	{
// 		TCHAR szBuffer[32];
// 		_snprintf_s(szBuffer,sizeof(szBuffer),"superUser%d",(i+1));
// 		unsigned long userID = GetPrivateProfileInt(TEXT("Rule"),szBuffer,-1,m_szIniFileName);
// 		if (userID == -1)
// 		{
// 			break;
// 		}
// 		else
// 		{
// 			m_superUser.push_back(userID);
// 		}
	}
}
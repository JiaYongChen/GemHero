#pragma once
#include "GamePlayingSceneData.h"
#include "CMD_ThreeEliminate.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
public:
	static CGameLogic& getInstance() {
		static CGameLogic _instance;

		return _instance;
	}

	//��������
public:
	//��������
	virtual ~CGameLogic();

private:
	//���캯��
	CGameLogic();
	CGameLogic(const CGameLogic&);
	CGameLogic& operator= (const CGameLogic&);
	
	

public:
	//��ȡ�ؿ�����
	void loadLevelDataFromeXML(int levelsID);
	void loadLevelDataFromeFile(int levelsID);
	bool loadLevelDataFromeData(const CGamePlayingSceneData& pPlayingData);
	//���¿�ʼ��Ϸ
	void restartGame();
	//��ȡ��ǰ�ؿ�����
	int getCurrentGateID(){return m_gateNumber;}
	//��ȡ��ǰ�ؿ�������
	std::string getCurrentLevelsData();
	//��ȡ��ǰ��ͼ����
	const PBaseData& getCurrentMapData();
	//��ȡ�ϰ�
	const PBaseData& getCurrentSpecialBrickData();
	//��ȡ��ʯ
	const PBaseData& getCurrentGemData();
	//��ȡ
	TileBrick& getCurrentTileBrick();
	//��ȡ����������Ϣ
	const TargetData& getSuccessCondition();
	//��ȡ��ǰ�ؿ��ĵ÷�����
	const tagStartScore& getScoreCondition();
	//���õ�ǰ�ؿ��Ĳ���
	void setCurrentGateStep(int step){m_currentStep = step;}
	//��ȡ��ǰ�ؿ�����
	int getCurrentGateStep(){return m_currentStep;}
	//��ȡ�ؿ�Ŀ��
	const TargetData& getCurrentSuccessCondition();
	//��ȡ��ǰ�ؿ���ȡ�ķ�ֵ
	int GetCurrentGateScore();
	//������Ϸ״̬
	void setGameStatus(LevelsGameStatus status){m_gameStatus = status;}
	//��ȡ��Ϸ״̬
	LevelsGameStatus getGameStatus(){return m_gameStatus;}
	//��Ϸ����״̬
	void setGamePlayingStatus(GamePlayingStatus status){m_gamePlayingStatus = status;}
	//��ȡ��Ϸ����״̬
	GamePlayingStatus getGamePlayingStatus(){return m_gamePlayingStatus;}
	//��ȡ��ǰ���е����ⱦʯ
	void getAllSpecilGem(std::vector<cocos2d::Vec2> &GemPos);
	//��ʼ����ʯ
	void setInitGemList(GemData& gemItem){m_initGemList = gemItem;}
	//��ȡ��Ϸ������
	int getPlayingGameListNumber(){return m_playingGameList.size();}
	//��ȡ������������ʯ����
	TileBrick getSpecialGem();
	//��ȡ��ǰ������
	int getCurrentComboNumber(){return m_currentComboNumber;}
	void setCurrentComboNumber(int combo){m_currentComboNumber = combo;}
	//�ж��Ƿ����ʹ�õ���
	bool isCanUseProp(int posX,int posY);
	bool isCanUseColorProp();
	//�߼�����
public:
	//����ƶ�
	swapGemStatus IsValidMove(unsigned short scoreX, unsigned short scoreY, unsigned short targetX, unsigned short targetY,bool flag = true);  // 0.��Ч������1.��ͨ������2.���ⱦʯ����
	//������ʯ
	bool checkConnect(std::vector<cocos2d::Vec2 > &ConnectGemPos,bool flag = true);
	//��ȡ���ⱦʯ
	void ObtainSpecialGems(std::vector<ObtainSpecialGemInfo> &outInfos, std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos);
	//��������Ч��
	void touchSpecialGems(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos,std::vector<ObtainSpecialGemInfo> &outInfos);
	//����Ч�������ı�ʯ
	void SpecialEffectsToEliminateGems(TileBrickData &pTile,std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos);
	//��ȡ����ı�ʯ����
	bool DropGem(std::vector<cocos2d::Vec2 > &dropPos, GemData &pGemList);
	//��ȡÿһ�е���С����
	int getMinRowNumber(int colNumber);
	//��ȡ��ǰ���������
	TileBrickData* currentTileBrickItem(int posX,int posY);
	//�жϵ�ǰλ���Ƿ���ڱ�ʯ���ϰ�:0.��ʯ�ϰ��������ڣ�1.��ʯ�ϰ������ڣ�2.ֻ���ڱ�ʯ��3.ֻ�����ϰ���4.����
	enBrickType IsHaveGemOrStumbling(const TileBrickData &pTile);
	//��ȡ��ǰ���ӿ����������ϰ���
	tagSpecialBrickData* getRemoveSpecialBrickItem(int posX,int posY);
	//���ⱦʯ����
	void swapSpecialGem(std::vector<cocos2d::Vec2> &connectGemPos,std::vector<ObtainSpecialGemInfo> &outInfos,SpecialBrickData &StumblingBlockPos);
	//�Ƿ��ܽ�����Ϸ
	bool IsCanEndGame();
	//������Ϸ����
	gameEndStatus GameEndStatus();
	//��ȡ��ǰ�ؿ���ȡ����ֵ
	int GetCurrentGateStar();
	//��ȡbonusTimeʱ��Ҫת���ɵ����ⱦʯ
	void getBonusTimeGem(std::vector<ObtainSpecialGemInfo> &outInfos);
	//�����ϰ���
	void removeSpecialBrickItm(int posX,int posY,TileBrickData *pTileBrick,SpecialBrickData &StumblingBlockPos);
	//�ı����Ŀ�������
	void changeTargetNum(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos,std::vector<ObtainSpecialGemInfo> &outInfos);
	//�����Ƿ��п��������ı�ʯ
	bool retrieveConnectGem(cocos2d::Vec2 point1 = cocos2d::Vec2(0,0),cocos2d::Vec2 point2 = cocos2d::Vec2(0,0));
	//���̳�ʼ�����п���ֱ�������ı�ʯ
	bool  retrieveTheBoard(bool flag = true);
	//���������ϰ���
	void ObtainSpecialBrickItems(MoveSpecialBrick &StumblingBlockPos);
	//�ı䱦ʯ��ɫ
	void changeGemColor(ChangeGemColour &outInfo);
	//���ӽ���
	void addGoldPod(tagGemData &pItem);
	//�÷ְٷֱ�
	void setScorePrecent(float precent){m_precentScore += precent;}
	//������һ��
	void goBack();
	//ʹ�õ�������
	void setUsePropType(enProp type){m_usePropType = type;}
	//��ȡʹ�õ�������
	enProp getUsePropType(){return m_usePropType;}
	//�任��ʯ
	bool changeGemType(int posX,int posY,GemStatus type);
	//��ȡ�Ƿ��ƶ��˱�ʯ
	bool getIsSwapGem(){return m_bSwapGem;}
	//�ı�ը����ʯ��ʱ��
	void changTimeBomGemTime();
	
	//���µ���
	bool dropBottom(TileBrickData *pTileBrick, TileBrickData *bottomBrick, cocos2d::Vec2 &dropPos);
	//�����µ���
	bool dropLeftBottom(TileBrickData *pTileBrick, TileBrickData *leftBottomBrick, cocos2d::Vec2 &dropPos);
	//�����µ���
	bool dropRightBottom(TileBrickData *pTileBrick, TileBrickData *rightBottomBrick, cocos2d::Vec2 &dropPos);
	//���ɱ�ʯ
	bool addNewGem(TileBrickData *pTileBrick, tagGemData &pGemList);
	//�жϵ�ǰ�����Ƿ����´�����
	bool IsHaveDownPortal(int posX, int PosY, tagSpecialBrickData *downPortal);
	//��ȡ��Ӧ�����ŵ�λ��
	tagSpecialBrickData* getAnotherPortal(const tagSpecialBrickData& pItem, bool flag);

public:
	unsigned short GetNextChair(unsigned short chair){return (chair+1)%GAME_PLAYER;}
	unsigned short GetAheadChair(unsigned short chair){return (GAME_PLAYER+chair-1)%GAME_PLAYER;}

	static tagGemData randomCreateGem(LevelsGem levelsGem);

private:
	template<class Type> 
	void addVectorData(std::vector<Type> &pVec,Type pData);

	//�ڲ�����
private:
	//��ʼ������
	void InitGame();
	//�������
	void clearListData();
	//�ܷ����ɱ�ʯ
	bool IsCanCreateGem(const TileBrickData &pTileBrick);
	//�����ϰ�
	void removeStumblingBlock(std::vector<cocos2d::Vec2> &connectGemPos,SpecialBrickData &StumblingBlockPos);
	//�ϰ��Ƿ�������
	bool  IsCanRemoveStumblingBlock(int posX,int posY,tagSpecialBrickData &pStumblingBlock);
	//�ϰ����Ƿ����ƶ�
	bool IsCanMoveStumblingBlock(tagSpecialBrickData &pStumblingBlock);
	//��ȡ��ߵ�����
	TileBrickData* leftTileBriteItem(int posX,int posY,TileBrickData *outTileBrick = NULL);
	//��ȡ�ұߵ�����
	TileBrickData* rightTileBriteItem(int posX,int posY,TileBrickData *outTileBrick  = NULL);
	//��ȡ�ϱߵ�����
	TileBrickData* upTileBriteItem(int posX,int posY,TileBrickData *outTileBrick  = NULL);
	//��ȡ���������
	TileBrickData* bottomTileBriteItem(int posX,int posY,TileBrickData *outTileBrick  = NULL);
	//�ܷ񽻻���ʯ
	bool IsCanSwapGem(const TileBrickData &tile,const TileBrickData &pTile,bool flag = true);
	//���ñ�ʯ
	void resetGem(bool flag = true);
	//������ʯ
	void swapGem(tagGemData &scoreGem,tagGemData &targetGem);
	//�����ϰ���
	void swapSpecialBrick(tagSpecialBrickData &sourceItem,tagSpecialBrickData targetItem);
	//��ȡ����Ч����ʯ
	GemStatus getSpecialGemType(cocos2d::Vec2 point);
	//�Ƿ�������ɱ�ʯ
	bool IsCanAddGem(TileBrickData *pTile);
	//�������
	void CalculateScore(const TileBrickData &pTile);
	//��ȡ���������ĸ���
	cocos2d::Vec2 getROWAndCOLCount(cocos2d::Vec2 point,std::vector<cocos2d::Vec2> &outPoint,std::vector<cocos2d::Vec2> &connectGemPos);
	//��ȡ��ǰ���ӵ����ϲ��ϰ�������
	tagSpecialBrickData* getTheTopSpecialBrickItem(int posX,int posY);
	//�жϵ�ǰ���Ƿ��б�ʯ��������
	bool IsHaveGemDrop(int posY,TileBrickData *pTileBrick,bool flag = true);
	//ë���ƶ�
	void moveBulb(MoveSpecialBrick &pSpecial);
	//���ӹ���
	void addJelly(MoveSpecialBrick &pSpecial);
	//�жϵ�ǰ�����Ƿ����ϴ�����
	bool IsHaveUpPortal(int posX,int posY,tagSpecialBrickData *upPortal = NULL);
	//�ж��Ƿ�������
	bool IsCanConnectGem(TileBrickData &tile,TileBrickData &otherTile);
	//�жϵ�ǰ���ӱ�ʯ�ܷ��ƶ�
	bool IsCanMoveGem(int posX,int PosY);
	//�жϵ�ǰ�����Ƿ��������
	bool IsCanConnect(int posX,int posY);
	//�����ϲ��ؿ�����
	void saveLevelData();
	//��ȡ�ƶ���ĵ�λ
	TileBrickData* getMovedPosition(int dir,int posX,int posY);
	//��ȡ��
	TileBrickData* getNewSpecialBlock(tagSpecialBrickData &pItem);
	//����Ŀ����Ŀ
	void changeTargetCount(tagTargetData target);
	//��ȡ��ǰ��ɫ��ʯ��Ŀ
	int getGemNumberByColor(GemColor color);
	//�Ƿ���timeBombGem
	GemData getTimeGem();

public:
	//�����ú�������ʾ��ǰ��ʯ
	void showCurrentTile();

private:
	TileBrickData		m_tileBrick[ROW_COUNT][COL_COUNT];		//��ǰ��Ļ��ʾש����������
	TileBrick				m_brickList;
//	std::vector<cocos2d::Vec2> m_currentConnect;						//��ǰҪ�����ı�ʯ������
	std::vector<TileBrick> m_playingGameList;							//��Ϸ��������

private: 
	int m_sourceX;
	int m_sourceY;
	int m_targetX;
	int m_targetY;

	//��ʼ�ؿ�����
private:
	PBaseData m_gemData;							//���б�ʯ����
	PBaseData m_specialBrick;			//�����ϰ�����
	PBaseData m_mapList;							//��ͼ��״
	TargetData m_targetList;						//����Ŀ��
	LevelsGem m_levelsGemList;					//�ؿ�����������ֵı�ʯ

	tagRulesData  m_rulesData;		//�ؿ�ʧ������
	tagStartScore  m_starScore;	//�ǵķ�ֵ
	std::vector<int>	m_oldStarScoreList;

	int m_gateNumber;				//��ǰ�ؿ�����

	GemData m_initGemList;		//��ʼ����ʯ����

private:
	TargetData m_currentTargetList;	//��ǰ�ؿ��Ѿ�������Ŀ������
	std::vector<TargetData> m_gameTargetList;
	LevelStartType m_levelStartType;	//�ؿ���ʼ����
	int m_scoreNumber;						//��ǰ�ؿ����÷���
	int m_currentStep;						//��ǰʣ�ಽ��
	int m_currentTime;						//��ǰʱ��
	int m_currentComboNumber;		//��ǰ������
	float m_precentScore;					//�÷ְٷֱ�

	//״̬��ʾ
	LevelsGameStatus m_gameStatus;
	GamePlayingStatus m_gamePlayingStatus;
	enProp m_usePropType; //ʹ�õ�������
	bool m_bSwapGem;
	bool m_bDisappearJelly;

	public:
		swapGemStatus m_swapGemStatus;
};

//////////////////////////////////////////////////////////////////////////

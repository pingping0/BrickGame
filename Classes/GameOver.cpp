#include "GameOver.h"
#include "GeneralManager.h"

CGameOver::CGameOver(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CGameOver::~CGameOver()
{
}


//初始化
void CGameOver::Init()
{
	//避免随机数互相影响，放在这里清除排除的无效值
	ClearInvalidNums();

	m_iRowIdx = 0;
	m_iColIdx = 0;
	m_iDirection = DIR_RIGHT;
	m_iAllCount = 0;
	m_iEndRowIdx = ROW_NUM - 1;
	m_iEndColIdx = COLUMN_NUM - 1;
	m_iBeginColIdx = -1;
	m_iBeginRowIdx = 0;

	//保存最高分数据
	CGeneralManager::getInstance()->SaveHighScoreToFile();
}


//更新
void CGameOver::Play(float dt)
{
	if (m_iAllCount == ROW_NUM * COLUMN_NUM)
	{
		m_pGameScene->RunScene(SCENE_CHOOSEGAME);
		return;
	}

	++m_iAllCount;

	//更新Brick
	m_pGameScene->UpdateBrick(m_iRowIdx, m_iColIdx, false, true);

	switch (m_iDirection)
	{
	case DIR_RIGHT:
		++m_iColIdx;
		break;
	case DIR_DOWN:
		++m_iRowIdx;
		break;
	case DIR_LEFT:
		--m_iColIdx;
		break;
	case DIR_UP:
		--m_iRowIdx;
		break;
	}
	//到达右上角
	if (m_iColIdx == m_iEndColIdx && m_iRowIdx == m_iBeginRowIdx)
	{
		m_iDirection = DIR_DOWN;
		++m_iBeginColIdx;
	}
	else if (m_iColIdx == m_iEndColIdx && m_iRowIdx == m_iEndRowIdx)
	{
		m_iDirection = DIR_LEFT;
		++m_iBeginRowIdx;
	}
	else if (m_iColIdx == m_iBeginColIdx && m_iRowIdx == m_iEndRowIdx)
	{
		m_iDirection = DIR_UP;
		--m_iEndColIdx;
	}
	else if (m_iColIdx == m_iBeginColIdx && m_iRowIdx == m_iBeginRowIdx)
	{
		m_iDirection = DIR_RIGHT;
		--m_iEndRowIdx;
	}
}


SCENE_INDEX CGameOver::GetSceneType()
{
	return SCENE_GAMEOVER;
}


void CGameOver::GetCurPos(int& iRowIndex, int& iColIndex)
{
	iRowIndex = m_iRowIdx;
	iColIndex = m_iColIdx;
}


void CGameOver::OnLeftBtnPressed()
{
	OnFireBtnPressed();
}


void CGameOver::OnRightBtnPressed()
{
	OnFireBtnPressed();
}


void CGameOver::OnDownBtnPressed()
{
	OnFireBtnPressed();
}


void CGameOver::OnUpBtnPressed()
{
	OnFireBtnPressed();
}


void CGameOver::OnFireBtnPressed()
{
	m_pGameScene->ResetBricks();
	m_pGameScene->RunScene(SCENE_CHOOSEGAME);
}


void CGameOver::OnStart()
{
	m_pGameScene->RunScene(SCENE_CHOOSEGAME);
}

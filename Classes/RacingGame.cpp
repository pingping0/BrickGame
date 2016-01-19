#include "RacingGame.h"


CRacingGame::CRacingGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CRacingGame::~CRacingGame()
{
}


//初始化
void CRacingGame::Init()
{
	m_iSpeed = GET_INTVALUE("SPEED", 0);
	m_iLevel = GET_INTVALUE("LEVEL", 0);

	//默认生命数为4
	m_iLife = GET_INTVALUE("LIFE", 4);

	//默认顶部三行已有路标
	m_iRoadSignCount = 3;

	//等待更新时间
	m_fWaitTime = 0;

	//初始化第一次显示标记
	m_bFirstShow = true;

	//游戏结束标记
	m_bGameOver = false;

	InitBrick();
}

//更新
void CRacingGame::Play(float dt)
{
	if (!m_bFirstShow)
	{
		m_fWaitTime += dt * 1000;
		if (m_fWaitTime < 500)
		{
			return;
		}

		//重置时间
		m_fWaitTime = 0;
	}
	else
	{
		m_bFirstShow = false;
	}
	
	AddNewLine();
	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CRacingGame::GetBrickState(int iRowIndex, int iColIndex)
{
	//赛车位置
	int iCarColIdx = m_iCarPos * 3 + 2;
	int iCarRowIdx = ROW_NUM - 2;

	if (m_bGameOver)
	{
		//爆炸
		if (   (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx + 1)	//四个角
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx - 2)
			|| (iRowIndex == iCarRowIdx - 2 && iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx - 2 && iColIndex == iCarColIdx - 2)
			|| (iRowIndex == iCarRowIdx		&& iColIndex == iCarColIdx - 1)	//中间
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx	  )
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx - 1)
			|| (iRowIndex == iCarRowIdx		&& iColIndex == iCarColIdx	  )
			)
		{
			return true;
		}
	}
	else
	{
		//画赛车自己
		if (   (iRowIndex == iCarRowIdx		&& iColIndex == iCarColIdx	  )
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx	  )
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx - 1)
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx - 1)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx	  )
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx - 2 && iColIndex == iCarColIdx	  )
			)
		{
			return true;
		}

	}
	return m_arrCurBrick[iRowIndex][iColIndex];
}


//获取游戏类型
SCENE_INDEX CRacingGame::GetSceneType()
{
	return SCENE_RACING;
}


//左
void CRacingGame::OnLeft()
{
	if (m_iCarPos > 0)
	{
		--m_iCarPos;
		m_pGameScene->UpdateBricks();
	}
}

//右
void CRacingGame::OnRight()
{
	if (m_iCarPos < ROAD_MAXINDEX)
	{
		++m_iCarPos;
		m_pGameScene->UpdateBricks();
	}
}

//上
void CRacingGame::OnUp()
{

}

//下
void CRacingGame::OnDown()
{

}

//Fire
void CRacingGame::OnFire()
{

}


//添加新行
void CRacingGame::AddNewLine()
{
	//将上一行挪到下一行
	for (int i = ROW_NUM - 1; i >= 1; --i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrCurBrick[i][j] = m_arrCurBrick[i - 1][j];
		}
	}

	if (m_iRoadSignCount < 3)
	{
		//新行，如果当前路标数量小于3，继续设置路标
		m_arrCurBrick[0][0] = true;
		m_arrCurBrick[0][COLUMN_NUM - 1] = true;
		for (int i = 1; i < COLUMN_NUM - 1; ++i)
		{
			m_arrCurBrick[0][i] = false;
		}
	}
	else
	{
		m_iRoadSignCount = -1;
		for (int i = 0; i < COLUMN_NUM; ++i)
		{
			m_arrCurBrick[0][i] = false;
		}
	}

	++m_iRoadSignCount;
}


//画赛车
void CRacingGame::DrawCar(int iRoadIdx, int iRowIdx)
{
	//　口
	//口口口
	//　口		位置以该行为准
	//口　口
	int iColIdx = iRoadIdx * 3 + 1;

	//第四行
	m_arrCurBrick[iRowIdx + 1][iColIdx - 1] = 1;
	m_arrCurBrick[iRowIdx + 1][iColIdx + 1] = 1;
	//第三行
	m_arrCurBrick[iRowIdx][iColIdx] = 1;
	//第二行
	m_arrCurBrick[iRowIdx - 1][iColIdx] = 1;
	m_arrCurBrick[iRowIdx - 1][iColIdx - 1] = 1;
	m_arrCurBrick[iRowIdx - 1][iColIdx + 1] = 1;
	//第一行
	m_arrCurBrick[iRowIdx - 2][iColIdx] = 1;
}


void CRacingGame::InitBrick()
{
	//初始化Brick
	int iCount = 0;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		int iBeginIndex = 0;
		int iEndIndex = COLUMN_NUM - 1;
		if (iCount < 3)
		{
			m_arrCurBrick[i][0] = true;
			m_arrCurBrick[i][iEndIndex] = true;

			iBeginIndex = 1;
			iEndIndex -= 1;
		}
		else
		{
			iCount = -1;
		}

		for (int j = iBeginIndex; j <= iEndIndex; ++j)
		{
			m_arrCurBrick[i][j] = false;
		}

		++iCount;
	}

	//随机自己赛车的位置
	srand((unsigned)time(nullptr));
	m_iCarPos = rand() % 4;
}


//是否游戏结束
bool CRacingGame::IsGameOver()
{
	return false;
}


//更新Brick
void CRacingGame::UpdateBricks()
{

}

#pragma once

#include <windows.h>
#include <thread>
#include <gdiplus.h>
#include <stack>
#include "WK_SudokuSolverMsgDef.h"

using namespace Gdiplus;
using namespace std;

class WK_SudokuCore
{
public:
  WK_SudokuCore();
  WK_SudokuCore(WK_SudokuCore* up);
  ~WK_SudokuCore();

  static WK_SudokuCore* GetInstance();

  void Start();
  void SetMsgNoti(HWND MsgNoti);
  void BeginSolver();
  void SetData(void* data);
  bool* GetPossable(Point pt);
  bool m_Running = false;
  HWND GetNoti() { return m_MsgNoti; }

  bool DoSolver();

private:

  void WorkoutPointPossable(Point pt);
  int CheckUndecideCnt();
  void SetAllDecided();
  void JangePanelPossable();
  void WorkoutAllPointPossable();
  void WorkoutSingelLinePossable();

  bool HasThisPossableinX(int zone, int y, int possable);
  bool NoThisPossableinX(int zone, int y, int possable);
  void DeleteOtherXPossable(int zone, int y, int possable);

  bool HasThisPossableinY(int zone, int x, int possable);
  bool NoThisPossableinY(int zone, int x, int possable);
  void DeleteOtherYPossable(int zone, int x, int possable);

  bool FindNoPossiblePt();
  int CntPtPossible(bool *possible);

  void SudokuMessage(unsigned int msgid, int msg = 0);
  bool FindOlnyPossablePt();
  void SetNumberPossable(int x, int y, int possable);
  bool FindOnlyPassable_One(int& x, int& y, int& possable);
  bool FindNextPtPossable(int& x, int& y, int& possable);

  bool FindOnlyPassable_Only(int x, int y, int possable);
  bool FindOnlyPassable_InZone(int x, int y, int possable);
  bool FindOnlyPassable_InX(int x, int y, int possable);
  bool FindOnlyPassable_InY(int x, int y, int possable);


  static WK_SudokuCore* g_instance;
  SudokuPanel_Data m_Data[9][9];
  SudokuPanel_PossAble m_Possable[9][9];
  HWND m_MsgNoti = nullptr;
  bool m_Guessing = false;
};


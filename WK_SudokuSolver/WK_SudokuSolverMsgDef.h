#pragma once

#define WK_NUMBER_SELECT                  WM_USER + 2
#define WK_SUDOKU_START                   WM_USER + 3
#define WK_SUDOKU_END                      WM_USER + 4
#define WK_SUDOKU_PROCESS              WM_USER + 5
#define WK_SUDOKU_NOANSWER         WM_USER + 6
#define WK_SUDOKU_NUMBER_SURE    WM_USER + 7
#define WK_SUDOKU_NUMBER_GUESS  WM_USER + 8
#define WK_SUDOKU_SUCCESS              WM_USER + 9

enum SUDOKU_PANEL_NUMBER_STATUS {
  NUMBER_UNSET = 0,
  NUMBER_SURE = 1,
  NUMBER_GUESS = 2,
  NUMBER_DISABLE = 3,
};

struct SudokuPanel_Data
{
  int num;
  SUDOKU_PANEL_NUMBER_STATUS stauts;
  SudokuPanel_Data() {
    num = -1;
    stauts = NUMBER_UNSET;
  }
};

struct SudokuPanel_PossAble
{
  bool possable[9];
  SudokuPanel_PossAble() {
    memset(possable, 1, sizeof(bool) * 9);
  }
};

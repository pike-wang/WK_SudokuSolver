#pragma once

#include <UIlib.h>
#include "WK_SudokuSolverSelectWnd.h"
#include "WK_SudokuCore.h"

using namespace DuiLib;

class CSudokuSolverWnd : public WindowImplBase
{
public:
  virtual LPCTSTR    GetWindowClassName() const { return _T("WK_SudokuSolver"); }
  virtual CDuiString GetSkinFile() { return _T("WK_SudokuSolverMainFrame.xml"); }
  virtual CDuiString GetSkinFolder() { return _T("xml"); }

  DUI_DECLARE_MESSAGE_MAP()

  virtual void OnClick(TNotifyUI& msg);
  virtual void InitWindow();
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual void CleanPorcess();

private:
  CProgressUI* m_progress = nullptr;
  CSudokuPanel* m_MainPanel = nullptr;
  CButtonUI* m_StartButton = nullptr;
  CButtonUI* m_CleanButton = nullptr;
  WK_SudokuSolverSelectWnd* m_SelectWnd = nullptr;
};


#pragma once

#include <UIlib.h>
#include "CWKStartWnd.h"
#include "CSudokuSolverWnd.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  CPaintManagerUI::SetInstance(hInstance);
  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) return 0;

  CWKStartWnd StartFrame;
  StartFrame.Create(NULL, _T("WK_Start"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
  StartFrame.CenterWindow();
  if (!StartFrame.ShowModal())
    return 0;

  CSudokuSolverWnd duiFrame;
  duiFrame.Create(NULL, _T("WK_SudokuSolver"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
  duiFrame.CenterWindow();
  duiFrame.ShowModal();

  ::CoUninitialize();
  return 0;
}
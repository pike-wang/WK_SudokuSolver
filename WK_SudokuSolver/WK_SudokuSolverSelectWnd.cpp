#include "WK_SudokuSolverSelectWnd.h"

DUI_BEGIN_MESSAGE_MAP(WK_SudokuSolverSelectWnd, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

LRESULT WK_SudokuSolverSelectWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ShowWindow(FALSE);
  bHandled = FALSE;
  return 0;
}

void WK_SudokuSolverSelectWnd::Init(HWND hWndParent, POINT ptPos)
{
  if (!m_Parent) m_Parent = hWndParent;
  Create(hWndParent, _T("WK_SudokuSolverSelectWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
  m_MainPanel = dynamic_cast<CSudokuPanel*>(m_PaintManager.FindControl(_T("selectpanel")));
  ::ClientToScreen(hWndParent, &ptPos);
  ::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

LRESULT WK_SudokuSolverSelectWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lRes = 0;
  BOOL    bHandled = TRUE;

  switch (uMsg)
  {
  case WM_KILLFOCUS:
    lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
    break;
  default:
    bHandled = FALSE;
  }

  if (bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
  {
    return lRes;
  }

  return __super::HandleMessage(uMsg, wParam, lParam);
}

void WK_SudokuSolverSelectWnd::SetPossible(bool* possable) {
  m_MainPanel->SetPossable(possable);
}

void WK_SudokuSolverSelectWnd::OnClick(TNotifyUI& msg) {
  if (msg.sType == _T("click"))
  {
    if (msg.pSender->GetName() == _T("selectpanel"))
    {
      ::PostMessage(m_Parent, WK_NUMBER_SELECT, msg.wParam ,msg.lParam);
      ShowWindow(false);
      return;
    }
  }
  return __super::OnClick(msg);
}

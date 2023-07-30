#include "CSudokuSolverWnd.h"

DUI_BEGIN_MESSAGE_MAP(CSudokuSolverWnd, CNotifyPump)
  DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

void CSudokuSolverWnd::InitWindow() {
  m_progress = dynamic_cast<CProgressUI*>(m_PaintManager.FindControl(_T("Progress")));
  m_MainPanel = dynamic_cast<CSudokuPanel*>(m_PaintManager.FindControl(_T("mainpanel")));
  m_StartButton = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("start")));
  m_CleanButton = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("clean")));

  WK_SudokuCore::GetInstance()->SetMsgNoti(m_hWnd);
  m_progress->SetValue(0);
  //m_MainPanel->SetTestData();
}

void CSudokuSolverWnd::OnClick(TNotifyUI& msg) {
  if (msg.sType == _T("click"))
  {
    if (msg.pSender->GetName() == _T("mainpanel"))
    {
      POINT pt = { msg.ptMouse.x - 75, msg.ptMouse.y - 75 };
      if (!m_SelectWnd) {
        m_SelectWnd = new WK_SudokuSolverSelectWnd(_T("WK_SudokuSolverSelectFrame.xml"));
        m_SelectWnd->Init(*this, pt);
      }
      ::ClientToScreen(*this, &pt);
      ::SetWindowPos(*m_SelectWnd, NULL, pt.x, pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
      WK_SudokuCore::GetInstance()->SetData(m_MainPanel->GetData());
      m_SelectWnd->SetPossible(WK_SudokuCore::GetInstance()->GetPossable(m_MainPanel->GetSelect()));
      m_SelectWnd->ShowWindow(TRUE);

      return;
    }
    if (msg.pSender->GetName() == _T("start"))
    {
      CleanPorcess();
      WK_SudokuCore::GetInstance()->SetData(m_MainPanel->GetData());
      WK_SudokuCore::GetInstance()->Start();
      return;
    }
    if (msg.pSender->GetName() == _T("clean"))
    {
      CleanPorcess();
      m_MainPanel->SetData(nullptr);
      return;
    }
    if (msg.pSender->GetName() == _T("closebtn"))
    {
      exit(0);
      return;
    }
  }
  return __super::OnClick(msg);
}

void CSudokuSolverWnd::CleanPorcess() {
  m_progress->SetBkColor(0);
  m_progress->SetText(L"");
  m_progress->SetValue(0);
}

LRESULT CSudokuSolverWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

  LRESULT lRes = 0;
  CDuiString msgtext;

  switch (uMsg)
  {
  case(WK_NUMBER_SELECT): 
    m_MainPanel->SetSelectedNumber(wParam); 
    CleanPorcess();
    return lRes;
  case(WK_SUDOKU_START): 
    m_StartButton->SetEnabled(false); 
    m_CleanButton->SetEnabled(false);
    return lRes;
  case(WK_SUDOKU_END): 
    m_StartButton->SetEnabled(true); 
    m_CleanButton->SetEnabled(true);
    return lRes;
  case(WK_SUDOKU_PROCESS): m_progress->SetValue(wParam); return lRes;
  case(WK_SUDOKU_NOANSWER): 
    m_progress->SetText(L"错误，此题无解！"); 
    m_progress->SetBkColor(0xffc85554);
    return lRes;
  case(WK_SUDOKU_NUMBER_GUESS): 
    m_MainPanel->SetData((void*)wParam);
    msgtext.Format(L"猜测 %d ,%d ,为 %d", lParam/100, (lParam%100)/10, lParam % 10);
    m_progress->SetText(msgtext);
    break;
  case(WK_SUDOKU_NUMBER_SURE): 
    m_MainPanel->SetData((void*)wParam);
    msgtext.Format(L"确定 %d ,%d ,为 %d", lParam / 100, (lParam % 100) / 10, lParam % 10);
    m_progress->SetText(msgtext);
    break;
  case(WK_SUDOKU_SUCCESS):
    m_MainPanel->SetData((void*)wParam);
    msgtext.Format(L"计算完成 用时%f秒", (double)(((DWORD)lParam)/1000.0));
    m_progress->SetText(msgtext);
    break;
  default: break;
  }

  if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
  
  return __super::HandleMessage( uMsg, wParam, lParam);
}
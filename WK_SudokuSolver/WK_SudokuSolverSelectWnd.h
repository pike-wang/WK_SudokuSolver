#pragma once

#include <UIlib.h>
#include "WK_SudokuSolverMsgDef.h"

using namespace DuiLib;

class WK_SudokuSolverSelectWnd: public WindowImplBase
{
protected:
  virtual ~WK_SudokuSolverSelectWnd() {};        // ˽�л����������������˶���ֻ��ͨ��new�����ɣ�������ֱ�Ӷ���������ͱ�֤��delete this�������
  CDuiString  m_strXMLPath;

  DUI_DECLARE_MESSAGE_MAP()

public:
  explicit WK_SudokuSolverSelectWnd(LPCTSTR pszXMLPath) : m_strXMLPath(pszXMLPath) {}
  virtual LPCTSTR    GetWindowClassName()const { return _T("WK_SudokuSolverSelectWnd"); }
  virtual CDuiString GetSkinFolder() { return _T("xml"); }
  virtual CDuiString GetSkinFile() { return m_strXMLPath; }
  virtual void OnFinalMessage(HWND hWnd) { delete this; }
  virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  void Init(HWND hWndParent, POINT ptPos);
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  void SetPossible(bool* possable);

private:
  CSudokuPanel* m_MainPanel = nullptr;
  HWND m_Parent = nullptr;

 virtual void OnClick(TNotifyUI& msg);
};


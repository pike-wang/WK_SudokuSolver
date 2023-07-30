/* -----------------------------------------------------------------------------------------------------------------
COPYRIGHT (c) 2020 - 2120
Wave Knight Industry.,
ALL RIGHTS RESERVED

This software is a copyrighted work and/or information protected as a trade
secret. Legal rights of Wave Knight Industry. in this software are distinct from
ownership of any medium in which the software is embodied. Copyright or trade
secret notices included must be reproduced in any copies authorized by
Wave Knight Industry. The information in this software is subject to change without
notice and should not be considered as a commitment by Wave Knight Industry.
----------------------------------------------------------------------------------------------------------------- */
#pragma once

#include <UIlib.h>
#include "resource.h"
#pragma comment(lib, "DuiLib_u.lib")

using namespace DuiLib;

static DuiLib::CDuiString g_starttext[] = { L"��ȴ׼����",L"�������¹̶���ɡ�",L"���������жԽ�λ������" ,
L"���ף�ֹͣ�źŲ���Ѿ��ų���", L"�˽⣬�������˨��", L"���ż�׵����ϵͳ��׼�����ӡ�",
L"����˨�̶���ϣ�������һ�νӴ���", L"LCL��ʼע�롣", L"LCLע���У�����ɻ���20��", L"LCLŨ��û���쳣��ѹ��������",
L"����Դ��ͨ���������������л�·��û�����⡣", L"���еڶ��νӴ���", L"A10���������쳣��", L"LCL������������˼����̬������Ϊ������",
L"���ڽӴ�û���κ����⡣", L"˫���·��ͨ��ͬ����41.3 % ��", L"г����Ϊ����ֵ���ڲ���ѹά����״������С�",
L"û��ʧ������", L"��������ϵͳ��ȷ��������", L"׼�����䣡", L" ��һ����˨�ͷš�", L"ȷ�Ͻ����", L"���ʽ�����ſ�ʼ�ƶ���",
L"�ڶ�����˨�ͷţ���һ�������Լ��ڶ��������Ƴ���", L"�����һ�ŵ�ʮ��ŵİ�ȫװ�á�", L"ȷ�Ͻ����", L"���ڳ��Ż���״̬��[Free]��",
L"�ڲ���Դ�����ϣ��ⲿ��Դ����һ��������", L"EVA���Ż��ƶ�������ڡ�", L"ALL GREEN��·ͨ����",
L"����׼����ɣ�", L"�k������!", };

class CWKStartWnd : public  DuiLib::WindowImplBase
{
public:
  virtual LPCTSTR    GetWindowClassName() const { return _T("WK_Start"); }
  virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  virtual  DuiLib::CDuiString GetSkinFile() { return _T("WK_StartFrame.xml"); }
  virtual  DuiLib::CDuiString GetSkinFolder() { return _T("xml"); }

  virtual void InitWindow();
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
  
  DuiLib::CUIRectProgress* m_progress = nullptr;
  DuiLib::CTextUI* m_text = nullptr;
  int m_num = 0;

};

void CWKStartWnd::InitWindow() {
  m_progress = dynamic_cast<DuiLib::CUIRectProgress*>(m_PaintManager.FindControl(_T("Progress")));
  m_text = dynamic_cast<DuiLib::CTextUI*>(m_PaintManager.FindControl(_T("maintext")));
  m_progress->SetValue(0);
  ::SetTimer(m_hWnd, 0, 1, NULL);
}

LRESULT CWKStartWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

  LRESULT lRes = 0;

  switch (uMsg)
  {
  case(WM_TIMER):
    if (wParam == 1) {
      ::KillTimer(m_hWnd, 1);
      Close(true);
      return lRes;
    }
    m_num += 4;
    if (m_num >= 400) {
      ::KillTimer(m_hWnd, 0);
      m_text->SetText(g_starttext[31]);
      m_progress->SetValue(400);
      ::SetTimer(m_hWnd, 1, 500, NULL);
      return lRes;
    }
    m_progress->SetValue(m_num);
    m_text->SetText(g_starttext[(int)((double)m_num / 400.0 * 31)]);
    return lRes;
  default: break;
  }


  if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
  return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWKStartWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
  styleValue &= ~WS_CAPTION;
  ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
  RECT rcClient;
  ::GetClientRect(*this, &rcClient);
  ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

  m_PaintManager.Init(m_hWnd);
  m_PaintManager.AddPreMessageFilter(this);

  CDialogBuilder builder;
  CDuiString strResourcePath = m_PaintManager.GetResourcePath();
  if (strResourcePath.IsEmpty())
  {
    strResourcePath = m_PaintManager.GetInstancePath();
    strResourcePath += GetSkinFolder().GetData();
  }
  m_PaintManager.SetResourcePath(strResourcePath.GetData());

  switch (GetResourceType())
  {
  case UILIB_ZIP:
    m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
    break;
  case UILIB_ZIPRESOURCE:
  {
    HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
    if (hResource == NULL)
      return 0L;
    DWORD dwSize = 0;
    HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
    if (hGlobal == NULL)
    {
#if defined(WIN32) && !defined(UNDER_CE)
      ::FreeResource(hResource);
#endif
      return 0L;
    }
    dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
    if (dwSize == 0)
      return 0L;
    m_lpResourceZIPBuffer = new BYTE[dwSize];
    if (m_lpResourceZIPBuffer != NULL)
    {
      ::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
    }
#if defined(WIN32) && !defined(UNDER_CE)
    ::FreeResource(hResource);
#endif
    m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
  }
  break;
  }

  CControlUI* pRoot = NULL;
  if (GetResourceType() == UILIB_RESOURCE)
  {
    STRINGorID xml(_ttoi(GetSkinFile().GetData()));
    pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
  }
  else
    pRoot = builder.Create(IDF_XML_START_FRAME, _T("xml"), this, &m_PaintManager);
  ASSERT(pRoot);
  if (pRoot == NULL)
  {
    MessageBox(NULL, _T("������Դ�ļ�ʧ��"), _T("Duilib"), MB_OK | MB_ICONERROR);
    ExitProcess(1);
    return 0;
  }
  m_PaintManager.AttachDialog(pRoot);
  m_PaintManager.AddNotifier(this);

  InitWindow();
  return 0;
}
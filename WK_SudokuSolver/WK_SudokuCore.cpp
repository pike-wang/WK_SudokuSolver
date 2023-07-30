#include "WK_SudokuCore.h"

WK_SudokuCore* WK_SudokuCore::g_instance = nullptr;

void Sudoku_Thread_Start()
{
  WK_SudokuCore::GetInstance()->BeginSolver();
}

WK_SudokuCore::WK_SudokuCore() {
  if(!g_instance)
    g_instance = this;
}

WK_SudokuCore::WK_SudokuCore(WK_SudokuCore* up) {
  memcpy(m_Data, up->m_Data, sizeof(SudokuPanel_Data) * 81);
  memcpy(m_Possable, up->m_Possable, sizeof(SudokuPanel_PossAble) * 81);
  m_Running = up->m_Running;
  m_Guessing = up->m_Guessing;
}

WK_SudokuCore::~WK_SudokuCore() {

}

WK_SudokuCore* WK_SudokuCore::GetInstance() {
  if (!g_instance)
    g_instance = new WK_SudokuCore();

  return g_instance;
}

void WK_SudokuCore::Start() {
  if (m_Running)
    return;
  std::thread t_start;
  t_start = std::thread(Sudoku_Thread_Start);
  t_start.detach();
}

void WK_SudokuCore::SetMsgNoti(HWND MsgNoti) {
  m_MsgNoti = MsgNoti;
}

void WK_SudokuCore::BeginSolver() {

  m_Running = true;
  m_Guessing = false;
  SudokuMessage(WK_SUDOKU_START);

  memset(m_Possable, 1, sizeof(bool) * 9 * 9 * 9);

  DWORD timerecond = ::GetTickCount();
  bool result = DoSolver();
  if (!result) {
    SudokuMessage(WK_SUDOKU_NOANSWER);
  }
  else {
    timerecond = ::GetTickCount() - timerecond;
    SetAllDecided();
    SudokuMessage(WK_SUDOKU_PROCESS, 100);
    SudokuMessage(WK_SUDOKU_SUCCESS,timerecond);
  }

  m_Running = false;
  SudokuMessage(WK_SUDOKU_END);
}

void WK_SudokuCore::SetData(void* data) {
  memcpy(m_Data, data, sizeof(SudokuPanel_Data) * 81);
}

bool* WK_SudokuCore::GetPossable(Point pt) {
  WorkoutPointPossable(pt);
  return m_Possable[pt.X][pt.Y].possable;
}

void WK_SudokuCore::WorkoutPointPossable(Point pt) {
  if (!m_Running) 
    memset(m_Possable[pt.X][pt.Y].possable, 1, sizeof(bool) * 9);

  int zone = (pt.Y / 3) + (pt.Y / 3) * 2 + pt.X / 3;

  stack<Point> needcheck;
  
  for (int i =0; i<9; i++)
  {
    needcheck.push(Point(i,pt.Y));
  }

  for (int j = 0; j < 9; j++)
  {
    needcheck.push(Point(pt.X, j));
  }

  int offset_x = (zone % 3) *3;
  int offset_y = (zone / 3) * 3;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      needcheck.push(Point(offset_x+i, offset_y+j));
    }
  }

  while (!needcheck.empty())
  {
    Point check = needcheck.top();
    needcheck.pop();
    if (check.Equals(pt)) 
      continue;

    if (m_Data[check.X][check.Y].stauts != NUMBER_SURE && m_Data[check.X][check.Y].stauts != NUMBER_GUESS)
      continue;
   
    m_Possable[pt.X][pt.Y].possable[m_Data[check.X][check.Y].num - 1] = 0;
  }
}

int  WK_SudokuCore::CheckUndecideCnt() {
  int cnt = 0;

  for (int i = 0; i<9;i++)
  {
    for (int j = 0; j < 9; j++)
    {
      if (m_Data[i][j].stauts == NUMBER_UNSET)
        cnt++;
    }
  }

  return cnt;
}

void WK_SudokuCore::SetAllDecided() {
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      m_Data[i][j].stauts = NUMBER_SURE;
    }
  }
}

void WK_SudokuCore::WorkoutAllPointPossable() {
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      Point pt(i,j);
      if(m_Data[i][j].stauts == NUMBER_UNSET)
         WorkoutPointPossable(pt);
    }
  }
}

void WK_SudokuCore::WorkoutSingelLinePossable() {
  for (int zone = 0; zone < 9; zone++) {

    for (int possable = 0; possable < 9; possable++)
    {
      for (int y = 0 ; y<3; y++)
      {
        if (HasThisPossableinX(zone, y, possable) && NoThisPossableinX(zone, y, possable))
          DeleteOtherXPossable(zone, y, possable);
      }

      for (int x = 0; x < 3; x++)
      {
        if (zone ==2 && x==2 && possable == 3) {
          possable = 3;
        }
        if (HasThisPossableinY(zone, x, possable) && NoThisPossableinY(zone, x, possable))
          DeleteOtherYPossable(zone, x, possable);
      }
    }
  }
}

void WK_SudokuCore::JangePanelPossable() {
  WorkoutAllPointPossable();
  WorkoutSingelLinePossable();
  WorkoutAllPointPossable();
  WorkoutSingelLinePossable();
}

bool WK_SudokuCore::HasThisPossableinX(int zone, int y, int possable) {

  int zonex = (zone % 3) * 3;
  int zoney = (zone / 3) * 3 + y;

  for (int offset = 0; offset < 3; offset++) {
    if (m_Data[zonex + offset][zoney].stauts == NUMBER_UNSET && m_Possable[zonex + offset][zoney].possable[possable] == true)
      return true;
  }
  return false;
}
bool WK_SudokuCore::NoThisPossableinX(int zone, int y, int possable) {
  int y1 = (y + 1) % 3;
  int y2 = (y1 + 1) % 3;

  return (!HasThisPossableinX(zone, y1, possable))&&(!HasThisPossableinX(zone, y2, possable));
}
void WK_SudokuCore::DeleteOtherXPossable(int zone, int y, int possable) {
  int zoney = y + (zone / 3) * 3;
  for (int x = 0; x<9 ; x++)
  {
    int ptzone = (zoney / 3) * 3 + x / 3;
    if (ptzone != zone && m_Data[x][zoney].stauts == NUMBER_UNSET)
      m_Possable[x][zoney].possable[possable] = 0;
  }
}

bool WK_SudokuCore::HasThisPossableinY(int zone, int x, int possable) {

  int zonex = (zone % 3) * 3 +x;
  int zoney = (zone / 3) * 3;

  for (int offset = 0; offset < 3; offset++) {
    if (m_Data[zonex][zoney+offset].stauts == NUMBER_UNSET && m_Possable[zonex][zoney + offset].possable[possable] == true)
      return true;
  }
  return false;
}
bool WK_SudokuCore::NoThisPossableinY(int zone, int x, int possable) {

  int x1 = (x + 1) % 3;
  int x2 = (x1 + 1) % 3;

  return (!HasThisPossableinY(zone, x1, possable)) && (!HasThisPossableinY(zone, x2, possable));
}
void WK_SudokuCore::DeleteOtherYPossable(int zone, int x, int possable) {
  int zonex = x + (zone %3) * 3;
  for (int y = 0; y < 9; y++)
  {
    int ptzone = (y / 3) * 3 + zonex / 3;
    if (ptzone != zone && m_Data[zonex][y].stauts == NUMBER_UNSET)
      m_Possable[zonex][y].possable[possable] = 0;
  }
}

int WK_SudokuCore::CntPtPossible(bool *possible) {
  int index = 0, cnt = 0;
  while (index< 9) {
    if (*possible)
      cnt++;
    index++;
    possible++;
  }
  return cnt;
}

bool WK_SudokuCore::FindNoPossiblePt() {

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (m_Data[i][j].stauts == NUMBER_UNSET && CntPtPossible(m_Possable[i][j].possable) == 0)
        return true;
    }
  }
  return false;
}

void WK_SudokuCore::SetNumberPossable(int x, int y, int possable) {
  m_Data[x][y].num = possable + 1;
  m_Data[x][y].stauts = m_Guessing ? NUMBER_GUESS : NUMBER_SURE;
  if (m_Guessing) {
    SudokuMessage(WK_SUDOKU_NUMBER_GUESS,x*100+y*10+possable);
    return;
  }
  SudokuMessage(WK_SUDOKU_NUMBER_SURE, x * 100 + y * 10 + possable);
}

bool WK_SudokuCore::FindOnlyPassable_One(int &x, int&y, int&possable) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (m_Data[i][j].stauts == NUMBER_UNSET && CntPtPossible(m_Possable[i][j].possable) == 1) {
        int index = 0;
        while (!m_Possable[i][j].possable[index])
        {
          index++;
        }
        x = i; y = j; possable = index;
        return true;
      }
    }
  }
  return false;
}
bool WK_SudokuCore::FindOnlyPassable_Only(int x, int y, int possable) {

  return FindOnlyPassable_InZone(x, y, possable) ||
             FindOnlyPassable_InX(x, y, possable)       ||
             FindOnlyPassable_InY(x, y, possable);
}
bool WK_SudokuCore::FindOnlyPassable_InZone(int x, int y, int possable) {
  int zone = (y / 3) * 3 + x / 3;
  int zonex = (zone % 3) * 3;
  int zoney = (zone / 3) * 3;

  for (int offsitex =0 ; offsitex <3;offsitex ++)
  {
    for (int offsitey = 0; offsitey < 3; offsitey++)
    {
      if(zonex + offsitex == x && zoney + offsitey == y)
        continue;

      if (m_Data[zonex + offsitex][zoney + offsitey].stauts!= NUMBER_UNSET)
        continue;

      if (m_Possable[zonex + offsitex][zoney + offsitey].possable[possable])
        return false;
    }
  }

  return true;
}
bool WK_SudokuCore::FindOnlyPassable_InX(int x, int y, int possable) {
  for (int xin = 0; xin<9; xin++)
  {
    if(xin == x || m_Data[xin][y].stauts != NUMBER_UNSET)
      continue;;

    if (m_Possable[xin][y].possable[possable])
      return false;
  }
  return true;
}
bool WK_SudokuCore::FindOnlyPassable_InY(int x, int y, int possable) {
  for (int yin = 0; yin < 9; yin++)
  {
    if (yin == y || m_Data[x][yin].stauts != NUMBER_UNSET)
      continue;;

    if (m_Possable[x][yin].possable[possable])
      return false;
  }
  return true;
}

bool WK_SudokuCore::FindNextPtPossable( int& x, int &y, int &possable) {
  while (true)
  {
    possable++;
    if (x >= 8 && y >= 8 && possable >= 9)
      return false;

    if (possable >= 9) {
      possable = 0;
      y++;
      if (y >= 9) {
        y = 0;
        x++;
      }
    }

    if (m_Data[x][y].stauts != NUMBER_UNSET)
      continue;

    if (m_Possable[x][y].possable[possable])
      return true;

  }
  return false;
}

bool WK_SudokuCore::FindOlnyPossablePt() {
  int x = 0, y = 0, possable = -1;
  if (FindOnlyPassable_One(x,y,possable)) {
    SetNumberPossable(x, y, possable);
    return true;
  }

  while (FindNextPtPossable(x, y, possable))
  {
    if (FindOnlyPassable_Only(x, y, possable)) {
      SetNumberPossable(x, y, possable);
      return true;
    }
  }
  return false;
}

bool WK_SudokuCore::DoSolver() {

  while (true)
  {

    if (CheckUndecideCnt() == 0) {
      SetAllDecided();
      return true;
    }

    JangePanelPossable();

    if (FindNoPossiblePt())
      return false;

    if (FindOlnyPossablePt()) {
      continue;
    }

    m_Guessing = true;
    int x = 0, y = 0, possible = -1;

    FindNextPtPossable(x, y, possible);

    while (true) {

      SetNumberPossable(x, y, possible);
      WK_SudokuCore* m_newguess = new WK_SudokuCore(this);
      if(m_newguess->DoSolver()) {
        memcpy(m_Data,m_newguess->m_Data,sizeof(SudokuPanel_Data)*81);
        delete m_newguess;
        return true;
      }
      else {
        delete m_newguess;
        m_Possable[x][y].possable[possible] = 0;
        if (CntPtPossible(m_Possable[x][y].possable) == 0)
          return false;
        m_Data[x][y].stauts = NUMBER_UNSET;
        break;
      }
    }

  }

  return true;
}

void WK_SudokuCore::SudokuMessage(unsigned int msgid, int msg) {

  if (!g_instance->GetNoti())
    return;

  switch (msgid)
  {
  case(WK_SUDOKU_START): ::PostMessage(g_instance->GetNoti(), WK_SUDOKU_START, 0, 0); break;
  case(WK_SUDOKU_END): ::PostMessage(g_instance->GetNoti(), WK_SUDOKU_END, 0, 0); break;
  case(WK_SUDOKU_PROCESS): ::PostMessage(g_instance->GetNoti(), WK_SUDOKU_PROCESS, msg, 0); break;
  case(WK_SUDOKU_NOANSWER): ::PostMessage(g_instance->GetNoti(), WK_SUDOKU_NOANSWER, 0, 0); break;
  case(WK_SUDOKU_NUMBER_SURE):
    ::SendMessage(g_instance->GetNoti(), WK_SUDOKU_NUMBER_SURE, (WPARAM)m_Data, msg); break;
  case(WK_SUDOKU_NUMBER_GUESS):
    ::SendMessage(g_instance->GetNoti(), WK_SUDOKU_NUMBER_GUESS, (WPARAM)m_Data, msg); break;
  case(WK_SUDOKU_SUCCESS):
    ::SendMessage(g_instance->GetNoti(), WK_SUDOKU_SUCCESS, (WPARAM)m_Data, msg); break;
  default: break;
  }
}
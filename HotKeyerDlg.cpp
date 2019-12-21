#include "stdafx.h"
#include "HotKeyer.h"
#include "HotKeyerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ICON_NOTIFY  WM_APP+10

//---------------------------------------------------------------------------------------
// CHotKeyerDlg dialog
CHotKeyerDlg::CHotKeyerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOTKEYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//---------------------------------------------------------------------------------------
void CHotKeyerDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_MSG, stMsg);
}

//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHotKeyerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
  ON_WM_TIMER()
ON_COMMAND(ID_HOTKEYER_EXIT, &CHotKeyerDlg::OnHotkeyerExit)
ON_WM_HOTKEY()
ON_WM_SIZE()
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
// CHotKeyerDlg message handlers
BOOL CHotKeyerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
  HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));  // Icon to use
  m_TrayIcon.Create(this, WM_ICON_NOTIFY, "HotKeyer", hIcon, IDR_MENU1);

  // https://docs.microsoft.com/de-de/windows/win32/inputdev/virtual-key-codes

  Ini.Open("HotKeyer.ini");

  if (!Ini.SetSection("hotkeys"))
  {
    AfxMessageBox("Can`t find section [hotkeys] in HotKeyer.ini!");
    PostMessage(WM_CLOSE, 0, NULL);
    return TRUE;
  }

  bFirst = true;
  srand((unsigned)time(NULL));

  CString entry, data;
  int i=0;
  seq = 1;

  while (Ini.GetNextEntry(entry, data))
  {
    char *p;
    int vk = strtol(entry.Mid(0, 2), &p, 16);
    UINT mods = 0;

    for (int s = 3; s < entry.GetLength(); s++)
    {
      char c = entry[s];

      switch (c)
      {
        case 'A': mods |= MOD_ALT;     break;
        case 'C': mods |= MOD_CONTROL; break;
        case 'S': mods |= MOD_SHIFT;   break;
        case 'W': mods |= MOD_WIN;     break;
      }
    }

    if (!RegisterHotKey(m_hWnd, ++i, mods, vk))
    {
      AfxMessageBox("Can`t register hotkey " + entry);
      PostMessage(WM_CLOSE, 0, NULL);
      return TRUE;
    }

  }

  timer = SetTimer(0, 1, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//---------------------------------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CHotKeyerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//---------------------------------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHotKeyerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//---------------------------------------------------------------------------------------
LRESULT CHotKeyerDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
  // Delegate all the work back to the default implementation in CSystemTray.
  return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

//---------------------------------------------------------------------------------------
void CHotKeyerDlg::OnTimer(UINT_PTR nIDEvent)
{
  KillTimer(0);
  timer = 0;

  if (bFirst)
  {
    bFirst = false;
    ShowWindow(SW_HIDE);
  }
  
  m_TrayIcon.HideBalloon();
}

//---------------------------------------------------------------------------------------
void CHotKeyerDlg::OnHotkeyerExit()
{
  PostMessage(WM_CLOSE, 0, NULL);
}

//---------------------------------------------------------------------------------------
void CHotKeyerDlg::doCMD(CString &data)
{
  CString txt;

  txt.Format("%04d", (int)((double)rand() / RAND_MAX * 9999));
  data.Replace("$rnd$", txt);

  txt.Format("%04d", seq);
  if (data.Replace("$seq$", txt))
  {
    seq++;
    if (seq > 9999)
      seq = 0;
  }

  CTime now = CTime::GetCurrentTime();

  txt.Format("%04d", now.GetYear());
  data.Replace("$YYYY$", txt);

  txt.Format("%02d", now.GetMonth());
  data.Replace("$MM$", txt);

  txt.Format("%02d", now.GetDay());
  data.Replace("$DD$", txt);

  txt.Format("%02d", now.GetHour());
  data.Replace("$hh$", txt);

  txt.Format("%02d", now.GetMinute());
  data.Replace("$mm$", txt);

  txt.Format("%02d", now.GetSecond());
  data.Replace("$ss$", txt);

  STARTUPINFO startup;
  PROCESS_INFORMATION process;

  memset(&startup, 0, sizeof(startup));
  startup.cb = sizeof(startup);

  if (!CreateProcess(NULL, data.GetBuffer(0), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startup, &process))
    return;

//  if(bWait)
//    WaitForSingleObject(process.hProcess, INFINITE);

  CloseHandle(process.hProcess);
  CloseHandle(process.hThread);
} 

//---------------------------------------------------------------------------------------
void CHotKeyerDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
  Ini.SetSection("hotkeys");

  int i = 0;
  CString entry, data, cmd;

  while (Ini.GetNextEntry(entry, data))
  {
    i++;
    if (i == nHotKeyId)
    {
      m_TrayIcon.ShowBalloon("Executing hotkey");

      KillTimer(timer);
      timer = SetTimer(0, 2000, NULL);

      int i = data.Find(':');
      if (i > -1)
      {
        cmd = data.Mid(0, i);
        data = data.Mid(i + 1);
      }

      if (cmd == "cmd")
        doCMD(data);

      break;
    }
  }

  CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

//---------------------------------------------------------------------------------------
void CHotKeyerDlg::OnSize(UINT nType, int cx, int cy)
{
  CDialogEx::OnSize(nType, cx, cy);

  if (nType == SIZE_MINIMIZED)
  {
    ShowWindow(SW_HIDE);
  }
}

//---------------------------------------------------------------------------------------
BOOL CHotKeyerDlg::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN)
  {
    CString txt;
    txt.Format("%02X", pMsg->wParam);
    stMsg.SetWindowText("Key Code: "+txt);
  }

  return CDialogEx::PreTranslateMessage(pMsg);
}

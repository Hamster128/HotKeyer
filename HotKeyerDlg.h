
// HotKeyerDlg.h : header file
//

#pragma once
#include "SystemTray.h"

// CHotKeyerDlg dialog
class CHotKeyerDlg : public CDialogEx
{
// Construction
public:
	CHotKeyerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
  void doCMD(CString & cmd);

	HICON m_hIcon;
  CSystemTray m_TrayIcon;
  CIniLib Ini;
  UINT_PTR timer;
  int seq;
  bool bFirst;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
//  afx_msg void OnAfxIdPreviewClose();
  afx_msg void OnHotkeyerExit();
  void doKey(CString & data);
  afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
  afx_msg void OnSize(UINT nType, int cx, int cy);
//  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  CStatic stMsg;
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};

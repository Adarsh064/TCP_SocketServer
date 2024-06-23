// SocketTestServerDlg.h : header file
//

#pragma once


// CSocketTestServerDlg dialog
class CSocketTestServerDlg : public CDialog
{
// Construction
public:
	CSocketTestServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOCKETTESTSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
public:
    void StartServer();
    void ProcessClientRequest();
protected:
    bool m_bRefershData;
    SOCKET m_serversocket;
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
    CString m_recvData;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};

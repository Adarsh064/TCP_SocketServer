// SocketTestServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketTestServer.h"
#include "SocketTestServerDlg.h"
#include <process.h>
#include <string>
#include <sys/types.h>
#pragma warning(push)
#pragma warning(disable:4101)
#pragma warning( disable :6031)
#pragma warning(disable:28193)
#pragma warning(disable:26495)
#pragma warning(disable:6385)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSocketTestServerDlg dialog




CSocketTestServerDlg::CSocketTestServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketTestServerDlg::IDD, pParent)
    , m_recvData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_recvData);
}

BEGIN_MESSAGE_MAP(CSocketTestServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CSocketTestServerDlg::OnBnClickedOk)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDCANCEL, &CSocketTestServerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSocketTestServerDlg message handlers

BOOL CSocketTestServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    StartServer();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketTestServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketTestServerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketTestServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocketTestServerDlg::StartServer() // accepting incoming client connections.
{
    SOCKADDR_IN serveraddr;//holds the server's address information.
    int portno = 6666;
    memset(&serveraddr,0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portno);
   // serveraddr.sin_addr.s_addr = inet_addr("192.168.242.92");
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    m_serversocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_serversocket == -1)
    {
        AfxMessageBox("Socket Initialiation Error");
    }

    if(bind(m_serversocket, (SOCKADDR*)&serveraddr,sizeof(SOCKADDR_IN)) < 0) 
    {
         AfxMessageBox("ERROR binding in the server socket");
         exit(1);
    }

    if(listen(m_serversocket,0) < 0)
    {
         AfxMessageBox("ERROR listening in the server socket");
         exit(1);
    }	

    SetTimer(0x01, 100, NULL);
}

static void f(void *p)
{
    CSocketTestServerDlg *pDlg = reinterpret_cast<CSocketTestServerDlg*>(p);
    pDlg->ProcessClientRequest();
}

void CSocketTestServerDlg::ProcessClientRequest()  // responsible for handling client connections.

{
    SOCKADDR_IN clientaddr;
    struct hostent *hostentry;
    int len = sizeof(clientaddr);
    SOCKET clientsocket = accept(m_serversocket, (sockaddr*)&clientaddr, &len);
    
    if(len == -1)
    {
        AfxMessageBox("Error accpeting the client socket");
    }
    else
    {
        char *p = inet_ntoa(clientaddr.sin_addr);
        int portno = ntohs(clientaddr.sin_port);
        // int inet_pton(int af, const char *restrict src, void *restrict dst);
 
        char rbuf[1024];
        recv(clientsocket, rbuf, 1024, 0);
        for(int i = 1024; i >= 1; i--)
        {
            if(rbuf[i] == '\n' && rbuf[i - 1] == '\r')
            {
                rbuf[i-1] = '\0';
                break;
            }
        }
        CString strRecvData;
        
        strRecvData.Format("%s\r\n%s %d\r\n%s\r\n\r\n", (const char*)CTime::GetCurrentTime().Format("%B %d, %Y %H:%M:%S"), p, portno, rbuf);
        m_recvData += strRecvData;
        m_bRefershData = true;
        strcat(rbuf, "\r\n");
        send(clientsocket, rbuf, 1024, 0);
        closesocket(clientsocket);
    }
}

void CSocketTestServerDlg::OnTimer(UINT_PTR nIDEvent)
{
    int idx = nIDEvent;

    if(m_bRefershData == true)
    {
        m_bRefershData = false;
        UpdateData(FALSE);
    }
 
    fd_set fds;
    struct timeval timeout;
    int result;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;

    FD_ZERO(&fds);
    FD_SET(m_serversocket, &fds);

    int rc = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);
    if (rc==-1) 
    {
      AfxMessageBox("ERROR selecting in the server socket");
      return;
    }
    else if(rc > 0)
    {
        if (FD_ISSET(m_serversocket, &fds)) 
        {
            m_bRefershData = false;
            HANDLE h = (HANDLE) ::_beginthread(f, 0, (void*) this);
        }
        
    }        
    CDialog::OnTimer(nIDEvent);
}
void CSocketTestServerDlg::OnBnClickedCancel()
{
      

    OnCancel();
}

void CSocketTestServerDlg::OnBnClickedOk()
{
         
    OnOK();
}
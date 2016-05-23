// NetDebugHelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDebugHelper.h"
#include "NetDebugHelperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetDebugHelperDlg dialog

CNetDebugHelperDlg::CNetDebugHelperDlg(CWnd* pParent /*=NULL*/)
: CDialog(CNetDebugHelperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetDebugHelperDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CNetDebugHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetDebugHelperDlg)
	DDX_Control(pDX, IDC_EDIT3, m_SendWnd);
	DDX_Control(pDX, IDC_EDIT2, m_RecvWnd);
	DDX_Control(pDX, IDC_EDIT1, m_PORT);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetDebugHelperDlg, CDialog)
//{{AFX_MSG_MAP(CNetDebugHelperDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_RADIO3, OnTcpServer)
ON_BN_CLICKED(IDC_RADIO4, OnTcpClient)
ON_BN_CLICKED(IDC_RADIO5, OnUdpServer)
ON_BN_CLICKED(IDC_RADIO6, OnUdpClient)
ON_BN_CLICKED(IDC_BUTTON1, OnListen)
ON_BN_CLICKED(IDC_BUTTON5, OnStop)
ON_BN_CLICKED(IDC_BUTTON4, OnConnect)
ON_BN_CLICKED(IDC_BUTTON6, OnDisConnect)
ON_BN_CLICKED(IDC_BUTTON3, OnSend)
ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// CNetDebugHelperDlg message handlers

BOOL CNetDebugHelperDlg::OnInitDialog()
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
	
	// ��ʼ������
	
//	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);  //Ĭ�Ϸ���ascll��
	m_beStopSendCir =  1;
	checkTcpServer();									  //Ĭ��ѡ��TCP������
	recvDataLen = 0;									  //����������Ϊ0
	WSADATA  Ws;
	//Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )			  //����SOCKET
	{
		printf("Init Windows Socket Failed::%d",GetLastError());
		return -1;
    }
	SkinH_Attach();										  //����Ƥ��
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/** �������ڶԻ���
*/
void CNetDebugHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)	
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

/** �����ͼ�� ������δ�漰
*/
void CNetDebugHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetDebugHelperDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/** OK��ť�Ѿ�ɾ�������ǰ��»س����ǻ�ִ���������������ʲôҲ����
*/
void CNetDebugHelperDlg::OnOK() 
{
	
}

/** ȡ�������� ��X��ʱ��ִ��
*/
void CNetDebugHelperDlg::OnCancel() 
{
	WSACleanup();
	CDialog::OnCancel();
}

/** ѡ��TCP������ʱ������������� ����checkTcpServer����Ӧ��ť���
*/
void CNetDebugHelperDlg::OnTcpServer() 
{
	checkTcpServer();
}

/** ѡ��TCP�ͻ���ʱ������������� ����checkTCPClient����Ӧ��ť���
*/
void CNetDebugHelperDlg::OnTcpClient() 
{
	checkTcpClient();
}

/** ѡ��UDP������ʱ������������� ����checkUdpServer����Ӧ��ť���
*/
void CNetDebugHelperDlg::OnUdpServer() 
{
	checkUdpServer();
}

/** ѡ��UDP�ͻ���ʱ������������� ����checkUDPClient����Ӧ��ť���
*/
void CNetDebugHelperDlg::OnUdpClient() 
{
	checkUdpClient();
}

/** TCP�������̺߳���
*  Pm���ǶԻ������ָ��
*/
DWORD WINAPI TcpServerFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	dh->TcpServerThreadFun(NULL);  //���������������ʼ�����˿�
	return 0;
}

/** UDP�������̺߳���
*  Pm���ǶԻ������ָ��
*/
DWORD WINAPI UdpServerFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	dh->UdpServerThreadFun(); //���������������ʼ�����˿�
	return 0;
}

/** �㿪ʼ������ť�����������
*/
void CNetDebugHelperDlg::OnListen() 
{
	StartListenState();		//��һЩ��ť��ң�һЩ����
	beStop = 0;				
	
	//����radio��ť��ѡ��״̬�ж�
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck()) //����TCP������		
		hServerThread = CreateThread(NULL, 0, TcpServerFun, this, 0, NULL); 
	
	else if(((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck())//����TCP������
		CreateThread(NULL, 0, UdpServerFun, this, 0, NULL);
	
}

/** ��ֹͣ������ť�����������
*/
void CNetDebugHelperDlg::OnStop() 
{
	//ֹͣ����������Щradio��ť������
	StopListenState();
	
	//�ڵȴ����߳̿���beStop��Ϊ1�Ͳ��ٵ��ˣ�ֱ���Ƴ�
	beStop = 1;
}

/** ����TCP���������̺߳���
*   pMΪ�Ի������ָ��
*/
DWORD WINAPI ConnectTcpServerFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	
	dh->connectToTcpServer(); //�������������ʼ���ӷ�����
	
	return 0;
}

/** �����ӵ���������ť�����������
*/
void CNetDebugHelperDlg::OnConnect() 
{
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("�������ӵ�������....");
	ConnectState();
	
	//�����߳�ȥ���ӷ�����
	CreateThread(NULL, 0, ConnectTcpServerFun, this, 0, NULL);
}

/** ��Ͽ���������ť�����������
*/
void CNetDebugHelperDlg::OnDisConnect() 
{
	beStop = 1;
	disconnectState();
}

/** �㷢�Ͱ�ť�����������
*/
void CNetDebugHelperDlg::OnSend() 
{
		char testCount[50];
	sprintf(testCount, "������%d�ֽ�", m_SendWnd.GetWindowTextLength());
	GetDlgItem(IDC_STATICSND)->SetWindowText(testCount);
	//���radio3 radio4 ��û�б�ѡ�У���ֱ���TCP���������߿ͻ���
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck() || ((CButton *)GetDlgItem(IDC_RADIO4))->GetCheck())
		sendtcpdata();
	//UDP��������������
	else if(((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck() )
		sendUdpData();
	//UDP�ͻ��˷�������
	else if(((CButton *)GetDlgItem(IDC_RADIO6))->GetCheck() )
		ClientSendUdpData();
}


/** ---------------------------------------------------------------
*    ������ЩCheck���������������ƽ��水ť��
*     ���統����� ��ʼ������ ��ô�� ��ʼ������ť����ң�
*     �Ծ͵ģ�ֹͣ������ť�����
*/


void CNetDebugHelperDlg::checkTcpServer()
{
	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	m_IPAddr.SetWindowText("0.0.0.0");
	m_IPAddr.EnableWindow(FALSE);
	m_PORT.SetWindowText("12345");

	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT4)->SetWindowText("1000");
}

void CNetDebugHelperDlg::checkTcpClient()
{ 
	((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);  
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	m_IPAddr.SetWindowText("127.0.0.1");
	m_IPAddr.EnableWindow(TRUE);
	m_PORT.SetWindowText("12345");
}
void CNetDebugHelperDlg::checkUdpServer()
{
	((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);  
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	m_IPAddr.SetWindowText("0.0.0.0");
	m_IPAddr.EnableWindow(FALSE);
	m_PORT.SetWindowText("12345");
}
void CNetDebugHelperDlg::checkUdpClient()
{
	((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(TRUE);  
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	m_IPAddr.SetWindowText("127.0.0.1");
	m_IPAddr.EnableWindow(TRUE);
	m_PORT.SetWindowText("12345");
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//���Է�����
	
}

void CNetDebugHelperDlg::StartListenState()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	
	if( ((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
	if( ((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck() )
		GetDlgItem(IDC_RADIO5)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
	
	//GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);
	//GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
	m_PORT.EnableWindow(FALSE);
}

void CNetDebugHelperDlg::StopListenState()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO4)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO5)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO6)->EnableWindow(TRUE);
	
	m_PORT.EnableWindow(TRUE);
}

void CNetDebugHelperDlg::ConnectState()
{
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO4)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
	
	m_PORT.EnableWindow(FALSE);
}

void CNetDebugHelperDlg::disconnectState()
{
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO4)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO5)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO6)->EnableWindow(TRUE);
	
	m_PORT.EnableWindow(TRUE);
}
//-------------������Ƶ��˽���---------------------------

/** ������TCP������
*/
DWORD __stdcall CNetDebugHelperDlg::TcpServerThreadFun(LPVOID pM)
{
	char ip[18];
	char szport[10];
	int AddrLen = 0;
	int Ret;
	int flag=1,len=sizeof(int);
	int port;

	//��ü���socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ServerSocket == INVALID_SOCKET )
	{
		MessageBox("Create Socket Failed", "Error", MB_OK);
		goto finish2;
	}

	//��ʼ��������ַ�Ͷ˿�
    LocalAddr.sin_family = AF_INET;
	m_IPAddr.GetWindowText(ip, 18);
	m_PORT.GetWindowText(szport, 10);
	port = atoi(szport);
	LocalAddr.sin_addr.s_addr = inet_addr(ip);
	LocalAddr.sin_port = htons(port);
	memset(LocalAddr.sin_zero, 0x00, 8);

	//���õ�ַ����
	if( setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, len) == -1) 
	{ 
		goto finish1;
	} 

	//���׽������ַ��Ϣ
	Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if ( Ret != 0 )
    {
		MessageBox("Bind Failed", "Error", MB_OK);
        goto finish1;
    }

	//����
    Ret = listen(ServerSocket, 10);
    if ( Ret != 0 )
    {
		MessageBox("listen Socket Failed", "Error", MB_OK);
        goto finish1;
    }
	beStop = 0;
	
	fd_set fdread;
	timeval tv;
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp�������Ѿ�����");
	while(!beStop)
	{
		
		FD_ZERO(&fdread);
		FD_SET(ServerSocket, &fdread);
		tv.tv_sec = 0;
		tv.tv_usec = 500;

		//select ����ֻ����һ�����ӣ���֧�ֶ����ӣ� ����һ���ͻ��ˡ�
		select(0, &fdread, NULL, NULL, &tv);
		if(!FD_ISSET(ServerSocket, &fdread))
			continue;

		//��������
		AddrLen = sizeof(ClientAddr);
		CientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
		if ( CientSocket == INVALID_SOCKET )
        {
			MessageBox("Accept Failed", "Error", MB_OK);
            break;
        }


		char RecvBuffer[1024]; 
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//���Է�����
		
		while ( !beStop )
		{
			memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
			int TimeOut=800; //���÷��ͳ�ʱ800ms
			if(::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
			{
				exit(-1);
			}

			Ret = recv(CientSocket, RecvBuffer, MAX_PATH, 0);


			if(GetLastError()==10060)				    //��ʱ
				continue;
			if ( Ret == 0 || Ret == SOCKET_ERROR )      //�ͻ��˶Ͽ�
				break;
			AddDataToRecvWindow(RecvBuffer, Ret);		//��ʾ��������
		}
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);   //�����Է�����
		closesocket(CientSocket);
	}
finish1:
	closesocket(ServerSocket);
finish2:
	StopListenState();									
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp�������Ѿ�ֹͣ");
	return 0;
}

/** ��������ʾ�����մ�����
 */
void CNetDebugHelperDlg::AddDataToRecvWindow(char *data, int dataLen)
{
	static char buff[BUF_SIZE];
	if(dataLen + recvDataLen > BUF_SIZE)
		recvDataLen = 0;
	memcpy(recvData+recvDataLen, data, dataLen);
	recvDataLen += dataLen;
	int i;
	int len = 0;
	char testCount[50];
	sprintf(testCount, "������%d�ֽ�", recvDataLen);
	GetDlgItem(IDC_STATICREC)->SetWindowText(testCount);
	//��ʽ������
	for(i = 0; i < recvDataLen; i++)
	{
		if( ((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck() )
			sprintf(buff+len, "%2x ", (unsigned char)recvData[i]);
		else
			sprintf(buff + len, "%c", isprint(recvData[i])?recvData[i]:'.');
		len = strlen(buff);
		if( (i+1)%16==0 || i == recvDataLen - 1)
		{
		/*	for(int k =0; k < (16-(i-i/16*16))*3; k++)
				buff[len++] = ' ';
				
			sprintf(buff + len, "\t");
			len = strlen(buff);
			for(j = i/16*16; j < i+1; j++ )
			{
				sprintf(buff + len, "%c", isprint(recvData[j])?recvData[j]:'.');
				len = strlen(buff);
			}*/
			sprintf(buff + len, "\r\n");
			len = strlen(buff);
		}
		
	}
	
	//��ʾ����
	m_RecvWnd.SetWindowText(buff);
	m_RecvWnd.LineScroll(m_RecvWnd.GetLineCount());
}

/* ��ս��մ��� */
void CNetDebugHelperDlg::OnButton2() 
{
	m_RecvWnd.SetWindowText("");
	recvDataLen = 0;
}

/* ��շ��ʹ��� */
void CNetDebugHelperDlg::OnButton7() 
{
	m_SendWnd.SetWindowText("");
}


/** ����TCP����
 */
void CNetDebugHelperDlg::sendtcpdata()
{
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);	//�����Է�����
	int TimeOut=800;								//���÷��ͳ�ʱ100ms
	::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

	//��ȡ���ʹ�������
	static char buff[BUF_SIZE];
	m_SendWnd.GetWindowText(buff, BUF_SIZE);

	//��������
	int Ret = send(CientSocket, buff, m_SendWnd.GetWindowTextLength(),0);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//���Է�����

	if (Ret == 0 || Ret == SOCKET_ERROR ) 
	{
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp����ʧ��");
	}
	else
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp���ͳɹ�");
}

/** ����������UDP����
 */
void CNetDebugHelperDlg::sendUdpData()
{
	int TimeOut=800;								//���÷��ͳ�ʱ800ms
	::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

	//��ȡ���ʹ�������
	static char buff[BUF_SIZE];
	m_SendWnd.GetWindowText(buff, BUF_SIZE);


	int Ret = sendto(ServerSocket, buff, m_SendWnd.GetWindowTextLength(), 0, (sockaddr *)&ClientAddr, sizeof(ClientAddr)); 


	if (Ret == 0 || Ret == SOCKET_ERROR ) 
	{
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp����ʧ��");
	}
	else
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp���ͳɹ�");
}

/* UDP�ͻ����߳� 
 * UDP�ͻ�������������������ݺ� ��Ҫ�����̵߳ȴ���������Ӧ
 */
DWORD WINAPI UdpClientFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	
	dh->UdpClientReadDataThread();
	
	return 0;
}


/* UDP�ͻ����߳� ���������������
 */
void CNetDebugHelperDlg::ClientSendUdpData()
{	
    int port;
	char ip[20];
	char szport[10];

	//��ȡsocket
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//����Ŀ���������ַ��Ϣ
	m_PORT.GetWindowText(szport, 20);
	port = atoi(szport);
	m_IPAddr.GetWindowText(ip, 20);
    sockaddr_in sin;
	memset(&sin, 0 ,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = inet_addr(ip);

	//��Ŀ�����������UDP��
    int len = sizeof(sin);
    ClientAddr = sin;
    int TimeOut=800; //���÷��ͳ�ʱ100ms
	::setsockopt(ServerSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut));
	static char buff[BUF_SIZE];
	m_SendWnd.GetWindowText(buff, BUF_SIZE);

	int ret = sendto(ServerSocket, buff, m_SendWnd.GetWindowTextLength(), 0, (sockaddr *)&sin, len);

	//�жϷ��ͽ��
	if(ret < 0)
	{
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp�ͻ��˷�������ʧ��!");
		return;
	}

	//�����̣߳��ȴ���������Ӧ
	CreateThread(NULL, 0, UdpClientFun, this, 0, NULL);
    
}


/** ���ӵ�TCP������
 */
void CNetDebugHelperDlg::connectToTcpServer()
{
	int port;
	char ip[20];
	char szport[10];

	//����socket
	CientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(CientSocket == INVALID_SOCKET)
    {
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("invalid socket !");
        goto finish;
    }

	//����Ŀ���������ַ��Ϣ
	m_PORT.GetWindowText(szport, 20);
	port = atoi(szport);
	m_IPAddr.GetWindowText(ip, 20);
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip); 
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("��������TCP������...");
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);		//�����Է�����

	//���ӷ�����
    if (connect(CientSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        closesocket(CientSocket);
        GetDlgItem(IDC_STATICSTATE)->SetWindowText("connect error !");
		goto finish2;
    }
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);		//���Է�����
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("����TCP�������ɹ�");

	//���շ���������
	beStop = 0;
	while(!beStop)
    {
		char recData[1024];
		int TimeOut=800;							    //���÷��ͳ�ʱ800ms
		if(::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			goto finish;
		}
		
		int ret = recv(CientSocket, recData, 1024, 0);

		if(GetLastError()==10060)						//��ʱ
			continue;

		if(ret > 0)				
		{
			AddDataToRecvWindow(recData, ret);			//��ʾ����
		}
		else											//����ֹͣ�ͻ���
		{
			beStop = 1;
			break;
		}
	}
    
	
finish:
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);//�����Է�����
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("�Ѿ��Ͽ�������");

finish2:
	closesocket(CientSocket);
	disconnectState();
}

/** UDP������
 */
void CNetDebugHelperDlg::UdpServerThreadFun()
{
	int port;
	char szport[10];
	int nAddrLen;

	//����socket
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(ServerSocket == INVALID_SOCKET)
    {
        printf("socket error !");
        goto finish2;
    }
	


	//���÷�������ַ��Ϣ
	m_PORT.GetWindowText(szport, 20);
	port = atoi(szport);
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//�󶨵�ַ��socket
    if(bind(ServerSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("bind error !");
        
        goto finish1;
    }
    
	//�����˿ڣ��ȴ�UPD��
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp�������Ѿ�����");
    nAddrLen = sizeof(ClientAddr); 
    while (!beStop)
    {
        static char Data[1024];  
		int TimeOut=800;									//���÷��ͳ�ʱ800ms
		if(::setsockopt(ServerSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			goto finish1;
		}

        int ret = recvfrom(ServerSocket, Data, 1024, 0, (sockaddr *)&ClientAddr, &nAddrLen);

		if(GetLastError()==10060)							//��ʱ
			continue;

        if (ret > 0)										
        {
			GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//���Է�����
            AddDataToRecvWindow(Data, ret);					//��ʾ�յ�������
        } 
		
    }
finish1:
    closesocket(ServerSocket); 

finish2:
	StopListenState();
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);			//�����Է�����
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp�������Ѿ�ֹͣ");
}


/** UDP �ͻ��˴ӷ�������������
 */
void CNetDebugHelperDlg::UdpClientReadDataThread()
{
	int i,nAddrLen;
	nAddrLen = sizeof(ClientAddr);

	//����5��
	for(i = 0; i < 5; i++)
	{
		char text[128];
		sprintf(text,"udp�ͻ��˳��Խ������ݣ���%d��", i+1);
		GetDlgItem(IDC_STATICSTATE)->SetWindowText(text);
		static char Data[1024];  
		int TimeOut=1000;									//���÷��ͳ�ʱ800ms
		if(::setsockopt(ServerSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			goto finish1;
		}

        int ret = recvfrom(ServerSocket, Data, 1024, 0, (sockaddr *)&ClientAddr, &nAddrLen);

		if(GetLastError()==10060)							//��ʱ
			continue;

        if (ret > 0)
        {
			GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp�ͻ��˳��Խ������ݳɹ�!");
            AddDataToRecvWindow(Data, ret);					//��ʾ�յ�������
			
			break;
        }
		else
		{
			beStop = 1;
			GetDlgItem(IDC_STATICSTATE)->SetWindowText("�������ݳɹ������Ƿ�����û��Ӧ!");
			break;
		}
		
	}
	if(i == 5)
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp�ͻ��˳��Խ�������ʧ�ܣ�������û����Ӧ!");
finish1:
	closesocket(ServerSocket);
}



void CNetDebugHelperDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	AddDataToRecvWindow(NULL, 0);
}

void CNetDebugHelperDlg::OnRadio2() 
{
	AddDataToRecvWindow(NULL, 0);
}

/** 
*   pMΪ�Ի������ָ��
*/
DWORD WINAPI sendCircle(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	
	while(!dh->m_beStopSendCir)
	{
		dh->OnSend();
		Sleep(dh->m_sendItv);
	}
	
	return 0;
}

void CNetDebugHelperDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	char ch1[10];
	GetDlgItem(IDC_EDIT4)->GetWindowText(ch1,10);

	if(!m_beStopSendCir)
	{
		m_beStopSendCir = 1;
		((CButton *)GetDlgItem(IDC_BUTTON8))->SetWindowText("��ʼ��ʱ����");
	}
	else
	{
		if(strlen(ch1) <= 0)
		{
			MessageBox("�����뷢��ʱ����", "����", MB_OK);
			return;
		}
		m_sendItv = atoi(ch1);
		m_beStopSendCir = 0;
			//�����̣߳��ȴ���������Ӧ
		CreateThread(NULL, 0, sendCircle, this, 0, NULL);
		((CButton *)GetDlgItem(IDC_BUTTON8))->SetWindowText("ֹͣ��ʱ����");
	}
	
}


char fileBuf[1024*1024];
void CNetDebugHelperDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here

	 CString filter;
	filter="�ı��ĵ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||";
	 CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,filter);
	 
	if(dlg.DoModal()==IDOK)
	{
		CString str;
		str=dlg.GetPathName();
	//	MessageBox(str);
		FILE *fin = fopen(str, "rb");
		int n = fread(fileBuf, 1, 1024*1024, fin);
		fclose(fin);
		m_SendWnd.SetWindowText("");
		m_SendWnd.SetWindowText(fileBuf);
	}
}

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
	
	// 初始化数据
	
//	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);  //默认发送ascll码
	m_beStopSendCir =  1;
	checkTcpServer();									  //默认选中TCP服务器
	recvDataLen = 0;									  //接收数据置为0
	WSADATA  Ws;
	//Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )			  //开起SOCKET
	{
		printf("Init Windows Socket Failed::%d",GetLastError());
		return -1;
    }
	SkinH_Attach();										  //加载皮肤
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/** 弹出关于对话框
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

/** 界面绘图， 本程序未涉及
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

/** OK按钮已经删除，可是按下回车还是会执行这个函数，这里什么也不做
*/
void CNetDebugHelperDlg::OnOK() 
{
	
}

/** 取消按键， 点X的时候执行
*/
void CNetDebugHelperDlg::OnCancel() 
{
	WSACleanup();
	CDialog::OnCancel();
}

/** 选中TCP服务器时调用这个函数， 调用checkTcpServer把相应按钮变灰
*/
void CNetDebugHelperDlg::OnTcpServer() 
{
	checkTcpServer();
}

/** 选中TCP客户端时调用这个函数， 调用checkTCPClient把相应按钮变灰
*/
void CNetDebugHelperDlg::OnTcpClient() 
{
	checkTcpClient();
}

/** 选中UDP服务器时调用这个函数， 调用checkUdpServer把相应按钮变灰
*/
void CNetDebugHelperDlg::OnUdpServer() 
{
	checkUdpServer();
}

/** 选中UDP客户端时调用这个函数， 调用checkUDPClient把相应按钮变灰
*/
void CNetDebugHelperDlg::OnUdpClient() 
{
	checkUdpClient();
}

/** TCP服务器线程函数
*  Pm就是对话框对象指针
*/
DWORD WINAPI TcpServerFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	dh->TcpServerThreadFun(NULL);  //调用了这个函数开始监听端口
	return 0;
}

/** UDP服务器线程函数
*  Pm就是对话框对象指针
*/
DWORD WINAPI UdpServerFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	dh->UdpServerThreadFun(); //调用了这个函数开始监听端口
	return 0;
}

/** 点开始监听按钮调用这个函数
*/
void CNetDebugHelperDlg::OnListen() 
{
	StartListenState();		//把一些按钮变灰，一些变亮
	beStop = 0;				
	
	//根据radio按钮的选择状态判断
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck()) //启动TCP服务器		
		hServerThread = CreateThread(NULL, 0, TcpServerFun, this, 0, NULL); 
	
	else if(((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck())//启动TCP服务器
		CreateThread(NULL, 0, UdpServerFun, this, 0, NULL);
	
}

/** 点停止监听按钮调用这个函数
*/
void CNetDebugHelperDlg::OnStop() 
{
	//停止监听，把那些radio按钮都变亮
	StopListenState();
	
	//在等待的线程看到beStop变为1就不再等了，直接推出
	beStop = 1;
}

/** 连接TCP服务器的线程函数
*   pM为对话框对象指针
*/
DWORD WINAPI ConnectTcpServerFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	
	dh->connectToTcpServer(); //调用这个函数开始连接服务器
	
	return 0;
}

/** 点连接到服务器按钮调用这个函数
*/
void CNetDebugHelperDlg::OnConnect() 
{
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("正在连接到服务器....");
	ConnectState();
	
	//创建线程去连接服务器
	CreateThread(NULL, 0, ConnectTcpServerFun, this, 0, NULL);
}

/** 点断开服务器按钮调用这个函数
*/
void CNetDebugHelperDlg::OnDisConnect() 
{
	beStop = 1;
	disconnectState();
}

/** 点发送按钮调用这个函数
*/
void CNetDebugHelperDlg::OnSend() 
{
		char testCount[50];
	sprintf(testCount, "发送了%d字节", m_SendWnd.GetWindowTextLength());
	GetDlgItem(IDC_STATICSND)->SetWindowText(testCount);
	//检查radio3 radio4 有没有被选中，这分别是TCP服务器或者客户端
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck() || ((CButton *)GetDlgItem(IDC_RADIO4))->GetCheck())
		sendtcpdata();
	//UDP服务器发送数据
	else if(((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck() )
		sendUdpData();
	//UDP客户端发送数据
	else if(((CButton *)GetDlgItem(IDC_RADIO6))->GetCheck() )
		ClientSendUdpData();
}


/** ---------------------------------------------------------------
*    下面这些Check函数都是用来控制界面按钮的
*     比如当你点下 开始监听， 那么， 开始监听按钮将变灰，
*     对就的，停止监听按钮会变亮
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
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//可以发送了
	
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
//-------------界面控制到此结束---------------------------

/** 真正的TCP服务器
*/
DWORD __stdcall CNetDebugHelperDlg::TcpServerThreadFun(LPVOID pM)
{
	char ip[18];
	char szport[10];
	int AddrLen = 0;
	int Ret;
	int flag=1,len=sizeof(int);
	int port;

	//获得监听socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ServerSocket == INVALID_SOCKET )
	{
		MessageBox("Create Socket Failed", "Error", MB_OK);
		goto finish2;
	}

	//初始化监听地址和端口
    LocalAddr.sin_family = AF_INET;
	m_IPAddr.GetWindowText(ip, 18);
	m_PORT.GetWindowText(szport, 10);
	port = atoi(szport);
	LocalAddr.sin_addr.s_addr = inet_addr(ip);
	LocalAddr.sin_port = htons(port);
	memset(LocalAddr.sin_zero, 0x00, 8);

	//设置地址复用
	if( setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, len) == -1) 
	{ 
		goto finish1;
	} 

	//绑定套接字与地址信息
	Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if ( Ret != 0 )
    {
		MessageBox("Bind Failed", "Error", MB_OK);
        goto finish1;
    }

	//监听
    Ret = listen(ServerSocket, 10);
    if ( Ret != 0 )
    {
		MessageBox("listen Socket Failed", "Error", MB_OK);
        goto finish1;
    }
	beStop = 0;
	
	fd_set fdread;
	timeval tv;
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp服务器已经启动");
	while(!beStop)
	{
		
		FD_ZERO(&fdread);
		FD_SET(ServerSocket, &fdread);
		tv.tv_sec = 0;
		tv.tv_usec = 500;

		//select 并且只接收一个连接，不支持多连接， 连接一个客户端。
		select(0, &fdread, NULL, NULL, &tv);
		if(!FD_ISSET(ServerSocket, &fdread))
			continue;

		//接受连接
		AddrLen = sizeof(ClientAddr);
		CientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
		if ( CientSocket == INVALID_SOCKET )
        {
			MessageBox("Accept Failed", "Error", MB_OK);
            break;
        }


		char RecvBuffer[1024]; 
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//可以发送了
		
		while ( !beStop )
		{
			memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
			int TimeOut=800; //设置发送超时800ms
			if(::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
			{
				exit(-1);
			}

			Ret = recv(CientSocket, RecvBuffer, MAX_PATH, 0);


			if(GetLastError()==10060)				    //超时
				continue;
			if ( Ret == 0 || Ret == SOCKET_ERROR )      //客户端断开
				break;
			AddDataToRecvWindow(RecvBuffer, Ret);		//显示接收数据
		}
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);   //不可以发送了
		closesocket(CientSocket);
	}
finish1:
	closesocket(ServerSocket);
finish2:
	StopListenState();									
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp服务器已经停止");
	return 0;
}

/** 把数据显示到接收窗口中
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
	sprintf(testCount, "接收了%d字节", recvDataLen);
	GetDlgItem(IDC_STATICREC)->SetWindowText(testCount);
	//格式化数据
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
	
	//显示数据
	m_RecvWnd.SetWindowText(buff);
	m_RecvWnd.LineScroll(m_RecvWnd.GetLineCount());
}

/* 清空接收窗口 */
void CNetDebugHelperDlg::OnButton2() 
{
	m_RecvWnd.SetWindowText("");
	recvDataLen = 0;
}

/* 清空发送窗口 */
void CNetDebugHelperDlg::OnButton7() 
{
	m_SendWnd.SetWindowText("");
}


/** 发送TCP数据
 */
void CNetDebugHelperDlg::sendtcpdata()
{
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);	//不可以发送了
	int TimeOut=800;								//设置发送超时100ms
	::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

	//获取发送窗口数据
	static char buff[BUF_SIZE];
	m_SendWnd.GetWindowText(buff, BUF_SIZE);

	//发送数据
	int Ret = send(CientSocket, buff, m_SendWnd.GetWindowTextLength(),0);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//可以发送了

	if (Ret == 0 || Ret == SOCKET_ERROR ) 
	{
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp发送失败");
	}
	else
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("tcp发送成功");
}

/** 服务器发送UDP数据
 */
void CNetDebugHelperDlg::sendUdpData()
{
	int TimeOut=800;								//设置发送超时800ms
	::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

	//获取发送窗口数据
	static char buff[BUF_SIZE];
	m_SendWnd.GetWindowText(buff, BUF_SIZE);


	int Ret = sendto(ServerSocket, buff, m_SendWnd.GetWindowTextLength(), 0, (sockaddr *)&ClientAddr, sizeof(ClientAddr)); 


	if (Ret == 0 || Ret == SOCKET_ERROR ) 
	{
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp发送失败");
	}
	else
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp发送成功");
}

/* UDP客户端线程 
 * UDP客户端在向服务器发送数据后， 需要创建线程等待服务器回应
 */
DWORD WINAPI UdpClientFun(LPVOID pM)
{
	CNetDebugHelperDlg * dh = (CNetDebugHelperDlg *)pM;
	
	dh->UdpClientReadDataThread();
	
	return 0;
}


/* UDP客户端线程 向服务器发送数据
 */
void CNetDebugHelperDlg::ClientSendUdpData()
{	
    int port;
	char ip[20];
	char szport[10];

	//获取socket
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//设置目标服务器地址信息
	m_PORT.GetWindowText(szport, 20);
	port = atoi(szport);
	m_IPAddr.GetWindowText(ip, 20);
    sockaddr_in sin;
	memset(&sin, 0 ,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = inet_addr(ip);

	//向目标服务器发送UDP包
    int len = sizeof(sin);
    ClientAddr = sin;
    int TimeOut=800; //设置发送超时100ms
	::setsockopt(ServerSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut));
	static char buff[BUF_SIZE];
	m_SendWnd.GetWindowText(buff, BUF_SIZE);

	int ret = sendto(ServerSocket, buff, m_SendWnd.GetWindowTextLength(), 0, (sockaddr *)&sin, len);

	//判断发送结果
	if(ret < 0)
	{
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp客户端发送数据失败!");
		return;
	}

	//创建线程，等待服务器回应
	CreateThread(NULL, 0, UdpClientFun, this, 0, NULL);
    
}


/** 连接到TCP服务器
 */
void CNetDebugHelperDlg::connectToTcpServer()
{
	int port;
	char ip[20];
	char szport[10];

	//创建socket
	CientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(CientSocket == INVALID_SOCKET)
    {
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("invalid socket !");
        goto finish;
    }

	//设置目标服务器地址信息
	m_PORT.GetWindowText(szport, 20);
	port = atoi(szport);
	m_IPAddr.GetWindowText(ip, 20);
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip); 
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("正在连接TCP服务器...");
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);		//不可以发送了

	//连接服务器
    if (connect(CientSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        closesocket(CientSocket);
        GetDlgItem(IDC_STATICSTATE)->SetWindowText("connect error !");
		goto finish2;
    }
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);		//可以发送了
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("连接TCP服务器成功");

	//接收服务器数据
	beStop = 0;
	while(!beStop)
    {
		char recData[1024];
		int TimeOut=800;							    //设置发送超时800ms
		if(::setsockopt(CientSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			goto finish;
		}
		
		int ret = recv(CientSocket, recData, 1024, 0);

		if(GetLastError()==10060)						//超时
			continue;

		if(ret > 0)				
		{
			AddDataToRecvWindow(recData, ret);			//显示数据
		}
		else											//出错，停止客户端
		{
			beStop = 1;
			break;
		}
	}
    
	
finish:
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);//不可以发送了
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("已经断开服务器");

finish2:
	closesocket(CientSocket);
	disconnectState();
}

/** UDP服务器
 */
void CNetDebugHelperDlg::UdpServerThreadFun()
{
	int port;
	char szport[10];
	int nAddrLen;

	//创建socket
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(ServerSocket == INVALID_SOCKET)
    {
        printf("socket error !");
        goto finish2;
    }
	


	//设置服务器地址信息
	m_PORT.GetWindowText(szport, 20);
	port = atoi(szport);
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定地址与socket
    if(bind(ServerSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("bind error !");
        
        goto finish1;
    }
    
	//监听端口，等待UPD包
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp服务器已经启动");
    nAddrLen = sizeof(ClientAddr); 
    while (!beStop)
    {
        static char Data[1024];  
		int TimeOut=800;									//设置发送超时800ms
		if(::setsockopt(ServerSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			goto finish1;
		}

        int ret = recvfrom(ServerSocket, Data, 1024, 0, (sockaddr *)&ClientAddr, &nAddrLen);

		if(GetLastError()==10060)							//超时
			continue;

        if (ret > 0)										
        {
			GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);	//可以发送了
            AddDataToRecvWindow(Data, ret);					//显示收到的数据
        } 
		
    }
finish1:
    closesocket(ServerSocket); 

finish2:
	StopListenState();
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);			//不可以发送了
	GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp服务器已经停止");
}


/** UDP 客户端从服务器接收数据
 */
void CNetDebugHelperDlg::UdpClientReadDataThread()
{
	int i,nAddrLen;
	nAddrLen = sizeof(ClientAddr);

	//尝试5次
	for(i = 0; i < 5; i++)
	{
		char text[128];
		sprintf(text,"udp客户端尝试接收数据，第%d次", i+1);
		GetDlgItem(IDC_STATICSTATE)->SetWindowText(text);
		static char Data[1024];  
		int TimeOut=1000;									//设置发送超时800ms
		if(::setsockopt(ServerSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			goto finish1;
		}

        int ret = recvfrom(ServerSocket, Data, 1024, 0, (sockaddr *)&ClientAddr, &nAddrLen);

		if(GetLastError()==10060)							//超时
			continue;

        if (ret > 0)
        {
			GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp客户端尝试接收数据成功!");
            AddDataToRecvWindow(Data, ret);					//显示收到的数据
			
			break;
        }
		else
		{
			beStop = 1;
			GetDlgItem(IDC_STATICSTATE)->SetWindowText("发送数据成功，但是服务器没响应!");
			break;
		}
		
	}
	if(i == 5)
		GetDlgItem(IDC_STATICSTATE)->SetWindowText("udp客户端尝试接收数据失败，服务器没有响应!");
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
*   pM为对话框对象指针
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
		((CButton *)GetDlgItem(IDC_BUTTON8))->SetWindowText("开始定时发送");
	}
	else
	{
		if(strlen(ch1) <= 0)
		{
			MessageBox("请输入发送时间间隔", "出错", MB_OK);
			return;
		}
		m_sendItv = atoi(ch1);
		m_beStopSendCir = 0;
			//创建线程，等待服务器回应
		CreateThread(NULL, 0, sendCircle, this, 0, NULL);
		((CButton *)GetDlgItem(IDC_BUTTON8))->SetWindowText("停止定时发送");
	}
	
}


char fileBuf[1024*1024];
void CNetDebugHelperDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here

	 CString filter;
	filter="文本文档(*.txt)|*.txt|所有文件(*.*)|*.*||";
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

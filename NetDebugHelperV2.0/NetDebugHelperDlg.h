// NetDebugHelperDlg.h : header file
//

#if !defined(AFX_NETDEBUGHELPERDLG_H__D6BC0484_FAC1_4530_B284_4B1D748923B6__INCLUDED_)
#define AFX_NETDEBUGHELPERDLG_H__D6BC0484_FAC1_4530_B284_4B1D748923B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNetDebugHelperDlg dialog
#define BUF_SIZE 8192
class CNetDebugHelperDlg : public CDialog
{
// Construction
public:
	int m_beStopSendCir;
	int m_sendItv;
	void UdpClientReadDataThread();
	void ClientSendUdpData();
	void checkUdpClient();
	void sendUdpData();
	void checkUdpServer();
	void UdpServerThreadFun();
	void connectToTcpServer();
	void sendtcpdata();
	HANDLE hServerThread;
	int recvDataLen;
	char recvData[BUF_SIZE];
	void AddDataToRecvWindow(char *data, int len);
	int beStop;
	struct sockaddr_in ClientAddr;
	struct sockaddr_in LocalAddr;
	SOCKET CientSocket;
	SOCKET ServerSocket;
	void disconnectState();
	void ConnectState();
	void StopListenState();
	void StartListenState();
	void checkTcpClient();
	void checkTcpServer();
	CNetDebugHelperDlg(CWnd* pParent = NULL);	// standard constructor
	DWORD __stdcall TcpServerThreadFun(LPVOID pM);
// Dialog Data
	//{{AFX_DATA(CNetDebugHelperDlg)
	enum { IDD = IDD_NETDEBUGHELPER_DIALOG };
	CEdit	m_SendWnd;
	CEdit	m_RecvWnd;
	CEdit	m_PORT;
	CIPAddressCtrl	m_IPAddr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetDebugHelperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNetDebugHelperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTcpServer();
	afx_msg void OnTcpClient();
	afx_msg void OnUdpServer();
	afx_msg void OnUdpClient();
	afx_msg void OnListen();
	afx_msg void OnStop();
	afx_msg void OnConnect();
	afx_msg void OnDisConnect();
	afx_msg void OnSend();
	afx_msg void OnButton2();
	afx_msg void OnButton7();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETDEBUGHELPERDLG_H__D6BC0484_FAC1_4530_B284_4B1D748923B6__INCLUDED_)

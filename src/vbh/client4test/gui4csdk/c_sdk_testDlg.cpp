
// c_sdk_testDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "c_sdk_test.h"
#include "c_sdk_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>

#include <fstream>
using namespace std;

#include "client_c_sdk/client_c_api.h"
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

//#include "vbh_cc_server/plugin/cc_comm/cc_comm_id_def.h"
#include <map>
#include <string>

#define  _KEY_FILE "C:\\work\\ca\\capi_test.key"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcsdktestDlg 对话框



CcsdktestDlg::CcsdktestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_C_SDK_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcsdktestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcsdktestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REG, &CcsdktestDlg::OnBnClickedBtnReg)
	ON_BN_CLICKED(IDC_BTN_CA_INIT, &CcsdktestDlg::OnBnClickedBtnCaInit)
	ON_BN_CLICKED(IDC_BTN_UNREG, &CcsdktestDlg::OnBnClickedBtnUnreg)
	//ON_BN_CLICKED(IDC_BTN_USER_INIT, &CcsdktestDlg::OnBnClickedBtnUserInit)
	ON_BN_CLICKED(IDC_BTN_USER_GET, &CcsdktestDlg::OnBnClickedBtnUserGet)
	ON_BN_CLICKED(IDC_BTN_USER_SET, &CcsdktestDlg::OnBnClickedBtnUserSet)

	ON_BN_CLICKED(IDC_BTN_GETTRANS_INFO, &CcsdktestDlg::OnBnClickedBtnGettransInfo)
	ON_BN_CLICKED(IDC_BTN_CA_ID, &CcsdktestDlg::OnBnClickedBtnCaId)
	ON_BN_CLICKED(IDC_BTN_REG_ID, &CcsdktestDlg::OnBnClickedBtnRegId)
	ON_BN_CLICKED(IDC_BTN_UNREG_ID, &CcsdktestDlg::OnBnClickedBtnUnregId)

	ON_BN_CLICKED(IDC_BTN_USER_INIT_ID, &CcsdktestDlg::OnBnClickedBtnUserInitId)
	ON_BN_CLICKED(IDC_BTN_USER_GET_ID, &CcsdktestDlg::OnBnClickedBtnUserGetId)
	ON_BN_CLICKED(IDC_BTN_USER_SET_ID, &CcsdktestDlg::OnBnClickedBtnUserSetId)
	ON_BN_CLICKED(IDC_BTN_GETTRANS_INFO_ID, &CcsdktestDlg::OnBnClickedBtnGettransInfoId)
	ON_BN_CLICKED(IDC_BTN_USER_INIT_INIT, &CcsdktestDlg::OnBnClickedBtnUserInitInit)
END_MESSAGE_MAP()


// CcsdktestDlg 消息处理程序

BOOL CcsdktestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
// 	GetDlgItem(IDC_BTN_REG)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_UNREG)->EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcsdktestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcsdktestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcsdktestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void writeCryptFile(const char* pCryptUserKey, int nCryptUserKeyLen)
{
	ofstream f(_KEY_FILE);
	f.write(pCryptUserKey, nCryptUserKeyLen);
	f.close();
}

std::string readCryptFile()
{
	ifstream ff(_KEY_FILE);
	int length;
	ff.seekg(0, std::ios::end);    // go to the end  
	length = ff.tellg();           // report location (this is the length)

	ff.seekg(0, std::ios::beg);    // go back to the beginning  
	char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
	ff.read(buffer, length);       // read the whole file into the buffer  
	ff.close();                    // close file handle  

	return std::string(buffer, length);
}

void OnReady_BcUserCSdk()
{
	OutputDebugString(_T("OnReady_BcUserCSdk\n"));
}

void OnAbnormal_BcUserCSdk()
{
	OutputDebugString(_T("OnAbnormal_BcUserCSdk\n"));
}

void OnExit_BcUserCSdk()
{
	OutputDebugString(_T("OnExit_BcUserCSdk\n"));
}

std::map <std::string, std::string> g_CryptUserkeys;

void OnRegistUserRsp(const int nReturnCode, const unsigned int nChannelID, const char* pUserKey,
	const unsigned int nUserKeyLen, const char* pCryptUserKey, const unsigned int nCryptUserKeyLen)
{
	USES_CONVERSION;
	CString strResult;
	if (nReturnCode == 0)
	{
		//CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pCryptUserKey=%s\n");
		CString strFormat = _T("%d %d %s %d %s %d\n");
		strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), nUserKeyLen, A2T(pCryptUserKey), nCryptUserKeyLen);
	}
	else
	{
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d\n");
		strResult.Format(strFormat, nReturnCode, nChannelID);
	}
	

	OutputDebugString(strResult);
	if (nReturnCode==0)
	{
		std::string strKey(pUserKey, nUserKeyLen);

		g_CryptUserkeys[strKey] = std::string(pCryptUserKey, nCryptUserKeyLen);
	}
}
void OnUnregistUserRsp(const int nReturnCode, const unsigned int nChannelID,
	const char* pUserKey, const unsigned int nUserKeyLen)
{
	USES_CONVERSION;
	CString strResult;
	if (nReturnCode == 0)
	{
		std::string userKey(pUserKey, nUserKeyLen);
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s\n");
		strResult.Format(strFormat, nReturnCode, nChannelID, A2T(userKey.c_str()));
	}
	else
	{
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d\n");
		strResult.Format(strFormat, nReturnCode, nChannelID);
	}
	OutputDebugString(strResult);
}

void OnQueryUserInfoRsp(const int nReturnCode, const unsigned int nChannelID, const char* pUserKey,
	const unsigned int nUserKeyLen, const char* pUserUserInfo, const unsigned int nUserUserInfoLen )
{
	USES_CONVERSION;
	CString strFormat = _T("OnQueryUserInfoRsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pUserUserInfo=%s\n");
	CString strResult;
	strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), A2T(pUserUserInfo));

	OutputDebugString(strResult);
}
void OnUpdateUserInfoRsp(const int nReturnCode, const unsigned int nChannelID,
	const char* pUserKey, const unsigned int nUserKeyLen,
	const char* pTransKey, const unsigned int nTransKeyLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnUpdateUserInfoRsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pTransKey = %s \n");
	CString strResult;
	CString strTranskey(pTransKey, nTransKeyLen);
	strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), strTranskey);

	OutputDebugString(strResult);
}

void OnGetBcTransInfoRsp(const int nReturnCode, const char* pTransKey, const int nTransKeyLen, const char* pTransInfo, const int nTransInfoLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnGetBcTransInfoRsp: nReturnCode=%d, pTransKey=%s, pTransInfo=%s\n");
	CString strResult;
	std::string strTrans(pTransInfo, nTransInfoLen);
	std::string strTransKey(pTransKey, nTransKeyLen);
	strResult.Format(strFormat, nReturnCode, A2T(strTransKey.c_str()), A2T(strTrans.c_str()));

	OutputDebugString(strResult);
}

void CcsdktestDlg::OnBnClickedBtnCaInit()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_vbh_c_sdk(1, OnReady_BcCaCSdk, OnAbnormal_BcCaCSdk, OnExit_BcCaCSdk, OnRegistUserRsp, OnUnregistUserRsp);
}

void CcsdktestDlg::OnBnClickedBtnReg()
{
	// TODO: 在此添加控件通知处理程序代码
	int nChannel = 123;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strUserInfo = "{\"BTC\":1000,\"ETH\":1000}";
	reg_vbh_user(nChannel, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strUserInfo.c_str(), strUserInfo.length());

	strUserKey = "{\"IDNumber\":4294967297}";
	reg_vbh_user(nChannel, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strUserInfo.c_str(), strUserInfo.length());
}

void CcsdktestDlg::OnBnClickedBtnUnreg()
{
	// TODO: 在此添加控件通知处理程序代码
	int nChannelID = 123;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	unreg_vbh_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length());
	strUserKey = "{\"IDNumber\":4294967297}";
	unreg_vbh_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length());
}

void CcsdktestDlg::OnBnClickedBtnUserInitInit()
{
	// TODO: 在此添加控件通知处理程序代码
	init_vbh_c_sdk(1, OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk,
		OnRegistUserRsp, OnUnregistUserRsp, OnQueryUserInfoRsp, OnUpdateUserInfoRsp, OnGetBcTransInfoRsp);
}


void CcsdktestDlg::OnBnClickedBtnUserInit()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_vbh_c_sdk(1, OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnQueryUserInfoRsp, OnUpdateUserInfoRsp, OnGetBcTransInfoRsp);
}


void CcsdktestDlg::OnBnClickedBtnUserGet()
{
	// TODO: 在此添加控件通知处理程序代码
	//std::string strCryptKey = readCryptFile();
	int nChannelID = 123;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];

	query_vbh_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strCryptUserKey.c_str(), strCryptUserKey.length());
}

void CcsdktestDlg::OnBnClickedBtnUserSet()
{
	//转账
	int nCcID = 142;
	int nAction = 999;
	int nChannelID = 123;
	//std::string strUserKey = "{ \"m_nIDNumber\":4294967296 }";
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strProposal = " {\"userKeyA\":{\"IDNumber\":4294967296},\"userKeyB\":{\"IDNumber\":4294967297},\"coinType\":1,\"coinNum\":1}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];

	vbh_propose(nCcID, nAction, nChannelID, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strProposal.c_str(), strProposal.length(), (char*)strCryptUserKey.c_str(), strCryptUserKey.length());
}


void CcsdktestDlg::OnBnClickedBtnGettransInfo()
{
	USES_CONVERSION;
	// 查询交易信息

	int nChannelID = 123;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];
	CString strWTransKey;
	GetDlgItemText(IDC_EDT_TRANS, strWTransKey);
	std::string strTransKey(T2A(strWTransKey));
	query_vbh_trans(nChannelID, (char*)strUserKey.c_str(), strUserKey.length()
		, (char*)strCryptUserKey.c_str(), strCryptUserKey.length(), (char*)strTransKey.c_str(), strTransKey.length());
}


//ID链
void OnIDRegistUserRsp(const int nReturnCode, const unsigned int nChannelID, const char* pUserKey,
	const unsigned int nUserKeyLen, const char* pCryptUserKey, const unsigned int nCryptUserKeyLen)
{
	USES_CONVERSION;
	CString strResult;
	if (nReturnCode == 0)
	{
		//CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pCryptUserKey=%s\n");
		CString strFormat = _T("%d %d %s %d %s %d\n");
		strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), nUserKeyLen, A2T(pCryptUserKey), nCryptUserKeyLen);
	}
	else
	{
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d\n");
		strResult.Format(strFormat, nReturnCode, nChannelID);
	}


	OutputDebugString(strResult);
	if (nReturnCode == 0)
	{
		std::string strKey(pUserKey, nUserKeyLen);

		g_CryptUserkeys[strKey] = std::string(pCryptUserKey, nCryptUserKeyLen);
	}
}

void OnIDUnregistUserRsp(const int nReturnCode, const unsigned int nChannelID,
	const char* pUserKey, const unsigned int nUserKeyLen)
{
	USES_CONVERSION;
	CString strResult;
	if (nReturnCode == 0)
	{
		std::string userKey(pUserKey, nUserKeyLen);
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s\n");
		strResult.Format(strFormat, nReturnCode, nChannelID, A2T(userKey.c_str()));
	}
	else
	{
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d, nChannelID=%d\n");
		strResult.Format(strFormat, nReturnCode, nChannelID);
	}
	OutputDebugString(strResult);
}



void OnIDQueryUserInfoRsp(const int nReturnCode, const unsigned int nChannelID, const char* pUserKey,
	const unsigned int nUserKeyLen, const char* pUserUserInfo, const unsigned int nUserUserInfoLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnQueryUserInfoRsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pUserUserInfo=%s\n");
	CString strResult;
	strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), A2T(pUserUserInfo));

	OutputDebugString(strResult);
}
void OnIDUpdateUserInfoRsp(const int nReturnCode, const unsigned int nChannelID,
	const char* pUserKey, const unsigned int nUserKeyLen,
	const char* pTransKey, const unsigned int nTransKeyLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnUpdateUserInfoRsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pTransKey = %s \n");
	CString strResult;
	CString strTranskey(pTransKey, nTransKeyLen);
	strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), strTranskey);

	OutputDebugString(strResult);
}

void OnIDGetBcTransInfoRsp(const int nReturnCode, const char* pTransKey, const int nTransKeyLen, const char* pTransInfo, const int nTransInfoLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnGetBcTransInfoRsp: nReturnCode=%d, pTransKey=%s, pTransInfo=%s\n");
	CString strResult;
	std::string strTrans(pTransInfo, nTransInfoLen);
	std::string strTransKey(pTransKey, nTransKeyLen);
	strResult.Format(strFormat, nReturnCode, A2T(strTransKey.c_str()), A2T(strTrans.c_str()));

	OutputDebugString(strResult);
}

void CcsdktestDlg::OnBnClickedBtnCaId()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_vbh_ca_c_sdk(2, OnReady_BcCaCSdk, OnAbnormal_BcCaCSdk, OnExit_BcCaCSdk, OnIDRegistUserRsp, OnIDUnregistUserRsp);
}


void CcsdktestDlg::OnBnClickedBtnRegId()
{
	// TODO: 在此添加控件通知处理程序代码
	int nChannel = 124;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strUserInfo = "{\"age\":18,\"phoneNumber\":1231231,\"userName\":\"9527\",\"address\":\"yuhang\"}";
	reg_vbh_user(nChannel, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strUserInfo.c_str(), strUserInfo.length());
}


void CcsdktestDlg::OnBnClickedBtnUnregId()
{
	// TODO: 在此添加控件通知处理程序代码
	int nChannelID = 124;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	unreg_vbh_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length());
}


void CcsdktestDlg::OnBnClickedBtnUserInitId()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_vbh_c_sdk(2, OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnIDQueryUserInfoRsp, OnIDUpdateUserInfoRsp, OnIDGetBcTransInfoRsp);
}


void CcsdktestDlg::OnBnClickedBtnUserGetId()
{
	// TODO: 在此添加控件通知处理程序代码
	int nChannelID = 124;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];

	query_vbh_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strCryptUserKey.c_str(), strCryptUserKey.length());
}


void CcsdktestDlg::OnBnClickedBtnUserSetId()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCcID = 144;
	int nAction = 1000;
	int nChannelID = 124;
	//std::string strUserKey = "{ \"m_nIDNumber\":4294967296 }";
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strProposal = "{\"user\":{\"userKey\":{\"IDNumber\":4294967296}, \"userInfo\" : {\"age\" : 19, \"phoneNumber\" : 1231231, \"userName\" : \"9527\", \"address\" : \"yuhang\"}}}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];

	vbh_propose(nCcID, nAction, nChannelID, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strProposal.c_str(), strProposal.length(), (char*)strCryptUserKey.c_str(), strCryptUserKey.length());
}


void CcsdktestDlg::OnBnClickedBtnGettransInfoId()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CString strWTransKey;
	GetDlgItemText(IDC_EDT_TRANS_ID, strWTransKey);
	std::string strTransKey(T2A(strWTransKey));

	int nChannelID = 124;
	std::string strUserKey = "{\"IDNumber\":4294967296}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];
	query_vbh_trans(nChannelID, (char*)strUserKey.c_str(), strUserKey.length()
		, (char*)strCryptUserKey.c_str(), strCryptUserKey.length(), (char*)strTransKey.c_str(), strTransKey.length());
}


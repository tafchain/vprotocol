
// c_sdk_testDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "c_sdk_test.h"
#include "c_sdk_testDlg.h"
#include "afxdialogex.h"

#include "idc_sdk/idc_client.h"
#include <string>

#include <fstream>
//using namespace std;

//#include "bc_ca_client_c_sdk/bc_ca_client_c_api.h"
//#include "bc_client_c_sdk/bc_client_c_api.h"
//#include "id_chain_app/idc_sdk/idc_ca_client_c_sdk/idc_ca_client_c_api.h"
//#include "id_chain_app/idc_sdk/idc_client_c_sdk/idc_client_c_api.h"
//#include "id_chain_app/idc_comm/idc_comm_msg_def.h"
//#include "id_chain_app/idc_sdk/i_dom_client_c_sdk/i_dom_client_c_api.h"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

//#include "bc_cc_server/plugin/cc_comm/cc_comm_id_def.h"
#include <map>
#include <string>

#define  _KEY_FILE "d:\\work\\ca\\bc.key"

#define TEST_USER_KEY   3333
#define TEST_USER_TARGET_KEY   4444

void OutputResult(CString strLog)
{
	CWnd* pWnd = AfxGetApp()->m_pMainWnd;
	reinterpret_cast<CcsdktestDlg*>(pWnd)->OutputResult(strLog);
}

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
	DDX_Control(pDX, IDC_LIST1, m_output);
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
	/*
	ON_BN_CLICKED(IDC_BTN_GETTRANS_INFO, &CcsdktestDlg::OnBnClickedBtnGettransInfo)
	ON_BN_CLICKED(IDC_BTN_CA_ID, &CcsdktestDlg::OnBnClickedBtnCaId)
	ON_BN_CLICKED(IDC_BTN_REG_ID, &CcsdktestDlg::OnBnClickedBtnRegId)
	ON_BN_CLICKED(IDC_BTN_UNREG_ID, &CcsdktestDlg::OnBnClickedBtnUnregId)

	ON_BN_CLICKED(IDC_BTN_USER_INIT_ID, &CcsdktestDlg::OnBnClickedBtnUserInitId)
	ON_BN_CLICKED(IDC_BTN_USER_GET_ID, &CcsdktestDlg::OnBnClickedBtnUserGetId)
	ON_BN_CLICKED(IDC_BTN_USER_SET_ID, &CcsdktestDlg::OnBnClickedBtnUserSetId)
	ON_BN_CLICKED(IDC_BTN_GETTRANS_INFO_ID, &CcsdktestDlg::OnBnClickedBtnGettransInfoId)
	ON_BN_CLICKED(IDC_BTN_USER_INIT_INIT, &CcsdktestDlg::OnBnClickedBtnUserInitInit)
	ON_BN_CLICKED(IDC_BTN_AUTH, &CcsdktestDlg::OnBnClickedBtnAuth)
	ON_BN_CLICKED(IDC_BTN_QUERY_ASSET, &CcsdktestDlg::OnBnClickedBtnQueryAsset)
	ON_BN_CLICKED(IDC_BTN_BOUNS, &CcsdktestDlg::OnBnClickedBtnBouns)
	ON_BN_CLICKED(IDC_BTN_EARLY_BIRD_ENTER, &CcsdktestDlg::OnBnClickedBtnEarlyBirdEnter)
	ON_BN_CLICKED(IDC_BTN_EARLY_BIRD_HISTORY, &CcsdktestDlg::OnBnClickedBtnEarlyBirdHistory)
	ON_BN_CLICKED(IDC_BTN_IDOM_INIT, &CcsdktestDlg::OnBnClickedBtnIdomInit)
	ON_BN_CLICKED(IDC_BTN_EARLY_BIRD_SIGNIN, &CcsdktestDlg::OnBnClickedBtnEarlyBirdSignin)
	ON_BN_CLICKED(IDC_BTN_GET_IDM_HISTORY, &CcsdktestDlg::OnBnClickedBtnGetIdmHistory)
	ON_BN_CLICKED(IDC_BTN_ADD_FORCE, &CcsdktestDlg::OnBnClickedBtnAddForce)
	ON_BN_CLICKED(IDC_BTN_FORCE_HISTORY, &CcsdktestDlg::OnBnClickedBtnForceHistory)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_SHAREINFO, &CcsdktestDlg::OnBnClickedBtnUploadShareinfo)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_SHAREINFO, &CcsdktestDlg::OnBnClickedBtnDownloadShareinfo)
	ON_BN_CLICKED(IDC_BTN_SHARE_HISTORY, &CcsdktestDlg::OnBnClickedBtnShareHistory)
	ON_BN_CLICKED(IDC_BTN_DELETE_SHAREINFO, &CcsdktestDlg::OnBnClickedBtnDeleteShareinfo)
	ON_BN_CLICKED(IDC_BTN_TRANSFER, &CcsdktestDlg::OnBnClickedBtnTransfer)
	ON_BN_CLICKED(IDC_BTN_EARLY_BIRD_PEOPLE_COUNT, &CcsdktestDlg::OnBnClickedBtnEarlyBirdPeopleCount)
	ON_BN_CLICKED(IDC_BTN_EARLY_BIRD_RANK_LIST, &CcsdktestDlg::OnBnClickedBtnEarlyBirdRankList)
	ON_BN_CLICKED(IDC_BTN_IF_EARLY_BIRD_SIGN, &CcsdktestDlg::OnBnClickedBtnIfEarlyBirdSign)
	*/
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

	m_output.SetHorizontalExtent(1000);

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

//ID链相关--init响应
void OnReady_BcCaCSdk()
{
	OutputDebugString(_T("OnReady_BcCaCSdk\n"));
	OutputResult(_T("OnReady_BcCaCSdk"));
}

void OnAbnormal_BcCaCSdk()
{
	OutputDebugString(_T("OnAbnormal_BcCaCSdk\n"));
	OutputResult(_T("OnAbnormal_BcCaCSdk"));
}

void OnExit_BcCaCSdk()
{
	OutputDebugString(_T("OnExit_BcCaCSdk\n"));
	OutputResult(_T("OnExit_BcCaCSdk"));
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

std::map <std::string, std::string> g_CryptUserkeys;

std::string g_strUserKeys;

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
	OutputResult(strResult);
	if (nReturnCode==0)
	{
		//std::string strKey(pUserKey, nUserKeyLen);

		g_CryptUserkeys[0] = std::string(pCryptUserKey, nCryptUserKeyLen);
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
	OutputResult(strResult);
}

void OnReady_BcUserCSdk()
{
	OutputDebugString(_T("OnReady_BcUserCSdk\n"));
	OutputResult(_T("OnReady_BcUserCSdk"));
}

void OnAbnormal_BcUserCSdk()
{
	OutputDebugString(_T("OnAbnormal_BcUserCSdk\n"));
	OutputResult(_T("OnAbnormal_BcUserCSdk"));
}

void OnExit_BcUserCSdk()
{
	OutputDebugString(_T("OnExit_BcUserCSdk\n"));
	OutputResult(_T("OnExit_BcUserCSdk"));
}

void OnQueryUserInfoRsp(const int nReturnCode, const unsigned int nChannelID, const char* pUserKey,
	const unsigned int nUserKeyLen, const char* pUserUserInfo, const unsigned int nUserUserInfoLen )
{
	USES_CONVERSION;
	CString strFormat = _T("OnQueryUserInfoRsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pUserUserInfo=%s\n");
	CString strResult;
	strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), A2T(pUserUserInfo));

	OutputDebugString(strResult);
	OutputResult(strResult);
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
	OutputResult(strResult);
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
	OutputResult(strResult);
}

class CMsgCallback : public IDC_SDK::CIdcMsgCallback
{
public:
	CMsgCallback() : IDC_SDK::CIdcMsgCallback() {}
	~CMsgCallback() {}
	virtual void OnReady() {
		OutputDebugString(_T("ready"));
		OutputResult(_T("ready"));
	}

	virtual void OnRegisterUserRsp(int nReqID, int nReturnCode, const char* cryptoUserKey, size_t cryptoUserKeyLen) {
		CString strResult;
		strResult.Format(L"reg rsp %s", cryptoUserKey);
		OutputDebugString(strResult);
		OutputResult(strResult);
	}

	void OnCommonRsp(IDC_SDK::CCommonRsp& rsp) {};
private:

};

IDC_SDK::CIdcClient* gClient;
void CcsdktestDlg::OnBnClickedBtnCaInit()
{
	auto callback = new CMsgCallback();
	gClient = new IDC_SDK::CIdcClient();

	gClient->Init(callback);

	// TODO: 在此添加控件通知处理程序代码
	//init_bc_ca_c_sdk(1, OnReady_BcCaCSdk, OnAbnormal_BcCaCSdk, OnExit_BcCaCSdk, OnRegistUserRsp, OnUnregistUserRsp);
}

void CcsdktestDlg::OnBnClickedBtnReg()
{
	// TODO: 在此添加控件通知处理程序代码
	//int nChannel = 123;
	//std::string strUserKey = "{\"IDNumber\":111}";
	//std::string strUserInfo = "{\"BTC\":1000,\"ETH\":1000}";
	//reg_bc_user(nChannel, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strUserInfo.c_str(), strUserInfo.length());

	//strUserKey = "{\"IDNumber\":4294967297}";
	//reg_bc_user(nChannel, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strUserInfo.c_str(), strUserInfo.length());
}

void CcsdktestDlg::OnBnClickedBtnUnreg()
{
	// TODO: 在此添加控件通知处理程序代码
	//int nChannelID = 123;
	//std::string strUserKey = "{\"IDNumber\":111}";
	//unreg_bc_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length());
	//strUserKey = "{\"IDNumber\":4294967297}";
	//unreg_bc_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length());
}

void CcsdktestDlg::OnBnClickedBtnUserInitInit()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_bc_c_sdk(1, OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnQueryUserInfoRsp, OnUpdateUserInfoRsp, OnGetBcTransInfoRsp);
}


void CcsdktestDlg::OnBnClickedBtnUserInit()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_bc_c_sdk(1, OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnQueryUserInfoRsp, OnUpdateUserInfoRsp, OnGetBcTransInfoRsp);
}


void CcsdktestDlg::OnBnClickedBtnUserGet()
{
	// TODO: 在此添加控件通知处理程序代码
	//std::string strCryptKey = readCryptFile();
	//int nChannelID = 123;
	//std::string strUserKey = "{\"IDNumber\":111}";
	//std::string strCryptUserKey = g_CryptUserkeys[strUserKey];

	//query_bc_user(nChannelID, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strCryptUserKey.c_str(), strCryptUserKey.length());
}

void CcsdktestDlg::OnBnClickedBtnUserSet()
{
	//转账
	//int nCcID = 142;
	//int nAction = 999;
	//int nChannelID = 123;
	////std::string strUserKey = "{ \"m_nIDNumber\":111 }";
	//std::string strUserKey = "{\"IDNumber\":111}";
	//std::string strProposal = " {\"userKeyA\":{\"IDNumber\":111},\"userKeyB\":{\"IDNumber\":4294967297},\"coinType\":1,\"coinNum\":1}";
	//std::string strCryptUserKey = g_CryptUserkeys[strUserKey];

	//launch_bc_trans(nCcID, nAction, nChannelID, (char*)strUserKey.c_str(), strUserKey.length(), (char*)strProposal.c_str(), strProposal.length(), (char*)strCryptUserKey.c_str(), strCryptUserKey.length());
}


void CcsdktestDlg::OnBnClickedBtnGettransInfo()
{
	USES_CONVERSION;
	// 查询交易信息

	//int nChannelID = 123;
	//std::string strUserKey = "{\"IDNumber\":111}";
	//std::string strCryptUserKey = g_CryptUserkeys[strUserKey];
	//CString strWTransKey;
	//GetDlgItemText(IDC_EDT_TRANS, strWTransKey);
	//std::string strTransKey(T2A(strWTransKey));
	//get_bc_trans_info(nChannelID, (char*)strUserKey.c_str(), strUserKey.length()
	//	, (char*)strCryptUserKey.c_str(), strCryptUserKey.length(), (char*)strTransKey.c_str(), strTransKey.length());
}


//==ID链测试相关----注册用户响应
void OnIDRegistUserRsp(const int nReturnCode, const char* pCryptUserKey, const unsigned int nCryptUserKeyLen)
{
	USES_CONVERSION;
	CString strResult;
	if (nReturnCode == 0)
	{
		CString strFormat = _T("%d  %s %d\n");
		strResult.Format(strFormat, nReturnCode, A2T(pCryptUserKey), nCryptUserKeyLen);
		
		char pUserInfo[] = "{    \
			\"name\": \"bc.wang\",    \
			\"idNum\" : \"111111111111\",  \
			\"mail\" : \"aaa@163.com\",   \
			\"phone\" : \"1111111111\",   \
			\"idm\" : 100000,     \
			\"force\" : 123.000000   \
		}";

		//launch_idc_trans(TEST_USER_KEY, 111, DSC_STRING_TYPE_PARAM(pUserInfo), (char*)pCryptUserKey, nCryptUserKeyLen);
	}
	else
	{
		CString strFormat = _T("on_regist_user_rsp: nReturnCode=%d\n");
		strResult.Format(strFormat, nReturnCode);
	}


	OutputDebugString(strResult);
	OutputResult(strResult);
	if (nReturnCode == 0)
	{
		//std::string strKey(pUserKey, nUserKeyLen);

		g_strUserKeys = std::string(pCryptUserKey, nCryptUserKeyLen);
	}
}

//==ID链测试相关----注销用户响应
void OnIDUnregistUserRsp(const int nReturnCode)
{
	USES_CONVERSION;
	CString strResult;
	if (nReturnCode == 0)
	{
		CString strFormat = _T("on_unregist_user_rsp: nReturnCode=%d \n");
		strResult.Format(strFormat, nReturnCode);
	}
	else
	{
		CString strFormat = _T("on_unregist_user_rsp: nReturnCode=%d \n");
		strResult.Format(strFormat, nReturnCode);
	}
	OutputDebugString(strResult);
	OutputResult(strResult);
}

//nType查询类型
// 0. 查询
// 1. 鉴权
// 2. 查资产
//==ID链测试相关----查询用户信息响应
void OnIDQueryUserInfoRsp(const int nReturnCode, int nReqID, int nType, const char* pUserUserInfo, const unsigned int nUserUserInfoLen)
{
	/*
	if (nType == 0)
	{
		//查询用户信息
		IDC::CIDMUserOriInfo oriInfo;
		DSC::Decode(oriInfo, pUserUserInfo, nUserUserInfoLen);


		CString strResult(pUserUserInfo, nUserUserInfoLen);
		OutputDebugString(strResult);
		OutputResult(strResult);
	}
	else if (nType == 1)
	{
		//鉴权
		CString strResult(_T("111111111111111111111111"));
		OutputDebugString(strResult);
		OutputResult(strResult);
	}
	else if (nType == 2)
	{
		//查询资产响应
		CString strResult(pUserUserInfo, nUserUserInfoLen);
		OutputDebugString(strResult);
		OutputResult(strResult);
	}
	else
	{

	}
	*/

	//IDC::CIDMUserOriInfo userInfo;
	//DSC::Decode(userInfo, pUserUserInfo, nUserUserInfoLen);
	
	//CString strResult;
	//CString strFormat = _T("OnQueryUserInfoRsp: m_strName=%s, m_strIDNum=%s, m_strEMail=%s, m_strMPNum=%s, m_dIDM=%lf, m_dForce=%lf \n");

	//CString strName(userInfo.m_strName.GetBuffer(), userInfo.m_strName.GetSize());
	//CString strMPNum(userInfo.m_strMPNum.GetBuffer(), userInfo.m_strMPNum.GetSize());
	//CString strIDNum(userInfo.m_strIDNum.GetBuffer(), userInfo.m_strIDNum.GetSize());
	//CString strEMail(userInfo.m_strEMail.GetBuffer(), userInfo.m_strEMail.GetSize());


	//strResult.Format(strFormat, strName, strIDNum, strEMail, strMPNum, userInfo.m_dIDM, userInfo.m_dForce);

	//OutputDebugString(strResult);
	//OutputResult(strResult);
	//strResult.Format(strFormat, );

	//CDscString authCode;
	//IDC::CAuthResultWrapper authResult(0, authCode);

	//if (DSC::Decode(authResult, pUserUserInfo, nUserUserInfoLen))
	//{
	//	CString strLog = _T("Decode failed");
	//	OutputDebugString(strLog);
	//	OutputResult(strLog);
	//}

	////
 //	USES_CONVERSION;
 //	CString strFormat = _T("OnQueryUserInfoRsp: nReturnCode=%d, nChannelID=%d, pUserKey=%s, pUserUserInfo=%s\n");
 //	CString strResult;
 //	strResult.Format(strFormat, nReturnCode, nChannelID, A2T(pUserKey), A2T(pUserUserInfo));
 //
 //	OutputDebugString(strResult);
 //	OutputResult(strResult);
}
unsigned int g_nShareUserInfoID = 0;
std::string g_strShareInfoUrl;
void OnIDShareUserInfoRsp(const int nShareInfoId, const int nReturnCode, char* pShareInfoUrl, const unsigned int nShareInfoUrlLen)
{
	//iDomDownloadShareInfo(331082133312098233, 333, "zhangsan", strlen("zhangsan"), pShareInfoUrl, nShareInfoUrlLen);
	//g_strShareInfoUrl.assign(pShareInfoUrl, nShareInfoUrlLen);
	//iDomDeleteShareInfo(222, g_pShareInfoUrl, g_nShareInfoUrlLen);
}

void OnIDDownLoadUserInfoRsp(const int nDownloadInfoId, const int nReturnCode,
	const char* pShareInfo, const unsigned int pShareInfoLen)
{
	//iDomDeleteShareInfo(222, (char *)g_strShareInfoUrl.c_str(), g_strShareInfoUrl.length());
}

void OnIDConfirmUserInfoRsp(const int nDeleteInfoId, const int nReturnCode)
{
}

//==ID链测试相关----更改用户信息响应
void OnIDUpdateUserInfoRsp(const int nReturnCode, int nReqID, int nType,
	const char* pTransKey, const unsigned int nTransKeyLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnUpdateUserInfoRsp: nReturnCode=%d\n");
	CString strResult;
	strResult.Format(strFormat, nReturnCode);

	OutputDebugString(strResult);
	OutputResult(strResult);
}

//==ID链测试相关----查询交易信息响应
void OnIDGetBcTransInfoRsp(const int nReturnCode, int nReqID, const char* pTransKey, const int nTransKeyLen, const char* pTransInfo, const int nTransInfoLen)
{
	USES_CONVERSION;
	CString strFormat = _T("OnGetBcTransInfoRsp: nReturnCode=%d, pTransKey=%s, pTransInfo=%s\n");
	CString strResult;
	std::string strTrans(pTransInfo, nTransInfoLen);
	std::string strTransKey(pTransKey, nTransKeyLen);
	strResult.Format(strFormat, nReturnCode, A2T(strTransKey.c_str()), A2T(strTrans.c_str()));

	OutputDebugString(strResult);
	OutputResult(strResult);
}

void onCommonRsp(const int nReturnCode, const unsigned int nReqID, const unsigned int nInterfaceType, const char* info, const unsigned int infoLen)
{
	switch (nInterfaceType)
	{
	//case IDC::EN_IDOM_INTERFACE_ID_SHARE_COMM:
		//分享回调
	//	break;

	default:
		OutputDebugString(_T("nInterfaceType error\n"));
		OutputResult(_T("nInterfaceType error"));
		break;
	}
}


//==ID链测试相关----初始化CA
void CcsdktestDlg::OnBnClickedBtnCaId()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_idc_ca_c_sdk(OnReady_BcCaCSdk, OnAbnormal_BcCaCSdk, OnExit_BcCaCSdk, OnIDRegistUserRsp, OnIDUnregistUserRsp);
}

//==ID链测试相关----注册
void CcsdktestDlg::OnBnClickedBtnRegId()
{
	// TODO: 在此添加控件通知处理程序代码
	//std::string strUserInfo = "{\"m_userInfo\":\"hello world\"}";

	//IDC::CIDCUserKey userKey;
	//userKey.IDNumber = 1;
	//char* pUserKey = NULL;
	//size_t nKeyLen = 0;

	////DEF_ENCODE(userKey, pUserKey, nKeyLen);

	//DSC::Encode(userKey, pUserKey, nKeyLen);

	//IDC::CIDCUserInfo userInfo;
	//userInfo.recordId = -1;
	//userInfo.userInfoHash = "000000";

	//char* pUserInfo = NULL;
	//size_t nUserInfoLen = 0;
	//DSC::Encode(userInfo, pUserInfo, nUserInfoLen);

	//reg_idc_user(TEST_USER_KEY);
}

//==ID链测试相关----注销
void CcsdktestDlg::OnBnClickedBtnUnregId()
{
	// TODO: 在此添加控件通知处理程序代码
	//IDC::CIDCUserKey userKey;
	//userKey.IDNumber = 1;
	//char* pUserKey;
	//size_t nKeyLen;

	//DEF_ENCODE(userKey, pUserKey, nKeyLen);
	//DSC::Encode(userKey, pUserKey, nKeyLen);
	//unreg_idc_user(TEST_USER_KEY);
}

//==ID链测试相关----初始化Client SDK
void CcsdktestDlg::OnBnClickedBtnUserInitId()
{
	// TODO: 在此添加控件通知处理程序代码
	//init_idc_c_sdk(OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnIDQueryUserInfoRsp, OnIDUpdateUserInfoRsp, OnIDGetBcTransInfoRsp);
	//iDomInitSdk(OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnIDShareUserInfoRsp, OnIDDownLoadUserInfoRsp, OnIDConfirmUserInfoRsp);
}

//==ID链测试相关----根据userkey,获取用户信息
void CcsdktestDlg::OnBnClickedBtnUserGetId()
{
	// TODO: 在此添加控件通知处理程序代码
	//IDC::CIDCUserKey userKey;
	//userKey.IDNumber = 1;
	//char* pUserKey;
	//size_t nKeyLen;

	//DEF_ENCODE(userKey, pUserKey, nKeyLen);

	//query_idc_user(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length());
}

//==ID链测试相关----根据userkey，发起提案，更改用户信息
void CcsdktestDlg::OnBnClickedBtnUserSetId()
{
	// TODO: 在此添加控件通知处理程序代码
	char pUserInfo[] = "{    \
			\"name\": \"bc.wang\",    \
			\"IDNum\" : \"41072111111111\",  \
			\"mail\" : \"yaddddddddddddddddd@163.com\",   \
			\"phone\" : \"13958188888\",   \
			\"idm\" : 10000,     \
			\"force\" : 123.000000   \
		}";

	//launch_idc_trans(TEST_USER_KEY, 111, DSC_STRING_TYPE_PARAM(pUserInfo), (char*)g_strUserKeys.c_str(), g_strUserKeys.length());
}

//==ID链测试相关----根据交易id，查新交易信息
void CcsdktestDlg::OnBnClickedBtnGettransInfoId()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CString strWTransKey;
	GetDlgItemText(IDC_EDT_TRANS_ID, strWTransKey);
	std::string strTransKey(T2A(strWTransKey));

	//int nChannelID = 124;
	std::string strUserKey = "{\"IDNumber\":111}";
	std::string strCryptUserKey = g_CryptUserkeys[strUserKey];
	//get_idc_trans_info((char*)strUserKey.c_str(), strUserKey.length()
		//, (char*)strCryptUserKey.c_str(), strCryptUserKey.length(), (char*)strTransKey.c_str(), strTransKey.length());
}

void CcsdktestDlg::OutputResult(CString strOutput)
{
	m_output.InsertString(m_output.GetCount(), strOutput);
}



void CcsdktestDlg::OnBnClickedBtnAuth()
{
	// TODO: 在此添加控件通知处理程序代码
	//auth_idc_user(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length());
}


void CcsdktestDlg::OnBnClickedBtnQueryAsset()
{
	// TODO: 在此添加控件通知处理程序代码
	//idcQueryUserAsset(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length());
}


void CcsdktestDlg::OnBnClickedBtnBouns()
{
	// TODO: 在此添加控件通知处理程序代码
	//idcReceiveBonus(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), 1);
}


void CcsdktestDlg::OnBnClickedBtnEarlyBirdEnter()
{
	// TODO: 在此添加控件通知处理程序代码
	//idcBet2EarlyBird(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length());
}


void CcsdktestDlg::OnBnClickedBtnEarlyBirdSignin()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomEarlyBirdSign(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length());
}


void CcsdktestDlg::OnBnClickedBtnEarlyBirdHistory()
{
	// TODO: 在此添加控件通知处理程序代码
	char key[] = "test";
	//iDomGetEarlyBirdHistory(TEST_USER_KEY, 111, key, 5, "2019-09-06", 10);
	//iDomGetEarlyBirdHistory(2, 123, key, 5, "1916074254944", 10);
	
}


void CcsdktestDlg::OnBnClickedBtnIdomInit()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomInitSdk(OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, OnIDShareUserInfoRsp, OnIDDownLoadUserInfoRsp, OnIDConfirmUserInfoRsp,
		//onIDomQueryShareHistoryInfoRsp, onIDomGetAssetHistoryInfoRsp, onIDomGetForceHistoryInfoRsp, onIDomGetEarlyBirdHistoryRsp, onEarlyBirdSign, onCommonRsp);
	//iDomInitSdk(OnReady_BcUserCSdk, OnAbnormal_BcUserCSdk, OnExit_BcUserCSdk, onCommonRsp);
}



void CcsdktestDlg::OnBnClickedBtnGetIdmHistory()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomGetAssetHistoryInfo(230711199705288480, 134, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), 1, 5, 0, 0);
}


void CcsdktestDlg::OnBnClickedBtnAddForce()
{
	// TODO: 在此添加控件通知处理程序代码
	//idcAddForce(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), 1, 1);
}


void CcsdktestDlg::OnBnClickedBtnForceHistory()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomGetForceHistoryInfo(TEST_USER_KEY, 145, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), 1, 5);
}


void CcsdktestDlg::OnBnClickedBtnUploadShareinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	char name[] = "bc.wang";
	//iDomShareInfo(TEST_USER_KEY, 1, name, strlen(name), "111111", 6);
}


void CcsdktestDlg::OnBnClickedBtnDownloadShareinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomDownloadShareInfo(TEST_USER_KEY, 1, "4444", 4, "url", 3);
}

void CcsdktestDlg::OnBnClickedBtnDeleteShareinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomDeleteShareInfo(1, "url", 3);
}


void CcsdktestDlg::OnBnClickedBtnShareHistory()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomQueryShareHistoryInfo(TEST_USER_KEY, 1, 0, 1, 5);
}

void CcsdktestDlg::OnBnClickedBtnTransfer()
{
	// TODO: 在此添加控件通知处理程序代码
	//idcTransferIDM(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), TEST_USER_TARGET_KEY, 10.00);
}


void CcsdktestDlg::OnBnClickedBtnEarlyBirdPeopleCount()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomGetEarlyBirdPeopleCount(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), "2019-09-06", strlen("2019-09-06"));
}


void CcsdktestDlg::OnBnClickedBtnEarlyBirdRankList()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomGetEarlyBirdRankList(TEST_USER_KEY, 111, (char*)g_strUserKeys.c_str(), g_strUserKeys.length(), "2019-09-13", strlen("2019-09-13"), 10);
}


void CcsdktestDlg::OnBnClickedBtnIfEarlyBirdSign()
{
	// TODO: 在此添加控件通知处理程序代码
	//iDomIfEarlyBirdSign(TEST_USER_KEY, 222);
}

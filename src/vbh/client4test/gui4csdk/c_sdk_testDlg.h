
// c_sdk_testDlg.h: 头文件
//

#pragma once


// CcsdktestDlg 对话框
class CcsdktestDlg : public CDialogEx
{
// 构造
public:
	CcsdktestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_C_SDK_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReg();
	afx_msg void OnBnClickedBtnCaInit();
	afx_msg void OnBnClickedBtnUnreg();
	afx_msg void OnBnClickedBtnUserInit();
	afx_msg void OnBnClickedBtnUserGet();
	afx_msg void OnBnClickedBtnUserSet();

private:
	
public:
	afx_msg void OnBnClickedBtnGettransInfo();
	afx_msg void OnBnClickedBtnCaId();
	afx_msg void OnBnClickedBtnRegId();
	afx_msg void OnBnClickedBtnUnregId();
	afx_msg void OnBnClickedBtnUserInitId();
	afx_msg void OnBnClickedBtnUserGetId();
	afx_msg void OnBnClickedBtnUserSetId();
	afx_msg void OnBnClickedBtnGettransInfoId();
	afx_msg void OnBnClickedBtnUserInitInit();
};

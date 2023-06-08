
// MIDI2ArrayDlg.h : ヘッダー ファイル
//

#pragma once


// CMIDI2ArrayDlg ダイアログ
class CMIDI2ArrayDlg : public CDialogEx
{
// コンストラクション
public:
	CMIDI2ArrayDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIDI2ARRAY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_List;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedConvert();
	afx_msg void OnBnClickedFoldersel();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedFilesel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFileclear();
};

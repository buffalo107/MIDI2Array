
// MIDI2ArrayDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MIDI2Array.h"
#include "MIDI2ArrayDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include "MidiFile.h"

using namespace smf;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CMIDI2ArrayDlg ダイアログ



CMIDI2ArrayDlg::CMIDI2ArrayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIDI2ARRAY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMIDI2ArrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CMIDI2ArrayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CONVERT, &CMIDI2ArrayDlg::OnBnClickedConvert)
	ON_BN_CLICKED(IDC_FOLDERSEL, &CMIDI2ArrayDlg::OnBnClickedFoldersel)
	ON_BN_CLICKED(IDC_FILESEL, &CMIDI2ArrayDlg::OnBnClickedFilesel)
	ON_BN_CLICKED(IDC_FILECLEAR, &CMIDI2ArrayDlg::OnBnClickedFileclear)
END_MESSAGE_MAP()


// CMIDI2ArrayDlg メッセージ ハンドラー

BOOL CMIDI2ArrayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	DragAcceptFiles();//ドラッグアンドドロップを受け付る

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMIDI2ArrayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMIDI2ArrayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMIDI2ArrayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int filenum = 0;									//合計ファイル数
vector<CString> filename{};							//ファイル名格納用配列
CString selectfolder = (CString)("0");				//出力先フォルダ記憶用変数
void MIDIconvert(string midifilename, string* txt,bool VolDataInclude);	//MIDIデータの変換用関数を宣言

//ファイルのドラッグ＆ドロップ
void CMIDI2ArrayDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	
	//ドロップされたファイルの数を取得
	UINT numdrop = DragQueryFile(hDropInfo, UINT( -1 ), NULL, 0);
	
	//ドロップされたファイルの数だけ繰り返す
	for (int i = 0;i < (int)numdrop;i++) {
		
		//i番目のファイル名の長さを取得
		UINT length = DragQueryFile(hDropInfo, i, NULL, 0);

		//ファイル名一時記憶用変数
		CString csfile;
		
		//i番目のファイル名を取得
		DragQueryFile(hDropInfo, i, csfile.GetBuffer(length + 1), length + 1);
		csfile.ReleaseBuffer();
		
		//ドロップされたファイルがMIDIファイルでなければ以下の処理をスキップ
		if (csfile.Right(4) != ".mid")continue;
		
		//ファイル重複判定用変数
		bool dup = 0;
		
		//ファイル名の配列を走査して同名のファイルがないか探す
		for (int j = 0;j < (int)filename.size();j++) {
			if (csfile == filename[j]) {//同名のファイルがあった場合はdupを1にして走査を終了
				dup = 1;
				break;
			}
		}
		
		//dupが１（＝同じ名前のファイルが存在している）の場合は以下の処理をスキップ
		if (dup)continue;
		
		//リストにファイル名を追加して表示
		m_List.AddString(csfile);
		
		//配列にファイル名を追加し、合計ファイル数を１増やす
		filename.push_back(csfile);
		filenum++;
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

//変換元のファイル選択ボタン
void CMIDI2ArrayDlg::OnBnClickedFilesel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	
	//CFileDialog fileDialog(true, nullptr, nullptr, OFN_ALLOWMULTISELECT);

	// 警告ダイアログ表示
	CFileDialog fileDialog(true, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, L"スタンダードMIDIファイル (*.mid)|*.mid||");

	// 選択可能なファイル数の最大値
	// (数値は任意)
	const unsigned long iCount=10000;

	// 選択ファイルのフルパスを格納するバッファーのサイズ
	// (1ファイル毎にnull終端、バッファー最後のnull終端を考慮)
	const unsigned long iLength = (MAX_PATH + 1) * iCount + 1;

	// バッファーの変数を初期化
	wchar_t* fullPathBuffer = new wchar_t[iLength];
	wmemset(fullPathBuffer, L'\0', iLength);

	// バッファーをOPENFILENAME構造体に登録
	fileDialog.GetOFN().lpstrFile = fullPathBuffer;
	fileDialog.GetOFN().nMaxFile = iLength;

	// ダイアログ表示
	if (fileDialog.DoModal() == IDOK)
	{
		// [開く]ボタン押下

		// バッファーの先頭を取得
		POSITION pos = fileDialog.GetStartPosition();

		while (pos != nullptr)
		{
			// 選択したファイル数ループする

			// フルパス取得
			CString fullPathName = fileDialog.GetNextPathName(pos);

			// ここでfullPathNameに格納されたフルパスを使用する
			
			//ファイル重複判定用変数
			bool dup = 0; 

			//ファイル名の配列を走査して同名のファイルがないか探す
			for (int j = 0;j < (int)filename.size();j++) {
				if (fullPathName == filename[j]) {//同名のファイルがあった場合はdupを1にして走査を終了
					dup = 1;
					break;
				}
			}

			//dupが１（＝同じ名前のファイルが存在している）の場合は以下の処理をスキップ
			if (dup)continue;

			//リストにファイル名を追加して表示
			m_List.AddString(fullPathName);

			//配列にファイル名を追加し、合計ファイル数を１増やす
			filename.push_back(fullPathName);
			filenum++;
		}
	}

	// 変数の解放
	delete[] fullPathBuffer;

}

//選択したファイルのクリアボタン
void CMIDI2ArrayDlg::OnBnClickedFileclear()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	
	//ファイルが選択されていた場合、
	if ((int)filename.size() > 0) {										
		//for (int i = 0;i < (int)filename.size();i++)filename.pop_back();//ファイル名格納用変数の中身を空にする
		filename.clear();//ファイル名格納用変数を初期化
		m_List.ResetContent();	//リストを初期化、
		filenum = 0;			//ファイル数も0にする
	}
}

//出力先のフォルダ選択ボタン
void CMIDI2ArrayDlg::OnBnClickedFoldersel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CFolderPickerDialog dlg;
	if (dlg.DoModal() != IDOK)return;

	//出力先フォルダ記憶用変数に選択したフォルダのパスを格納
	selectfolder = dlg.GetPathName();

	//出力先のフォルダをピクチャーコントロールに表示
	CWnd* cwdPict = GetDlgItem(IDC_PATHVIEW);
	CDC* dcPict = cwdPict->GetWindowDC();
	dcPict->TextOut(0, 0, selectfolder);
}

//変換ボタン
void CMIDI2ArrayDlg::OnBnClickedConvert()
{
	//データが選択されていなかったらエラーを表示
	if((int)filename.size() == 0 && selectfolder == (CString)("0"))MessageBox(TEXT("MIDIファイルと出力フォルダを選択してください"), TEXT("エラー"), MB_OK);
	else if ((int)filename.size() == 0)MessageBox(TEXT("MIDIファイルを選択してください"), TEXT("エラー"), MB_OK);
	//データが選択されていてもフォルダが選択されていなかったらエラーを表示
	else if (selectfolder == (CString)("0"))MessageBox(TEXT("出力フォルダを選択してください"), TEXT("エラー"), MB_OK);
	//データもフォルダも選択されていたら変換を行う
	else {
		//ファイルの数だけ繰り返す
		for (int i = 0;i < (int)filename.size();i++) {

			//ファイル名（パスや拡張子も含む）からファイルのタイトルのみを抽出（"C:\sample.mid"→"sample"）
			CString filetitle = filename[i].Right(filename[i].GetLength() - filename[i].ReverseFind(_T('\\')) - 1);//ファイル名の一番右側にある「\\」よりも左側の部分を抽出、一時記憶用変数に保存（パスの削除）
			filetitle = filetitle.Left(filetitle.ReverseFind(_T('.')));//保存したファイル名の「.」から右側を抽出（拡張子の削除）
						
			//作成するファイル名を「フォルダのパス＋抽出した変換元ファイルのタイトル＋テキストファイルの拡張子（.txt）にする」
			CString newfilename = selectfolder + "/" + filetitle + ".txt";

			//MIDIデータ変換用関数とやり取りするための配列を用意
			string MIDItxt;

			//音量データを含めるかどうかのチェックボックスの値を取得
			CButton* VolDataCheckBox = (CButton*)GetDlgItem(IDC_VOLUMEDATA);
			bool VolDataInclude = VolDataCheckBox->GetCheck();

			//MIDIデータを変換。引数は変換元のファイル名と、変換したデータを格納する配列のポインタ、音量データを含めるかどうかの判定
			MIDIconvert(CStringA(filename[i]).GetBuffer(), &MIDItxt, VolDataInclude);

			//ファイルの出力
			ofstream writing_file;

			//出力先ファイルを開く
			writing_file.open(newfilename, ios::out);

			//ファイル書き込み用の配列に変換したMIDIデータを格納
			string writing_text = MIDItxt;
			
			//ファイルにデータを書き込み
			writing_file << writing_text << endl;

			//ファイルを閉じる
			writing_file.close();
			
			
		}
		//処理の終了を知らせる
		MessageBox(TEXT("データ処理が終わりました"), TEXT("結果"), MB_OK);
	}

}

//MIDIファイル解析・変換用関数
void MIDIconvert(string midifilename,string* txt,bool VolDataInclude) {
	// MIDIファイルを解析
	MidiFile midi_file;
	
	//MIDIファイルを読み込み
	midi_file.read(midifilename);
	
	// マスタートラックのテンポを元に、全MIDIイベントの時間(秒)を計算
	midi_file.doTimeAnalysis();

	//曲の最初と最後の時間を取得
	vector <double> starttime{};
	vector <double> endtime{};

	for (int track = 0; track < midi_file.getTrackCount(); track++) {
		
		for (int event = 0; event < midi_file[track].size(); event++) {
			if (midi_file[track][event].isNoteOn()) {
				starttime.push_back(midi_file[track][event].seconds);
				break;
			}
		}

		for (int event = midi_file[track].size()-1; event > 0; event--) {
			if (midi_file[track][event].isNoteOff()) {
				endtime.push_back(midi_file[track][event].seconds);
				break;
			}
		}
	}

	double timemin = *min_element(starttime.begin(), starttime.end());
	double timemax = *max_element(endtime.begin(), endtime.end());
	
	// 全トラックのMIDIイベントから音階、強弱、秒数を取得
	for (int track = 0; track < midi_file.getTrackCount(); track++) {
		
		//要素数カウント用変数
		int cnt = 0;

		//トラックの中にあるすべてのMIDIイベントを走査
		for (int event = 0; event < midi_file[track].size(); event++) {

			//ノートオンを検出したら以下の処理を実行
			if (midi_file[track][event].isNoteOn()) {

				if (cnt == 0) {
					//配列のフォーマットで出力する
					*txt += "int track";
					*txt += to_string(track);
					*txt += "[] = {\r\n	";
					
					//曲の開始時間とトラックの開始時間の差を求める
					double trackstarttime = (midi_file[track][event].seconds-timemin)* 1000.0000000000;

					//もし曲の開始時間とトラックの開始時間に差があったら無音区間を作る
					if (trackstarttime != 0) {
						*txt += "128";	//MIDIの音階は0～127で表されるので、出力する音階がない場合に128を割り当てた
						*txt += ",";
						if (VolDataInclude) {//音量データの出力が選択されていたら実行
							*txt += "0";	//音の強さは0
							*txt += ",";
						}
						*txt += to_string(trackstarttime).erase(to_string(trackstarttime).find('.'));
						*txt += ", ";

						cnt++;
					}
				}

				//ノートオンした音の音階と強さを取得、秒数を格納する変数
				int KeyNumber=midi_file[track][event].getKeyNumber();
				int Velocity = midi_file[track][event].getVelocity();
				double NoteOnLength=0;
				
				//ノートオンした場所から先を走査して、音の区切りを見つける
				for (int i = event+1;i < midi_file[track].size();i++) {
					//もし新しいノートオンを見つけるか、同じ音のノートオフを見つけたら、ノートオンからそこまでの秒数を計算して走査終了
					if ((midi_file[track][i].isNoteOn()) || ((midi_file[track][i].isNoteOff()) && (midi_file[track][i].getKeyNumber() == midi_file[track][event].getKeyNumber()))) {
						NoteOnLength = ((midi_file[track][i].seconds) - (midi_file[track][event].seconds))*1000.0000000000;//秒数は1000倍してミリ秒単位で出力
						break;
					}
				}
				
				//配列に音階、強弱、秒数を出力
				*txt += to_string(KeyNumber);
				*txt += ",";
				if (VolDataInclude) {//音量データの出力が選択されていたら実行
					*txt += to_string(Velocity);
					*txt += ",";
				}
				*txt += to_string(NoteOnLength).erase(to_string(NoteOnLength).find('.'));//秒数は小数点以下を切り捨てて出力
				*txt += ", ";

				//カウントを１増やし、10セットごとに改行を入れる
				cnt++;
				if (cnt % 10 == 0)*txt += "\r\n	";

			}
			//ノートオフからノートオンまでの間に、必要に応じて無音の区間を作る
			else if (midi_file[track][event].isNoteOff() && midi_file[track][event + 1].isNoteOn()) {
				
				//秒数格納用変数
				double NoteOnLength = 0;

				//ノートオフを検出しても、別の音がすでに鳴っていたら無音区間は作らない
				bool silence = 0;//無音区間処実行フラグ
				//ノートオフした場所から手前を走査してノートオンを見つける
				for (int i = event - 1;i >= 0;i--) {
					//見つかったノートオンが違う音なら無音区間処理のフラグを下ろす、同じ音なら立てる
					if ((midi_file[track][i].isNoteOn())){
						if (midi_file[track][i].getKeyNumber() != midi_file[track][event].getKeyNumber())silence = 0;
						else if (midi_file[track][i].getKeyNumber() == midi_file[track][event].getKeyNumber())silence = 1;
						break;
					}
				}
				
				//無音区間処理のフラグが下りていたら以降の処理をスキップ	
				if (!silence)continue;

					//ノートオフから次のノートオンまでの時間に違いがあったら
				if (midi_file[track][event].seconds!= midi_file[track][event+1].seconds) {
					//その時間を記憶
					NoteOnLength= (midi_file[track][event + 1].seconds - midi_file[track][event].seconds) * 1000.0000000000;//秒数は1000倍してミリ秒単位で出力

					//配列に音階、強弱、秒数を出力
					*txt += "128";	//MIDIの音階は0～127で表されるので、出力する音階がない場合に128を割り当てた
					*txt += ",";
					if (VolDataInclude) {//音量データの出力が選択されていたら実行
						*txt += "0";	//音の強さは0
						*txt += ",";
					}
					*txt += to_string(NoteOnLength).erase(to_string(NoteOnLength).find('.'));
					*txt += ", ";
					
					//カウントを１増やし、10セットごとに改行を入れる
					cnt++;
					if (cnt % 10 == 0)*txt += "\r\n	";
				}
			}
			
			//ループの最後にcntが0でなかったら処理を実行
			if (event == midi_file[track].size() - 1 && cnt > 0) {

				//最後のノートオフから曲の終了まで無音区間を作る
				double trackendtime = 0;
				for (int i = event; i > 0; i--) {
					if (midi_file[track][i].isNoteOff()) {
						trackendtime = (timemax - midi_file[track][i].seconds) * 1000.0000000000;
						break;
					}
				}
				*txt += "128";	//MIDIの音階は0～127で表されるので、出力する音階がない場合に128を割り当てた
				*txt += ",";
				if (VolDataInclude) {//音量データの出力が選択されていたら実行
					*txt += "0";	//音の強さは0
					*txt += ",";
				}
				*txt += to_string(trackendtime).erase(to_string(trackendtime).find('.'));
				*txt += ", ";

				//配列の終端
				*txt += "\r\n};\r\n\r\n";
			}
			
		}

		

	}
}


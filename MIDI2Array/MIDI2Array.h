
// MIDI2Array.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMIDI2ArrayApp:
// このクラスの実装については、MIDI2Array.cpp を参照してください
//

class CMIDI2ArrayApp : public CWinApp
{
public:
	CMIDI2ArrayApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMIDI2ArrayApp theApp;

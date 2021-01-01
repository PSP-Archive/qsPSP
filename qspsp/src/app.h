#ifndef _APP_H_
#define _APP_H_

#include <mgf.h>
#include <mgftexture.h>
#include <mgfsprite.h>
#include <mgfutils.h>
#include <mgffont.h>
#include <mgfaudio.h>
#include "widget.h"
#include "qsp/declarations.h"
#include "qsp/objects.h"
#include "mainwidget.h"
#include "maindescwidget.h"
#include "varsdescwidget.h"
#include "actionswidget.h"
#include "objectswidget.h"
#include "objectmenuwidget.h"
#include "messageboxwidget.h"
#include "inputboxwidget.h"
#include "picviewwidget.h"
#include "gamebrowserwidget.h"
#include "savebrowserwidget.h"
#include "appdialog.h"

enum {
  W_MAIN = 1,
  W_MAINDESC,
	W_VARSDESC,
	W_ACTIONS,
	W_OBJECTS,
	W_OBJECTMENU,
	W_MESSAGEBOX,
	W_INPUTBOX,
	W_PICVIEW,
	W_GAMEBROWSER,
	W_SAVEBROWSER
};

enum {
  MSG_OK = 1,
  MSG_CANCEL
};

struct CRes {
	char *skin;
	MGFFONT *font;
	float fontScale;
	int fontColor;
	int bgColor;
	int selColor;
	int ramColor;
	MGFTEXTURE *texMain, *texMsg, *texMenu, *texInput;
	MGFSPRITE *sprMain, *sprMsg, *sprMenu, *sprInput;
	MGFAUDIO *Audio;
	
	CRes(void) {
		skin = 0;
		font = 0;
		fontColor = 0xFFFFFFFF;
	  bgColor = 0xFF000000;
	  selColor = 0xFFFF0000;
	  ramColor = 0xFF0000FF;
		texMain = texMsg = texMenu = texInput = 0;
		sprMain = sprMsg = sprMenu = sprInput = 0;
	}
	
	void Loading(MGF *mgf) {
	  if(!mgf || !font) return;
	  mgf->Gfx_BeginScene();
	  mgf->Gfx_Clear(bgColor);
	  font->SetAngle(0);
	  font->SetScale(fontScale);
	  font->SetColor(fontColor);
	  font->Render(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, MGFFONT_CENTER, "Loading...");
	  mgf->Gfx_EndScene();
	}
};

class CApp : public CWidgetEngine {
public:
	CApp(void);
	~CApp(void);
	
	void Init(void);
	bool Update(void);
	void Render(void);
	
	// CallBacks
	static void RefreshInt(QSP_BOOL isRedraw);
	static void SetTimer(long msecs);
	static void SetInputStrText(QSP_CHAR *text);
	static QSP_BOOL IsPlay(QSP_CHAR *file);
	static void CloseFile(QSP_CHAR *file);
	static void PlayFile(QSP_CHAR *file, long volume);
	static void ShowPane(long type, QSP_BOOL isShow);
	static void Sleep(long msecs);
	static long GetMSCount();
	static void Msg(QSP_CHAR *str);
	static void DeleteMenu();
	static void AddMenuItem(QSP_CHAR *name, QSP_CHAR *imgPath);
	static void ShowMenu();
	static void Input(QSP_CHAR *text, QSP_CHAR *buffer, long maxLen);
	static void ShowImage(QSP_CHAR *file);
	static void OpenGameStatus();
	static void SaveGameStatus();
	
	static bool GetVarValue(QSP_CHAR *name, long *num, QSP_CHAR **str);
	//MGFFONT *GetFont(void) { return font; };
	//float GetFontScale(void) { return fontScale; };
	//int GetFontColor(void) { return fontColor; }
	void DelHtml(char *txt);
	void ShowError(void);
	void ClearSkin(void);
	void SetSkin(const char *name);
	
	// public data
	CMainWidget *MainWidget;
	CMainDescWidget *MainDescWidget;
	CVarsDescWidget *VarsDescWidget;
	CActionsWidget *ActionsWidget;
	CObjectsWidget *ObjectsWidget;
	CObjectMenuWidget *ObjectMenuWidget;
	CMessageBoxWidget *MessageBoxWidget;
	CInputBoxWidget *InputBoxWidget;
	CPicViewWidget *PicViewWidget;
	CGameBrowserWidget *GameBrowserWidget;
	CSaveBrowserWidget *SaveBrowserWidget;
	
	CAppDialog *AppDialog;
	
	// qsp timer
	float qspTime, qspTimer, qspSleepTimer;
	float MSCount;
	// qsp
	bool isScroll;
	bool isCanSave;
	bool isHtml;
	
	bool isNowUpdate;
	
protected:	
	static CApp *App;
	bool isInit;
	char str[260];
	CRes Res;

	void InitCallBacks(void);
};

#endif


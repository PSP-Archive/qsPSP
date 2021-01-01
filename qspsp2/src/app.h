#ifndef _APP_H_
#define _APP_H_

#include <mgf.h>
#include <mgftexture.h>
#include <mgfsprite.h>
#include <mgfutils.h>
#include <mgffont.h>
#include <mgfaudio.h>
#include "widget.h"
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

struct CSkin {
	char *name;
	MGFFONT *font;
	int font_color;
	int font_color_select;
	int font_color_active;
	
	MGFTEXTURE *tex;
	MGFSPRITE *spr_0_0, *spr_1_0, *spr_2_0, *spr_0_1, *spr_1_1, *spr_2_1, *spr_0_2, *spr_1_2, *spr_2_2;
	
	CSkin(void) {
		name = 0;
		font = 0;
		font_color = 0xFFFFFFFF;
	  font_color_select = 0xFFFF0000;
	  font_color_active = 0xFF0000FF;
		tex = 0;
		spr_0_0 = spr_1_0 = spr_2_0 = spr_0_1 = spr_1_1 = spr_2_1 = spr_0_2 = spr_1_2 = spr_2_2 = 0;
	}
};

class CApp : public CWidgetEngine {
public:
	CApp(void);
	~CApp(void);
	
	void init(void);
	bool update(void);
	void render(void);
	
	// CallBacks
	/*static void qsp_RefreshInt(QSP_BOOL isRedraw);
	static void qsp_SetTimer(long msecs);
	static void qsp_SetInputStrText(QSP_CHAR *text);
	static QSP_BOOL qsp_IsPlay(QSP_CHAR *file);
	static void qsp_CloseFile(QSP_CHAR *file);
	static void qsp_PlayFile(QSP_CHAR *file, long volume);
	static void qsp_ShowPane(long type, QSP_BOOL isShow);
	static void qsp_Sleep(long msecs);
	static long qsp_GetMSCount();
	static void qsp_Msg(QSP_CHAR *str);
	static void qsp_DeleteMenu();
	static void qsp_AddMenuItem(QSP_CHAR *name, QSP_CHAR *imgPath);
	static void qsp_ShowMenu();
	static void qsp_Input(QSP_CHAR *text, QSP_CHAR *buffer, long maxLen);
	static void qsp_ShowImage(QSP_CHAR *file);
	static void qsp_OpenGameStatus();
	static void qsp_SaveGameStatus();*/
	
	//static bool getVarValue(QSP_CHAR *name, long *num, QSP_CHAR **str);
	//MGFFONT *GetFont(void) { return font; };
	//float GetFontScale(void) { return fontScale; };
	//int GetFontColor(void) { return fontColor; }
	void showError(void);
	void clearSkin(void);
	bool setSkin(const char *name);
	
	// public data
	/*CMainWidget *MainWidget;
	CMainDescWidget *MainDescWidget;
	CVarsDescWidget *VarsDescWidget;
	CActionsWidget *ActionsWidget;
	CObjectsWidget *ObjectsWidget;
	CObjectMenuWidget *ObjectMenuWidget;
	CMessageBoxWidget *MessageBoxWidget;
	CInputBoxWidget *InputBoxWidget;
	CPicViewWidget *PicViewWidget;
	CGameBrowserWidget *GameBrowserWidget;
	CSaveBrowserWidget *SaveBrowserWidget;*/
	
	CAppDialog *app_dialog;
	
	// qsp timer
	float qsp_time, qsp_timer, qsp_sleep_timer;
	float ms_count;
	// qsp
	bool is_scroll;
	bool is_can_save;
	bool is_html;
	
	bool is_now_update;
	
protected:	
	static CApp *app;
	bool is_init;
	char str[260];
	CSkin *skin;
	MGFAUDIO *audio;

	//void initCallBacks(void);
};

#endif


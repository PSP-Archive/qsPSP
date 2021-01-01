#ifndef _MAINWIDGET_H_
#define _MAINWIDGET_H_

#include "widget.h"
#include <mgffont.h>

class CApp;
struct CRes;

class CMainWidget : public CWidget {
public:
  CMainWidget(int id = -1);
  ~CMainWidget(void);
  
  void OnRegister(void);
  void OnShow(void);
  void OnHide(void);
  void OnActive(void);
  
  bool Update(float delta);
  void Render(void);
  
  void ShowGameBrowser(void);
  void ShowSaveBrowser(int state = -1);
  char *GetGameName(void) { return GameName; }
  char *GetGameDir(void) { return GameDir; }
  
  //CWidget *Left, *Right, **Active;
  bool isLeft;
  
protected:
  CApp *App;
  CRes *Res;
  
  char GameName[MGF_MAX_PATH];
  char GameDir[MGF_MAX_PATH];
  bool isRTrig;
};

#endif

#ifndef _MAINDESCWIDGET_H_
#define _MAINDESCWIDGET_H_

#include "widget.h"
#include <mgffont.h>

#define MAINDESCWIDGET_SPEED 120.f

class CApp;
struct CRes;

class CMainDescWidget : public CWidget {
public:
  enum {
		SCROLL_UP = 0,
		SCROLL_NULL,
		SCROLL_DOWN
	};

  CMainDescWidget(int id = -1);
  ~CMainDescWidget(void);
  
  void OnRegister(void);
  void OnShow(void);
  void OnHide(void);
  void OnActive(void);
  
  bool Update(float delta);
  void Render(void);
  
  void SetText(const char *txt, int scroll, bool html = false);
  
protected:
  CApp *App;
  CRes *Res;
  
  float Scroll;
  float ScrollHeight;
  
  char *Text;
  int cntBuf;
};

#endif


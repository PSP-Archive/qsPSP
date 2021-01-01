#ifndef _INPUTBOXWIDGET_H_
#define _INPUTBOXWIDGET_H_

#include "widget.h"

#define IB_TEXTMAX 64

enum {
  IBS_NOTHREAD = 0,
  IBS_THREAD
};

class CApp;
struct CRes;

class CInputBoxWidget : public CWidget {
public:
  CInputBoxWidget(int id = -1);
  ~CInputBoxWidget(void);
  
  void OnRegister(void);
  void OnShow(void);
  
  bool Update(float delta);
  void Render(void);
  
  void SetMode(int mode) { Mode = mode; if(Mode == IBS_THREAD) isMsg = true; else isMsg = false; };
  void SetText(const char *txt);
  char *GetText(void) { return Text; };
  
protected:
	CApp *App;
  CRes *Res;
  
  int Lang;
  int Mode;
  char Text[IB_TEXTMAX];
  int cur;
  bool isMsg;
  
  int sX, sY;
};

#endif

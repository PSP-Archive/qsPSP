#ifndef _ACTIONSWIDGET_H_
#define _ACTIONSWIDGET_H_

#include "widget.h"
#include <mgffont.h>

#define ACTIONSWIDGET_SPEED 100.f
#define ACTIONSWIDGET_MAX 100

class CApp;
struct CRes;

struct CActionItem {
  char *str;
  MGFTEXTURE *tex;
  int lines;
  int align;
  int param;
  
  CActionItem(void) {
	  str = 0;
	  tex = 0;
	  lines = 0;
	  align = MGFFONT_LEFT;
	  param = 0;
	}
};

class CActionsWidget : public CWidget {
public:
  enum {
		SCROLL_UP = 0,
		SCROLL_NULL,
		SCROLL_DOWN
	};
	
  CActionsWidget(int id = -1);
  ~CActionsWidget(void);
  
  void OnRegister(void);
  void OnShow(void);
  void OnHide(void);
  void OnActive(void);
  
  bool Update(float delta);
  void Render(void);
  
  void Clear(int scroll);
  void Add(const char *img, const char *str, bool ishtml, int align = MGFFONT_LEFT, int param = 0);
  void SetSelection(int sel);
  virtual char *GetText(int id = -1);
  
  virtual void OnSelectItem(bool new_sel);
  virtual void OnActiveItem(void);
  
protected:
  CApp *App;
  CRes *Res;
  
  float Scroll;
  
  CActionItem *Item[ACTIONSWIDGET_MAX];
  int Count;
  int Sel, Cur;
};

#endif


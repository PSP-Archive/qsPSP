#ifndef _GAMEBROWSERWIDGET_H_
#define _GAMEBROWSERWIDGET_H_

#include "actionswidget.h"
#include <mgffont.h>

enum {
  GBM_NONE = 0,
  GBM_OK,
  GBM_CANCEL
};

class CGameBrowserWidget : public CActionsWidget {
public:
  CGameBrowserWidget(int id = -1);
  ~CGameBrowserWidget(void);
  
  //void OnRegister(void);
  void OnShow(void);
  //void OnHide(void);
  //void OnActive(void);
  
  bool Update(float delta);
  void Render(void);
  
  int Result(void) { int res = numResult; numResult = GBM_NONE; return res; };
  
  virtual void OnSelectItem(bool new_sel);
  virtual void OnActiveItem(void);
  virtual char *GetFileName(int id = -1);
  virtual char *GetDir(void) { return CurrDir; };
  
protected:  
  int numResult;
  char CurrDir[MGF_MAX_PATH];
  char Text[MGF_MAX_PATH];
  int SelDir[100];
  int SelDirIndex;
  
  void _scan(const char *scandir);
};

#endif


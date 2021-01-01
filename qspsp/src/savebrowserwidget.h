#ifndef _SAVEBROWSERWIDGET_H_
#define _SAVEBROWSERWIDGET_H_

#include "actionswidget.h"
#include <mgffont.h>

#define SLOT_MAX 16

enum {
  SBM_NONE = 0,
  SBM_OK,
  SBM_CANCEL
};
enum {
  SBS_SAVE = 0,
  SBS_LOAD
};

class CSaveBrowserWidget : public CActionsWidget {
public:
  CSaveBrowserWidget(int id = -1);
  ~CSaveBrowserWidget(void);
  
  //void OnRegister(void);
  void OnShow(void);
  //void OnHide(void);
  //void OnActive(void);
  
  bool Update(float delta);
  void Render(void);
  
  void Set(const char *name, int state = -1);
  int Result(void) { int res = numResult; numResult = SBM_NONE; return res; };
  
  virtual char *GetText(int id = -1);
  virtual void OnSelectItem(bool new_sel);
  virtual void OnActiveItem(void);
  
protected:  
  int State;
  char Name[128];
  char Path[128];
  int numResult;
};

#endif


#ifndef _OBJECTMENUWIDGET_H_
#define _OBJECTMENUWIDGET_H_

#include "actionswidget.h"

class CObjectMenuWidget : public CActionsWidget {
public:
  CObjectMenuWidget(int id = -1);
  ~CObjectMenuWidget(void);
  
  void OnShow(void);
  
  bool Update(float delta);
  void Render(void);
  
  virtual void OnSelectItem(bool new_sel) {};
  virtual void OnActiveItem(void);
  
protected:
  bool isAct;
};

#endif


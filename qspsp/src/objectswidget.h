#ifndef _OBJECTSWIDGET_H_
#define _OBJECTSWIDGET_H_

#include "actionswidget.h"

class CObjectsWidget : public CActionsWidget {
public:
  CObjectsWidget(int id = -1);
  ~CObjectsWidget(void);
  
  virtual void OnSelectItem(bool new_sel) {};
  virtual void OnActiveItem(void);
};

#endif


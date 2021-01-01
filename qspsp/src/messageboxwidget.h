#ifndef _MESSAGEBOXWIDGET_H_
#define _MESSAGEBOXWIDGET_H_

#include "maindescwidget.h"

class CMessageBoxWidget : public CMainDescWidget {
public:
  CMessageBoxWidget(int id = -1);
  ~CMessageBoxWidget(void);
  
  bool Update(float delta);
  void Render(void);
};

#endif


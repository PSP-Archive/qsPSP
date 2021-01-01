#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <mgf.h>
#include "mhalist.h"

class CWidget {
public:
  CWidget(int id = -1);
  ~CWidget(void);

  virtual void onRegister(void) {};
  virtual void onShow(void) {};
  virtual void onHide(void) {};
  virtual void onActive(void) {};
  
  virtual bool update(float delta) { return false; };
  virtual void render(void) {};

  // public data
  int id;
  bool modal;
  
protected:
  static MGF *mgf;
  float x, y;
  float width, height;
};

class CWidgetEngine : public MGFAPP {
public:
	CWidgetEngine(void);
	~CWidgetEngine(void);
	
	bool reg(CWidget *widget, int newid = -1);
	void clear(void);
	CWidget *find(int id);
	void show(int id, bool modal = false, bool rmodal = false);
	void hide(int id);
	
	bool update(void);
	void render(void);

protected:
	mhaList *widget_reg;
	mhaList *widget_update;
	mhaList *widget_render;
	int render_modal_id;
	
	void _render(void);
};

#endif

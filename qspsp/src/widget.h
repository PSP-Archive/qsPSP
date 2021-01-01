#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <mgf.h>
#include "mhalist.h"

class CWidget {
public:
  CWidget(int id = -1);
  ~CWidget(void);

  virtual void OnRegister(void) {};
  virtual void OnShow(void) {};
  virtual void OnHide(void) {};
  virtual void OnActive(void) {};
  
  virtual bool Update(float delta) { return false; };
  virtual void Render(void) {};

  // public data
  int WidgetId;
  bool WidgetModal;
  
protected:
  static MGF *mgf;
  float X, Y;
  float Width, Height;
};

class CWidgetEngine : public MGFAPP {
public:
	CWidgetEngine(void);
	~CWidgetEngine(void);
	
	bool Register(CWidget *widget, int newid = -1);
	void Clear(void);
	CWidget *Find(int id);
	void Show(int id, bool modal = false, bool rmodal = false);
	void Hide(int id);
	
	bool Update(void);
	void Render(void);

protected:
	mhaList *widgetReg;
	mhaList *widgetUpdate;
	mhaList *widgetRender;
	int RenderModal;
	
	void _render(void);
};

#endif

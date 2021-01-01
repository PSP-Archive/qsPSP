#ifndef _APPDIALOG_H_
#define _APPDIALOG_H_

#include <mgf.h>
#include <mgftexture.h>
#include <mgfsprite.h>
#include <mgfutils.h>
#include <mgffont.h>
#include "widget.h"
#include "qsp/declarations.h"

class CApp;
struct CRes;

class CAppDialog : public MGFAPP {
public:
	CAppDialog(CApp *app);
	~CAppDialog(void);
	
	void init(void);
	void free(void) { widget = 0; };
	bool update(void);
	void render(void);
	
	void setWidget(CWidget *widget, bool isshow = true, bool isactive = true) {
		this->widget = widget;
		is_on_active = isactive;
		is_on_show = isshow;
	}

protected:	
	CApp *app;
	CRes *res;
	bool is_init;
	
	CWidget *widget;
	bool is_on_active, is_on_show;
};

#endif


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
	
	void Init(void);
	void Deinit(void) { Widget = 0; };
	bool Update(void);
	void Render(void);
	
	void SetWidget(CWidget *widget, bool isshow = true, bool isactive = true) { Widget = widget; isOnActive = isactive; isOnShow = isshow; };

protected:	
	CApp *App;
	CRes *Res;
	bool isInit;
	
	CWidget *Widget;
	bool isOnActive, isOnShow;
};

#endif


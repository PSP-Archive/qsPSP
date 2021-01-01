#ifndef _PICVIEWWIDGET_H_
#define _PICVIEWWIDGET_H_

#include "widget.h"
#include <mgf.h>
#include <mgfsprite.h>

class CApp;

class CPicViewWidget : public CWidget {
public:
	CPicViewWidget(int id);
	~CPicViewWidget(void);
	
	bool Update(float delta);
	void Render(void);
	
	bool SetPic(const char *picname);
	
protected:
	CApp *App;

	MGFTEXTURE *texPic;
	MGFSPRITE *sprPic;
	float x, y;
};

#endif

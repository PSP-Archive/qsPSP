#include "appdialog.h"

#include "app.h"

CAppDialog::CAppDialog(CApp *app) : MGFAPP() {
	this->app = app;
	is_init = false;
	widget = 0;
}

CAppDialog::~CAppDialog(void) {
}

void CAppDialog::init(void) {
	if(!is_init) {
		res = (CRes*)mgf->System_GetState(MGF_USERDATA);
		is_init = true;
  }
  if(widget) {
		if(is_on_active) widget->onActive();
		if(is_on_show) widget->onShow();
  }
}

bool CAppDialog::update(void) {
	if(!widget) return true;
	
	float delta = mgf->Timer_GetDelta();
	
	return widget->update(delta);
}

void CAppDialog::render(void) {
	if(!widget) return;
	mgf->Gfx_BeginScene();
	// -----	
	mgf->Texture_Enable(true);
	mgf->Gfx_Clear(0xFF000000);
	// -
	mgf->Gfx_SetBlend(MGFBLEND_SUB);
	
	widget->render();

	mgf->Gfx_EndScene();
}

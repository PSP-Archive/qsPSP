#include "appdialog.h"

#include "app.h"

CAppDialog::CAppDialog(CApp *app) : MGFAPP() {
	App = app;
	isInit = false;
	Widget = 0;
}

CAppDialog::~CAppDialog(void) {
}

void CAppDialog::Init(void) {
	if(!isInit) {
		Res = (CRes*)mgf->System_GetState(MGF_USERDATA);
		isInit = true;
  }
  if(Widget) {
		if(isOnActive) Widget->OnActive();
		if(isOnShow) Widget->OnShow();
  }
}

bool CAppDialog::Update(void) {
	if(!Widget) return true;
	
	float delta = mgf->Timer_GetDelta();
	
	return Widget->Update(delta);
}

void CAppDialog::Render(void) {
	if(!Widget) return;
	mgf->Gfx_BeginScene();
	// -----	
	mgf->Texture_Enable(true);
	mgf->Gfx_Clear(Res->bgColor);
	// -
	mgf->Gfx_SetBlend(MGFBLEND_SUB);
	
	Widget->Render();

	mgf->Gfx_EndScene();
}

#include "picviewwidget.h"

#include "app.h"

CPicViewWidget::CPicViewWidget(int id) : CWidget(id) {
	App = (CApp*)mgf->System_GetState(MGF_APP);
	texPic = 0;
	sprPic = 0;
	x = 0;
	y = 0;
}

CPicViewWidget::~CPicViewWidget(void) {
	if(texPic) mgf->Texture_Remove(texPic);
	if(sprPic) delete sprPic;
}
	
bool CPicViewWidget::Update(float delta) {
	if(mgf->Input_IsKeyClick(PSP_CTRL_CROSS)) {
		App->Hide(W_PICVIEW);
	}
	
	return false;
}

void CPicViewWidget::Render(void) {
	if(sprPic) sprPic->Render(x, y);
}
	
bool CPicViewWidget::SetPic(const char *picname) {	
	if(texPic) {
		mgf->Texture_Remove(texPic);
		texPic = 0;
	}
	if(sprPic) {
		delete sprPic;
		sprPic = 0;
	}
	if(!picname || !*picname) return false;
	mgf->System_Log("ViewPic '%s'\n", picname);
	texPic = mgf->Texture_Add(picname);
	if(!texPic) {
		mgf->System_Log("Picture '%s' not found.\n", picname);
		return false;
	}
	sprPic = new MGFSPRITE(texPic, texPic->Width(true), texPic->Height(true), 1, 0, 0, false);
	x = SCREEN_WIDTH * 0.5f - (float)texPic->Width(true) * 0.5f;
	y = SCREEN_HEIGHT * 0.5f - (float)texPic->Height(true) * 0.5f;
	
	mgf->System_Log("X=%0.2f, Y=%0.2f\n", x, y);
	
	return true;
}

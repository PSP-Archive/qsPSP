#include "messageboxwidget.h"

#include "app.h"

CMessageBoxWidget::CMessageBoxWidget(int id) : CMainDescWidget(id) {
  X = 10;
  Y = 10;
  Width = SCREEN_WIDTH - 20;
  Height = SCREEN_HEIGHT - 20;
}

CMessageBoxWidget::~CMessageBoxWidget(void) {
}
  
bool CMessageBoxWidget::Update(float delta) {
	//bool res = CMainDescWidget::Update(delta);
	
	if(mgf->Input_IsKeyClick(PSP_CTRL_CIRCLE) || mgf->Input_IsKeyClick(PSP_CTRL_CROSS)) {
		return true;
	}
	
	return false;
}

void CMessageBoxWidget::Render(void) {
	if(Res && Res->font) {
	  if(Res->sprMsg) Res->sprMsg->Render(0, 0);
		Res->font->SetScale(Res->fontScale);
		Res->font->SetColor(Res->fontColor);
		int ln = Res->font->GetLinesCount(Text);
		Res->font->Render(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - ln * Res->font->GetHeight() * 0.5f, MGFFONT_CENTER, Text);
	}
}
  

#include "objectmenuwidget.h"

#include "app.h"

CObjectMenuWidget::CObjectMenuWidget(int id) : CActionsWidget(id) {
	X = 10;
  Y = 10;
  Width = SCREEN_WIDTH - 20;
  Height = SCREEN_HEIGHT - 20;
}

CObjectMenuWidget::~CObjectMenuWidget(void) {
};

void CObjectMenuWidget::OnShow(void) {
  CActionsWidget::OnShow();
  isAct = false;
}

bool CObjectMenuWidget::Update(float delta) {
  //mgf->System_Log("start(%d)\n", (int)isAct);
	bool res = CActionsWidget::Update(delta);
  
  if(mgf->Input_IsKeyClick(PSP_CTRL_CROSS) || isAct) {
	  //mgf->System_Log("finish(%d)\n", (int)isAct);
		//App->Hide(W_OBJECTMENU);
	  return true;
	}
	
	//mgf->System_Log("upd(%d)\n", (int)res);
  
  return res;
}

void CObjectMenuWidget::Render(void) {
  if(Res && Res->sprMenu) Res->sprMenu->Render(0, 0);
  CActionsWidget::Render();
}

void CObjectMenuWidget::OnActiveItem(void) {
  QSPSelectMenuItem(Sel);
  //App->Hide(W_OBJECTMENU);
  App->isNowUpdate = true;
  isAct = true;
}

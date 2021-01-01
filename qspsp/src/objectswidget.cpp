#include "objectswidget.h"

#include "app.h"

CObjectsWidget::CObjectsWidget(int id) : CActionsWidget(id) {
	X = 321;
	Y = 10;
	Width = 150;
	Height = 122;
}

CObjectsWidget::~CObjectsWidget(void) {
};

void CObjectsWidget::OnActiveItem(void) {
  mgf->System_Log("\tselect object(%d)\n", Sel);
  if(!QSPSetSelObjectIndex(Sel, QSP_TRUE)) App->ShowError();
  App->isNowUpdate = true;
}


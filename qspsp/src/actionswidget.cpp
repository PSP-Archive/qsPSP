#include "actionswidget.h"

#include "app.h"
#include <stdio.h>

CActionsWidget::CActionsWidget(int id) : CWidget(id) {
  App = (CApp*)mgf->System_GetState(MGF_APP);
  X = 10;
  Y = 194;
	Width = 300;
	Height = 68;
  for(int i = 0; i < ACTIONSWIDGET_MAX; i++) Item[i] = 0;
	Count = 0;
	Sel = -1;
	Cur = 0;
	Scroll = 0.f;
	Res = 0;
	
	//mgf->System_Log("id=%d, width=%d\n", WidgetId, (int)Width);
};

CActionsWidget::~CActionsWidget(void) {
  Clear(true);
}
  
void CActionsWidget::OnRegister(void) {
	Res = (CRes*)mgf->System_GetState(MGF_USERDATA);
	if(!Res) {
	  mgf->System_Log("[%d] not res...", WidgetId);
	}
}

void CActionsWidget::OnShow(void) {
}

void CActionsWidget::OnHide(void) {

}

void CActionsWidget::OnActive(void) {

}
  
bool CActionsWidget::Update(float delta) {
  int oldCur = Cur;
  if(mgf->Input_IsKeyClick(PSP_CTRL_DOWN)) {
	  Cur++;
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_UP)) {
	  Cur--;
	}
	if(Cur >= Count) Cur = 0;
	if(Cur < 0) Cur = Count-1;
	
	if(oldCur != Cur && Cur != -1 && Count > 0) {
	  OnSelectItem(true);
	  
	  if(Res && Res->font) {
		  Res->font->SetScale(Res->fontScale);
		  float h = Res->font->GetHeight();
		  Scroll = 0;
			for(int i = 0; i < Cur; i++)
	  	  Scroll += Item[i]->lines * h;
	  	Scroll += Item[Cur]->lines * h * 0.5f;
	  }
	}
	
	if(mgf->Input_IsKeyClick(PSP_CTRL_CIRCLE) && Cur != -1) {
	  Sel = Cur;
	  OnActiveItem();
	}
	return false;
}

void CActionsWidget::Render(void) {
  if(Count <= 0) return;
  
	int line = 0;
  if(Res && Res->font) {
	  mgf->Gfx_SetClipping(X, Y, Width, Height);
		Res->font->SetColor(Res->fontColor);
	  Res->font->SetScale(Res->fontScale);
	  float h = Res->font->GetHeight();
	  for(int i = 0; i < Count; i++) {
	    if(i == Cur) Res->font->SetColor(Res->selColor);
	    //if(i == Sel) Res->font->SetColor(Res->ramColor);
			//Res->font->Render(X, Y + h * line - (float)((int)Scroll) * Res->fontScale, MGFFONT_LEFT, Item[i]->str);
			float offx = 0.f;
			if(Item[i]->align == MGFFONT_CENTER) offx = (int)(Width * 0.5f);
			else if(Item[i]->align == MGFFONT_RIGHT) offx = (int)Width;
			if(i == Sel) offx += 3;
			Res->font->RenderInBox((int)(X + offx), (int)(Y + h * line + Height * 0.5f - Scroll), X-1, Y-1, Width+2, Height+2, Item[i]->align, Item[i]->str);
	    line += Item[i]->lines;
	    if(i == Cur) Res->font->SetColor(Res->fontColor);
	  }
	  mgf->Gfx_SetClipping();
	}
}

void CActionsWidget::Clear(int scroll) {
  for(int i = 0; i < Count; i++) {
	  if(Item[i]) {
	    if(Item[i]->str) delete[] Item[i]->str;
	    if(Item[i]->tex) mgf->Texture_Remove(Item[i]->tex);
		  delete Item[i];
		}
	  Item[i] = 0;
	}
	Count = 0;
  if(scroll != SCROLL_NULL) {
	  Scroll = 0;
	  Sel = -1;
	  Cur = 0;
	}
}

void CActionsWidget::Add(const char *img, const char *str, bool ishtml, int align, int param) {
  if(Count >= ACTIONSWIDGET_MAX) return;
  if(!str || !*str) return;
  Item[Count] = new CActionItem();
  
  Item[Count]->str = new char[strlen(str) + 1];
  strcpy(Item[Count]->str, str);
  Item[Count]->align = align;
  Item[Count]->param = param;
  
  // удаляем HTML-теги
  if(ishtml) App->DelHtml(Item[Count]->str);
  
  if(Res && Res->font) {
    Res->font->SetScale(Res->fontScale);
    // форматируем переносы строк
    Res->font->CreateWidth(Width, Item[Count]->str);
    Item[Count]->lines = Res->font->GetLinesCount(Item[Count]->str);
    //mgf->System_Log("> %d:%d, %s : %d\n", WidgetId, (int)Width, Item[Count]->str, Item[Count]->lines);
  } else {
	  mgf->System_Log("[%d] res error.\n", WidgetId);
	}
  
	//mgf->System_Log("%0.2d. [%d]%s\n", Count, WidgetId, Item[Count]->str);
  
  Count++;
  if(Sel == -1) {
	  Sel = Cur = 0;
	  if(Res && Res->font) {
		  Res->font->SetScale(Res->fontScale);
		  float h = Res->font->GetHeight();
	  	Scroll = Item[Sel]->lines * h * 0.5f;
	  }
	}
}

void CActionsWidget::SetSelection(int sel) {
  if(sel < 0 || sel >= Count) return;
  int oldSel = Sel;
	Sel = Cur = sel;
	if(Res && Res->font) {
	  Res->font->SetScale(Res->fontScale);
	  float h = Res->font->GetHeight();
	  Scroll = 0;
		for(int i = 0; i < Sel; i++)
  	  Scroll += Item[i]->lines * h;
  	Scroll += Item[Sel]->lines * h * 0.5f;
  }
  OnSelectItem(oldSel != Sel);
}

char *CActionsWidget::GetText(int id) {
  if(id == -1) id = Sel;
  if(id < 0 || id >= Count || !Item[id]) return 0;
  return Item[id]->str;
}

void CActionsWidget::OnSelectItem(bool new_sel) {
  if(new_sel) {
    Sel = Cur;
	  if(!QSPSetSelActionIndex(Sel, QSP_FALSE)) App->ShowError();
	}
}

void CActionsWidget::OnActiveItem(void) {
  if(!QSPSetSelActionIndex(Sel, QSP_TRUE)) App->ShowError();
  App->isNowUpdate = true;
  if(!QSPExecuteSelActionCode(QSP_TRUE)) App->ShowError();
  App->MainWidget->isLeft = true; //App->MainDescWidget;
}


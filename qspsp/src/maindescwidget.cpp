#include "maindescwidget.h"

#include "app.h"
#include <stdio.h>

CMainDescWidget::CMainDescWidget(int id) : CWidget(id) {
  App = (CApp*)mgf->System_GetState(MGF_APP);
  X = 10;
  Y = 10;
  Width = 300;
  Height = 175;
	Text = 0;
	Scroll = 0.f;
	ScrollHeight = 0.f;
	cntBuf = 0;
};

CMainDescWidget::~CMainDescWidget(void) {
  if(Text && *Text) delete Text;
}

void CMainDescWidget::OnRegister(void) {
  Res = (CRes*)mgf->System_GetState(MGF_USERDATA);
}

void CMainDescWidget::OnShow(void) {

}

void CMainDescWidget::OnHide(void) {

}

void CMainDescWidget::OnActive(void) {

}
  
bool CMainDescWidget::Update(float delta) {
  /*if(mgf->Input_GetKeyState(PSP_CTRL_DOWN)) {
	  Scroll += delta * MAINDESCWIDGET_SPEED;
	}
	if(mgf->Input_GetKeyState(PSP_CTRL_UP)) {
	  Scroll -= delta * MAINDESCWIDGET_SPEED;
	}*/
	int ax, ay;
	mgf->Input_GetAnalog(&ax, &ay);
	if(!(ay < 16 && ay > -16)) {
	  Scroll += delta * MAINDESCWIDGET_SPEED * (float)ay / 128.f;
	}
	
	if(Scroll < 0.f) Scroll = 0.f;
	if(Scroll > ScrollHeight) Scroll = ScrollHeight;
	
	//mgf->System_Log("Update-id-%d\n", WidgetId);
}

void CMainDescWidget::Render(void) {
  if(Res && Res->font && Text && *Text) {
	  mgf->Gfx_SetClipping(X, Y, Width, Height);
		Res->font->SetColor(Res->fontColor);
	  Res->font->SetScale(Res->fontScale);
	  Res->font->RenderInBox(X, Y - (float)((int)Scroll) * Res->fontScale, X-1, Y-1, Width+2, Height+2, MGFFONT_LEFT, Text);
	  mgf->Gfx_SetClipping();
	}
	//mgf->System_Log("Render-Id-%d\n", WidgetId);
}

void CMainDescWidget::SetText(const char *txt, int scroll, bool html) {
	if(!txt || !*txt) {
    if(Text && *Text) Text[0] = '\0';
	  return;
	}
	
	if(scroll == SCROLL_UP) Scroll = 0.f;
  
  int c = strlen(txt) + 1;
  if(c > cntBuf) {
    //if(c > 1024) c = 1024;
    cntBuf = c + 64;
    if(Text) {
	    delete[] Text;
	  }
    Text = new char[cntBuf];
  }
  strncpy(Text, txt, cntBuf-1);
  Text[c-1] = '\0';


  // удаляем HTML-теги
  if(html) App->DelHtml(Text);
  
  if(Res && Res->font && Text && *Text) {
    Res->font->SetScale(Res->fontScale);
    // форматируем переносы строк
    //int ls = font->GetLinesCount(Text);
    Res->font->CreateWidth(Width, Text);
    //int lines = font->GetLinesCount(Text);
    //mgf->System_Log("%d:%d, %d=%d\n", WidgetId, (int)Width, ls, lines);
    // Считаем высоту
	  ScrollHeight = Res->font->GetLinesCount(Text) * Res->font->GetHeight();
	  if(ScrollHeight > Height) ScrollHeight -= Height;// - Res->font->GetHeight();
	  else ScrollHeight = 0.f;
	  if(scroll == SCROLL_DOWN) Scroll = ScrollHeight;
	} else {
	  Height = 0.f;
	  Scroll = 0.f;
	}
	
	//mgf->System_Log("%s\n", Text);
	//mgf->System_Log("=%d\n", strlen(Text));
}


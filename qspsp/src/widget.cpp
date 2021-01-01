#include "widget.h"

/*   Widget   */

MGF *CWidget::mgf = 0;

CWidget::CWidget(int id) {
  mgf = mgfCreate();
  WidgetId = id;
  WidgetModal = false;
  X = 0;
  Y = 0;
  Width = SCREEN_WIDTH;
  Height = SCREEN_HEIGHT;
}

CWidget::~CWidget(void) {
  mgf->Release();
}

/*   Widget Engine   */

CWidgetEngine::CWidgetEngine(void) {
	widgetReg = new mhaList(false);
	widgetUpdate = new mhaList(false);
	widgetRender = new mhaList(false);
	RenderModal = -1;
}

CWidgetEngine::~CWidgetEngine(void) {	
	// delete widgets
	delete widgetReg;
	delete widgetUpdate;
	delete widgetRender;
}
	
bool CWidgetEngine::Register(CWidget *widget, int newid) {
	if(!widget) return false;
	if(newid > -1) widget->WidgetId = newid;
	mgf->System_Log("register(%d);\n", widget->WidgetId);
	widget->OnRegister();
	widgetReg->AddLast(widget);
	return true;
}

void CWidgetEngine::Clear(void) {
	// exec OnHide
	widgetUpdate->Begin();
	CWidget *widget = (CWidget*)widgetUpdate->Next();
	while(widget) {
		widget->OnHide();
		widget = (CWidget*)widgetUpdate->Next();
	}

	// clear lists
	widgetUpdate->Clear();
	widgetRender->Clear();
	RenderModal = -1;
}

CWidget *CWidgetEngine::Find(int id) {
	widgetReg->Begin();
	CWidget *widget = (CWidget*)widgetReg->Next();
	
	while(widget) {
		if(widget->WidgetId == id) {
			return widget;
		}
		widget = (CWidget*)widgetReg->Next();
	}
	return 0;
}


void CWidgetEngine::Show(int id, bool modal, bool rmodal) {
	CWidget *widget = Find(id);
	if(!widget) return;
	widget->WidgetModal = modal;
	widgetUpdate->AddFirst(widget);
	widgetRender->AddFirst(widget);
	if(rmodal) RenderModal = id;
	//mgf->System_Log("show(%d);\n", widget->WidgetId);
	widget->OnActive();
	widget->OnShow();
}

void CWidgetEngine::Hide(int id) {
	CWidget *widget = Find(id);
	if(!widget) return;
	widget->OnHide();
	if(widget->WidgetModal) {
		widgetUpdate->SetCurr(widget);
		widgetUpdate->Next();
		CWidget *w = (CWidget*)widgetUpdate->Next();
		while(w) {
			w->OnActive();
			if(w->WidgetModal) break;
			w = (CWidget*)widgetUpdate->Next();
		}
	}
	//mgf->System_Log("hide(%d);\n", widget->WidgetId);
	widgetUpdate->Remove(widget);
	widgetRender->Remove(widget);
	if(RenderModal == id) RenderModal = -1;
}

bool CWidgetEngine::Update(void) {
	float delta = mgf->Timer_GetDelta();
	
	widgetUpdate->Begin();
	CWidget *widget = (CWidget*)widgetUpdate->Next();
	bool res = false;
	while(widget) {
		res = res || widget->Update(delta);
		if(widget->WidgetModal) return res;

		widget = (CWidget*)widgetUpdate->Next();
	}
	return res;
}

void CWidgetEngine::Render(void) {
	mgf->Gfx_BeginScene();
	_render();
	mgf->Gfx_EndScene();
}

void CWidgetEngine::_render(void) {
	widgetRender->End();
	CWidget *widget = (CWidget*)widgetRender->Prev();

	bool rndr = true;
	if(RenderModal != -1) rndr = false;
	while(widget) {
	  //mgf->System_Log("render(%d);\n", widget->WidgetId);
		if(!rndr && RenderModal != -1 && widget->WidgetId == RenderModal) rndr = true;
		if(rndr) {
		  widget->Render();
		}

		widget = (CWidget*)widgetRender->Prev();
	}
}




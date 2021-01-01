#include "widget.h"

/*   Widget   */

MGF *CWidget::mgf = 0;

CWidget::CWidget(int id) {
  mgf = mgfCreate();
	this->id = id;
  modal = false;
  x = 0;
  y = 0;
  width = SCREEN_WIDTH;
  height = SCREEN_HEIGHT;
}

CWidget::~CWidget(void) {
  mgf->Release();
}

/*   Widget Engine   */

CWidgetEngine::CWidgetEngine(void) {
	widget_reg = new mhaList(false);
	widget_update = new mhaList(false);
	widget_render = new mhaList(false);
	render_modal_id = -1;
}

CWidgetEngine::~CWidgetEngine(void) {	
	// delete widgets
	delete widget_reg;
	delete widget_update;
	delete widget_render;
}
	
bool CWidgetEngine::reg(CWidget *widget, int newid) {
	if(!widget) return false;
	if(newid > -1) widget->id = newid;
	mgf->System_Log("register(%d);\n", widget->id);
	widget->onRegister();
	widget_reg->addLast(widget);
	return true;
}

void CWidgetEngine::clear(void) {
	// exec OnHide
	widget_update->begin();
	CWidget *widget = (CWidget*)widget_update->next();
	while(widget) {
		widget->onHide();
		widget = (CWidget*)widget_update->next();
	}

	// clear lists
	widget_update->clear();
	widget_render->clear();
	render_modal_id = -1;
}

CWidget *CWidgetEngine::find(int id) {
	widget_reg->begin();
	CWidget *widget = (CWidget*)widget_reg->next();
	
	while(widget) {
		if(widget->id == id) {
			return widget;
		}
		widget = (CWidget*)widget_reg->next();
	}
	return 0;
}


void CWidgetEngine::show(int id, bool modal, bool rmodal) {
	CWidget *widget = find(id);
	if(!widget) return;
	widget->modal = modal;
	widget_update->addFirst(widget);
	widget_render->addFirst(widget);
	if(rmodal) render_modal_id = id;
	//mgf->System_Log("show(%d);\n", widget->WidgetId);
	widget->onActive();
	widget->onShow();
}

void CWidgetEngine::hide(int id) {
	CWidget *widget = find(id);
	if(!widget) return;
	widget->onHide();
	if(widget->modal) {
		widget_update->setCurr(widget);
		widget_update->next();
		CWidget *w = (CWidget*)widget_update->next();
		while(w) {
			w->onActive();
			if(w->modal) break;
			w = (CWidget*)widget_update->next();
		}
	}
	//mgf->System_Log("hide(%d);\n", widget->WidgetId);
	widget_update->remove(widget);
	widget_render->remove(widget);
	if(render_modal_id == id) render_modal_id = -1;
}

bool CWidgetEngine::update(void) {
	float delta = mgf->Timer_GetDelta();
	
	widget_update->begin();
	CWidget *widget = (CWidget*)widget_update->next();
	bool res = false;
	while(widget) {
		res = res || widget->update(delta);
		if(widget->modal) return res;

		widget = (CWidget*)widget_update->next();
	}
	return res;
}

void CWidgetEngine::render(void) {
	mgf->Gfx_BeginScene();
	_render();
	mgf->Gfx_EndScene();
}

void CWidgetEngine::_render(void) {
	widget_render->end();
	CWidget *widget = (CWidget*)widget_render->prev();

	bool rndr = true;
	if(render_modal_id != -1) rndr = false;
	while(widget) {
	  //mgf->System_Log("render(%d);\n", widget->WidgetId);
		if(!rndr && render_modal_id != -1 && widget->id == render_modal_id) rndr = true;
		if(rndr) {
		  widget->render();
		}

		widget = (CWidget*)widget_render->prev();
	}
}

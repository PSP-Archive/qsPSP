#include "inputboxwidget.h"

#include "app.h"

static char *Symbols[2] = {"ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÜÛÚİŞßàáâãäåæçèéêëìíîïğñòóôúö÷øùüûúışÿ_+-[]{}*=,.0123456789 <?",
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz;\"'^&%$#@!()_+-[]{}*=,.0123456789 <?"};

CInputBoxWidget::CInputBoxWidget(int id) : CWidget(id) {
	App = (CApp*)mgf->System_GetState(MGF_APP);
	Lang = 0;
}

CInputBoxWidget::~CInputBoxWidget(void) {
}

void CInputBoxWidget::OnRegister(void) {
	Res = (CRes*)mgf->System_GetState(MGF_USERDATA);
	if(!Res) mgf->System_Log("InputBoxWidget-OnRegister: res error...\n");
	Text[0] = '\0';
	cur = 0;
	sX = sY = 0;
	Mode = IBS_NOTHREAD;
}

void CInputBoxWidget::OnShow(void) {
  Res = (CRes*)mgf->System_GetState(MGF_USERDATA);
	if(!Res) mgf->System_Log("InputBoxWidget-OnShow: res error...\n");
}
  
bool CInputBoxWidget::Update(float delta) {
  //mgf->System_Log("ibw-upd.\n");
	if(mgf->Input_IsKeyClick(PSP_CTRL_CROSS)) {
	  cur = 0;
		Text[0] = '\0';
		if(Mode == IBS_NOTHREAD)
  		App->Hide(W_INPUTBOX);
  	else if(Mode == IBS_THREAD)
  	  return true;
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_START)) {
	  if(Mode == IBS_NOTHREAD) {
	    if(!QSPExecUserInput(QSP_TRUE))
				App->ShowError();
			App->isNowUpdate = true;
  		App->Hide(W_INPUTBOX);
  	} else if(Mode == IBS_THREAD) {
  	  App->isNowUpdate = true;
  	  return true;
  	}
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_TRIANGLE)) {
	  Lang++;
	  if(Lang > 1) Lang = 0;
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_CIRCLE)) {
	  if(isMsg) {
		  cur = 0;
		  Text[0] = '\0';
		  isMsg = false;
		}
		int num = sY * 8 + sX;
		if(Symbols[Lang][num] == '?') {
		  if(Mode == IBS_NOTHREAD) {
		    if(!QSPExecUserInput(QSP_TRUE))
					App->ShowError();
				App->isNowUpdate = true;
	  		App->Hide(W_INPUTBOX);
	  	} else if(Mode == IBS_THREAD) {
	  	  App->isNowUpdate = true;
	  	  return true;
	  	}
		} else if(Symbols[Lang][num] == '<') {
		  cur = 0;
		  Text[0] = '\0';
		} else {
			if(cur < IB_TEXTMAX) {
				Text[cur] = Symbols[Lang][num];
				cur++;
				Text[cur] = '\0';
				QSPSetInputStrText(Text);
			}
		}
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_SQUARE)) { // delete char
		if(isMsg) {
		  cur = 0;
		  Text[0] = '\0';
		  isMsg = false;
		}
		if(cur > 0) {
			cur--;
			Text[cur] = '\0';
		}
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_RIGHT)) {
	  sX++;
	  if(sX > 7) sX = 0;
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_LEFT)) {
	  sX--;
	  if(sX < 0) sX = 7;
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_DOWN)) {
	  sY++;
	  if(sY > 10) sY = 0;
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_UP)) {
	  sY--;
	  if(sY < 0) sY = 10;
	}
	return false;
}

void CInputBoxWidget::Render(void) {
  //mgf->System_Log("ibw-rnd.\n");
	if(Res && Res->font) {
	  if(Res->sprInput) Res->sprInput->Render(0, 0);
		Res->font->SetScale(Res->fontScale);
		Res->font->SetColor(Res->fontColor);
		//int ln = Res->font->GetLinesCount(Text);
		Res->font->Render(SCREEN_WIDTH * 0.5f, 20, MGFFONT_CENTER, Text);
		
		char *s = Symbols[Lang];
		char out[4];
		int x = 0, y = 0;
		while(*s) {
			if(x == sX && y == sY) Res->font->SetColor(Res->selColor);
			else Res->font->SetColor(Res->fontColor);
			out[0] = *s;
			out[1] = '\0';
			out[2] = '\0';
			out[3] = '\0';
			if(*out == ' ') strcpy(out, "SP");
			else if(*out == '?') strcpy(out, "OK");
			else if(*out == '<') strcpy(out, "CLR");
			Res->font->Render(SCREEN_WIDTH * 0.5f - 32 * 4 + x * 32 + 16, 80 + y * 16, MGFFONT_CENTER, out);
			x++;
			if(x > 7) {
				x = 0;
				y++;
			}
			s++;
			//mgf->System_Log("%c", (unsigned char)*s);
		}
		//mgf->System_Log("\n");
	}
}

void CInputBoxWidget::SetText(const char *txt) {
	if(!txt || !*txt) {
		Text[0] = '\0';
		cur = 0;
		return;
	}
	
	strncpy(Text, txt, IB_TEXTMAX-1);
	Text[IB_TEXTMAX-1] = '\0';
	cur = strlen(Text);
}
  

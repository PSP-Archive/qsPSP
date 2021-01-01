#include "mainwidget.h"

#include "app.h"
#include <stdio.h>

CMainWidget::CMainWidget(int id) : CWidget(id) {
  App = (CApp*)mgf->System_GetState(MGF_APP);
};

CMainWidget::~CMainWidget(void) {
}

void CMainWidget::OnRegister(void) {
  Res = (CRes*)mgf->System_GetState(MGF_USERDATA);
}

void CMainWidget::OnShow(void) {
	GameName[0] = '\0';
  isRTrig = false;
  isLeft = true;
}

void CMainWidget::OnHide(void) {

}

void CMainWidget::OnActive(void) {

}
  
bool CMainWidget::Update(float delta) {
	// update qsp timer
  if(App->qspSleepTimer > 0.f) {
    App->qspSleepTimer -= delta;
    if(App->qspSleepTimer < 0.f) {
		  App->qspSleepTimer = 0.f;
		  App->isNowUpdate = true;
		  return true;
		} else
		  return false;
  }
  if(App->qspTimer != -1.f) {
	  App->qspTime += delta;
	  if(App->qspTime >= App->qspTimer) {
		  App->qspTime -= App->qspTimer;
		  if(!QSPExecCounter(QSP_TRUE)) {
			  App->ShowError();
			}
		}
	}
	App->MSCount += delta;
	
	if(mgf->Input_IsKeyClick(PSP_CTRL_SELECT)) {
		ShowGameBrowser();
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_START)) {
    ShowSaveBrowser();
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_TRIANGLE)) {
	  App->InputBoxWidget->SetMode(IBS_NOTHREAD);
		App->Show(W_INPUTBOX, true, true);
	}
	
	if(mgf->Input_IsKeyClick(PSP_CTRL_LTRIGGER)) isLeft = !isLeft;
	isRTrig = mgf->Input_GetKeyState(PSP_CTRL_RTRIGGER);
	
	if((isLeft && !isRTrig) || (!isLeft && isRTrig)) {
	  App->MainDescWidget->Update(delta);
	  App->ActionsWidget->Update(delta);
	} else {
	  App->VarsDescWidget->Update(delta);
	  App->ObjectsWidget->Update(delta);
	}
	
	return false;
}

void CMainWidget::Render(void) {
	//Left->Render();
	//Right->Render();
	if(Res->sprMain) Res->sprMain->Render(0, 0);
	App->MainDescWidget->Render();
	App->ActionsWidget->Render();
	App->VarsDescWidget->Render();
	App->ObjectsWidget->Render();
	if((isLeft && !isRTrig) || (!isLeft && isRTrig)) {
	  mgf->Gfx_RenderLine(5, 5, 315, 5, Res->ramColor); mgf->Gfx_RenderLine(4, 4, 316, 4, Res->ramColor);
	  mgf->Gfx_RenderLine(5, 5, 5, SCREEN_HEIGHT-5, Res->ramColor); mgf->Gfx_RenderLine(4, 4, 4, SCREEN_HEIGHT-4, Res->ramColor);
	  mgf->Gfx_RenderLine(5, SCREEN_HEIGHT-5, 315, SCREEN_HEIGHT-5, Res->ramColor); mgf->Gfx_RenderLine(4, SCREEN_HEIGHT-4, 316, SCREEN_HEIGHT-4, Res->ramColor);
	  mgf->Gfx_RenderLine(315, 5, 315, SCREEN_HEIGHT-5, Res->ramColor); mgf->Gfx_RenderLine(316, 4, 316, SCREEN_HEIGHT-4, Res->ramColor);
	} else {
	  mgf->Gfx_RenderLine(315, 5, SCREEN_WIDTH-5, 5, Res->ramColor); mgf->Gfx_RenderLine(316, 4, SCREEN_WIDTH-4, 4, Res->ramColor);
	  mgf->Gfx_RenderLine(315, 5, 315, SCREEN_HEIGHT-5, Res->ramColor); mgf->Gfx_RenderLine(316, 4, 316, SCREEN_HEIGHT-4, Res->ramColor);
	  mgf->Gfx_RenderLine(315, SCREEN_HEIGHT-5, SCREEN_WIDTH-5, SCREEN_HEIGHT-5, Res->ramColor); mgf->Gfx_RenderLine(316, SCREEN_HEIGHT-4, SCREEN_WIDTH-4, SCREEN_HEIGHT-4, Res->ramColor);
	  mgf->Gfx_RenderLine(SCREEN_WIDTH-5, 5, SCREEN_WIDTH-5, SCREEN_HEIGHT-5, Res->ramColor); mgf->Gfx_RenderLine(SCREEN_WIDTH-4, 4, SCREEN_WIDTH-4, SCREEN_HEIGHT-4, Res->ramColor);
	}
}

void CMainWidget::ShowGameBrowser(void) {
  App->AppDialog->SetWidget(App->GameBrowserWidget);
  if(mgf->System_Run(App->AppDialog) == GBM_OK) {
	  App->MainDescWidget->SetText(0, true, false);
    App->VarsDescWidget->SetText(0, true, false);
    App->ActionsWidget->Clear(true);
    App->ObjectsWidget->Clear(true);
		strncpy(GameName, App->GameBrowserWidget->GetText(), MGF_MAX_PATH-1);
		strncpy(GameDir, App->GameBrowserWidget->GetDir(), MGF_MAX_PATH-1);
		char *fn = App->GameBrowserWidget->GetFileName();
    mgf->System_Log("Open game '%s'.\n", fn);
		Res->Audio->Stop();
		App->SetSkin(0);
		Res->fontColor = 0xFFFFFFFF;
    Res->bgColor = 0xFF000000;
    Res->selColor = 0xFFFF0000;
	  Res->ramColor = 0xFF0000FF;
		if(QSPLoadGameWorld(fn)) {
		  App->isNowUpdate = true;
      QSPRestartGame(QSP_TRUE);
    } else {
		  App->ShowError();
		}
	}
}

void CMainWidget::ShowSaveBrowser(int state) {
  if(GameName[0] != '\0') {
    App->SaveBrowserWidget->Set(GameName, state);
    App->AppDialog->SetWidget(App->SaveBrowserWidget);
    mgf->System_Run(App->AppDialog);
  }
}


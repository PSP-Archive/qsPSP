#include "app.h"

#include <mgfutils.h>

CApp *CApp::app = 0;

CApp::CApp(void) : CWidgetEngine() {
	app = this;
	mgf->System_SetState(MGF_DEBUG, false);
	mgf->System_SetState(MGF_VSYNC, true);
	
	mgf->System_Log("wchar_t -> %d\n", sizeof(wchar_t));
	
	is_scroll = false;
	is_can_save = true;
	is_html = false;
	
	is_now_update = false;
	
	is_init = false;
}

CApp::~CApp(void) {
  /*delete MainWidget;
  delete MainDescWidget;
  delete VarsDescWidget;
  delete ActionsWidget;
  delete ObjectsWidget;
  delete ObjectMenuWidget;
  delete MessageBoxWidget;
  delete InputBoxWidget;
  delete PicViewWidget;
  delete GameBrowserWidget;
  delete SaveBrowserWidget;
  
  delete AppDialog;*/
  
	clearSkin();
	delete skin;

	delete audio;
	
	QSPDeInit();
}

void CApp::init(void) {
  mgf->Gfx_SetProjection(MGF_ORTHO);
	
	if(!is_init) {
	  skin = new CSkin();
	  setSkin(0);
	  
		audio = new MGFAUDIO(1, 1);
	  
	  mgf->System_SetState(MGF_USERDATA, (void*)skin);
	  
	  /*MainWidget = new CMainWidget(W_MAIN);
	  Register(MainWidget);
	  MainDescWidget = new CMainDescWidget(W_MAINDESC);
	  Register(MainDescWidget);
	  VarsDescWidget = new CVarsDescWidget(W_VARSDESC);
	  Register(VarsDescWidget);
	  ActionsWidget = new CActionsWidget(W_ACTIONS);
	  Register(ActionsWidget);
	  ObjectsWidget = new CObjectsWidget(W_OBJECTS);
	  Register(ObjectsWidget);
	  ObjectMenuWidget = new CObjectMenuWidget(W_OBJECTMENU);
	  Register(ObjectMenuWidget);
	  MessageBoxWidget = new CMessageBoxWidget(W_MESSAGEBOX);
	  Register(MessageBoxWidget);
	  InputBoxWidget = new CInputBoxWidget(W_INPUTBOX);
	  Register(InputBoxWidget);
	  PicViewWidget = new CPicViewWidget(W_PICVIEW);
	  Register(PicViewWidget);
	  GameBrowserWidget = new CGameBrowserWidget(W_GAMEBROWSER);
	  Register(GameBrowserWidget);
	  SaveBrowserWidget = new CSaveBrowserWidget(W_SAVEBROWSER);
	  Register(SaveBrowserWidget);
	  
	  AppDialog = new CAppDialog(this);*/
  }
  
  clear();
  //show(W_MAIN);
  
  /*Left = MainDescWidget;
  Right = ObjectsWidget;
  Active = &Left;*/
  
  qsp_time = qsp_timer = qsp_sleep_timer = 0.f;
  QSPInit();
  //initCallBacks();
  QSPLoadGameWorld("./help.qsp");
  is_now_update = true;
  QSPRestartGame(QSP_TRUE);
}

bool CApp::update(void) {
	float delta = mgf->Timer_GetDelta();
	//mgf->System_Log("\t%f\t%d\n", delta, mgf->Timer_GetFps());

  // update qsp timer
  /*if(qspTimer != -1.f) {
	  qspTime += delta;
	  if(qspTime >= qspTimer) {
		  qspTime -= qspTimer;
		  if(!QSPExecCounter(QSP_TRUE)) {
			  ShowError();
			}
		}
	}
	MSCount += delta;*/
	
	//if(mgf->Input_IsKeyClick(PSP_CTRL_SELECT)) {
		//Show(W_GAMEBROWSER, true, true);
	  
		/*int res = mgf->System_Run(AppGameManager);
	  if(res == MSG_OK) {
	    MainWidget->SetText(0, true);
	    VarsWidget->SetText(0, true);
	    ActionWidget->Clear(true);
	    ObjectWidget->Clear(true);
	    char *fn = AppGameManager->GetFilename();
	    mgf->System_Log("OK - Open game '%s'.\n", fn);
			QSPLoadGameWorld(fn);
			isNowUpdate = true;
      QSPRestartGame(QSP_TRUE);
		} else {
		  mgf->System_Log("CANCEL.\n", res);
		}*/
	//}
	
	//if(mgf->Input_IsKeyClick(PSP_CTRL_TRIANGLE)) {
	  /*MainWidget->SetText(0, true);
    VarsWidget->SetText(0, true);
    ActionWidget->Clear(true);
    ObjectWidget->Clear(true);
		QSPLoadGameWorld("./help.gam");
    QSPRestartGame(QSP_TRUE);*/
    //isNowUpdate = true;
    //RefreshInt(QSP_TRUE);
	//}
	
	/*if(mgf->Input_IsKeyClick(PSP_CTRL_RTRIGGER)) {
	  if(Right == ObjectsWidget) {
		  Right = VarsDescWidget;
		} else {
		  Right = ObjectsWidget;
		}
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_LTRIGGER)) {
	  if(Left == MainDescWidget) {
		  Left = ActionsWidget;
		} else {
		  Left = MainDescWidget;
		}
	}
	if(mgf->Input_IsKeyClick(PSP_CTRL_SQUARE)) {
	  if(Active == &Left) {
		  Active = &Right;
		} else {
		  Active = &Left;
		}
	}*/
	/*if(mgf->Input_IsKeyClick(PSP_CTRL_START)) {
	  MainDescWidget->SetText(0, true);
	  VarsDescWidget->SetText(0, true);
	  ActionsWidget->Clear(true);
	  ObjectsWidget->Clear(true);
	  isNowUpdate = true;
	  QSPRestartGame(QSP_TRUE);
	}*/
	/*if(mgf->Input_IsKeyClick(PSP_CTRL_CIRCLE)) {
	  int rnd = mgf->Random_Int(0, 3);
	  mgf->System_Log("Set action %d.\n", rnd);
	  if(!QSPSetSelActionIndex(rnd, QSP_TRUE)) ShowError();
	  if(!QSPExecuteSelActionCode(QSP_TRUE)) ShowError(); 
	}*/
	
	//if(Active && *Active) (*Active)->Update(delta);

	sprintf(str, "fps: %d", mgf->Timer_GetFps());
	
	return CWidgetEngine::update();
}

void CApp::render(void) {
	mgf->Gfx_BeginScene();
	// -----	
	mgf->Texture_Enable(true);
	mgf->Gfx_Clear(0xFF000000);
	// -
	mgf->Gfx_SetBlend(MGFBLEND_SUB);
	
	/*Res.font->SetScale(0.7f);
	Res.font->SetColor(0xFFFFFFFF);
	Res.font->SetAngle(0.f);
	Res.font->Render(0, -1, MGFFONT_LEFT, str);
	Res.font->Render(0, -1, MGFFONT_LEFT, str);*/
	
	/*Left->Render();
	Right->Render();*/
	//if(Active && *Active) (*Active)->Render();
	
	_render();

	mgf->Gfx_EndScene();
}

void CApp::showError(void) {
	QSP_CHAR *loc;
	QSP_CHAR *desc;
	long code, where, line;
	QSPGetLastErrorData(&code, &loc, &where, &line);
	desc = (QSP_CHAR*)QSPGetErrorDesc(code);
  
  mgf->System_Log("QSP Error!\n");
  QSP_CHAR str[1024];
  if(loc) {
    sprintf(str, "  Location: %s\n  Area: %s\n  Line: %ld\n  Code: %ld\n  Desc: %s", loc, where < 0 ? "on visit" : "on action", line,	code, desc);
  } else {
    sprintf(str, "  Code: %ld\n  Desc: %s", code, desc);
	}
	mgf->System_Log(str);
	//Msg(str);
}

void CApp::clearSkin(void) {
	if(skin->name && *skin->name) delete skin->name;
	if(skin->font) {
		delete skin->font;
		mgf->Texture_Remove(skin->tex);
		delete skin->spr_0_0;
		delete skin->spr_1_0;
		delete skin->spr_2_0;
		delete skin->spr_0_1;
		delete skin->spr_1_1;
		delete skin->spr_2_1;
		delete skin->spr_0_2;
		delete skin->spr_1_2;
		delete skin->spr_2_2;
	}
	
	skin->name = 0;
	skin->font = 0;
	skin->font_color = 0xFFFFFFFF;
  skin->font_color_select = 0xFFFF0000;
  skin->font_color_active = 0xFF0000FF;
	skin->tex = 0;
	skin->spr_0_0 = skin->spr_1_0 = skin->spr_2_0 = skin->spr_0_1 = skin->spr_1_1 = skin->spr_2_1 = skin->spr_0_2 = skin->spr_1_2 = skin->spr_2_2 = 0;
}

bool CApp::setSkin(const char *name) {
	if(name && *name && skin->name && *skin->name && !strcmp(skin->name, name)) return false;
	if((!name || !*name) && (!skin->name || !*skin->name) && skin->font) return false;
	
	clearSkin();
	if(!name || !*name) {
	  mgf->System_Log("Set default skin.\n");
		skin->font = new MGFFONT("data/default.fb");
	  skin->tex = mgf->Texture_Add("data/default.jpg");
		// sprites...
	} else {
	  mgf->System_Log("Set '%s' skin.\n", name);
		char p[128];
		skin->name = new char[strlen(name) + 1];
		strcpy(skin->name, name);
		sprintf(p, "%s%s.fb", /*main_widget->getGameDir()*/"games/", skin->name);
		skin->font = new MGFFONT(p);
		sprintf(p, "%s%s.jpg", /*main_widget->getGameDir()*/"games/", skin->name);
		skin->tex = mgf->Texture_Add(p);
	  // sprites...
	}
	return true;
}

/*bool CApp::getVarValue(QSP_CHAR *name, long *num, QSP_CHAR **str) {
	if (QSPGetVarValuesCount(name, num) && *num) {
		QSPGetVarValues(name, 0, num, str);
		return true;
	}
	return false;
}


void CApp::initCallBacks(void) {
  QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&qsp_SetTimer);
	QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)&qsp_RefreshInt);
	QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&qsp_SetInputStrText);
	QSPSetCallBack(QSP_CALL_ISPLAYINGFILE, (QSP_CALLBACK)&qsp_IsPlay);
	QSPSetCallBack(QSP_CALL_PLAYFILE, (QSP_CALLBACK)&qsp_PlayFile);
	QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&qsp_CloseFile);
	QSPSetCallBack(QSP_CALL_SHOWMSGSTR, (QSP_CALLBACK)&qsp_Msg);
	QSPSetCallBack(QSP_CALL_SLEEP, (QSP_CALLBACK)&qsp_Sleep);
	QSPSetCallBack(QSP_CALL_GETMSCOUNT, (QSP_CALLBACK)&qsp_GetMSCount);
	QSPSetCallBack(QSP_CALL_DELETEMENU, (QSP_CALLBACK)&qsp_DeleteMenu);
	QSPSetCallBack(QSP_CALL_ADDMENUITEM, (QSP_CALLBACK)&qsp_AddMenuItem);
	QSPSetCallBack(QSP_CALL_SHOWMENU, (QSP_CALLBACK)&qsp_ShowMenu);
	QSPSetCallBack(QSP_CALL_INPUTBOX, (QSP_CALLBACK)&qsp_Input);
	QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&qsp_ShowImage);
	QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&qsp_ShowPane);
	QSPSetCallBack(QSP_CALL_OPENGAMESTATUS, (QSP_CALLBACK)&qsp_OpenGameStatus);
	QSPSetCallBack(QSP_CALL_SAVEGAMESTATUS, (QSP_CALLBACK)&qsp_SaveGameStatus);
}

void CApp::qsp_SetTimer(long msecs) {
	if(msecs) {
		App->qspTime = 0.f;
		App->qspTimer = (float)msecs / 1000.f;
	} else {
	  App->qspTime = 0.f;
		App->qspTimer = -1.f;
	}
}

void CApp::qsp_RefreshInt(QSP_BOOL isRedraw) {
	static long oldFullRefreshCount = 0;
	long i, numVal;
	QSP_CHAR *strVal, *imgPath;
	// -------------------------------
	//UpdateGamePath();
	// -------------------------------

	App->isNowUpdate = false;
	
	int mainScroll = CMainDescWidget::SCROLL_NULL, varsScroll = CMainDescWidget::SCROLL_NULL;

	// -------------------------------
	//App->isHtml = GetVarValue(QSP_FMT("USEHTML"), &numVal, &strVal) && numVal;
	// -------------------------------

	long fullRefreshCount = QSPGetFullRefreshCount();
	if(oldFullRefreshCount != fullRefreshCount) {
		oldFullRefreshCount = fullRefreshCount;
	}
	// -------------------------------
	if(QSPIsMainDescChanged()) {
		const QSP_CHAR *mainDesc = QSPGetMainDesc();
		App->MainDescWidget->SetText(mainDesc, mainScroll, App->isHtml);
	}
	if(QSPIsVarsDescChanged()) {
	  if(App->isScroll) varsScroll = CMainDescWidget::SCROLL_DOWN;
		const QSP_CHAR *varsDesc = QSPGetVarsDesc();
		App->VarsDescWidget->SetText(varsDesc, varsScroll, App->isHtml);
	}
	// -------------------------------
	int s = CActionsWidget::SCROLL_NULL;
	if(QSPIsActionsChanged()) s = CActionsWidget::SCROLL_UP;
	long actionsCount = QSPGetActionsCount();
	App->ActionsWidget->Clear(s);
	for (i = 0; i < actionsCount; ++i) {
		QSPGetActionData(i, &imgPath, &strVal);
		App->ActionsWidget->Add(imgPath, strVal, App->isHtml);
	}
	if(s == CActionsWidget::SCROLL_NULL) App->ActionsWidget->SetSelection(QSPGetSelActionIndex());
	
	s = CActionsWidget::SCROLL_NULL;
	if(QSPIsObjectsChanged()) s = CActionsWidget::SCROLL_UP;
	long objectsCount = QSPGetObjectsCount();
	App->ObjectsWidget->Clear(s);
	for (i = 0; i < objectsCount; ++i) {
		QSPGetObjectData(i, &imgPath, &strVal);
		App->ObjectsWidget->Add(imgPath, strVal, App->isHtml);
	}
	if(s == CActionsWidget::SCROLL_NULL) App->ObjectsWidget->SetSelection(QSPGetSelObjectIndex());
}

void CApp::SetInputStrText(QSP_CHAR *text) {
	App->InputBoxWidget->SetText(text);
}

QSP_BOOL CApp::IsPlay(QSP_CHAR *file) {
	return app->audio->Playing;
}

void CApp::CloseFile(QSP_CHAR *file) {
	app->audio->Stop();
}

void CApp::PlayFile(QSP_CHAR *file, long volume) {
	mgf->System_Log("Play '%s'.\n", file);
	app->audio->PlayFile(file);
}

void CApp::ShowPane(long type, QSP_BOOL isShow) {
	!!!switch (type)
	{
	case QSP_WIN_ACTS:
		m_frame->ShowPane(ID_ACTIONS, isShow != QSP_FALSE);
		break;
	case QSP_WIN_OBJS:
		m_frame->ShowPane(ID_OBJECTS, isShow != QSP_FALSE);
		break;
	case QSP_WIN_VARS:
		m_frame->ShowPane(ID_VARSDESC, isShow != QSP_FALSE);
		break;
	case QSP_WIN_INPUT:
		m_frame->ShowPane(ID_INPUT, isShow != QSP_FALSE);
		break;
	}
}

void CApp::Sleep(long msecs) {
	App->qspSleepTimer = (float)msecs * 0.001f;
	//App->AppDialog->SetWidget(App->MainWidget, false);
	mgf->System_Run(App, true);
}

long CApp::GetMSCount() {
	long ret = (long)(App->MSCount * 1000.f);
	App->MSCount = 0.f;
	return ret;
}

void CApp::Msg(QSP_CHAR *str) {
	App->MessageBoxWidget->SetText(str, true, App->isHtml);
	App->AppDialog->SetWidget(App->MessageBoxWidget);
	mgf->System_Run(App->AppDialog);
	App->isNowUpdate = true;
}

void CApp::DeleteMenu() {
	App->ObjectMenuWidget->Clear(true);
}

void CApp::AddMenuItem(QSP_CHAR *name, QSP_CHAR *imgPath) {
	App->ObjectMenuWidget->Add(imgPath, name, App->isHtml, MGFFONT_CENTER);
}

void CApp::ShowMenu() {
	App->AppDialog->SetWidget(App->ObjectMenuWidget);
	mgf->System_Run(App->AppDialog);
	qspCurSelObject = -1;
	App->isNowUpdate = true;	
}

void CApp::Input(QSP_CHAR *text, QSP_CHAR *buffer, long maxLen) {
	App->InputBoxWidget->SetMode(IBS_THREAD);
	App->InputBoxWidget->SetText(text);
	App->AppDialog->SetWidget(App->InputBoxWidget);
	mgf->System_Run(App->AppDialog);
	strncpy(buffer, (QSP_CHAR*)App->InputBoxWidget->GetText(), maxLen);
	buffer[maxLen] = '\0';
}

void CApp::ShowImage(QSP_CHAR *file) {
	if(App->PicViewWidget->SetPic(file))
  	App->Show(W_PICVIEW, false, true);
  else
    App->Hide(W_PICVIEW);
}

void CApp::OpenGameStatus() {
	App->MainWidget->ShowSaveBrowser(SBS_LOAD);
}

void CApp::SaveGameStatus() {
	app->MainWidget->ShowSaveBrowser(SBS_SAVE);
}*/

/*void CApp::UpdateGamePath()
{
	wxFileName file(QSPGetQstFullPath());
	m_gamePath = file.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
	m_frame->GetDesc()->SetGamePath(m_gamePath);
	m_frame->GetObjects()->SetGamePath(m_gamePath);
	m_frame->GetActions()->SetGamePath(m_gamePath);
	m_frame->GetVars()->SetGamePath(m_gamePath);
}

bool CApp::SetVolume(QSP_CHAR *file, long volume)
{
	FMOD_CHANNEL *channel;
	if (!IsPlay(file)) return false;
	wxString strFile(file);
	strFile.Replace(wxT("\\"), wxT("/"));
	QSPSounds::iterator elem = m_sounds.find(strFile.Upper());
	channel = ((QSPSound)(elem->second)).Channel;
	FMOD_Channel_SetVolume(channel, (float)volume / 100);
	return true;
}*/



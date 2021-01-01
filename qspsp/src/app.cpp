#include "app.h"

#include <mgfutils.h>

CApp *CApp::App = 0;

CApp::CApp(void) : CWidgetEngine() {
  App = this;
	mgf->System_SetState(MGF_DEBUG, false);
	mgf->System_SetState(MGF_VSYNC, true);
	
	mgf->System_Log("wchar_t -> %d\n", sizeof(wchar_t));
	
	isScroll = false;
	isCanSave = true;
	isHtml = false;
	
	isNowUpdate = false;
	
	isInit = false;
}

CApp::~CApp(void) {
  delete MainWidget;
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
  
  delete AppDialog;
  
	ClearSkin();
	delete Res.Audio;
	
	QSPDeInit();
}

void CApp::Init(void) {
  mgf->Gfx_SetProjection(MGF_ORTHO);
	
	if(!isInit) {
	  Res.fontScale = 1.f;
	  Res.fontColor = 0xFFFFFFFF;
	  Res.bgColor = 0xFF000000;
	  Res.selColor = 0xFFFF0000;
	  SetSkin(0);
	  Res.Audio = new MGFAUDIO(1, 1);
	  
	  mgf->System_SetState(MGF_USERDATA, (void*)&Res);
	  
	  MainWidget = new CMainWidget(W_MAIN);
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
	  
	  AppDialog = new CAppDialog(this);
  }
  
  Clear();
  Show(W_MAIN);
  
  /*Left = MainDescWidget;
  Right = ObjectsWidget;
  Active = &Left;*/
  
  qspTime = qspTimer = qspSleepTimer = 0.f;
  QSPInit();
  InitCallBacks();
  QSPLoadGameWorld("./help.qsp");
  isNowUpdate = true;
  QSPRestartGame(QSP_TRUE);
}

bool CApp::Update(void) {  
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
	
	return CWidgetEngine::Update();
}

void CApp::Render(void) {
	mgf->Gfx_BeginScene();
	// -----	
	mgf->Texture_Enable(true);
	mgf->Gfx_Clear(Res.bgColor);
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

void CApp::DelHtml(char *txt) {
  bool html = false;
  for(int i = 0; i < strlen(txt); i++) {
	  if(txt[i] == '<') html = true;
	  if(txt[i] == '>') {
		  txt[i] = 1;
			html = false;
		}
	  if(html) txt[i] = '\1';
	}
}

void CApp::ShowError(void) {
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

void CApp::ClearSkin(void) {
	if(Res.skin && *Res.skin) delete Res.skin;
	if(Res.font) delete Res.font;
	if(Res.texMain) mgf->Texture_Remove(Res.texMain);
	if(Res.texMsg) mgf->Texture_Remove(Res.texMsg);
	if(Res.texMenu) mgf->Texture_Remove(Res.texMenu);
	if(Res.texInput) mgf->Texture_Remove(Res.texInput);	
	if(Res.sprMain) delete Res.sprMain;
	if(Res.sprMsg) delete Res.sprMsg;
	if(Res.sprMenu) delete Res.sprMenu;
	if(Res.sprInput) delete Res.sprInput;
	
	Res.skin = 0;
	Res.font = 0;
	Res.texMain = 0;
	Res.texMsg = 0;
	Res.texMenu = 0;
	Res.texInput = 0;	
	Res.sprMain = 0;
	Res.sprMsg = 0;
	Res.sprMenu = 0;
	Res.sprInput = 0;
}

void CApp::SetSkin(const char *name) {
	if(name && *name && Res.skin && *Res.skin && !strcmp(Res.skin, name)) return;
	if((!name || !*name) && (!Res.skin || !*Res.skin) && Res.font) return;
	
	ClearSkin();
	if(!name || !*name) {
	  mgf->System_Log("Set default skin.\n");
		Res.font = new MGFFONT("data/skin_00_font.fb");
	  Res.texMain = mgf->Texture_Add("data/skin_00_main.jpg");
	  Res.sprMain = new MGFSPRITE(Res.texMain, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	  Res.texMsg = mgf->Texture_Add("data/skin_00_msg.jpg");
	  Res.sprMsg = new MGFSPRITE(Res.texMsg, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	  Res.texMenu = mgf->Texture_Add("data/skin_00_menu.jpg");
	  Res.sprMenu = new MGFSPRITE(Res.texMenu, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	  Res.texInput = mgf->Texture_Add("data/skin_00_input.jpg");
	  Res.sprInput = new MGFSPRITE(Res.texInput, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	} else {
	  mgf->System_Log("Set '%s' skin.\n", name);
		char p[128];
		Res.skin = new char[strlen(name) + 1];
		strcpy(Res.skin, name);
		sprintf(p, "%s%s_font.fb", MainWidget->GetGameDir(), Res.skin);
		Res.font = new MGFFONT(p);
		sprintf(p, "%s%s_main.jpg", MainWidget->GetGameDir(), Res.skin);
		Res.texMain = mgf->Texture_Add(p);
	  Res.sprMain = new MGFSPRITE(Res.texMain, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	  sprintf(p, "%s%s_msg.jpg", MainWidget->GetGameDir(), Res.skin);
	  Res.texMsg = mgf->Texture_Add(p);
	  Res.sprMsg = new MGFSPRITE(Res.texMsg, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	  sprintf(p, "%s%s_menu.jpg", MainWidget->GetGameDir(), Res.skin);
	  Res.texMenu = mgf->Texture_Add(p);
	  Res.sprMenu = new MGFSPRITE(Res.texMenu, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	  sprintf(p, "%s%s_input.jpg", MainWidget->GetGameDir(), Res.skin);
	  Res.texInput = mgf->Texture_Add(p);
	  Res.sprInput = new MGFSPRITE(Res.texInput, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 0, false);
	}
}

bool CApp::GetVarValue(QSP_CHAR *name, long *num, QSP_CHAR **str) {
	if (QSPGetVarValuesCount(name, num) && *num) {
		QSPGetVarValues(name, 0, num, str);
		return true;
	}
	return false;
}


void CApp::InitCallBacks(void) {
  QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&SetTimer);
	QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)&RefreshInt);
	QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&SetInputStrText);
	QSPSetCallBack(QSP_CALL_ISPLAYINGFILE, (QSP_CALLBACK)&IsPlay);
	QSPSetCallBack(QSP_CALL_PLAYFILE, (QSP_CALLBACK)&PlayFile);
	QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&CloseFile);
	QSPSetCallBack(QSP_CALL_SHOWMSGSTR, (QSP_CALLBACK)&Msg);
	QSPSetCallBack(QSP_CALL_SLEEP, (QSP_CALLBACK)&Sleep);
	QSPSetCallBack(QSP_CALL_GETMSCOUNT, (QSP_CALLBACK)&GetMSCount);
	QSPSetCallBack(QSP_CALL_DELETEMENU, (QSP_CALLBACK)&DeleteMenu);
	QSPSetCallBack(QSP_CALL_ADDMENUITEM, (QSP_CALLBACK)&AddMenuItem);
	QSPSetCallBack(QSP_CALL_SHOWMENU, (QSP_CALLBACK)&ShowMenu);
	QSPSetCallBack(QSP_CALL_INPUTBOX, (QSP_CALLBACK)&Input);
	QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&ShowImage);
	QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&ShowPane);
	QSPSetCallBack(QSP_CALL_OPENGAMESTATUS, (QSP_CALLBACK)&OpenGameStatus);
	QSPSetCallBack(QSP_CALL_SAVEGAMESTATUS, (QSP_CALLBACK)&SaveGameStatus);
}

void CApp::SetTimer(long msecs) {
	if(msecs) {
		App->qspTime = 0.f;
		App->qspTimer = (float)msecs / 1000.f;
	} else {
	  App->qspTime = 0.f;
		App->qspTimer = -1.f;
	}
}

void CApp::RefreshInt(QSP_BOOL isRedraw) {
	static long oldFullRefreshCount = 0;
	long i, numVal;
	QSP_CHAR *strVal, *imgPath;
	// -------------------------------
	//UpdateGamePath();
	// -------------------------------
	//if(App->isNowUpdate || mgf->Random_Int(0, 100) == 0 || true) {
		App->isNowUpdate = false;
		
		int mainScroll = CMainDescWidget::SCROLL_NULL, varsScroll = CMainDescWidget::SCROLL_NULL;

		// -------------------------------
		/*App->isScroll = !(GetVarValue(QSP_FMT("DISABLESCROLL"), &numVal, &strVal) && numVal);
		App->isCanSave = !(GetVarValue(QSP_FMT("NOSAVE"), &numVal, &strVal) && numVal);
		App->isHtml = GetVarValue(QSP_FMT("USEHTML"), &numVal, &strVal) && numVal;*/
		// -------------------------------
		//if(QSPIsMainDescChanged()) mainScroll = CMainDescWidget::SCROLL_DOWN;
		//if(QSPIsVarsDescChanged()) varsScroll = CMainDescWidget::SCROLL_DOWN;
		long fullRefreshCount = QSPGetFullRefreshCount();
		if(oldFullRefreshCount != fullRefreshCount) {
		  //if(App->isScroll) mainScroll = CMainDescWidget::SCROLL_UP;
			oldFullRefreshCount = fullRefreshCount;
			//mgf->System_Log("FullRefreshCount = %d\n", (int)fullRefreshCount);
		}
		// -------------------------------
		if(QSPIsMainDescChanged() /*&& mainScroll != CMainDescWidget::SCROLL_NULL*/) {
		  //if(App->isScroll) mainScroll = CMainDescWidget::SCROLL_DOWN;
			const QSP_CHAR *mainDesc = QSPGetMainDesc();
			App->MainDescWidget->SetText(mainDesc, mainScroll, App->isHtml);
		}
		if(QSPIsVarsDescChanged() /*&& varsScroll != CMainDescWidget::SCROLL_NULL*/) {
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
	//}
	// -------------------------------
	/*if(GetVarValue(QSP_FMT("SKINNAME"), &numVal, &strVal) && strVal) {
		App->SetSkin(strVal);
	}
	if(GetVarValue(QSP_FMT("BCOLOR"), &numVal, &strVal)) {
		if(numVal != 0) App->Res.bgColor = SETA(numVal, 0xFF);
		else App->Res.bgColor = 0xFF000000;
	}
	if(GetVarValue(QSP_FMT("FCOLOR"), &numVal, &strVal)) {
		if(numVal != 0) App->Res.fontColor = SETA(numVal, 0xFF);
		else App->Res.fontColor = 0xFFFFFFFF;
	}
	if(GetVarValue(QSP_FMT("SCOLOR"), &numVal, &strVal)) {
		if(numVal != 0) App->Res.selColor = SETA(numVal, 0xFF);
		else App->Res.selColor = 0xFFFF0000;
	}
	if(GetVarValue(QSP_FMT("RCOLOR"), &numVal, &strVal)) {
		if(numVal != 0) App->Res.ramColor = SETA(numVal, 0xFF);
		else App->Res.ramColor = 0xFF0000FF;
	}*/
	
	/*m_frame->ShowPane(ID_BACKPIC,
		GetVarValue(QSP_FMT("BACKIMAGE"), &numVal, &strVal) &&
		strVal && *strVal && m_frame->GetImgBack()->OpenFile(m_gamePath + strVal)
	);*/
	
	//QSPPrepareExecution();
}

void CApp::SetInputStrText(QSP_CHAR *text) {
	App->InputBoxWidget->SetText(text);
}

QSP_BOOL CApp::IsPlay(QSP_CHAR *file) {
	/*FMOD_CHANNEL *channel;
	FMOD_BOOL playing = FALSE;
	wxString strFile(file);
	strFile.Replace(wxT("\\"), wxT("/"));
	QSPSounds::iterator elem = m_sounds.find(strFile.Upper());
	if (elem != m_sounds.end())
	{
		channel = ((QSPSound)(elem->second)).Channel;
		FMOD_Channel_IsPlaying(channel, &playing);
	}
	return (playing == TRUE);*/
	return App->Res.Audio->Playing;
}

void CApp::CloseFile(QSP_CHAR *file) {
	/*if (file)
	{
		wxString strFile(file);
		strFile.Replace(wxT("\\"), wxT("/"));
		QSPSounds::iterator elem = m_sounds.find(strFile.Upper());
		if (elem != m_sounds.end())
		{
			((QSPSound)(elem->second)).Free();
			m_sounds.erase(elem);
		}
	}
	else
	{
		for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
			((QSPSound)(i->second)).Free();
		m_sounds.clear();
	}*/
	App->Res.Audio->Stop();
}

void CApp::PlayFile(QSP_CHAR *file, long volume) {
	/*FMOD_SOUND *newSound;
	FMOD_CHANNEL *newChannel;
	QSPSound snd;
	if (SetVolume(file, volume)) return;
	CloseFile(file);
	wxString strFile(file);
	strFile.Replace(wxT("\\"), wxT("/"));
	if (!FMOD_System_CreateSound(m_sys, wxConvFile.cWX2MB(strFile.wx_str()), FMOD_SOFTWARE | FMOD_CREATESTREAM, 0, &newSound))
	{
		FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE, newSound, FALSE, &newChannel);
		snd.Channel = newChannel;
		snd.Sound = newSound;
		m_sounds.insert(QSPSounds::value_type(strFile.Upper(), snd));
		SetVolume(file, volume);
	}*/
	mgf->System_Log("Play '%s'.\n", file);
	App->Res.Audio->PlayFile(file);
}

void CApp::ShowPane(long type, QSP_BOOL isShow) {
	/*switch (type)
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
	}*/
}

void CApp::Sleep(long msecs) {
  /*Sleep(msecs);
	if (m_frame->GetIsQuit()) return;
	bool isSave = m_frame->GetGameMenu()->IsEnabled(ID_SAVEGAMESTAT);
	m_frame->EnableControls(false, true);
	long i, count = msecs / 50, rem = msecs % 50;
	for (i = 0; i < count; ++i)
	{
		wxThread::Sleep(50);
		wxTheApp->Yield(true);
		if (m_frame->GetIsQuit()) break;
	}
	if (!m_frame->GetIsQuit())
	{
		wxThread::Sleep(rem);
		wxTheApp->Yield(true);
	}
	m_frame->EnableControls(true, true);
	m_frame->GetGameMenu()->Enable(ID_SAVEGAMESTAT, isSave);*/
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
	/*bool oldIsProcessEvents;
	wxMessageDialog msgDialog(m_frame, wxString(str), _("Info"), wxOK | wxICON_INFORMATION);
	oldIsProcessEvents = m_frame->GetIsProcessEvents();
	m_frame->SetIsProcessEvents(false);
	msgDialog.ShowModal();
	m_frame->SetIsProcessEvents(oldIsProcessEvents);*/
	/*
	App->MessageWidget->SetText(str);
	App->MessageWidget->SetButton(MESSAGEWIDGET_BUTTONOK);
	App->Add(W_MESSAGE, true);
	*/
	/*int cnt = strlen(str) + 12;
	char code[cnt];
	sprintf(code, "*NL\n*P '%s'", str);
	code[cnt-1] = '\0';
	mgf->System_Log("Exec: %s\n", code);
	QSPExecString(code, QSP_TRUE);
	App->isNowUpdate = true;*/
	App->MessageBoxWidget->SetText(str, true, App->isHtml);
	App->AppDialog->SetWidget(App->MessageBoxWidget);
	mgf->System_Run(App->AppDialog);
	App->isNowUpdate = true;
}

void CApp::DeleteMenu() {
  //mgf->System_Log("delete menu\n");
	/*m_frame->DeleteMenu();*/
	App->ObjectMenuWidget->Clear(true);
}

void CApp::AddMenuItem(QSP_CHAR *name, QSP_CHAR *imgPath) {
	/*m_frame->AddMenuItem(wxString(name), wxString(imgPath));*/
	//mgf->System_Log("add menu(%s)\n", name);
	App->ObjectMenuWidget->Add(imgPath, name, App->isHtml, MGFFONT_CENTER);
}

void CApp::ShowMenu() {
	/*m_frame->ShowMenu();*/
	App->AppDialog->SetWidget(App->ObjectMenuWidget);
	mgf->System_Run(App->AppDialog);
	qspCurSelObject = -1;
	App->isNowUpdate = true;	
}

void CApp::Input(QSP_CHAR *text, QSP_CHAR *buffer, long maxLen) {
	/*bool oldIsProcessEvents;
	RefreshInt(QSP_FALSE);
	QSPInputDlg *inputDlg = new QSPInputDlg(m_frame,
		wxID_ANY,
		m_frame->GetDesc()->GetBackgroundColour(),
		m_frame->GetDesc()->GetForegroundColour(),
		m_frame->GetDesc()->GetTextFont(),
		_("Input data"),
		wxString(text),
		m_isHtml,
		m_gamePath
	);
	oldIsProcessEvents = m_frame->GetIsProcessEvents();
	m_frame->SetIsProcessEvents(false);
	inputDlg->ShowModal();
	m_frame->SetIsProcessEvents(oldIsProcessEvents);
	#ifdef _UNICODE
		wcsncpy(buffer, inputDlg->GetText().wx_str(), maxLen);
	#else
		strncpy(buffer, inputDlg->GetText().wx_str(), maxLen);
	#endif
	buffer[maxLen] = 0;
	inputDlg->Destroy();*/
	App->InputBoxWidget->SetMode(IBS_THREAD);
	App->InputBoxWidget->SetText(text);
	App->AppDialog->SetWidget(App->InputBoxWidget);
	mgf->System_Run(App->AppDialog);
	strncpy(buffer, (QSP_CHAR*)App->InputBoxWidget->GetText(), maxLen);
	buffer[maxLen] = '\0';
}

void CApp::ShowImage(QSP_CHAR *file) {
	/*m_frame->ShowPane(ID_VIEWPIC, m_frame->GetImgView()->OpenFile(wxString(file)));*/
	if(App->PicViewWidget->SetPic(file))
  	App->Show(W_PICVIEW, false, true);
  else
    App->Hide(W_PICVIEW);
}

void CApp::OpenGameStatus() {
	/*wxFileDialog dialog(m_frame, _("Select saved game file"), wxEmptyString, wxEmptyString, _("Saved game files (*.sav)|*.sav"), wxFD_OPEN);
	if (dialog.ShowModal() == wxID_OK)
		QSPOpenSavedGame((QSP_CHAR *)dialog.GetPath().wx_str(), QSP_FALSE);*/
	App->MainWidget->ShowSaveBrowser(SBS_LOAD);
}

void CApp::SaveGameStatus() {
	/*wxFileDialog dialog(m_frame, _("Select file to save"), wxEmptyString, wxEmptyString, _("Saved game files (*.sav)|*.sav"), wxFD_SAVE);
	if (dialog.ShowModal() == wxID_OK)
		QSPSaveGame((QSP_CHAR *)dialog.GetPath().wx_str());*/
	App->MainWidget->ShowSaveBrowser(SBS_SAVE);
}

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



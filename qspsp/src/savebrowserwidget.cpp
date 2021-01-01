#include "savebrowserwidget.h"

#include "app.h"
#include <stdio.h>

CSaveBrowserWidget::CSaveBrowserWidget(int id) : CActionsWidget(id) {
  App = (CApp*)mgf->System_GetState(MGF_APP);
  X = 10;
  Y = 10;
	Width = SCREEN_WIDTH - 20;
	Height = SCREEN_HEIGHT - 20;
	State = SBS_SAVE;
};

CSaveBrowserWidget::~CSaveBrowserWidget(void) {
  Clear(true);
}
  
/*void CSaveBrowserWidget::OnRegister(void) {
}*/

void CSaveBrowserWidget::OnShow(void) {
	CActionsWidget::OnShow();
	
	numResult = SBM_NONE;

  char p[128]; // MGF_MAX_PATH
  char *lcnm;
  SceIoStat ioState, ios;
  int c;

  Clear(true);
  for(int i = 0; i < SLOT_MAX; i++) {
    sprintf(p, "SAVES/GAME_%s.%d.SAV", Name, i);
    //mgf->System_Log("find '%s'\n", p);
	  if(sceIoGetstat(p, &ioState) < 0) {
		  sprintf(p, "%0.2d. <пусто>", (i+1));
		} else {
			sprintf(p, "SAVES/GAME_%s.%d.TXT", Name, i);
			int f = sceIoOpen(p, PSP_O_RDONLY, 0744);
			if(f >= 0) {
				sceIoRead(f, &c, 4);
				lcnm = new char[c+1];
				sceIoRead(f, lcnm, c);
				lcnm[c] = '\0';
				sprintf(p, "%0.2d. %0.2d:%0.2d, %0.2d.%0.2d.%0.2d - %s", (i+1), ioState.st_mtime.hour, ioState.st_mtime.minute, ioState.st_mtime.day, ioState.st_mtime.month, ioState.st_mtime.year, lcnm);
				delete lcnm;
				sceIoClose(f);
			} else {
				sprintf(p, "%0.2d. %0.2d:%0.2d, %0.2d.%0.2d.%0.2d", (i+1), ioState.st_mtime.hour, ioState.st_mtime.minute, ioState.st_mtime.day, ioState.st_mtime.month, ioState.st_mtime.year);
			}
		}
		//mgf->System_Log("add '%s'\n", p);
		Add(0, p, false);
	}
	/*SceIoDirent dir;
	int ok;
	int dfd = sceIoDopen("./SAVES");
	if(dfd >= 0) {
		for(;;) {
			memset(&dir, 0, sizeof(SceIoDirent));
			ok = sceIoDread(dfd, &dir);
			if(ok <= 0) break;
			mgf->System_Log("\t%s\n", dir.d_name);
			if(strcmp(dir.d_name, "..") && strcmp(dir.d_name, ".")) {
				//char* suffix = strrchr(dir.d_name, '.');
				if((dir.d_stat.st_attr & FIO_SO_IFDIR) == 0 ) { //&& !stricmp(suffix, ".GAM")
					sprintf(p, "./GAMES/%s", dir.d_name);
					CActionsWidget::Add(0, p);
				}
			}
		}
	}
	sceIoDclose(dfd);
	*/
	mgf->System_Log("End save list.\n", p);
}

/*void CSaveBrowserWidget::OnHide(void) {
}*/

/*void CSaveBrowserWidget::OnActive(void) {
}*/
  
bool CSaveBrowserWidget::Update(float delta) {
  bool res = CActionsWidget::Update(delta);
  
  /*if(mgf->Input_IsKeyClick(PSP_CTRL_CIRCLE)) {
		isOk = true;
		App->Remove(W_GAMEBROWSER);
		return false;
  }*/
  if(mgf->Input_IsKeyClick(PSP_CTRL_SQUARE)) {
    if(Item[Sel]->str[4] != '<') {
	    char p[128];
	    sprintf(p, "SAVES/GAME_%s.%d.SAV", Name, Sel);
		  sceIoRemove(p);
		  delete Item[Sel]->str;
		  sprintf(p, "%0.2d. <пусто>", (Sel+1));
		  Item[Sel]->str = new char[strlen(p) + 1];
		  strcpy(Item[Sel]->str, p);
	  }
	}
  if(mgf->Input_IsKeyClick(PSP_CTRL_TRIANGLE)) {
	  if(State == SBS_LOAD) State = SBS_SAVE;
	  else State = SBS_LOAD;
	}
  if(mgf->Input_IsKeyClick(PSP_CTRL_CROSS) || mgf->Input_IsKeyClick(PSP_CTRL_START)) numResult = SBM_CANCEL;
	if(numResult != SBM_NONE) {
		mgf->System_SetState(MGF_APPRESULT, numResult);
		return true;
  }
  
	return res;
}

void CSaveBrowserWidget::Render(void) {
  if(Res && Res->sprMsg) Res->sprMsg->Render(0, 0);
	CActionsWidget::Render();
  if(Res && Res->font) {
    Res->font->SetScale(Res->fontScale);
    Res->font->SetColor(Res->fontColor);
    Res->font->Render(SCREEN_WIDTH-10, 10, MGFFONT_RIGHT, (State == SBS_SAVE) ? ("SAVE") : ("LOAD"));
  }
}

char *CSaveBrowserWidget::GetText(int id) {
  if(id == -1) id = Sel;
  if(id < 0 || id >= Count || !Item[id]) return 0;
  return Path;
}

void CSaveBrowserWidget::OnSelectItem(bool new_sel) {
}

void CSaveBrowserWidget::OnActiveItem(void) {
  sprintf(Path, "SAVES/GAME_%s.%d.SAV", Name, Sel);
  
	if(State == SBS_LOAD) {
	  if(Item[Sel]->str[4] == '<') return;
	  App->isNowUpdate = true;
	  QSPOpenSavedGame(Path, QSP_TRUE);
	  mgf->System_Log("Load from '%s'.\n", Path);
	} else if(State == SBS_SAVE) {
	  mgf->System_Log("Save to '%s'.\n", Path);
	  QSPSaveGame(Path, QSP_TRUE);
	  char p[128];
	  sprintf(p, "SAVES/GAME_%s.%d.TXT", Name, Sel);
	  QSP_CHAR *curlocname = (QSP_CHAR*)QSPGetCurLoc();
		long num; int n;
	  //if(App->GetVarValue(QSP_FMT("CURLOC"), &num, &curlocname)) {
	    int f = sceIoOpen(p, PSP_O_WRONLY|PSP_O_CREAT|PSP_O_TRUNC, 0777);
			n = strlen(curlocname);
		  sceIoWrite(f, &n, 4);
		  sceIoWrite(f, curlocname, n);
		  sceIoClose(f);
		  mgf->System_Log("Save in '%s' location.\n", curlocname);
	  //}
	}
	
	numResult = GBM_OK;
  App->isNowUpdate = true;
	
  //if(Res) Res->Loading(mgf);
}

void CSaveBrowserWidget::Set(const char *name, int state) {
  if(state != -1) State = state;
  mgf->System_Log("sb.s=%d[%d:%d]\n", state, SBS_SAVE, SBS_LOAD);
	if(name && *name)
	  strncpy(Name, name, 127);
	else
	  Name[0] = '\0';
	Name[127] = '\0';
}


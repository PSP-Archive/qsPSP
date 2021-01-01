#include "gamebrowserwidget.h"

#include "app.h"
#include <stdio.h>
#include "qsp/declarations.h"
#include "qsp/coding.h"

CGameBrowserWidget::CGameBrowserWidget(int id) : CActionsWidget(id) {
  X = 10;
  Y = 10;
	Width = SCREEN_WIDTH - 20;
	Height = SCREEN_HEIGHT - 20;
	SelDirIndex = 0;
	strcpy(CurrDir, "GAMES/");
};

CGameBrowserWidget::~CGameBrowserWidget(void) {
  Clear(true);
}
  
/*void CGameBrowserWidget::OnRegister(void) {
}*/

void CGameBrowserWidget::OnShow(void) {
	CActionsWidget::OnShow();
	
	numResult = GBM_NONE;

  _scan(CurrDir);//"GAMES/");
}

/*void CGameBrowserWidget::OnHide(void) {
}*/

/*void CGameBrowserWidget::OnActive(void) {
}*/
  
bool CGameBrowserWidget::Update(float delta) {
  bool res = CActionsWidget::Update(delta);
  
  /*if(mgf->Input_IsKeyClick(PSP_CTRL_CIRCLE)) {
		isOk = true;
		App->Remove(W_GAMEBROWSER);
		return false;
  }*/
  if(mgf->Input_IsKeyClick(PSP_CTRL_CROSS) || mgf->Input_IsKeyClick(PSP_CTRL_SELECT)) numResult = GBM_CANCEL;
	if(numResult != GBM_NONE) {
	  mgf->System_SetState(MGF_APPRESULT, numResult);
	  numResult = GBM_NONE;
		return true;
  }
  
	return res;
}

void CGameBrowserWidget::Render(void) {
  if(Res && Res->sprMsg) Res->sprMsg->Render(0, 0);
	CActionsWidget::Render();
	if(Res && Res->font) {
    //mgf->Gfx_Clear(Res->bgColor);
    Res->font->SetScale(Res->fontScale);
    Res->font->SetColor(Res->fontColor);
    Res->font->Render(SCREEN_WIDTH-10, 10, MGFFONT_RIGHT, "OPEN");
  }
}

void CGameBrowserWidget::OnSelectItem(bool new_sel) {
  Sel = Cur;
}

void CGameBrowserWidget::OnActiveItem(void) {
  char p[MGF_MAX_PATH], pp[MGF_MAX_PATH];
	if(Item[Sel]->param == 1) {
	  SelDir[SelDirIndex] = Sel;
	  SelDirIndex++;
		strncpy(p, &Item[Sel]->str[1], strlen(Item[Sel]->str)-2);
	  p[strlen(Item[Sel]->str)-2] = '\0';
	  mgf->System_Log("p = '%s'\n", p);
	  //sscanf(Item[Sel]->str, "[%s]", p);
	  sprintf(pp, "%s%s/", CurrDir, p);
	  _scan(pp);
	} else if(Item[Sel]->param == 99) {
	  strcpy(p, CurrDir);
		for(int i = strlen(p)-2; i >=0; i--) {
		  if(p[i] == '/' || p[i] == '\\') {
			  p[i+1] = '\0';
			  break;
			}
		}
		_scan(p);
		SelDirIndex--;
		SetSelection(SelDir[SelDirIndex]);
	} else {
		numResult = GBM_OK;
	  App->isNowUpdate = true;
  }
}

void CGameBrowserWidget::_scan(const char *scandir) {
	mgf->System_Log("Scan '%s'.\n", scandir);
	Clear(true);
	if(!scandir || !*scandir) return;
	strncpy(CurrDir, scandir, MGF_MAX_PATH);
	CurrDir[MGF_MAX_PATH-1] = '\0';
	SceIoDirent dir;
	char p[MGF_MAX_PATH];
	QSP_CHAR *pn;
	int ok;
	int dfd = sceIoDopen(CurrDir);
	if(dfd >= 0) {
		for(;;) {
			memset(&dir, 0, sizeof(SceIoDirent));
			ok = sceIoDread(dfd, &dir);
			if(ok <= 0) break;
			mgf->System_Log("\t%s\n", dir.d_name);
			if(strcmp(dir.d_name, ".")) {
			  if(!strcmp(dir.d_name, "..")) {
				  if(strcmp(CurrDir, "GAMES/")) {
						sprintf(p, "[%s]", dir.d_name);
					  Add(0, p, false, MGFFONT_CENTER, 99);
				  }
				} else {
					char* suffix = strrchr(dir.d_name, '.');
					if(dir.d_stat.st_attr & FIO_SO_IFDIR || !stricmp(suffix, ".GAM") || !stricmp(suffix, ".QSP")) {
					  if(dir.d_stat.st_attr & FIO_SO_IFDIR) {
							ok = 1;
							sprintf(p, "[%s]", dir.d_name);
						} else {
							ok = 0;
							sprintf(p, "%s", dir.d_name);
						}
						//setlocale(LC_ALL, QSP_LOCALE);
						//pn = qspToQSPString(p);
						Add(0, p, false, MGFFONT_CENTER, ok);
						//free(pn);
					}
				}
			}
		}
	}
	sceIoDclose(dfd);
	mgf->System_Log("End game list.\n", p);
}

char *CGameBrowserWidget::GetFileName(int id) {
  if(id == -1) id = Sel;
  if(id == -1 || Item[Sel]->param != 0) return 0;
  sprintf(Text, "%s%s", CurrDir, Item[Sel]->str);
  return Text;
}


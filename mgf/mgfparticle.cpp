#include <mgfparticle.h>

#include <mgfutils.h>

MGF *MGFEMITTER2D::mgf = 0;

MGFEMITTER2D::MGFEMITTER2D(MGFSPRITE *spr, int blend, int max) {
	mgf = mgfCreate();
	list = new MGFPARTICLE2D[max];
	sprite = spr;
	parMax = max;
	parCount = 0;
	blendMode = blend;
}

MGFEMITTER2D::~MGFEMITTER2D(void) {
	delete[] list;
	mgf->Release();
}
	
void MGFEMITTER2D::Add(MGFPARTICLE2D *p) {
	if(!p || parCount >= parMax) return;
	
	for(int i = 0; i < parMax; i++) {
		if(!list[i].active) {
			//list[i] = *p;
			list[i].active = true;
			list[i].pos = p->pos;
			list[i].speed = p->speed;
			list[i].speedSpeed = p->speedSpeed;
			list[i].rotate = p->rotate;
			list[i].rotateSpeed = p->rotateSpeed;
			list[i].angle = p->angle;
			list[i].angleSpeed = p->angleSpeed;
			list[i].scale = p->scale;
			list[i].scaleSpeed = p->scaleSpeed;
			list[i].lifeMax = p->lifeMax;
			list[i].life = p->life;
			list[i].colorStart = p->colorStart;
			list[i].colorEnd = p->colorEnd;
			parCount++;
			mgf->System_Log("par-add : %d\n", parCount);
			return;
		}
	}
}
	
void MGFEMITTER2D::Update(float delta) {
	MGFCOLOR col;
	float lf;
	for(int i = 0; i < parMax; i++) {
		if(!list[i].active) continue;		
		
		list[i].life -= delta;
		if(list[i].life > 0.f) {
			if(list[i].speedSpeed != 0) {
				list[i].speed += list[i].speedSpeed * delta;
				if(list[i].speed <= 0.f) {
					list[i].speed = 0.f;
					list[i].speedSpeed = 0.f;
				}
			}
			if(list[i].rotateSpeed != 0) list[i].rotate += list[i].rotateSpeed * delta;
			if(list[i].angleSpeed != 0.f) list[i].angle += list[i].angleSpeed * delta;
			if(list[i].scaleSpeed != 0.f) list[i].scale += list[i].scaleSpeed * delta;
			if(list[i].speed > 0.f) list[i].pos.Move(list[i].speed * delta, list[i].angle);
			lf = 1.f / list[i].lifeMax * (list[i].lifeMax - list[i].life);
			col = list[i].colorEnd - list[i].colorStart;
			list[i].color = list[i].colorStart + col * lf;
			//p->color.a = col.a;
			//p->color = p->colorEnd - col * lf;
		} else {
			list[i].active = false;
			parCount--;
			mgf->System_Log("par-del : %d\n", parCount);
		}
	}
}

void MGFEMITTER2D::Render(MGFVECTOR2D scroll) {
	mgf->Gfx_SetBlend(blendMode);
	for(int i = 0; i < parMax; i++) {
		if(!list[i].active) continue;
		sprite->SetColor(list[i].color.GetColor());
		sprite->Render(list[i].pos.x - scroll.x, list[i].pos.y - scroll.y, list[i].rotate, list[i].scale);
	}
}

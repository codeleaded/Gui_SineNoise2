#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/SineNoise.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"

#define PERLINNOISE_SIZE	1.0f


float Function_2D(float x,float y){
	return SineNoise_2D_Get(x,y) * PERLINNOISE_SIZE;
}

TransformedView tv;

void Setup(AlxWindow* w){
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	SineNoise_Set(3,Random_f64_MinMax(0.0,1.0),Random_f64_MinMax(0.0,1.0),Random_f64_MinMax(0.0,1.0));
}

void Update(AlxWindow* w){
	tv.ZoomSpeed = 10.0f * w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

	Clear(BLACK);

	Vec2 wvls = { 0.0f,TransformedView_ScreenWorldY(&tv,0.0f) };
	Vec2 wvle = { 0.0f,TransformedView_ScreenWorldY(&tv,GetHeight()) };
	Vec2 whls = { TransformedView_ScreenWorldX(&tv,0.0f),0.0f };
	Vec2 whle = { TransformedView_ScreenWorldX(&tv,GetWidth()),0.0f };

	Vec2 svls = TransformedView_WorldScreenPos(&tv,wvls);
	Vec2 svle = TransformedView_WorldScreenPos(&tv,wvle);
	Vec2 shls = TransformedView_WorldScreenPos(&tv,whls);
	Vec2 shle = TransformedView_WorldScreenPos(&tv,whle);

	for(int i = 0;i<GetHeight();i++){
		for(int j = 0;j<GetWidth();j++){
			const Vec2 Pos = TransformedView_ScreenWorldPos(&tv,(Vec2){ j,i });
			const float out = Function_2D(Pos.x,Pos.y);
			const float pix = out - (int)floorf(out);
			window.Buffer[i * GetWidth() + j] = Pixel_Mulf(WHITE,pix);
		}
	}

	Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
	String str = String_Format("P: X: %f, Y: %f",p.x,p.y);
	RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
	String_Free(&str);
}

void Delete(AlxWindow* w){
	
}

int main(){
    if(Create("SN 2D",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}
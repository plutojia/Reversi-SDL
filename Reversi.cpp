// Reversi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "ai.h"
using namespace std;
//#define FontPath "/system/fonts/DroidSansFallback.ttf"
#define FontPath "simsun.ttf"
#define WindowW 432
#define WindowH 768
#define W1080 double(540)
#define H1080 double(960)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backbuffer = NULL;
SDL_Texture *output;
SDL_Thread *calthread = NULL;
stringstream os;
char *message;
int scene = 0;
qp test;
int cal(void *ptr);
void init();
/********render text********/
SDL_Texture *text(const char *Fonts, int FontSize, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 0)
{
	TTF_Init();
	TTF_Font *font = NULL;
	//打开字体/* 字体路径 /*字体大小
	font = TTF_OpenFont(FontPath, FontSize);
	SDL_Color color = { r,g,b,a };

	SDL_Surface *temp = NULL;
	SDL_Texture *fonts = NULL;
	temp = TTF_RenderUTF8_Blended(font, Fonts, color);
	fonts = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	TTF_CloseFont(font);
	if (fonts != NULL)
		return fonts;
}
/*******class*******/
// class texture
class ltexture {
public:
	ltexture();
	void load(char* path);
	void render(int xx, int yy, int ww, int hh, SDL_Rect *rect = NULL);
	void render(int xx, int yy, int ww, int hh, int x, int y, int w, int h);
	void setalpha(int a);
	~ltexture();
private:
	SDL_Texture * texture;
};
ltexture::ltexture() {
}
void ltexture::load(char* path) {
	SDL_Surface *surface = IMG_Load(path);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
}
void ltexture::setalpha(int a) {
	SDL_SetTextureAlphaMod(texture, a);
}
void ltexture::render(int xx, int yy, int ww, int hh, int x, int y, int w, int h) {
	SDL_Rect temp1 = { xx, yy, ww, hh };
	SDL_Rect temp2 = { x,y,w,h };
	SDL_RenderCopy(renderer, texture, &temp1, &temp2);
}
void ltexture::render(int x, int y, int w, int h, SDL_Rect *rect) {
	SDL_Rect temp1 = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, &temp1, rect);
}
ltexture::~ltexture() {
	SDL_DestroyTexture(texture);
}
//*********全局******
ltexture white;
ltexture blank;
//**********button**********
class button {
public:
	Sint32 x = 0, y = 0, w = 100, h = 50;
	bool isclicked = false;
	bool enabled = true;
	int curtexture = 0;
	button() {}
	void load(char *path, int i = 0);
	void render(int xx, int yy, int ww, int hh);
	void render(int xx, int yy, int ww, int hh, SDL_Rect *rect);
	void settexture(int i);
	void handleevent(SDL_Event e);
private:
	ltexture tex[3];
};
void button::load(char *path, int i) {
	tex[i].load(path);

}
void button::settexture(int i) {
	curtexture = i;
}
void button::handleevent(SDL_Event e) {
	if (enabled == false) return;
	if (e.motion.x*(W1080/WindowW)<x || e.motion.x*(W1080 / WindowW)>x + w || e.motion.y*(H1080 / WindowH)<y || e.motion.y*(H1080 / WindowH)>y + h) {
		if (e.type == SDL_MOUSEMOTION)
		{
			settexture(0); return;
		}
	}
	else {
		if (e.type == SDL_MOUSEBUTTONDOWN)
			settexture(1);
		if (e.type == SDL_MOUSEBUTTONUP) {
			settexture(0);
			isclicked = true;
		}
	}
}
void button::render(int xx, int yy, int ww, int hh) {
	SDL_Rect temp1 = { xx, yy, ww, hh };
	SDL_Rect temp2 = { x,y,w,h };
	tex[curtexture].render(xx, yy, ww, hh, &temp2);
}
void button::render(int x, int y, int w, int h, SDL_Rect *rect) {
	tex[curtexture].render(x, y, w, h, rect);
}

//***********board**************
class board {
public:
	Sint32 x = 45, y = 225, w = 450, h = 450;
	bool enabled = true;
	board();
	void load(char *path);
	void render(int xx, int yy, int ww, int hh);
	void render(int xx, int yy, int ww, int hh, SDL_Rect *rect);
	void handleevent(SDL_Event e);
	int q[8][8];
	int flag = 1;
private:
	ltexture texture;
};
board::board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			q[i][j] = 0;
	}
	q[3][3] = 1;
	q[3][4] = -1;
	q[4][3] = -1;
	q[4][4] = 1;
}
void board::load(char *path) {
	texture.load(path);
}
void board::render(int xx, int yy, int ww, int hh) {
	SDL_Rect temp1 = { xx, yy, ww, hh };
	SDL_Rect temp2 = { x, y, w, h };
	texture.render(xx, yy, ww, hh, &temp2);
	for (int i = 0; i<8; i++)
		for (int j = 0; j<8; j++) {
			if (q[i][j] == 1) {
				SDL_Rect temp = { x + 28 + 50 * j,y + 28 + 50 * i,45,45 };
				blank.render(0, 0, 45, 45, &temp);
			}
			else if (q[i][j] == -1) {
				SDL_Rect temp = { x + 28 + 50 * j,y + 28 + 50 * i,45,45 };
				white.render(0, 0, 90, 90, &temp);

			}
		}
}
void board::render(int x, int y, int w, int h, SDL_Rect *rect) {
	texture.render(x, y, w, h, rect);
}
void board::handleevent(SDL_Event e) {
	if (enabled == false) return;
	if (e.motion.x * (W1080 / WindowW)<x || e.motion.x * (W1080 / WindowW)>x + w || e.motion.y * (H1080 / WindowH)<y || e.motion.y * (H1080 / WindowH)>y + h) { return; }
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int xx = (e.motion.x * (W1080 / WindowW) - x - 450 / 18) / 50;
		int yy = (e.motion.y * (H1080 / WindowH) - y - 450 / 18) / 50;
		char a[10];
		sprintf_s(a,"%d,%d", xx + 1, yy + 1);
		output = text(a, 100, 25, 0, 0, 0);
		os.clear();
		if (test.pvc(yy + 1, xx + 1) != -1) {
			test.getarray(q);
			calthread = SDL_CreateThread(cal, "Thread", (void *)NULL);
		}
	}

	if (e.type == SDL_MOUSEBUTTONUP) {
		;

	}
}
class scoreboard {
public:
	Sint32 x = 150, y = 45, w = 250, h = 150;
	scoreboard() {}
	void load(char *path);
	void render(int xx, int yy, int ww, int hh);
	void render(int xx, int yy, int ww, int hh, SDL_Rect *rect);
private:
	ltexture texture;
};
void scoreboard::load(char *path) {
	texture.load(path);
	texture.setalpha(200);
}
void scoreboard::render(int xx, int yy, int ww, int hh) {
	SDL_Rect temp1 = { xx, yy, ww, hh };
	SDL_Rect temp2 = { x,y,w,h };
	texture.render(xx, yy, ww, hh, &temp2);
	temp1 = { x + 30,y + 25,40,40 };
	temp2 = { x + 130,y + 25,40,40 };
	blank.render(0, 0, 45, 45, &temp1);
	white.render(0, 0, 45, 45, &temp2);
	SDL_Rect box;
	char a[15];
	sprintf_s(a, "%s", message);
	
	output = text(a, 30, 25, 0, 0, 0);
	SDL_QueryTexture(output, NULL, NULL, &box.w, &box.h);
	box.x = x + (w - box.w)*0.5;
	box.y = y + 75; 
	SDL_RenderCopy(renderer, output, NULL, &box);
	SDL_DestroyTexture(output);
	
	if (scene == 1) {
		char a[10];
		sprintf_s(a, "%d", test.xsum);
		output = text(a, 35, 25, 0, 0, 0);
		os.clear();
	}
	else { output = text("0", 35, 25, 0, 0, 0); }
	SDL_QueryTexture(output, NULL, NULL, &box.w, &box.h);
	box.x = x + 80;
	box.y = y + 25; 
	SDL_RenderCopy(renderer, output, NULL, &box);
	SDL_DestroyTexture(output);
	if (scene == 1) {
		char a[10];
		sprintf_s(a, "%d", test.osum);
		output = text(a, 35, 25, 0, 0, 0);
		os.clear();
	}
	else { output = text("0", 35, 25, 0, 0, 0); }
	SDL_QueryTexture(output, NULL, NULL, &box.w, &box.h);
	box.x = x + 180;
	box.y = y + 25; 
	SDL_RenderCopy(renderer, output, NULL, &box);
	SDL_DestroyTexture(output);
	
}
void scoreboard::render(int xx, int yy, int ww, int hh, SDL_Rect *rect) {
	texture.render(x, y, w, h, rect);
}
//**********全局变量声明区*******
ltexture background;
button start;
button back;
button pass;
button touxiang;
board qipan;
scoreboard sb;
//**********calthread************
int cal(void *ptr) {
	pass.enabled = false;
	back.enabled = false;
	message = "Thinking...";
	test.cvc();
	test.getarray(qipan.q);
	pass.enabled = true;
	back.enabled = true;
	message = "Your turn";
	return 0;
}
//init
void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	window = SDL_CreateWindow("Reversi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowW, WindowH, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC || SDL_RENDERER_ACCELERATED || SDL_RENDERER_TARGETTEXTURE);
	backbuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 540, 960);
	message = "Reversi";
}
void load() {
	background.load("./asset/background2.png");
	sb.load("./asset/scoreboard.png");
	white.load("./asset/white.png");
	blank.load("./asset/blank.png");
	start.load("./asset/start.png", 0);
	start.load("./asset/start1.png", 1);
	back.load("./asset/back.png", 0);
	back.load("./asset/back1.png", 1);
	pass.load("./asset/pass.png", 0);
	pass.load("./asset/pass1.png", 1);
	touxiang.load("./asset/touxiang.png", 0);
	touxiang.load("./asset/touxiang1.png", 1);
	qipan.load("./asset/qipan.png");
}
void sset() {
	start.x = 45;
	start.y = 750;
	back.x = 160;
	back.y = 750;
	pass.x = 275;
	pass.y = 750;
	touxiang.x = 390;
	touxiang.y = 750;
	back.enabled = false;
	pass.enabled = false;
	touxiang.enabled = false;
	qipan.enabled = false;
}
void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(backbuffer);
	SDL_Quit();
}
//main
void buttonevent() {
	if (start.isclicked) {
		output = text("Start isclickde", 100, 25, 0, 0, 0);
		message = "Thinking...";
		back.enabled = true;
		pass.enabled = true;
		touxiang.enabled = true;
		qipan.enabled = true;
		test = *new(qp);
		test.setdifficulty(3);
		srand((unsigned)time(NULL));
		int r = rand() % (2);
		if (r == 1) { test.cvc(); message = "Back hand"; }
		else message = "First hand";
		test.getarray(qipan.q);
		scene = 1;
		start.isclicked = false;
	}
	else if (back.isclicked) {
		output = text("back isclickde", 100, 25, 0, 0, 0);
		message = "Back";
		if (test.back(2) != -1)
			test.getarray(qipan.q);
		back.isclicked = false;
	}
	else if (pass.isclicked) {
		output = text("Pass isclickde", 100, 25, 0, 0, 0);
		message = "Pass";
		test.pass();
		calthread = SDL_CreateThread(cal, "Thread", (void *)NULL);
		scene = 0;
		pass.isclicked = false;
	}
	else if (touxiang.isclicked) {
		SDL_WaitThread(calthread, (int*)NULL);
		output = text("You lose", 100, 25, 0, 0, 0);
		message = "You lose";
		test = *new(qp);
		test.getarray(qipan.q);
		touxiang.isclicked = false;
	}
	else {
		//output = NULL;
	}

}
int main(int argc, char* args[]) {
	init();
	load();
	sset();
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
	SDL_RenderClear(renderer);
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(renderer);
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT: quit = 1; break;
			default:
				start.handleevent(e);
				pass.handleevent(e);
				touxiang.handleevent(e);
				back.handleevent(e);
				qipan.handleevent(e);
			}
		}
		buttonevent();
		SDL_SetRenderTarget(renderer, backbuffer);
		SDL_RenderClear(renderer);
		background.render(0, 0, 1080, 1920);
		qipan.render(0, 0, 450, 450);
		SDL_Rect temp = { 49,120,29,29 };
		start.render(0, 0, 100, 50);
		back.render(0, 0, 100, 50);
		pass.render(0, 0, 100, 50);
		touxiang.render(0, 0, 100, 500);
		sb.render(0, 0, 500, 300);
		temp = { 0,0,200,40 };
		//SDL_RenderCopy(renderer,output,NULL,&temp); 
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, backbuffer, NULL, NULL);


		SDL_RenderPresent(renderer);
	}
	SDL_WaitThread(calthread, NULL);
	close();
	return 0;
}

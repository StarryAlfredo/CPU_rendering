#include "renderWindow.h"

renderWindow::renderWindow(int w, int h,std::string title):windowHeight(h),windowWidth(w){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
	}

	win = SDL_CreateWindow(title.c_str(), 400, 100, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (ren == nullptr) {
		logSDLError(std::cout, "CreateRender");
		SDL_DestroyWindow(win);
		SDL_Quit();
	}

	if (win == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
	}
}

renderWindow::~renderWindow(){
	
	int len = img.size();
	for (int i = 0; i < len; ++i) {
		SDL_DestroyTexture(img[i]);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void renderWindow::renderTexture(int x, int y, int w, int h,int index){
	SDL_Rect dst;
	
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;

	SDL_RenderCopy(ren, img[index], nullptr, &dst);
}

void renderWindow::loadTexture(const std::vector<std::string>& file){
	
	int len = file.size();
	for (int i = 0; i < len; ++i) {
		SDL_Texture * tex = IMG_LoadTexture(ren, file[i].c_str());
		img.push_back(tex);
		if (tex == nullptr) {
			logSDLError(std::cout, "CreateTextureFormSurface");
		}
	}
}

void renderWindow::renderTexture(int x, int y,int index){
	SDL_Rect dst;

	dst.x = x;
	dst.y = y;

	int w, h;
	SDL_QueryTexture(img[index], NULL, NULL, &w, &h);
	dst.w = w;
	dst.h = h;

	SDL_RenderCopy(ren, img[index], nullptr, &dst);
}

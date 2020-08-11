#include "../tools/renderWindow.h"

renderWindow::renderWindow(int w, int h,std::string title):windowHeight(h),windowWidth(w){
	

	zBuffer = new float[w * h];
	depthBuffer = new float[w * h];
	colorBuffer = new TGAColor[w * h];

	ClearZBuffer();
	ClearColor();
	
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
	else {
		SDL_SetRenderDrawColor(ren, 0x2F, 0x4F, 0x4F, 0xFF);
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
	if (zBuffer != nullptr)
		delete zBuffer;
	if (depthBuffer != nullptr) {
		delete depthBuffer;
	}
	if (colorBuffer) {
		delete colorBuffer;
	}
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

void renderWindow::ClearColor() {
	memset(colorBuffer, 0, sizeof(TGAColor) * GetWidth() * GetHeight());
}

void renderWindow::DrawPointWithColor()
{	
	int nums = GetWidth() * GetHeight();
	int x, y;
	int height = GetHeight();
	int width = GetWidth();
	for (int i = 0; i < nums; ++i) {
		x = i % width ; 
		y = i / width;
		TGAColor color = colorBuffer[i];
		SDL_SetRenderDrawColor(ren, color[0], color[1], color[2], color[3]);
		SDL_RenderDrawPoint(ren, x, y);
	}
	
}

void renderWindow::ResetMouse(Uint32 x, Uint32 y)
{
	SDL_WarpMouseInWindow(win,
		x,
		y);

}

int renderWindow::GetWidth()
{	
	return windowWidth;
}

int renderWindow::GetHeight()
{	
	return windowWidth;
}

void renderWindow::ClearZBuffer() {
	memset(zBuffer, 0x7f, GetWidth() * GetHeight() * sizeof(float));
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


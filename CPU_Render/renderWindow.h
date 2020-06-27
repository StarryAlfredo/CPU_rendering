
#ifndef  _RENDERWINDOW_
#define  _RENDERWINDOW_
#include<string>
#include<iostream>
#include<vector>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

class renderWindow{
public:
	renderWindow(int w = 640, int h = 480,std::string title = "CPU_render");
	~renderWindow();
	void renderTexture(int x, int y,int index);
	void renderTexture(int x, int y, int w, int h,int index);
	void loadTexture(const std::vector<std::string> &file);
	void clearRender() { SDL_SetRenderDrawColor(ren, 0x70, 0x80, 0x90, 0xFF);  SDL_RenderClear(ren); }
	void renderPresent() { SDL_RenderPresent(ren); }
	void DrawPointWithColor(SDL_Color &r,int x,int y);
	void ResetMouse(Uint32 x, Uint32 y);
	int	 GetWidth();
	int  GetHeight();
private:
	int windowHeight;
	int windowWidth;
	SDL_Window *win;
	SDL_Renderer *ren;
	std::vector<SDL_Texture *> img;	

private:
	void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << "error" << SDL_GetError() << std::endl;
	}
};


#endif 



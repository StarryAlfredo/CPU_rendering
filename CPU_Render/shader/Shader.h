#ifndef _SHADER_
#define _SHADER_
#include"../tools/Math.h"
#include<SDL.h>
#include"../tools/renderWindow.h"
#include<cmath>


class Shader {
public:	
	virtual Vec4f  vertexShader(void *vIn ,void *vOut, void *uniform) = 0;
	virtual SDL_Color pixelShader(void *pIn ,void * uniform,bool &discard, int backface) = 0;
public:
	virtual ~Shader();
	Shader(){}
};


#endif // !_SHADER_
	
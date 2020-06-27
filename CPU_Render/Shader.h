#ifndef _SHADER_
#define _SHADER_
#include"geometry.h"
#include"model.h"
#include"camera.h"
#include<SDL.h>
#include"renderWindow.h"
#include"Light.h"
class Shader {

public:	
	virtual Vec3f vertexShader(Vec3f vin) = 0;
	virtual void  rasterStage(Vec3f (&s)[3],Vec3f (&v)[3],int width,int height) = 0;
	//, Vec3f(&normal)[3], TGAColor(&color)[3], std::vector<Light *> &Light
	virtual bool  pixelShader(Vec3f bar,SDL_Color &color, Vec3f(&normal)[3],Vec3f (&v)[3], std::vector<Light *> &Light) = 0;

	virtual ~Shader();

};

class MyShader : public Shader{		
	
public:
	MyShader(Model * model,FirstPersonCamera * camera_) :myModel(model),camera(camera_) {
	}
	~MyShader() {
	}	
	virtual Vec3f vertexShader(Vec3f vin);
	virtual bool  pixelShader(Vec3f bar, SDL_Color &color, Vec3f(&normal)[3], Vec3f(&v)[3], std::vector<Light *> &Light);
	void rasterStage(Vec3f(&s)[3], Vec3f(&v)[3],int width, int height);
private:
	Model* myModel;
	Camera *camera;		//≥ÈœÛ∏∏¿‡
};

void triangle(Vec2f (&uv)[3],Vec3f(&t)[3], Shader &shader, renderWindow &win, float *zBuffer,TGAColor (&Color)[3], Vec3f(&normal)[3], std::vector<Light *> &Light, Model *model);

 #endif // !_SHADER_

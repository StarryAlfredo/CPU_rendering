//列矩阵,右手坐标系

#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdio>
#include<string>
#include"Shader.h"
#include"Light.h"


using namespace std;


const int SCREEN_WIDTH	= 600;
const int SCREEN_HEIGHT = 600;

renderWindow myWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

//摄像机的位置
Vec3f eyePosition(0.f, 0, 2.f);
Vec3f focusePosition(0 ,0 ,-1.f);
Vec3f up(0,1.f,0);

//光源
std::vector<Light *> light;

//视口
float nearZ    = .1f  ;
float farZ	   = 100.f;
float FovAngle = 90;

float mouse_sensitivity = 0.00000000001f;
bool  firstMoveMouse = false;

Sint32 mouseLastX;
Sint32 mouseLastY;

float yaw	 =  0.0f;
float pitch  =  0.0f; 

float zBuffer[SCREEN_HEIGHT * SCREEN_WIDTH];

TGAImage image(SCREEN_HEIGHT, SCREEN_WIDTH, TGAImage::RGB);
Model * model;


void pipelineRun(Shader &myShader){

	int face = model->nfaces();	
	Vec3f screen[3];
	Vec3f normal[3];
	Vec2f uv[3];

	for (int i = 0; i < face; ++i){
		
		TGAColor color[3] = { TGAColor(255, 255, 255, 255),TGAColor(255, 255, 255, 255),TGAColor(255, 255, 255, 255) };
		
		for (int j = 0; j < 3; ++j){
			screen[j] = model->vert(i, j);	
			normal[j] = model->normal(i, j);
			uv[j] = model->uv(i,j);
		}
		triangle(uv,screen,myShader,myWindow ,zBuffer,color,normal,light, model);
	}
}

int main(int argc, char ** argv) {

	model = new Model("african_head.obj");

	Vec3f screen_coords[3];
	
	FirstPersonCamera *camera = new FirstPersonCamera;
	
	//相机初始化
	camera->LookAtLH(eyePosition, focusePosition, up);
	camera->PerspectiveFovLH(FovAngle, SCREEN_WIDTH / SCREEN_HEIGHT, nearZ, farZ);
	camera->UpdateViewMatrix();
	
	//光源初始化

	PointLight pointLight;
	DirectionalLight dLight;
	SpotLight		 spotLight;
	
	//平行光
	dLight.SetAmbientLight(Vec3f(0.3f,0.3f,0.3f));
	dLight.SetambientK(Vec3f(0.2f, 0.2f, 0.2f));
	dLight.SetDirection(Vec3f(0.f,0.f, 1.f)); 
	dLight.SetdiffuseK(Vec3f(0.8f, 0.8f, 0.8f));

	dLight.SetLightOfDiffuse(Vec3f(0.8f, 0.8f, 0.8f));
	dLight.SetLightOfSpecular(Vec3f(0.8f,0.8f,0.8f));
	dLight.SetSpecularK(Vec3f(0.3f, 0.3f, 0.3f));
	dLight.SetEyePosition(eyePosition);
	dLight.SetM(200.f);

	//点光
	pointLight.SetPosition(Vec3f(0.f,0.f,1.5f));
	pointLight.SetAmbientLight(Vec3f(0.3f, 0.3f, 0.3f));
	pointLight.SetambientK(Vec3f(0.1f, 0.1f, 0.1f));
	pointLight.SetdiffuseK(Vec3f(0.8f, 0.8f, 0.8f));

	pointLight.SetLightOfDiffuse(Vec3f(0.8f, 0.8f, 0.8f));
	pointLight.SetLightOfSpecular(Vec3f(0.5f, 0.5f, 0.5f));
	pointLight.SetSpecularK(Vec3f(0.1f, 0.1f, 0.1f));
	pointLight.SetEyePosition(eyePosition);
	pointLight.SetM(50.f);

	//聚光灯
	spotLight.SetPosition(Vec3f(0.f, 5.0f, 0.0f));
	spotLight.SetAmbientLight(Vec3f(0.3f, 0.3f, 0.3f));
	spotLight.SetambientK(Vec3f(0.8f, 0.8f, 0.8f));
	spotLight.SetdiffuseK(Vec3f(0.8f, 0.8f, 0.8f));
	spotLight.SetSpotDir(Vec3f(0.f, 6.f, 0.f));
	spotLight.SetSpot(2.f);

	spotLight.SetLightOfDiffuse(Vec3f(0.8f, 0.8f, 0.8f));
	spotLight.SetLightOfSpecular(Vec3f(0.5f, 0.5f, 0.5f));
	spotLight.SetSpecularK(Vec3f(0.3f, 0.3f, 0.3f));
	spotLight.SetEyePosition(eyePosition);
	spotLight.SetM(2.f);

	
	light.push_back(&dLight);

	MyShader shader(model,camera);
	
	//运行管线
	//pipelineRun(shader);
	//DEBUG输出结果
	//image.flip_vertically();
	//image.write_tga_file("output.tga");
	
	SDL_Event e;
	bool quit = false;
	Uint32 start = SDL_GetTicks();
	
	//SDL_TimerID timer = SDL_AddTimer(16, callback, &shader);
	
	
	Uint32 currentTime = 0;
	Uint32 lastTime = 0;
	float  deltaTime = 0;
	float  frequnce = 1.0f / 1000;

	
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	while (!quit) {
		currentTime = SDL_GetTicks();
		deltaTime = (float)(currentTime - lastTime ) * frequnce;
		lastTime = currentTime;

		if (deltaTime < 0.0f) {
			deltaTime = 0.f;
		}
		
		while (SDL_PollEvent(&e)){
			
			if (quit) {
				break;
			}
			if (e.type == SDL_QUIT){
				quit = true;
			}
			if (e.type == SDL_MOUSEMOTION) {
				//quit = true;
				if (firstMoveMouse) {
					mouseLastX = e.motion.x;
					mouseLastY = e.motion.y;
					firstMoveMouse = false;
				}

				//float offsetX = (e.motion.x - mouseLastX) * mouse_sensitivity;
				//float offsetY = (e.motion.y - mouseLastY)  * mouse_sensitivity;
				Uint32 offsetX = (e.motion.x - mouseLastX);
				Uint32 offsetY = (e.motion.y - mouseLastY);
			
			
				printf("%d  %d  %d\n",offsetX , e.motion.x, e.motion.y);
				
				//x = (x - inputObject.Delta.X * SENSITIVITY.X * ZoomSensitivity()) % (math.pi * 2)
				//y = math.clamp(y - inputObject.Delta.Y * SENSITIVITY.Y * ZoomSensitivity(), MIN_Y, MAX_Y)

					yaw = offsetX * deltaTime * mouse_sensitivity;
				//	pitch += offsetY;
				
			//	camera->RotateY(yaw);
				//camera->Pitch(pitch * deltaTime);

				//myWindow.ResetMouse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
				//mouseLastX = e.motion.x;
				//mouseLastY = e.motion.y;
				//mouseLastX = 300;
				//mouseLastY = 300;

			  //  camera->SetDirty(true);
			}
			
			if(e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
					case SDLK_w:		
						camera->Walk(3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_s:
						camera->Walk(-3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_a:
						camera->Strafe(3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_d:
						camera->Strafe(-3.0f * deltaTime);
						camera->SetDirty(true);
						break;

					case SDLK_F1:
						light.pop_back();
						light.push_back(&dLight);
						break;
					case SDLK_F2:
						light.pop_back();
						light.push_back(&pointLight);
						break;
					case SDLK_F3:
						light.pop_back();
						light.push_back(&spotLight);
						break;
					case SDLK_ESCAPE:
						quit = true;
						printf("%f %f %f",camera->GetPosition().x, camera->GetPosition().y,camera->GetPosition().z);
						break;
				}
			}
	
		}

		if (camera->GetIsDirty()) {
			camera->UpdateViewMatrix();
			camera->SetDirty(false);
			pointLight.SetEyePosition(camera->GetPosition());
			spotLight.SetEyePosition(camera->GetPosition());

		}
		memset(zBuffer ,0xfe , sizeof(zBuffer));
	
		myWindow.clearRender();
		pipelineRun(shader);
		myWindow.renderPresent();	

	}

	//SDL_RemoveTimer(timer);
	
	delete model;
	delete camera;

	return 0;
}

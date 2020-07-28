/*
****************************

Object -> Pipeline -> Shader
Author:Alfredo
StartTime:2020.5.30

****************************
*/

#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdio>
#include<string>
#include"../shader/Shader.h"
#include"../tools/Light.h"
#include"../tools/Object.h"
#include"../shader/Pipeline.h"

using namespace std;


const int SCREEN_WIDTH	= 600;
const int SCREEN_HEIGHT = 600;

renderWindow myWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

//摄像机的位置
Vec3f eyePosition(0.f, 0.f, -1.8f);
Vec3f focusePosition(0.f, 0.f,1.f);
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


int main(int argc, char ** argv) {

	//资源定义
	blinn_uniform uniform;
	model = new Model("assets//helmet//helmet.obj");	
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
	dLight.SetDirection(Vec3f(0.f,0.0f, 1.0f)); 
	dLight.SetLightOfDiffuse(Vec3f(0.8f, 0.8f, 0.8f));
	dLight.SetLightOfSpecular(Vec3f(0.8f,0.8f,0.8f));
	dLight.SetEyePosition(eyePosition);

	//点光
	pointLight.SetPosition(Vec3f(0.f,10.f,3.5f));
	pointLight.SetAmbientLight(Vec3f(0.3f, 0.3f, 0.3f));
	pointLight.SetLightOfDiffuse(Vec3f(0.8f, 0.8f, 0.8f));
	pointLight.SetLightOfSpecular(Vec3f(0.5f, 0.5f, 0.5f));
	pointLight.SetEyePosition(eyePosition);

	//聚光灯
	spotLight.SetPosition(Vec3f(0.f, 5.0f, 0.0f));
	spotLight.SetAmbientLight(Vec3f(0.3f, 0.3f, 0.3f));
	spotLight.SetSpotDir(Vec3f(0.f, 6.f, 0.f));
	spotLight.SetSpot(2.f);
	spotLight.SetLightOfDiffuse(Vec3f(0.8f, 0.8f, 0.8f));
	spotLight.SetLightOfSpecular(Vec3f(0.5f, 0.5f, 0.5f));
	spotLight.SetEyePosition(eyePosition);
	light.push_back(&dLight);

	//设置材质
	Material material;
	material.name = "stone";
	material.DiffuseAlbedo = Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
	material.Roughness = Vec4f(0.5f, 0.5f, 0.5f, 0.0f);

	//世界矩阵
	Matrix matrix;
	matrix = MatrixRotationX(- M_PI / 2.f);

	//初始化资源
	uniform.camera = camera;
	uniform.light = &light;
	uniform.Model = model;
	uniform.worldMatrix = &matrix;
	uniform.material = &material;

	//创建渲染对象
	string name = "BlinnShader";
	Object object(name, model);

	Shader *shader = new BlinnShader;
	Pipeline pipeline(shader, sizeof(blinn_vertexIn), sizeof(blinn_vertexOut), sizeof(blinn_uniform), (void *)&uniform);
	
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
		
		while (SDL_PollEvent(&e)) {
			
			if (quit) {
				break;
			}
			if (e.type == SDL_QUIT) {
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
				// float offsetY = (e.motion.y - mouseLastY)  * mouse_sensitivity;
				//Uint32 offsetX = (e.motion.x - mouseLastX);
				//Uint32 offsetY = (e.motion.y - mouseLastY);
				
			
				//printf("%d  %d  %d\n",offsetX , e.motion.x, e.motion.y);
				
				//x = (x - inputObject.Delta.X * SENSITIVITY.X * ZoomSensitivity()) % (math.pi * 2)
				//y = math.clamp(y - inputObject.Delta.Y * SENSITIVITY.Y * ZoomSensitivity(), MIN_Y, MAX_Y)

				//	yaw = offsetX * deltaTime * mouse_sensitivity;
				//	pitch += offsetY;
				
				//	camera->RotateY(yaw);
				//camera->Pitch(pitch * deltaTime);

				//myWindow.ResetMouse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
				//mouseLastX = e.motion.x;
				//mouseLastY = e.motion.y;
				//mouseLastX = 300;
				//mouseLastY = 300;

			   // camera->SetDirty(true);
			}
			
			if(e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_w:		
						camera->Walk(3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_s:
						camera->Walk(-3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_a:
						camera->Strafe(-3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_d:
						camera->Strafe(3.0f * deltaTime);
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

		myWindow.clearRender();
		object.Draw(pipeline, myWindow);
		myWindow.renderPresent();	

	}

	//SDL_RemoveTimer(timer);
	
	delete model;
	delete camera;
	delete shader;

	return 0;
}

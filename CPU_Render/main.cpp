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
#include"tools/Color.h"
using namespace std;


const int SCREEN_WIDTH	= 600;
const int SCREEN_HEIGHT = 600;

renderWindow myWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

//摄像机的位置
Vec3f eyePosition(0.f, 0.f, 0.0f);
Vec3f focusePosition(0.f, 0.f, 1.f);
Vec3f up(0,1.f,0);

//光源
std::vector<Light *> light;

//视口
float nearZ    = .1f;
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
Model * model_of_helmet, *model_of_sphere;


int main(int argc, char ** argv) {

	//资源定义
	blinn_uniform blin_uniform;
	skyboxshader_uniform sky_uniform;
	model_of_helmet = new Model("assets//helmet//helmet.obj");
	model_of_sphere = new Model("assets//sphere//sphere.obj");
	model_of_sphere->LoadCubeTexture("assets//sky//sunset//sunset.");

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
	material.DiffuseAlbedo = Vec4f(0.8f, 0.8f, 0.8f, 0.5f);
	material.Roughness = Vec4f(0.5f, 0.5f, 0.5f, 0.0f);

	//世界矩阵
	Matrix matrix;
	matrix =  MatrixTranslation(Vec3f(0.0f, 0.0f, 2.0f)) * MatrixRotationX(- M_PI / 2.f) ;

	//初始化头盔资源
	blin_uniform.camera = camera;
	blin_uniform.light = &light;
	blin_uniform.Model = model_of_helmet;
	blin_uniform.worldMatrix = &matrix;
	blin_uniform.material = &material;
	int blinn_vertex_in_size = sizeof(blinn_vertexIn);
	int blinn__vertex_out_size = sizeof(blinn_vertexOut);
	int blinn_uniform_size = sizeof(blinn_uniform);

	//初始化天空盒资源
	Matrix matrix_sphere = MatrixScale(Vec3f(1000.0f, 1000.0f, 1000.f)) * MatrixRotationX(M_PI);
	sky_uniform.Model = model_of_sphere;
	sky_uniform.worldMatrix = &matrix_sphere;
	sky_uniform.camera = camera;
	int sky_vertex_in_size = sizeof(skyboxshader_vertex_in);
	int sky_vertex_out_size = sizeof(skyboxshader_vertex_out);
	int sky_uniform_size = sizeof(skyboxshader_uniform);

	//创建渲染对象
	Object object_of_helment(string("BlinnShader"), model_of_helmet);
	Object object_of_sphere(string("SkyBoxShader"), model_of_sphere);

	Shader *shader_of_blinn = new BlinnShader;
	Shader *shader_of_sky = new SkyboxShader;

	bool wireFrame = false;

	//不同的着色器渲染管线
	Pipeline pipeline_of_blinn(shader_of_blinn, blinn_vertex_in_size,blinn__vertex_out_size ,blinn_uniform_size , (void *)&blin_uniform, wireFrame);
	pipeline_of_blinn.zBufferState = true;
	pipeline_of_blinn.backCull = true;
	
	Pipeline pipeline_of_sky(shader_of_sky, sky_vertex_in_size,sky_vertex_out_size, sky_uniform_size, (void *)&sky_uniform, wireFrame);
	pipeline_of_sky.backCull = false;
	pipeline_of_sky.zBufferState = false;

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
					case SDLK_SPACE:
						camera->UP(-3.0f * deltaTime);
						camera->SetDirty(true);
						break;
					case SDLK_e:
						pipeline_of_blinn.zBuffer_is_write = !pipeline_of_blinn.zBuffer_is_write;
						pipeline_of_blinn.color_factor_src_ = pipeline_of_blinn.zBuffer_is_write ? BLEND_FACTOR_ONE : BLEND_FACTOR_SRC_ALPHA;
						pipeline_of_blinn.color_factor_dst_ = pipeline_of_blinn.zBuffer_is_write ? BLEND_FACTOR_ZERO : BLEND_FACTOR_INV_SRC_ALPHA;
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
					case SDLK_q:
						wireFrame = !wireFrame;
						pipeline_of_blinn.SetWireframe(wireFrame);
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
		object_of_sphere.Draw(pipeline_of_sky, myWindow);
		object_of_helment.Draw(pipeline_of_blinn, myWindow);
		
		myWindow.DrawPointWithColor();
		myWindow.renderPresent();	

	}

	//SDL_RemoveTimer(timer);
	
	delete model_of_helmet;
	delete model_of_sphere;
	delete camera;
	delete shader_of_blinn;
	delete shader_of_sky;

	return 0;
}

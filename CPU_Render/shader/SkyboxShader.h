#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H
#include"Shader.h"
#include"../tools/model.h"
#include"../tools/camera.h"
struct skyboxshader_vertex_in {
	Vec3f PosL;
};

struct skyboxshader_vertex_out {
	Vec3f Direction;
};

struct  skyboxshader_uniform {
	Model*  Model;
	Matrix*  worldMatrix;
	Camera*  camera;
};

class SkyboxShader: public Shader {
public:
	Vec4f  vertexShader(void *vIn, void *vOut, void *uniform);
	TGAColor pixelShader(void *pIn, void * uniform, bool &discard, int backface);
};


#endif // !SKYBOX_H

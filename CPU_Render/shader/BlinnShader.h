#ifndef BLINNSHADER_H
#define BLINNSHADER_H
#include"../shader/Shader.h"
#include"../tools/camera.h"
#include"../tools/Light.h"
#include"../tools/model.h"
#include"../tools/Material.h"
#include"../tools/Color.h"
struct blinn_vertexIn{
	Vec3f PosL;
	Vec3f NormalL;
	Vec2f Texcoord;
	Vec3f TangentL;
};

struct blinn_vertexOut{	
	Vec4f depthPos;
	Vec3f PosW;
	Vec3f NormalW;
	Vec3f TangentW;
	Vec2f Texcoord;
};

struct blinn_uniform {
	std::vector<Light*>* light;
	Camera*	camera;
	Model*  Model;
	Matrix* worldMatrix;
	Matrix* nomal_Matrix;
	Material* material;
	Matrix* light_vp;
	renderWindow* ren;
	float alpha_cut;
	bool shadow;
};

class BlinnShader: public Shader{

public:
	Vec4f  vertexShader(void *vIn, void *vOut, void *uniform) override;
	TGAColor pixelShader (void *pIn, void * uniform, bool& discard, int backface) override;
};

#endif
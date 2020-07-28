#ifndef BLINNSHADER_H
#define BLINNSHADER_H
#include"../shader/Shader.h"
#include"../tools/camera.h"
#include"../tools/Light.h"
#include"../tools/model.h"
struct blinn_vertexIn{
	Vec3f PosL;
	Vec3f NormalL;
	Vec2f Texcoord;
};

struct blinn_vertexOut{
	Vec3f PosW;
	Vec3f NormalW;
	Vec2f Texcoord;
};

struct blinn_uniform {
	std::vector<Light*>	 *	light;
	Camera				 *	camera;
	Model				 *  Model;
	Matrix				 *  worldMatrix;
};
struct blinn_material {
	Vec4f	DiffuseAlbedo;
	Vec4f	FresnelR0;
	float	Roughness;
};
class BlinnShader: public Shader{

public:
	Vec4f  vertexShader(void *vIn, void *vOut, void *uniform) override;
	SDL_Color pixelShader (void *pIn, void * uniform, bool& discard, int backface) override;
};

#endif
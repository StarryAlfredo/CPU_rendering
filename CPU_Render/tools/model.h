#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "Math.h"
#include "../tools/tgaimage.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
	TGAImage skybox_[6];
	void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f normal(int iface, int nthvert);
	Vec3f normal(Vec2f uv);
	Vec3f vert(int i);
	Vec3f vert(int iface, int nthvert);
	Vec2f uv(int iface, int nthvert);
	TGAColor diffuse(Vec2f uv);
	TGAColor diffuse(Vec2f uv, int face);
	float specular(Vec2f uv);
	std::vector<int> face(int idx);
	void LoadCubeTexture(std::string filename);
	TGAColor cube_map(Vec3f Direction);
};
#endif //__MODEL_H__
#include <iostream>
#include <fstream>
#include <sstream>
#include "../tools/model.h"

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_() {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			Vec3f n;
			for (int i = 0; i < 3; i++) iss >> n.raw[i];
			norms_.push_back(n);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec2f uv;
			for (int i = 0; i < 2; i++)  {
				iss >> uv.raw[i];
				if (uv.raw[i] > 1.f)
					uv.raw[i] -= 1.f;
			}
			uv_.push_back(uv);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<Vec3i> f;
			Vec3i tmp;
			iss >> trash;
			while (iss >> tmp.raw[0] >> trash >> tmp.raw[1] >> trash >> tmp.raw[2]) {
				for (int i = 0; i < 3; i++) tmp.raw[i]--; // in wavefront obj all indices start at 1, not zero
				f.push_back(tmp);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
	load_texture(filename, "_diffuse.tga", diffusemap_);
	load_texture(filename, "_nm.tga", normalmap_);
	load_texture(filename, "_spec.tga", specularmap_);
}

Model::~Model() {}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
	std::vector<int> face;
	for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i].raw[0]);
	return face;
}

void Model::LoadCubeTexture(std::string filename) {

	load_texture(filename, "_posX.tga", skybox_[0]);
	load_texture(filename, "_posY.tga", skybox_[1]);
	load_texture(filename, "_posZ.tga", skybox_[2]);
	load_texture(filename, "_negX.tga", skybox_[3]);
	load_texture(filename, "_negY.tga", skybox_[4]);
	load_texture(filename, "_negZ.tga", skybox_[5]);
}

TGAColor Model::cube_map(Vec3f Direction) {
	int MAX = 2;

		for (int i = 0; i < 3; ++i) {
			if (std::fabs(Direction[i]) > std::fabs(Direction[MAX])) {
			MAX = i;
		}
	}
	const float max = abs(Direction[MAX]);
	MAX = MAX + (Direction[MAX] > 0.f ? 0 : 3);
	
	float sc, tc;

	switch (MAX) {
		case 0:
			sc = -Direction.z;
			tc = -Direction.y;//+x
			break;
		case 1:
			sc = Direction.x;//+y
			tc = Direction.z;
			break;
		case 2:
			sc = Direction.x;//+z
			tc = -Direction.y;
			break;
		case 3:
			sc = Direction.z;//-x
			tc = -Direction.y;
			break;
		case 4:
			sc = Direction.x;//-y
			tc = -Direction.z;
			break;
		case 5:
			sc = -Direction.x;//-z
			tc = -Direction.y;
			break;
	}

	Vec2f texcoord;
	texcoord.x = (sc / max + 1) * 0.5f;
	texcoord.y = (tc / max + 1) * 0.5f;

	texcoord.y = 1.f - texcoord.y;

	texcoord.x -= (float)floor(texcoord.x);
	texcoord.y -=  (float)floor(texcoord.y);

	return diffuse(texcoord, MAX);
}

Vec3f Model::vert(int i) {
	return verts_[i];
}

Vec3f Model::vert(int iface, int nthvert) {
	return verts_[faces_[iface][nthvert][0]];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img.flip_vertically();
	}
}

TGAColor Model::diffuse(Vec2f uvf) {

	float u = uvf[0] * diffusemap_.get_width();
	float v = uvf[1] * diffusemap_.get_height();
	
	Vec2i x[4];

	int width = diffusemap_.get_width();
	int height = diffusemap_.get_height();
	int srcU = (int)u, srcV = int(v);

	x[0] = Vec2i(srcU, srcV);
	x[1] = Vec2i(srcU, srcV + 1 > height ? srcV - 1 : srcV + 1);
	x[2] = Vec2i(srcU + 1 > width ? srcU - 1 : srcU + 1, srcV + 1 > height ? srcV - 1 : srcV + 1);
	x[3] = Vec2i(srcU + 1 > width ? srcU - 1 : srcU + 1, srcV);

	float s = 1.f - (u - srcU);
	float t = 1.f - (v - srcV);

	TGAColor color_A;
	TGAColor color_B;
	TGAColor color[4];

	for (int i = 0; i < 4; ++i) {
		color[i] = diffusemap_.get(x[i].u, x[i].v);
	}

	color_A = Lerp(color[0], color[3], s);
	color_B = Lerp(color[1], color[2], s);
	
	TGAColor result = Lerp(color_A, color_B, t);

	return result;
}

TGAColor Model::diffuse(Vec2f uvf, int face) {
	int u = uvf[0] * skybox_[face].get_width();
	int v = uvf[1] * skybox_[face].get_height();
	return skybox_[face].get(u, v);
	/*float u = uvf[0] * skybox_[face].get_width();
	float v = uvf[1] * skybox_[face].get_height();

	Vec2i x[4];

	int width = skybox_[face].get_width();
	int height = skybox_[face].get_height();
	int srcU = (int)u, srcV = (int)v;

	x[0] = Vec2i(srcU, srcV);
	x[1] = Vec2i(srcU, srcV + 1 > height ? srcV - 1 : srcV + 1);
	x[2] = Vec2i(srcU + 1 > width ? srcU - 1 : srcU + 1, srcV + 1 > height ? srcV - 1 : srcV + 1);
	x[3] = Vec2i(srcU + 1 > width ? srcU - 1 : srcU + 1, srcV);

	float s = 1.f - (u - srcU);
	float t = 1.f - (v - srcV);

	TGAColor color_A;
	TGAColor color_B;
	TGAColor color[4];

	for (int i = 0; i < 4; ++i) {
		color[i] = skybox_[face].get(x[i].u, x[i].v);
	}

	color_A = Lerp(color[0], color[3], s);
	color_B = Lerp(color[1], color[2], s);

	TGAColor result = Lerp(color_A, color_B, t);

	return result;*/

}

Vec3f Model::normal(Vec2f uvf) {
	Vec2i uv(uvf.raw[0] * normalmap_.get_width(), uvf.raw[1] * normalmap_.get_height());
	TGAColor c = normalmap_.get(uv.raw[0], uv[1]);
	Vec3f res;
	for (int i = 0; i < 3; i++)
		res.raw[2 - i] = (float)c[i] / 255.f*2.f - 1.f;
	return res;
}

Vec2f Model::uv(int iface, int nthvert) {
	return uv_[faces_[iface][nthvert][1]];
}

float Model::specular(Vec2f uvf) {
	Vec2i uv(uvf.raw[0] * specularmap_.get_width(), uvf.raw[1] * specularmap_.get_height());
	return specularmap_.get(uv.raw[0], uv.raw[1])[0] / 1.f;
}

Vec3f Model::normal(int iface, int nthvert) {
	int idx = faces_[iface][nthvert][2];
	Vec3f temp = norms_[idx];
	temp.normalize();
	return temp;
}
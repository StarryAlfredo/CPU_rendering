#ifndef MATERIAL_H
#define MATERIAL_H
#include"Math.h"

class Material {  
 public:
	 
 public:
	 std::string  name;
	 Vec4f	DiffuseAlbedo;
	 Vec4f	FresnelR0;
	 Vec4f	Roughness;

};

#endif // !MATERIAL_H

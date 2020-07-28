#include "../shader/BlinnShader.h"

Vec4f BlinnShader::vertexShader(void * vIn, void * vOut, void * uniform) {

	blinn_vertexIn* VIN	= static_cast<blinn_vertexIn*>(vIn);
	blinn_vertexOut* VOUT = static_cast<blinn_vertexOut*>(vOut);
	blinn_uniform * UNIFORM	= static_cast<blinn_uniform*>(uniform);
		
	Vec4f PosW = *(UNIFORM->worldMatrix) * Vec4f(VIN->PosL, 1.0f);
	Vec4f PosH =  (UNIFORM->camera->GetProjection()) * ((UNIFORM->camera->GetView()) * PosW);

	VOUT->NormalW =   VIN->NormalL;
	VOUT->PosW =  Vec3f(PosW.x, PosW.y, PosW.z);
	VOUT->Texcoord = VIN->Texcoord;

	return PosH;
}

TGAColor  BlinnShader::pixelShader(void* pIn, void * uniform,  bool& discard, int backface){
	
	blinn_vertexOut * PIN = static_cast<blinn_vertexOut*>(pIn);
	blinn_uniform * UNIFORM = static_cast<blinn_uniform*>(uniform);

	TGAColor color = UNIFORM->Model->diffuse(PIN->Texcoord);

	Vec3f intensity;
	float alpha = UNIFORM->material->DiffuseAlbedo.a;

	if (UNIFORM->alpha_cut > 0 && alpha < UNIFORM ->alpha_cut) {
		discard = 1;
		return TGAColor();
	} 

	PIN->NormalW.normalize();

	int size = UNIFORM->light->size();
	std::vector<Light*> &light = *(UNIFORM->light);

	for (int i = 0; i < size; ++i) {
		if (dynamic_cast<PointLight*>(light[i]) != nullptr || dynamic_cast<SpotLight*>(light[i]))
			light[i]->SetDirection(PIN->PosW);
		intensity = light[i]->ColorShader(PIN->NormalW, *(UNIFORM->material));

	}
	color = color * intensity;
	color[3] = alpha * 255;
	return color;
}

#include "../shader/BlinnShader.h"
bool IsInShadow(Vec3f ndc, renderWindow& ren, float n_dot_l) {
	float u = (ndc.x + 1) * 0.5f;
	float v = (ndc.y + 1) * 0.5f;
	float d = (ndc.z + 1) * 0.5f;

	u = u - (float)floor(u);
	v = v - (float)floor(v);

	float depth_bias = std::max(0.05f * (1 - n_dot_l), 0.005f);
	float current_depth = d - depth_bias;

	int c = (int)((ren.GetWidth() - 1) * u);
	int r = (int)((ren.GetHeight() - 1) * v);

	int index = r * ren.GetWidth() + c;
	
	float clost  = ren.depthBuffer[index];

	return clost < d - 0.005;

	
}

Vec4f BlinnShader::vertexShader(void * vIn, void * vOut, void * uniform) {

	blinn_vertexIn* VIN	= static_cast<blinn_vertexIn*>(vIn);
	blinn_vertexOut* VOUT = static_cast<blinn_vertexOut*>(vOut);
	blinn_uniform * UNIFORM	= static_cast<blinn_uniform*>(uniform);
	
	Vec4f PosW = *(UNIFORM->worldMatrix) * Vec4f(VIN->PosL, 1.0f);
	Vec4f PosH;

	if(UNIFORM->shadow)
		PosH = (*(UNIFORM->light_vp) * PosW);
	else
		PosH =  (UNIFORM->camera->GetProjection()) * ((UNIFORM->camera->GetView()) * PosW);

	Vec4f nomalW = *(UNIFORM->worldMatrix) * Vec4f(VIN->NormalL, 1.0f);
	
	VOUT->depthPos = *(UNIFORM->light_vp) * PosW;
	VOUT->NormalW = nomalW.rgb().GetNormalize();
	VOUT->TangentW = *(UNIFORM->worldMatrix) * VIN->TangentL;
	VOUT->PosW = Vec3f(PosW.x, PosW.y, PosW.z);
	VOUT->Texcoord = VIN->Texcoord;
			
	return PosH;
}

TGAColor  BlinnShader::pixelShader(void* pIn, void * uniform,  bool& discard, int backface){
	

	blinn_vertexOut * PIN = static_cast<blinn_vertexOut*>(pIn);
	blinn_uniform * UNIFORM = static_cast<blinn_uniform*>(uniform);
	
	if (UNIFORM->shadow)
		return TGAColor(255, 255, 255, 255);

	const int width = UNIFORM->ren->GetWidth();
	const int height = UNIFORM->ren->GetHeight();

	Vec3f map_normal = UNIFORM->Model->normal(PIN->Texcoord);
	//float k = 1.f / 255.f;
	//map_normal = map_normal * k;
	Vec3f normal = NormalSampleToWorldSpace(map_normal, PIN->NormalW, PIN->TangentW);

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

	float t = 1.f / PIN->depthPos.w;
	Vec3f In = Vec3f(PIN->depthPos.x * t, PIN->depthPos.y * t, PIN->depthPos.z * t);
	float visible = 1.0f;
	float dot = light[0]->GetDirection().Dot(PIN->NormalW);
	if (IsInShadow(In, *(UNIFORM->ren), dot)) {
		visible = 0.5f;
		//return TGAColor(255, 255, 255, 255);
	}

	for (int i = 0; i < size; ++i) {	
		if (dynamic_cast<PointLight*>(light[i]) != nullptr || dynamic_cast<SpotLight*>(light[i]))
			light[i]->SetDirection(PIN->PosW);
		intensity = light[i]->ColorShader(normal, *(UNIFORM->material), visible);
	}


	color = color * intensity * visible;
	color[3] = alpha * 255;
	return color;
}

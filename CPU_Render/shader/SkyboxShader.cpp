#include "SkyboxShader.h"

Vec4f SkyboxShader::vertexShader(void * vIn, void * vOut, void * uniform) {
	skyboxshader_vertex_in * VIN = static_cast<skyboxshader_vertex_in*>(vIn);
	skyboxshader_vertex_out * VOUT = static_cast<skyboxshader_vertex_out*>(vOut);
	skyboxshader_uniform * UNIFORM = static_cast<skyboxshader_uniform*>(uniform);

	Matrix PV = UNIFORM->camera->GetProjection() * UNIFORM->camera->GetView();

	Vec4f PosW =  (PV * (*(UNIFORM->worldMatrix) * Vec4f(VIN->PosL, 1.0f)));
	
	PosW.z = PosW.w;
	VOUT->Direction =VIN->PosL;

	return PosW;
}

TGAColor SkyboxShader::pixelShader(void * pIn, void * uniform, bool & discard, int backface) {
	skyboxshader_vertex_out * VOUT = static_cast<skyboxshader_vertex_out*>(pIn);
	skyboxshader_uniform * UNIFORM = static_cast<skyboxshader_uniform*>(uniform);

	TGAColor color =  UNIFORM->Model->cube_map(VOUT->Direction);
	return color;
}

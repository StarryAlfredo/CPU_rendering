#include "../tools/Object.h"

Object::Object(std::string name_t,
			   Model * model_t)
	: shaderName_(name_t),
	  mesh_(model_t) {	

}

Object::~Object() {
	
}

void Object::Draw(Pipeline& pipeline, renderWindow& ren)
{	
	if (shaderName_ == "BlinnShader") {
		int faceOfNum = mesh_->nfaces();
		
		for (int i = 0; i < faceOfNum; ++i) {
			for (int j = 0; j < 3; ++j) {
				Vec3f pos = mesh_->vert(i, j);
				Vec3f normal = mesh_->normal(i, j);

				blinn_vertexIn* vertex_In = (blinn_vertexIn*)pipeline.GetShaderVertexIn(j);
				vertex_In->PosL = pos;
				vertex_In->NormalL = normal;
				vertex_In->Texcoord = mesh_->uv(i, j);
				vertex_In->TangentL = mesh_->tangent(i, j);
			}
			pipeline.PipelineRun(ren);
		}

		

	} else if (shaderName_ == "SkyBoxShader") {
		int faceOfNum = mesh_->nfaces();

		for (int i = 0; i < faceOfNum; ++i) {
			for (int j = 0; j < 3; ++j) {
				Vec3f pos = mesh_->vert(i, j);
				skyboxshader_vertex_in* vertex_In = (skyboxshader_vertex_in*)pipeline.GetShaderVertexIn(j);
				vertex_In->PosL = pos;
			}
			pipeline.PipelineRun(ren);

		}
	} else if (shaderName_ == "PBRShader") {


	}
}

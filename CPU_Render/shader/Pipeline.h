/**************
 	
 Author:Alfredo
 PIPELINE:  
 -> Run -> use vertexShader -> clipping -> draw Traingle -> use pixelTraingle; 

**************/

#ifndef PIPELINE_H
#define PIPELINE_H
#include"../shader/Shader.h"
#include<cassert>
#include "../tools/Color.h"

#define MAX_VERTEX 20
#define EPSILON 1e-5

class Pipeline{
 public:
	Pipeline(Shader* shader, int sizeof_vertex_in,
			 int sizeof_vertex_out,
			 int sizeof_uniform,
			 void* uniform,
		     bool  wireframe);

	~Pipeline();
	void* GetShaderVertexIn(int i);
	void PipelineRun(renderWindow& ren);
	void SetWireframe(bool temp);
	void ChangeShader(Shader* shader, int sizeof_vertex_in,
					  int sizeof_vertex_out,
					  int sizeof_uniform,
				 	  void* uniform);
	void* GetUniform();
	void SaveDelete();
	

 public:
	//设置颜色混合或者透明物体
	BLEND_FACTOR color_factor_src_;
	BLEND_FACTOR color_factor_dst_;
	BLEND_OP	 color_op_;
	bool zBuffer_is_write;
	bool zBufferState;
	bool backCull;
	
 private:
	bool RasterzieTriangle(Vec4f clip_coords[3], void* vOut[3], renderWindow& ren);
	void DrawFragment(Vec2i P, float depth, int backface, renderWindow& ren);
	void DrawFragment(Vec2i start, Vec2i End, renderWindow& ren);
	bool IsVertexVisible(Vec4f &v);
	int  ClippingTraingle();
	//Vec3f Rasterzie(Vec3f& clip_coords, int& width, int& height);
	void InterpolateVertexOut(void* vertex_out[3], Vec3f& weights, float weight_argum[3]);
	
 private:	

	 Shader* shader_;
	 int sizeof_vertex_in_;
	 int sizeof_vertex_out_;
	 int sizeof_uniform_;
	 bool wireframe_;
	 /* shader var */

	 void* shader_vertex_in_[3];
	 void* shader_vertex_out_;
	 void* shader_uniform_;

	/* clipping var */
	 Vec4f in_coords_[MAX_VERTEX];	
	 Vec4f out_coords_[MAX_VERTEX];
	 void* vertex_out_in_[MAX_VERTEX];
	 void* vertex_out_out_[MAX_VERTEX];
 
};



#endif
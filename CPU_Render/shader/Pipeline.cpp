#include "../shader/Pipeline.h"
#include<algorithm>

enum plane {
	POSITIVE_W,
	POSITIVE_X,
	NEGATIVE_X,
	POSITIVE_Y,
	NEGATIVE_Y,
	POSITIVE_Z,
	NEGATIVE_Z,
};

bool IsInsidePlane(Vec4f& coord, plane plane_t) {

	switch (plane_t) {
	case POSITIVE_W:
		return coord.w >= EPSILON;
	case POSITIVE_X:
		return coord.x <= +coord.w;
	case NEGATIVE_X:
		return coord.x >= -coord.w;
	case POSITIVE_Y:
		return coord.y <= +coord.w;
	case NEGATIVE_Y:
		return coord.y >= -coord.w;
	case POSITIVE_Z:
		return coord.z <= +coord.w;
	case NEGATIVE_Z:
		return coord.z >= -coord.w;
	}														
}


float GetInsersectRatio(Vec4f& pre, Vec4f& cur, plane plane_t) {

	switch (plane_t) {
		case POSITIVE_W:
			return (pre.w - EPSILON) / (pre.w - cur.w);
		case POSITIVE_X:
			return (pre.w - pre.x) / ((pre.w - pre.x) - (cur.w - cur.x));
		case NEGATIVE_X:
			return (pre.w + pre.x) / ((pre.w + pre.x) - (cur.w + cur.x));
		case POSITIVE_Y:
			return (pre.w - pre.y) / ((pre.w - pre.y) - (cur.w - cur.y));
		case NEGATIVE_Y:
			return (pre.w + pre.y) / ((pre.w + pre.y) - (cur.w + cur.y));
		case POSITIVE_Z:
			return (pre.w - pre.z) / ((pre.w - pre.z) - (cur.w - cur.z));
		case NEGATIVE_Z:
			return (pre.w + pre.z) / ((pre.w + pre.z) - (cur.w + cur.z));
	}
}


bool IsBackFace(Vec3f t[3]) {
	Vec3f a = t[0];
	Vec3f b = t[1];
	Vec3f c = t[2];

	float signed_area = 0.0f;

	signed_area += a.x * b.y - a.y * b.x;
	signed_area += b.x * c.y - c.x * b.y;
	signed_area += c.x * a.y - c.y * a.x;

	return signed_area <= 0;
}


int HomogeneousClipping(plane plane_t, int num_vertex, int varying_num_floats,
	Vec4f in_coords[MAX_VERTEX], void* in_varyings[MAX_VERTEX],
	Vec4f out_coords[MAX_VERTEX], void* out_varyings[MAX_VERTEX]) {
	int out_nums = 0;

	assert(num_vertex >= 3 && num_vertex <= MAX_VERTEX);
	for (int i = 0; i < num_vertex; ++i) {
		int pre_index = (i - 1 + num_vertex) % num_vertex;
		int cur_index = i;

		Vec4f pre = in_coords[pre_index];
		Vec4f cur = in_coords[cur_index];
		float *pre_varyings = (float*)in_varyings[pre_index];
		float *cur_varyings = (float*)in_varyings[cur_index];

		bool pre_inside = IsInsidePlane(pre, plane_t);
		bool cur_inside = IsInsidePlane(cur, plane_t);

		if (pre_inside != cur_inside) {
			float ratio = GetInsersectRatio(pre, cur, plane_t);

			out_coords[out_nums] = Lerp(pre, cur, ratio);

			for (int i = 0; i < varying_num_floats; ++i) {
				float* dest = (float*)out_varyings[i];
				*dest = Lerp(pre_varyings[i], cur_varyings[i], ratio);
			}
			out_nums += 1;
		}

		if (cur_inside) {

			Vec4f *dest_coord = &out_coords[out_nums];
			float *dest_varyings = (float*)out_varyings[out_nums];
			int sizeof_varyings = varying_num_floats * sizeof(float);

			*dest_coord = cur;
 			memcpy(dest_varyings, cur_varyings, sizeof_varyings);

			out_nums += 1;
		}

	}

	assert(out_nums <= MAX_VERTEX);
	return out_nums;
}



#define CLIP_IN2OUT(plane_t)                                                  \
    do {																	  \
        nums = HomogeneousClipping(											  \
            plane_t, nums, num_of_vertexout_float,							  \
            in_coords_, vertex_out_in_, out_coords_, vertex_out_out_);        \
        if (nums < 3) {														  \
            return 0;                                                         \
        }                                                                     \
    } while (0)

#define CLIP_OUT2IN(plane_t)                                                  \
    do {                                                                      \
        nums = HomogeneousClipping(											  \
            plane_t, nums, num_of_vertexout_float,                            \
            out_coords_, vertex_out_out_, in_coords_, vertex_out_in_);        \
        if (nums < 3) {														  \
            return 0;                                                         \
        }                                                                     \
    } while (0)



Pipeline::Pipeline(Shader* shader,
	int sizeof_vertex_in,
	int sizeof_vertex_out,
	int sizeof_uniform,
	void* uniform )
	: shader_(shader),
	sizeof_vertex_in_(sizeof_vertex_in),
	sizeof_vertex_out_(sizeof_vertex_out),
	sizeof_uniform_(sizeof_uniform) {
	
	for(int i = 0; i < 3; ++i)
		shader_vertex_in_[i] = malloc(sizeof_vertex_in);

	for(int i = 0; i < 10; ++i) {
		vertex_out_in_[i] = malloc(sizeof_vertex_out);
		vertex_out_out_[i] = malloc(sizeof_vertex_out);
	}

	shader_vertex_out_ = malloc(sizeof_vertex_out_);
	shader_uniform_ = uniform;
}

Pipeline::~Pipeline(){
	


	for (int i = 0; i < 3; ++i) {
		free(shader_vertex_in_[i]);
	}
	for (int i = 0; i < 10; ++i) {
		free(vertex_out_in_[i]);
		free(vertex_out_out_[i]);
	}

	free(shader_vertex_out_);
}

void* Pipeline::GetShaderVertexIn(int i){
	if (i >= 0 && i < 3)
		return shader_vertex_in_[i];
}

void Pipeline::PipelineRun(renderWindow& ren){
	
	Vec4f  in_coords[3];	
	for (int i = 0; i < 3; ++i) {
		in_coords[i]  = shader_->vertexShader(shader_vertex_in_[i], vertex_out_in_[i], shader_uniform_);
		in_coords_[i] = in_coords[i];
	}

	int num_vertices = ClippingTraingle();

	for (int i = 0; i < num_vertices - 2; ++i) {
		
		int index = 0;
		int index1 = i + 1;
		int index2 = i + 2;
		Vec4f clip_coord[3];
		void* vertex_out[3];
		
		clip_coord[0] = out_coords_[index];
		clip_coord[1] = out_coords_[index1];
		clip_coord[2] = out_coords_[index2];

		vertex_out[0] = vertex_out_out_[index];
		vertex_out[1] = vertex_out_out_[index1];
		vertex_out[2] = vertex_out_out_[index2];

		bool is_culled = RasterzieTriangle(clip_coord, vertex_out, ren);

		if(is_culled) {
			break;
		}
	}

}

bool Pipeline::RasterzieTriangle(Vec4f clip_coords[3], void* vOut[3], renderWindow& ren) {

	int width = ren.GetWidth();
	int height = ren.GetHeight();
	
	Vec2f screen_coords[3];
	float screen_depth[3];
	Vec3f ndc_coords[3];
	

	for (int i = 0; i < 3; ++i) {
		ndc_coords[i] = Vec3f(clip_coords[i][0] / clip_coords[i][3], clip_coords[i][1] / clip_coords[i][3], clip_coords[i][2] / clip_coords[i][3]);
	}

	if(IsBackFace(ndc_coords)) {
		return true;
	}
	
	float w_argum[3];

	for (int i = 0; i < 3; ++i) {
		Vec3f v = Rasterzie(ndc_coords[i], width, height);
		screen_coords[i] = Vec2f(v.x, v.y);
		screen_depth[i] = v.z;
	}

	for (int i = 0; i < 3; ++i) {
		w_argum[i] = 1.f / screen_depth[i];
	}

	Vec2i box[2];

	box[0].x = std::min(std::min(screen_coords[0].x, screen_coords[1].x), screen_coords[2].x);
	box[0].y = std::max(std::max(screen_coords[0].x, screen_coords[1].x), screen_coords[2].x);
	box[1].x = std::min(std::min(screen_coords[0].y, screen_coords[1].y), screen_coords[2].y);
	box[1].y = std::max(std::max(screen_coords[0].y, screen_coords[1].y), screen_coords[2].y);

	box[0].x = std::max(box[0].x, 0);
	box[0].y = std::min(width - 1, box[0].y);
	box[1].x = std::max(0, box[1].x);
	box[1].y = std::min(height - 1, box[1].y);

	Vec2i P(0, 0);
	Vec3f barycentric;

	for (P.x = box[0].x; P.x <= box[0].y; P.x++) {
		
		for (P.y = box[1].x; P.y <= box[1].y; P.y++) {
			Vec2f temp = Vec2f(P.x, P.y);
		
			if (isInTrangle(screen_coords, temp, barycentric)) {
				int index = P.y * width + P.x;
				//插值
				float z = 0.0f;
				for (int i = 0; i < 3; ++i) {
					z += barycentric[i] * w_argum[i];
				}
				z = 1.f / z;
				if (z <= ren.zBuffer[index]) {
					//再插值
					InterpolateVertexOut(vOut, barycentric, w_argum);
					//绘制
					DrawFragment(P, z, 0, ren);
				}
			}
		}
	}
	return false;
}

void Pipeline::DrawFragment(Vec2i P, float depth, int backface, renderWindow& ren) {
	int width = ren.GetWidth();
	int height = ren.GetHeight();
	int index = P.y * width + P.x;

	bool discord = false;

	TGAColor color = shader_->pixelShader(shader_vertex_out_, shader_uniform_, discord, backface);

	if (discord) {
		return;
	}

	TGAColor result = OMSetBlendState(color, ren.colorBuffer[index], op, factor_src, factor_dst);

	ren.colorBuffer[index] = result;
	ren.zBuffer[index] = depth;
}

bool Pipeline::IsVertexVisible(Vec4f & v) {
	return fabs(v.x) <= v.w && fabs(v.x) <= v.w && fabs(v.x) <= v.w;
}

int Pipeline::ClippingTraingle() {
	if (IsVertexVisible(in_coords_[0]) && IsVertexVisible(in_coords_[1]) && IsVertexVisible(in_coords_[2])) {
		out_coords_[0] = in_coords_[0];
		out_coords_[1] = in_coords_[1];
		out_coords_[2] = in_coords_[2];

		//拷贝输出属性
		memcpy(vertex_out_out_[0], vertex_out_in_[0], sizeof_vertex_out_);
		memcpy(vertex_out_out_[1], vertex_out_in_[1], sizeof_vertex_out_);
		memcpy(vertex_out_out_[2], vertex_out_in_[2], sizeof_vertex_out_);

		return 3;
	}
	else {
		int nums = 3;
		int num_of_vertexout_float = sizeof_vertex_out_ / sizeof(float);

		CLIP_IN2OUT(POSITIVE_W);
		CLIP_OUT2IN(POSITIVE_X);
		CLIP_IN2OUT(NEGATIVE_X);
		CLIP_OUT2IN(POSITIVE_Y);
		CLIP_IN2OUT(NEGATIVE_Y);
		CLIP_OUT2IN(POSITIVE_Z);
		CLIP_IN2OUT(NEGATIVE_Z);

		return nums;
	}
}

Vec3f Pipeline::Rasterzie(Vec3f& clip_coords, int& width, int& height) {
	return Vec3f((int)((clip_coords[0] + 1.f) * 0.5f * width + 0.5f), (int)((clip_coords[1] + 1.f) * 0.5f * height + 0.5f), (clip_coords[2] + 1.f) * 0.5f);
}

void Pipeline::InterpolateVertexOut(void* vertex_out[3], Vec3f& weights, float weight_argum[3]) {
	int nums_float = sizeof_vertex_out_ / sizeof(float);
	
	float* dst = (float*)shader_vertex_out_;

	float* str0 = (float*)vertex_out[0];
	float* str1 = (float*)vertex_out[1];
	float* str2 = (float*)vertex_out[2];

	float weights0 = weight_argum[0] * weights[0];
	float weights1 = weight_argum[1] * weights[1];
	float weights2 = weight_argum[2] * weights[2];

	float normalize = 1.f / (weights0 + weights1 + weights2);

	for (int i = 0; i < nums_float; ++i) {
		float sum = weights0 * str0[i] + weights1 * str1[i] + weights2 * str2[i];
		dst[i] = sum * normalize;
	}

}

#include"Shader.h"
#include<vector>
#include<iostream>
#include<algorithm>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"renderWindow.h"
Shader::~Shader(){
	
}


bool isInTrangle(Vec2f(&raster)[3], Vec2f P, Vec3f &barycentric){//计算重心坐标
	bool  inside = true;

	Vec2f AP;	
	Vec2f abc[] = { raster[1] - raster[0] ,raster[2] - raster[1] ,raster[0] - raster[2] };
	float S = abc[0].crossProductValue(raster[2] - raster[0]);		//待优化


	for (int i = 0; i < 3; ++i){	//
		AP = P - raster[i];
		float w = abc[i].crossProductValue(AP);
		inside &= (w == 0 ? (abc[i].y == 0 && abc[i].x > 0) || abc[i].y > 0 : w >= 0);

		barycentric.raw[(i + 2) % 3] = w / S;
	}

	return inside;
}


void triangle(Vec2f(&uv)[3], Vec3f(&t)[3], Shader &shader, renderWindow &win, float *zBuffer,TGAColor (&Color)[3], Vec3f(&normal)[3], std::vector<Light *> &Light,Model *model){
	
	int width = win.GetWidth();
	int height = win.GetHeight();

	//顶点变化
	Vec3f screen[3];
	for (int i = 0; i < 3; ++i){
		screen [i] = shader.vertexShader(t[i]);
	}
	
	Vec3f raster[3];
	shader.rasterStage(screen,raster,width,height);
	
	//计算三个顶点颜色
	int size = Light.size();
	Vec3f k[3];
	for (int j = 0; j < size; ++j) {
		for (int i = 0; i < 3 ; ++i){

			if(dynamic_cast<PointLight*>(Light[j]) != nullptr || dynamic_cast<SpotLight*>(Light[j]))
				Light[j]->SetDirection(t[i]);

			k[i] = Light[j]->ColorShader(normal[i]);
		}
	}

	Vec2i box[2];

	box[0].x = std::min(std::min(raster[0].x, raster[1].x), raster[2].x);
	box[0].y = std::max(std::max(raster[0].x, raster[1].x), raster[2].x);
	box[1].x = std::min(std::min(raster[0].y, raster[1].y), raster[2].y);
	box[1].y = std::max(std::max(raster[0].y, raster[1].y), raster[2].y);

	box[0].x = std::max(box[0].x,0);
	box[0].y = std::min(width - 1,box[0].y);
	box[1].x = std::max(0,box[1].x);
	box[1].y = std::min(height - 1,box[1].y);


	Vec2i P;
	Vec2f abc[3];

	for (int i = 0; i < 3; ++i){
		abc[i] = Vec2f(raster[i].x, raster[i].y);
	}
	
	for (int i = 0; i < 3; ++i) {
		raster[i].z = 1.f / raster[i].z;
	}

	Vec3f barycentric;

	for (P.x = box[0].x; P.x <= box[0].y; P.x++) {
		for (P.y = box[1].x; P.y <= box[1].y; P.y++) {
			if (isInTrangle(abc, Vec2f(P.x, P.y), barycentric)) {
				float z = 0;
				SDL_Color color;
				color.a = 0xFF;

				for (int i = 0; i < 3; ++i) {
					z += barycentric.raw[i] * raster[i].z;
				}

				z = 1.f / z;

				Vec3f temp;
					
				temp.raw[0] = (k[0].raw[0] * barycentric.raw[0] * raster[0].z + k[1].raw[0] * barycentric.raw[1] * raster[1].z + k[2].raw[0] * barycentric.raw[2] * raster[2].z)*z;
				temp.raw[1] = (k[0].raw[1] * barycentric.raw[0] * raster[0].z + k[1].raw[1] * barycentric.raw[1] * raster[1].z + k[2].raw[1] * barycentric.raw[2] * raster[2].z)*z;
				temp.raw[2] = (k[0].raw[2] * barycentric.raw[0] * raster[0].z + k[1].raw[2] * barycentric.raw[1] * raster[1].z + k[2].raw[2] * barycentric.raw[2] * raster[2].z)*z;
				
				Vec2f UV;
				
				UV.raw[0] = (uv[0].raw[0] * barycentric.raw[0] * raster[0].z + uv[1].raw[0] * barycentric.raw[1] * raster[1].z + uv[2].raw[0] * barycentric.raw[2] * raster[2].z)*z;
				UV.raw[1] = (uv[0].raw[1] * barycentric.raw[0] * raster[0].z + uv[1].raw[1] * barycentric.raw[1] * raster[1].z + uv[2].raw[1] * barycentric.raw[2] * raster[2].z)*z;

				TGAColor tempColor =  model->diffuse(UV);

				color.r = tempColor[0] * temp.raw[0];
				color.g = tempColor[1] * temp.raw[1];
				color.b = tempColor[2] * temp.raw[2];

				if (z <= zBuffer[P.x + P.y * height] || z < -1.f || z > 1.f)
						continue;
				
				//bool discard = shader.pixelShader(barycentric,color);
				
				//color.r = color.g = color.b = 255;

				//printf("%d %d %d %d\n",color.r,color.g,color.b,color.a);
				
				
				//if (!discard) {
					zBuffer[P.x  + P.y * height] = z;
					win.DrawPointWithColor(color,P.x,P.y);
				//image->set(P.x,P.y,TGAColor(color.r,color.g,color.b,255));
				//}
			}
		} 
	}
}


Vec3f MyShader::vertexShader(Vec3f v){

	Vec4f gl_vertex =  Vec4f(v,1.0f);
	//！！！可以考虑的优化，每个矩阵都是相同的，每次循环都调用赋值，很浪费时间，最+好能够放在局部变量指针传递
	gl_vertex = camera->GetProjection() * (camera->GetView() * gl_vertex);

	return Vec3f(gl_vertex.raw[0], gl_vertex.raw[1], gl_vertex.raw[2]);
}

bool MyShader::pixelShader(Vec3f bar, SDL_Color &color, Vec3f(&normal)[3], Vec3f(&v)[3], std::vector<Light *> &Light) {
	int size = Light.size();


	return false;
}

void MyShader::rasterStage(Vec3f(&s)[3], Vec3f(&v)[3],int width, int height){
	
	for (int i = 0; i < 3; ++i){
		v[i] = Vec3f((int)((s[i].raw[0] + 1.f) / 2.f * width + 0.5f),(int)((s[i].raw[1] + 1.f) / 2.f * height + 0.5f),s[i].raw[2]);
	}
}


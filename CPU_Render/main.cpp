#include<iostream>
#include<algorithm>
#include<cmath>
#include"tgaimage.h"
#include"geometry.h"
#include"model.h"
using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const int width = 800;
const int height = 800;

float zBuffer[width * height];

Model model("african_head.obj");

Matrix LookAtLH(Vec3f EyePosition ,Vec3f FocusPosition ,Vec3f UpDirection) {
	Matrix view(4);
	
	Vec3f w = FocusPosition - EyePosition;
	w.normalize();
	Vec3f u = w.crossProduct(UpDirection);
	u.normalize();
	Vec3f j = u.crossProduct(w);
	
	view[0] = vector<float>{ w.x,w.y,w.z,0};
	view[1] = vector<float>{ u.x,u.y,u.z,0};
	view[2] = vector<float>{ j.x,j.y,j.z,0};
	view[3] = vector<float>{ 0, 0, 0, 1};

	return view;
}
Matrix PerspectiveFovLH() {

	Matrix p;
	
	//p();

	return	p;
}
 

void line(const Vec2i &t0, const Vec2i &t1,TGAImage &image,const TGAColor &color){	
	int x0 =t0.x, x1=t1.x, y0=t0.y, y1 = t1.y;
	bool steep = false;
	
	if(abs(x0 - x1) < abs(y0 - y1))			//如果直线的宽大于高
	{
		swap(x0, y0);						//等操作执行完再置为true
		swap(x1, y1);
		steep = true;
	}


	if (x0 > x1)                              
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int y = y0; //取小的点
	int	deltaX = (x1 - x0);					//优化，局部性原理
	int	deltaY = (y1 - y0);					//优化，局部性原理
	float dError = abs(static_cast<float>(deltaY)) * 2.0f;
	float error = .0f;
	
	if (steep)								//将分支提取出来，进行优化速度
	{
		for (int x = x0; x <= x1; ++x)
		{
			image.set(y, x, color);
		
			error += dError;
			if (error > deltaX)	//取大的点
			{
				y += (y0 < y1 ? 1 : -1);
				error -= deltaX * 2;
			}
		}
	}
	else
	{
		for (int x = x0; x <= x1; ++x)
		{	
			image.set(x, y, color);
			error += dError;
			if (error > deltaX)	//取大的点
			{
				y += (y0 < y1 ? 1 : -1);
				error -= deltaX * 2;
			}
		}
	}
}

Vec3f isInTrangle(Vec3f (&t)[3],Vec3f P) 
{	
	Vec3f AB(t[2].x - t[0].x, t[1].x - t[0].x, t[0].x - P.x);
	Vec3f AC(t[2].y - t[0].y, t[1].y - t[0].y, t[0].y - P.y);

	Vec3f u = AB.crossProduct(AC);

	if (abs(u.z) > 1e-2)	//判断三角形是不是直线
		return Vec3f(1.f - (u.x + u.y) / u.z,u.x / u.z,  u.y / u.z);
	return  Vec3f(-1, 1, 1); //
}


bool isInTrangle(Vec2f(&t)[3], Vec2f P, Vec3f &barycentric)
{	
	bool  inside = true;
	Vec2f AP;
	Vec2f abc[] = { t[1] - t[0] ,t[2] - t[1] ,t[0] - t[2]};
	float S = abc[0].crossProductValue(t[2] - t[0]);


	for (int i = 0; i <3 ; ++i)
	{	
		AP = P - t[i];
		float w = abc[i].crossProductValue(AP);
		inside &= (w == 0 ? (abc[i].y == 0 && abc[i].x > 0) || abc[i].y > 0 : w > 0);
		
		barycentric.raw[(i + 2) % 3] =   w / S;
	}
	
	return inside;

}

void triangle(Vec3f (&t)[3],Vec2f (&uv)[3],TGAImage &image, const float intensity)
{
	Vec2f box[2];	
	
	box[0].x = min(min(t[0].x,t[1].x) , t[2].x);
	box[0].y = max(max(t[0].x, t[1].x), t[2].x);

	box[1].x = min(min(t[0].y, t[1].y), t[2].y);
	box[1].y = max(max(t[0].y, t[1].y), t[2].y);
	
	Vec3f P;
	Vec2f abc[3];

	for (int i = 0; i < 3; ++i)
	{
		abc[i] = Vec2f(t[i].x,t[i].y);
	}

	Vec3f barycentric;

	for (P.x = box[0].x; P.x <= box[0].y; P.x++) {
		for (P.y = box[1].x; P.y <= box[1].y; P.y++) {
			
			if (isInTrangle(abc, Vec2f(P.x, P.y),barycentric))
			{	
				P.z = 0;
				for (int i = 0; i < 3; ++i)
				{	
					P.z += barycentric.raw[i] * (1 / t[i].z);
				}
				P.z = 1 / P.z;
				int index = P.y * height + P.x;
				if (P.z < zBuffer[index])
				{	
					zBuffer[index] = P.z;
					image.set(P.x, P.y, TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255));
				}
			}
			else
				continue;
			
		/*	Vec3f barycentric = isInTrangle(t,P);
			
			if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0)
				continue;
			
			P.z = 0;

			for (int i = 0; i < 3; ++i)
			{
				P.z += t[i].z * barycentric.raw[i];
			}

			Vec2f position(barycentric.x * uv[0].x + barycentric.y * uv[1].x, barycentric.x * uv[0].y + barycentric.y * uv[1].y);
			
			int index = (int)(P.x  + P.y * width);
		
			if (P.z > zBuffer[index])
			{	
				TGAColor color = model.diffuse(position);
				zBuffer[index] = P.z;
				image.set(P.x, P.y, TGAColor(color.r * intensity,color.g * intensity ,color.b * intensity,255));	
			}*/
		}
	}
}

int main(int argc, char ** argv)
{
	TGAImage image(width,height,TGAImage::RGB);

	Vec3f screen_coords[3];
	Vec3f lightDir(0.0, 0.0, -1.0f);
	
	vector<int> face,face2;
	
	float intensity;
	
	float minFloat =   numeric_limits<float>::max();

	for (int i = width * height; i--; zBuffer[i] = minFloat);
		
	int faces = model.nfaces();
		
	for (int i = 0; i < faces ; ++i)
	{	
		face = model.face(i);
		Vec3f world_coords[3];
		Vec3f v,u;

		for (int j = 0; j < 3; ++j)
		{	
			v = model.vert(face[j]);
			screen_coords[j] = Vec3f(int((v.x + 1.) * width / 2.f + 0.5), int((v.y + 1.) * height / 2.f + 0.5),-v.z);
			world_coords[j]  = v;
		}
		
		Vec3f AB = world_coords[2] - world_coords[0];
		Vec3f AC = world_coords[1] - world_coords[0];
		Vec3f n  = AB.crossProduct(AC);
		
		n.normalize();
		intensity = n * lightDir;
		//
	//	if (intensity > 0)
	//	{	
			Vec2f uv[3];
	//		for (int k = 0; k < 3; ++k)
	//		{
	//			uv[k] = model.getUV(i, k);
	//		}
			triangle(screen_coords, uv, image, intensity);
	//	}
	}
	
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

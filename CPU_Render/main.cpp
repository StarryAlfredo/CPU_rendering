//列矩阵

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
//Model model("cube.obj");
//Model model("teapot.obj");

Matrix LookAtLH(Vec3f EyePosition ,Vec3f FocusPosition ,Vec3f UpDirection) {
	Matrix view(4,4);
	
	Vec3f w =   EyePosition - FocusPosition;
	w.normalize();
	Vec3f u = UpDirection.crossProduct(w);
	u.normalize();
	Vec3f j = w.crossProduct(u);
	j.normalize();

	view[0] = vector<float>{ u.x,  u.y, u.z, -(EyePosition * u)};
	view[1] = vector<float>{ j.x,  j.y,	j.z, -(EyePosition * j)};
	view[2] = vector<float>{ w.x,  w.y,	w.z, -(EyePosition * w)};
	view[3] = vector<float>{   0,	 0,	  0, 1};

	return view;
}
Matrix PerspectiveFovLH(float FovAngle,float Aspect ,float NearZ,float FarZ) {
		
	Matrix p(4,4);
	
	float t	 =  tan(FovAngle * 0.5f * PI/180) * NearZ;	 // width / height top = tan / 2  * near
	float b  = -t;
	float r  =  t * Aspect;
	float l	 = -r;
	p[0] = vector<float>{2 * NearZ /(r-l),	0,				( r + l)/(r-l),													0};
	p[1] = vector<float>{0,					2*NearZ/(t-b),	(t+b)/(t-b),													0};
	p[2] = vector<float>{0,					0,				-(FarZ + NearZ)/(FarZ - NearZ),-2.0f *FarZ*NearZ / (FarZ - NearZ)};
	p[3] = vector<float>{0,					0,				-1 ,															0};
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


bool isInTrangle(Vec2f(&t)[3], Vec2f P, Vec3f &barycentric)	//计算重心坐标
{	
	bool  inside = true;
	Vec2f AP;
	Vec2f abc[] = { t[1] - t[0] ,t[2] - t[1] ,t[0] - t[2]};
	float S = abc[0].crossProductValue(t[2] - t[0]);		//待优化


	for (int i = 0; i <3 ; ++i)
	{	//w == 0 ? (abc[i].y == 0 && abc[i].x > 0) || abc[i].y > 0 : 
		AP = P - t[i];
		float w = abc[i].crossProductValue(AP);
		inside &= (w >= 0);
		
		barycentric.raw[(i + 2) % 3] =   w / S;
	}
	
	return inside;

}

void triangle(Vec3f (&t)[3],Vec2f (&uv)[3],TGAImage &image, const float intensity,TGAColor (&Color)[3])
{
	Vec2f box[2];

	box[0].x = min(min(t[0].x,t[1].x) , t[2].x);
	box[0].y = max(max(t[0].x, t[1].x), t[2].x);
	box[1].x = min(min(t[0].y, t[1].y), t[2].y);
	box[1].y = max(max(t[0].y, t[1].y), t[2].y);
	
	box[0].x = max(box[0].x, 0.f);
	box[0].y = min(box[0].y, (float)(width - 1));
	
	box[1].x = max(box[1].x, 0.f);
	box[1].y = min(box[1].y, (float)(height - 1));

	Vec3f P;
	Vec2f abc[3];

	for (int i = 0; i < 3; ++i)
	{
		abc[i] = Vec2f(t[i].x,t[i].y);
	}

	Vec3f barycentric;

	for (P.x = box[0].x; P.x <= box[0].y; P.x++) {
		for (P.y = box[1].x; P.y <= box[1].y; P.y++) {

			if (isInTrangle(abc, Vec2f(P.x, P.y), barycentric))
			{
				P.z = 0;
				TGAColor color;
				for (int i = 0; i < 3; ++i)
				{
					P.z += barycentric.raw[i] * t[i].z;

				}
				P.z = 1 / P.z;

				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						color.raw[i] += Color[j].raw[i] * t[j].z * barycentric.raw[j];
					}
					color.raw[i] *= P.z;
				}
				
				int index = P.y* height  + P.x ;
			
				if (P.z > zBuffer[index])
				{
					zBuffer[index] = P.z;
					image.set(P.x, P.y, color);
				}
			}	
		}
	}
}

int main(int argc, char ** argv)
{
	TGAImage image(width,height,TGAImage::RGB);

	Vec3f screen_coords[3];
	Vec3f lightDir(0.0, 0.0, 1.0f);
	vector<int> face,face2;
	Matrix view		   = LookAtLH(Vec3f(0, 1, 2),Vec3f(0,0,1),Vec3f(0,1,0));
	Matrix perspective = PerspectiveFovLH(90,width/height,0.1f, 200.f);

	
	float intensity = 1.0f;
	
	float minFloat =   -numeric_limits<float>::max();

	for (int i = width * height; i--; zBuffer[i] = minFloat);
		
	int faces = model.nfaces();
		
	for (int i = 0; i < faces ; ++i)
	{	
		face = model.face(i);
		Vec3f world_coords[3];
		Vec3f v;
		Vec3f u;

		for (int j = 0; j < 3; ++j)
		{	
			v = model.vert(face[j]);
			
			Vec4f temp(v.x,v.y,v.z,1.0f);
			Vec4f temp2 = view * temp;
			Vec4f s = perspective * temp2;
			world_coords[j] = Vec3f(temp2.x,temp2.y,temp2.z);
			v = Vec3f(s.x, s.y, v.z);

			screen_coords[j] = Vec3f(int((v.x + 1.) * width / 2.f + 0.5), int((v.y + 1.) * height / 2.f + 0.5),s.z);
			//光栅化
		}
		
		Vec3f AB = world_coords[0] - world_coords[1];
		Vec3f AC = world_coords[1] - world_coords[2];
		Vec3f n  = AB.crossProduct(AC);
		TGAColor c[3] = { TGAColor(255,0,0,255),TGAColor(0,255,0,255),TGAColor(0,0,255,255) };

		n.normalize();
		intensity = n * lightDir;
		//intensity = intensity;

		if (intensity > 0)
		{	
			Vec2f uv[3];
			//for (int k = 0; k < 3; ++k)
			//{
			//	uv[k] = model.getUV(i, k);
			//}
			TGAColor color = TGAColor(255 * intensity, 255 * intensity , 255 * intensity, 255);
			triangle(screen_coords, uv, image, intensity ,c);
		}
	}
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

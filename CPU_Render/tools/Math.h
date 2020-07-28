#ifndef MATH_H
#define	MATH_H
#include<cmath>
#include<vector>



/***********************
  ��ά����
  ֧�� + - * / 
  �������㻹�е���Լ����
************************/

template<typename T>  
class Vec2{
public:
	union				
	{
		struct { T u, v; };
		struct { T x, y; };
		T raw[2];
	};

public:
	Vec2() :u(0), v(0) {}
	Vec2(const T& _u,const T& _v):u(_u),v(_v){}
	Vec2(const Vec2<T>& V):u(V.u), v(V.v) {}

public:
	Vec2<T> operator + (const Vec2<T>& V){  return Vec2<T>(x + V.x, y + V.y); }
	Vec2<T> operator - (const Vec2<T>& V) {  return  Vec2<T>(x - V.x, y - V.y  ); }
	Vec2<T> operator * (const Vec2<T>& V) {  return  Vec2<T>(x * V.x, y * V.y ); }
	Vec2<T> operator * (float f)					   {  return  Vec2<T>(x * f    , y * f    ); }
	T &          operator [](int i)						   {  return  raw[i];					   }
	 
	void Normalize() { 
		T length = x * x + y * y;
		if (length != 0)
		{	
			length = 1 / sqrt(length);
			x *= length;
			y *= length;
		}
	}
	Vec2<T> GetNormalize() {
		float length = x * x + y * y;
		T a = x;
		T b = y;

		if (length != 0){
			length = 1.f / sqrt(length);
			a *= length;
			b *= length;
		}	
		return Vec2<T>(a,b);
	}

	Vec2<T> CrossProductVector(const Vec2<T>&vec) {
		return Vec2<T>(
			0, 0, 0);
	}
	T CrossProductValue(const Vec2<T>&vec){
		return x * vec.y - vec.x*y;
	}
	T Dot(const Vec2<T> &vec) {
		return x * vec.x + y * vec.y;
	}
};

//��ά����
template<typename T>  
class Vec3{

public:
	union					
	{
		struct { T ivert, inv, inform; };
		struct { T x, y, z; };
		struct { T r, g, b; };
		T raw[3];
	};

public:
	Vec3() :x(0), y(0) ,z(0){}
	Vec3(const T& _u, const T& _v ,const T& _w) :x(_u), y(_v), z(_w) {}
	Vec3(const Vec3<T>& V) :x(V.x), y(V.y), z(V.z) {}

public:
	Vec3<T> operator + (const Vec3<T>& V) { return Vec3<T>(x + V.x, y + V.y, z + V.z); }
	Vec3<T> operator - (const Vec3<T>& V) { return Vec3<T>(x - V.x, y - V.y, z - V.z); }
	Vec3<T>	operator * (const Vec3<T>& V) { return Vec3<T>(V.x * x , V.y * y ,V.z * z);}
	Vec3<T> operator * (T f)							   { return Vec3<T>(f * x, f * y, f *z);        }
	T&      operator[] (int i)							   { return raw[i];							   }

public:
	void normalize() {
		T length = x * x + y * y + z*z;
		if (length != 0)
		{
			length = 1 / sqrt(length);
			x *= length;
			y *= length;
			z *= length; 
		}
	}
	Vec3<T> GetNormalize() {
		T length = x * x + y * y + z * z;;
		T a = x;
		T b = y;
		T c = z;
		if (length != 0) {
			length = 1.f / sqrt(length);
			a *= length;
			b *= length;
			c *= length;
		}
		return Vec3<T>(a, b, c);
	}

	//�������
	Vec3<T> CrossProduct(const Vec3<T>&vec){
		return Vec3<T>(
		y * vec.z - z * vec.y,
		z * vec.x - x * vec.z,
		x * vec.y - y * vec.x
		);
	}
	//�������
	T Dot(const Vec3<T>&vec) {
		return vec.x * x + vec.y * y + vec.z * z;
	}
	//��ȡģ��
	T length(){ return sqrt(x*x + y*y + z*z);}
	
	
};

template<typename T>  
class Vec4 {
public:
	union										//��union ����ʡ�ڴ�ռ䣬����һ�����������Ա����
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T raw[4];
	};

public:
	Vec4() :x(0), y(0), z(0),w(0) {}
	Vec4(const T& _u, const T& _v, const T& _z, const T& _w) :x(_u), y(_v), z(_z),w(_w) {}
	Vec4(const Vec4<T>& V) :x(V.x), y(V.y), z(V.z),w(V.w) {}
	Vec4(const Vec3<T> &V, float _w):x(V.x), y(V.y), z(V.z), w(_w){}


public:
	Vec4<T> operator + (const Vec4<T>& V) { return Vec4<T>(x + V.x, y + V.y, z - V.z, V.w + V.w); }
	Vec4<T> operator - (const Vec4<T>& V) { return Vec4<T>(x - V.x, y - V.y, z - V.z, V.w - V.w); }
	Vec4<T> operator * (const Vec4<T>& V) { return Vec4<T>(x * V.x, y * V.y, z * V.z, V.w * V.w); }
	Vec4<T> operator * (T f)			  { return Vec4<T>(f * x  , f * y  , f * z  , w ); }
	T&      operator[] (int i) { return raw[i]; }

	//����ģ��
	float length(){ return sqrt(x*x + y * y + z * z + w * w); }
	//�������
	T Dot(const Vec4<T>&vec) { return vec.x * x + vec.y * y + vec.z * z + vec.w * w; }
	//��ֵ
	Vec4<T> Lerp(Vec4<T>B, float t) { return (*this * (1 - t) + t * B); }
};



typedef Vec3<float>		Vec3f;
typedef Vec3<int>		Vec3i;

typedef Vec4<float>		Vec4f;
typedef Vec4<int>		Vec4i;

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;
typedef Vec3<unsigned char> Color3;
typedef Vec4<unsigned char> Color4;

const int DEFAULT_ALLOC = 4;

//����
class Matrix {
	std::vector<std::vector<float>> m;
	int rows, cols;
public:
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	//Matrix();
	inline int nrows();
	inline int ncols();
	
	static Matrix identity(int dimensions);
	std::vector<float>& operator [](const int i);
	//Matrix operator * (const Matrix& a);
	Vec4f  operator * (const Vec4f& a);
	Matrix transpose();
	Matrix inverse();
};

//��y����ת
Matrix MatrixRotationY(float rad);
//��x����ת
Matrix MatrixRotationX(float rad);
//��z����ת
Matrix MatrixRotationZ(float rad);
//����������ת
Matrix MatrixRotationAxis(Vec3f &axi, float theta);
//��üн�
float VectorGetY(Vec3f &f);
//�Ƿ����������ڲ���ȡ��������
bool isInTrangle(Vec2f(&raster)[3], Vec2f P, Vec3f &barycentric);

inline float Lerp(float p1, float p2, float t) { 
	return  p1 * (1 - t) + p2 * t;
}
inline Vec4f Lerp(Vec4f& v1, Vec4f& v2, float t) {
	return v1 * (1 - t) + v2 * t;
}

#endif

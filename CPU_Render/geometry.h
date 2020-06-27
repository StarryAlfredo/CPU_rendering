#ifndef GEOMETRYA_H
#define	GEOMETRYA_H
#include<cmath>
#include<vector>

#define PI 3.1415926

//仅提供重要操作，尽量使用const
template<typename T>  struct Vec2{
	
	union					//	用	unio 来节省内存空间，可以一个向量多个成员名字
	{
		struct { T u, v; };
		struct { T x, y; };
		T raw[2];
	};
	//构造函数
	Vec2() :u(0), v(0) {}
	Vec2(const T& _u,const T& _v):u(_u),v(_v){}
	Vec2(const Vec2<T>& V):u(V.u), v(V.v) {}
	
	//向量 +
	inline Vec2<T> operator + (const Vec2<T>& V) { return Vec2<T>(x + V.x, y + V.y); }
	//向量 -
	inline Vec2<T> operator - (const Vec2<T>& V) { return Vec2<T>(x - V.x, y - V.y); }
	//向量 *
	inline T   operator * (const Vec2<T>& V)     { return (V.x * x + V.y * y); }
	//向量 *
	inline Vec2<T> operator * (float f)				 { return Vec2<T>(f * x ,f * y); }
	 
	//向量标准化
	inline void normalize() { T length = x * x + y * y;
		if (length != 0)
		{	
			length = 1 / sqrt(length);
			x *= length;
			y *= length;
		}
		
	}
	//向量叉乘
	inline Vec2<T> crossProductVector(const Vec2<T>&vec)
	{

		return Vec2<T>(
					0,0,0
			);
	}
	inline T crossProductValue(const Vec2<T>&vec)
	{
		return x * vec.y - vec.x*y;
	}

};

template<typename T>  struct Vec3 {

	union					//	用	unio 来节省内存空间，可以一个向量多个成员名字
	{
		struct { T ivert, inv, inform; };
		struct { T x, y, z; };
		T raw[3];
	};

	Vec3() :x(0), y(0) ,z(0){}
	Vec3(const T& _u, const T& _v ,const T& _w) :x(_u), y(_v), z(_w) {}
	Vec3(const Vec3<T>& V) :x(V.x), y(V.y), z(V.z) {}
	inline Vec3<T> operator + (const Vec3<T>& V) { return Vec3<T>(x + V.x, y + V.y, z + V.z); }
	inline Vec3<T> operator - (const Vec3<T>& V) { return Vec3<T>(x - V.x, y - V.y, z - V.z); }
	inline T	   operator * (const Vec3<T>& V) { return (V.x * x + V.y * y + V.z * z); }
	inline Vec3<T> operator * (T f)				 { return Vec3<T>(f * x, f * y, f *z); }
	inline Vec3<T> ColorProduct(const Vec3<T>& V) { return Vec3<T>(V.x * x, V.y * y, V.z * z); }


	//向量标准化
	inline void normalize() {
		T length = x * x + y * y + z*z;
		if (length != 0)
		{
			length = 1 / sqrt(length);
			x *= length;
			y *= length;
			z *= length;
		}
	}
	//向量叉乘
	inline Vec3<T> crossProduct(const Vec3<T>&vec)
	{
		
		return Vec3<T>(
		y * vec.z - z * vec.y,
		z * vec.x - x * vec.z,
		x * vec.y - y * vec.x
		);
	}
	inline T length()
	{
		return sqrt(x*x + y*y + z*z);
	}
};

template<typename T>  struct Vec4 {

	union					//	用	unio 来节省内存空间，可以一个向量多个成员名字
	{
		struct { T ivert, inv, inform,n; };
		struct { T x, y, z,w; };
		T raw[4];
	};

	Vec4() :x(0), y(0), z(0),w(0) {}
	Vec4(const T& _u, const T& _v, const T& _z, const T& _w) :x(_u), y(_v), z(_z),w(_w) {}
	Vec4(const Vec4<T>& V) :x(V.x), y(V.y), z(V.z),w(V.w) {}
	Vec4(const Vec3<T> &V, float _w):x(V.x), y(V.y), z(V.z), w(_w){}
	inline Vec4<T> operator + (const Vec3<T>& V) { return Vec4<T>(x + V.x, y + V.y, z - V.z,1.0); }
	inline Vec4<T> operator - (const Vec3<T>& V) { return Vec4<T>(x - V.x, y - V.y, z - V.z,1.0); }
	inline T	   operator * (const Vec3<T>& V) { return (V.x * x + V.y * y + V.z * z); }
	inline Vec4<T> operator * (T f) { return Vec4<T>(f * x, f * y, f *z,f.w*w); }


	//向量标准化
	inline void normalize() {
		T length = x * x + y * y + z * z;
		if (length != 0)
		{
			length = 1 / sqrt(length);
			x *= length;
			y *= length;
			z *= length;
		}
	}
	//向量叉乘
	inline Vec4<T> crossProduct(const Vec4<T>&vec)
	{

		return Vec4<T>(
			y * vec.z - z * vec.y,
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x,
			w);
	}
	inline T length()
	{
		return sqrt(x*x + y * y + z * z);
	}
};



typedef Vec3<float>		Vec3f;
typedef Vec3<int>		Vec3i;

typedef Vec4<float>		Vec4f;
typedef Vec4<int>		Vec4i;

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;

const int DEFAULT_ALLOC = 4;

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

//绕y轴旋转
Matrix MatrixRotationY(float rad);
//绕x轴旋转
Matrix MatrixRotationX(float rad);
//绕z轴旋转
Matrix MatrixRotationZ(float rad);
//绕任意轴旋转
Matrix MatrixRotationAxis(Vec3f &axi, float theta);

float VectorGetY(Vec3f &f);
#endif

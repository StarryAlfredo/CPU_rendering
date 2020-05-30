#ifndef GEOMETRYA_H
#define	GEOMETRYA_H
#include<cmath>
#include<vector>
//���ṩ��Ҫ����������ʹ��const
template<typename T>  struct Vec2{
	
	union					//	��	unio ����ʡ�ڴ�ռ䣬����һ�����������Ա����
	{
		struct { T u, v; };
		struct { T x, y; };
		T raw[2];
	};
	//���캯��
	Vec2() :u(0), v(0) {}
	Vec2(const T& _u,const T& _v):u(_u),v(_v){}
	Vec2(const Vec2<T>& V):u(V.u), v(V.v) {}
	
	//���� +
	inline Vec2<T> operator + (const Vec2<T>& V) { return Vec2<T>(x + V.x, y + V.y); }
	//���� -
	inline Vec2<T> operator - (const Vec2<T>& V) { return Vec2<T>(x - V.x, y - V.y); }
	//���� *
	inline T   operator * (const Vec2<T>& V)     { return (V.x * x + V.y * y); }
	//���� *
	inline Vec2<T> operator * (float f)				 { return Vec2<T>(f * x ,f * y); }
	 
	//������׼��
	inline void normalize() { T length = x * x + y * y;
		if (length != 0)
		{	
			length = 1 / sqrt(length);
			x *= length;
			y *= length;
		}
	}
	//�������
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

	union					//	��	unio ����ʡ�ڴ�ռ䣬����һ�����������Ա����
	{
		struct { T ivert, inv, inform; };
		struct { T x, y, z; };
		T raw[3];
	};

	Vec3() :x(0), y(0) ,z(0){}
	Vec3(const T& _u, const T& _v ,const T& _w) :x(_u), y(_v), z(_w) {}
	Vec3(const Vec3<T>& V) :x(V.x), y(V.y), z(V.z) {}
	inline Vec3<T> operator + (const Vec3<T>& V) { return Vec3<T>(x + V.x, y + V.y, z - V.z); }
	inline Vec3<T> operator - (const Vec3<T>& V) { return Vec3<T>(x - V.x, y - V.y, z - V.z); }
	inline T	   operator * (const Vec3<T>& V) { return (V.x * x + V.y * y + V.z * z); }
	inline Vec3<T> operator * (T f)				 { return Vec3<T>(f * x, f * y, f *z); }
	//������׼��
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
	//�������
	inline Vec3<T> crossProduct(const Vec3<T>&vec)
	{
		
		return Vec3<T>(
		y * vec.z - z * vec.y,
		z * vec.x - x * vec.z,
		x * vec.y - y * vec.x
		);
	}
};

typedef Vec3<float>		Vec3f;
typedef Vec3<int>		Vec3i;

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;

const int DEFAULT_ALLOC = 4;

class Matrix {
	std::vector<std::vector<float>> m;
	int rows, cols;
public:
	Matrix(int r = DECIMAL_DIG, int c = DBL_DECIMAL_DIG);
	inline int nrows();
	inline int ncols();
	
	static Matrix identity(int dimensions);
	std::vector<float>& operator [](const int i);
	Matrix operator * (const Matrix& a);
	Matrix transpose();
	Matrix inverse();
};

#endif
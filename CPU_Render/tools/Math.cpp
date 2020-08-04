#include"Math.h"
#include<cassert>


Matrix::Matrix(int r, int c): m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }

inline int Matrix::nrows(){
	return rows;
}

inline int Matrix::ncols(){
	return cols;
}
Matrix Matrix::identity(int dimensions)
{	
	Matrix E(dimensions, dimensions);
	for (int i = 0; i < dimensions; ++i)
	{
		for (int j = 0; j < dimensions; ++j)
		{
			E[i][j] = (i == j ? 1.0f : 0.0f);
		}
	}
	return E;
}

std::vector<float>& Matrix::operator[](const int i)
{
	assert(i >= 0 && i< rows);
	return m[i];
}

Matrix Matrix::operator*(Matrix a) {
	Matrix r;

	float temp;

	for (int i = 0; i < rows; i++) {
		for(int k = 0; k < cols; k++) {
			temp = m[i][k];
			for (int j = 0; j < cols; ++j) {
				r[i][j] = r[i][j] + a[k][j] * temp;
			}
		}

	}

	return r;
}


Vec4f Matrix::operator*(const Vec4f & a)
{	
	Vec4f temp;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{	
			temp.raw[i] += a.raw[j] * m[i][j];
		}
	}
	return temp;
}

Vec3f Matrix::operator*(const Vec3f & a)
{
	Vec3f temp;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			temp.raw[i] += a.raw[j] * m[i][j];
		}
	}
	return temp;
}

Matrix Matrix::transpose()
{
	Matrix result(cols, rows);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result[j][i] = m[i][j];
	return result;
}

Matrix Matrix::inverse()
{
	assert(rows == cols);

	Matrix result(rows, cols * 2);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result[i][j] = m[i][j];
	for (int i = 0; i < rows; i++)
		result[i][i + cols] = 1;

	for (int i = 0; i < rows - 1; i++) {
		for (int j = result.cols - 1; j >= 0; j--)
			result[i][j] /= result[i][i];
		for (int k = i + 1; k < rows; k++) {
			float coeff = result[k][i];
			for (int j = 0; j < result.cols; j++) {
				result[k][j] -= result[i][j] * coeff;
			}
		}
	}
	for (int j = result.cols - 1; j >= rows - 1; j--)
		result[rows - 1][j] /= result[rows - 1][rows - 1];
	for (int i = rows - 1; i > 0; i--) {
		for (int k = i - 1; k >= 0; k--) {
			float coeff = result[k][i];
			for (int j = 0; j < result.cols; j++) {
				result[k][j] -= result[i][j] * coeff;
			}
		}
	}
	Matrix truncate(rows, cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			truncate[i][j] = result[i][j + cols];
	return truncate;
}

Matrix MatrixRotationY(float rad){

	Matrix v;

	v[0] = std::vector<float>{ cos(rad) ,0	 ,sin(rad)	 ,0 };
	v[1] = std::vector<float>{ 0        ,1   ,0          ,0 };
	v[2] = std::vector<float>{ -sin(rad),0   ,cos(rad)   ,0 };
	v[3] = std::vector<float>{ 0        ,0   ,0          ,1 };

	return v;
}

Matrix MatrixRotationX(float rad){

	Matrix v;

	v[0] = std::vector<float>{ 1,	0	    , 0	       ,0 };
	v[1] = std::vector<float>{ 0,   cos(rad), -sin(rad),0 };
	v[2] = std::vector<float>{ 0,   sin(rad), cos(rad) ,0 };
	v[3] = std::vector<float>{ 0,   0       , 0        ,1 };

	return v;
}

Matrix MatrixRotationZ(float rad){

	Matrix v;

	v[0] = std::vector<float>{ cos(rad),-sin(rad),0	,0 };
	v[1] = std::vector<float>{ sin(rad),cos(rad) ,0 ,0 };
	v[2] = std::vector<float>{ 0       ,0        ,1 ,0 };
	v[3] = std::vector<float>{ 0       ,0        ,0 ,1 };

	return v;
}

Matrix MatrixRotationAxis(Vec3f & axi, float theta){

		Matrix r;
		float u = axi.x;
		float v = axi.y;
		float w = axi.z;

		r[0][0] = cosf(theta) + (u * u) * (1 - cosf(theta));
		r[0][1] = u * v * (1 - cosf(theta)) + w * sinf(theta);
		r[0][2] = u * w * (1 - cosf(theta)) - v * sinf(theta);
		r[0][3] = 0; 

		r[1][0] = u * v * (1 - cosf(theta)) - w * sinf(theta);
		r[1][1] = cosf(theta) + v * v * (1 - cosf(theta));
		r[1][2] = w * v * (1 - cosf(theta)) + u * sinf(theta);
		r[1][3] = 0;

		r[2][0] = u * w * (1 - cosf(theta)) + v * sinf(theta);

		r[2][1] = v * w * (1 - cosf(theta)) - u * sinf(theta);
		r[2][2] = cosf(theta) + w * w * (1 - cosf(theta));
		r[2][3] = 0;

		r[3][0] = 0;
		r[3][1] = 0;
		r[3][2] = 0;
		r[3][3] = 1;

		return r;
}

Matrix MatrixScale(Vec3f scale) {
	Matrix r;
	r = r.identity(4);
	r[0][0] = scale[0];
	r[1][1] = scale[1];
	r[2][2] = scale[2];
	return r;
}

Matrix MatrixTranslation(Vec3f trans) {
	Matrix r;
	r = r.identity(4);
	r[0][3] = trans[0];
	r[1][3] = trans[1];
	r[2][3] = trans[2];
	return r;
}


float VectorGetY(Vec3f & f){ return f.Dot(Vec3f(0, 1, 0)); }

Vec3f NormalSampleToWorldSpace(Vec3f normalMapSample, Vec3f normalW, Vec3f tangentW){
	Vec3f result;
	
	Vec3f N = normalW.GetNormalize();
	Vec3f T = (tangentW - (N * tangentW.Dot(N))).GetNormalize();
	Vec3f B = N.CrossProduct(T);
	
	Matrix m(3);

	m[0] = std::vector<float>{T.x, B.x, N.x};
	m[1] = std::vector<float>{T.y, B.y, N.y};
	m[2] = std::vector<float>{T.z, B.z, N.z};

	result = m * normalW;

	return result;
}


bool isInTrangle(Vec2f(&raster)[3], Vec2f P, Vec3f &barycentric) {//计算重心坐标
	
	bool  inside = true;
	
	Vec2f abc[] = { raster[1] - raster[0] ,raster[2] - raster[1] ,raster[0] - raster[2] };
	Vec2f ac = raster[2] - raster[0];
	
	Vec2f AP = P - raster[0];
	
	float S = 1.f / (abc[0].CrossProductValue(ac));		
/*
	for (int i = 0; i < 3; ++i) {	
		AP = P - raster[i];
		
		float w = AP.CrossProductValue(abc[i]);
		
		inside &=(w >= 0);

		barycentric.raw[(i + 2)%3] = w * S;
	}
*/
	AP = P - raster[0];
	float s = AP.CrossProductValue(ac) * S;
	float t = abc[0].CrossProductValue(AP) * S;
	float q = 1 - s - t;
	inside = ((s >= 0) & (t >= 0) & (q >= 0));

	barycentric.x = q;
	barycentric.y = s;
	barycentric.z = t;

	return inside;
}

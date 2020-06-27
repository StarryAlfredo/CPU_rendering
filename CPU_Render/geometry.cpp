#include"geometry.h"
#include<cassert>


Matrix::Matrix(int r, int c): m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }
//Matrix::Matrix() : m(std::vector<std::vector<float> >(4, std::vector<float>(4, 0.f))), rows(4), cols(4) { }


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
	if (temp.w != 1)
	{
		for (int i = 0; i < 4; ++i)
		{
			temp.raw[i] /= temp.w;
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
	// augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
	Matrix result(rows, cols * 2);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result[i][j] = m[i][j];
	for (int i = 0; i < rows; i++)
		result[i][i + cols] = 1;
	// first pass
	for (int i = 0; i < rows - 1; i++) {
		// normalize the first row
		for (int j = result.cols - 1; j >= 0; j--)
			result[i][j] /= result[i][i];
		for (int k = i + 1; k < rows; k++) {
			float coeff = result[k][i];
			for (int j = 0; j < result.cols; j++) {
				result[k][j] -= result[i][j] * coeff;
			}
		}
	}
	// normalize the last row
	for (int j = result.cols - 1; j >= rows - 1; j--)
		result[rows - 1][j] /= result[rows - 1][rows - 1];
	// second pass
	for (int i = rows - 1; i > 0; i--) {
		for (int k = i - 1; k >= 0; k--) {
			float coeff = result[k][i];
			for (int j = 0; j < result.cols; j++) {
				result[k][j] -= result[i][j] * coeff;
			}
		}
	}
	// cut the identity matrix back
	Matrix truncate(rows, cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			truncate[i][j] = result[i][j + cols];
	return truncate;
}

Matrix MatrixRotationY(float rad){

	Matrix v;

	v[0] = std::vector<float>{ cos(rad),	0	,sin(rad)	,0 };
	v[1] = std::vector<float>{ 0,1,0,0 };
	v[2] = std::vector<float>{ -sin(rad),0,cos(rad),0 };
	v[3] = std::vector<float>{ 0,0,0,1 };

	return v;
}

Matrix MatrixRotationX(float rad){

	Matrix v;

	v[0] = std::vector<float>{ 1,	0	,0	,0 };
	v[1] = std::vector<float>{ 0,cos(rad),-sin(rad),0 };
	v[2] = std::vector<float>{ 0,sin(rad),cos(rad),0 };
	v[3] = std::vector<float>{ 0,0,0,1 };

	return v;
}

Matrix MatrixRotationZ(float rad){

	Matrix v;

	v[0] = std::vector<float>{ cos(rad),-sin(rad),0	,0 };
	v[1] = std::vector<float>{ sin(rad),cos(rad),0,0 };
	v[2] = std::vector<float>{ 0,0,1,0 };
	v[3] = std::vector<float>{ 0,0,0,1 };

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


float VectorGetY(Vec3f & f){

	return f * Vec3f(0, 1, 0);
}

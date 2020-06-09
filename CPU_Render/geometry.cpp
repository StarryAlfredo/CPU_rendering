#include"geometry.h"
#include<cassert>


Matrix::Matrix(int r, int c): m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }

inline int Matrix::nrows()
{
	return rows;
}

inline int Matrix::ncols()
{
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

Matrix Matrix::operator*(const Matrix& a)
{
	return Matrix();
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
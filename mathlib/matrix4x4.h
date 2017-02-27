//
//  matrix4x4.h
//  theraytracer
//
//  Represents a linear-algebraic compatible
//  4x4 dimensional matrix
//
//  Created by Klas Henrikkson   
//  Copyright c 2017 bajsko. All rights reserved
//

#ifndef matrix4x4_h
#define matrix4x4_h

#include <stdint.h>

template<typename T>
class Matrix4x4
{

public:
	Matrix4x4() {}
	Matrix4x4(const T* array) { memcpy((void*)m, (void*)array, sizeof(T) * 16); }

	const T* operator [] (uint8_t i) const { return m[i]; }
	T* operator [] (uint8_t i) { return m[i]; }

	Matrix4x4<T> operator * (const Matrix4x4<T>& r) const
	{
		Matrix4x4<T> mult;
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 4; j++) {
				mult[i][j] = m[i][0] * r[0][j] +
					m[i][1] * r[1][j] +
					m[i][2] * r[2][j] +
					m[i][3] * r[3][j];
			}
		}

		return mult;
	}

	void multVec(const Vec3<T>& src, Vec3<T>& dst) const
	{
		dst.x = src.x * m[0][0] + src.y * m[1][0] + src.z * m[2][0] + m[3][0];
		dst.y = src.x * m[0][1] + src.y * m[1][1] + src.z * m[2][1] + m[3][1];
		dst.z = src.x * m[0][2] + src.y * m[1][2] + src.z * m[2][2] + m[3][2];
		T w = src.x * m[0][3] + src.y * m[1][3] + src.z * m[2][3] + m[3][3];
		if (w != 1 && w != 0)
		{
			dst.x = dst.x / w;
			dst.y = dst.y / w;
			dst.z = dst.z / w;
		}
	}

	void multDirVec(const Vec3<T>& src, Vec3<T> dst) const
	{
		dst.x = src.x * m[0][0] + src.y * m[1][0] + src.z * m[2][0] + m[3][0];
		dst.y = src.y * m[0][1] + src.y * m[1][1] + src.z * m[2][1] + m[3][1];
		dst.z = src.z * m[0][2] + src.y * m[1][2] + src.z * m[2][2] + m[3][2];
	}

	Matrix4x4<T> transpose() const
	{
		Matrix4x4<T> trans;
		for (uint8_t i = 0; i < 4; i++)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				trans[i][j] = m[j][i];
			}
		}

		return trans;
	}

	Matrix4x4<T> inverse()
	{
		int i, j, k;
		Matrix4x4<T> s;
		Matrix4x4<T> t(*this);

		// Forward elimination
		for (i = 0; i < 3; i++) {
			int pivot = i;

			T pivotsize = t[i][i];

			if (pivotsize < 0)
				pivotsize = -pivotsize;

			for (j = i + 1; j < 4; j++) {
				T tmp = t[j][i];

				if (tmp < 0)
					tmp = -tmp;

				if (tmp > pivotsize) {
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0) {
				// Cannot invert singular matrix
				return Matrix4x4<T>();
			}

			if (pivot != i) {
				for (j = 0; j < 4; j++) {
					T tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < 4; j++) {
				T f = t[j][i] / t[i][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		// Backward substitution
		for (i = 3; i >= 0; --i) {
			T f;

			if ((f = t[i][i]) == 0) {
				// Cannot invert singular matrix
				return Matrix4x4<T>();
			}

			for (j = 0; j < 4; j++) {
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++) {
				f = t[j][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		return s;
	}

	T m[4][4] = { { 1, 0, 0, 0 },{ 0, 1, 0, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 1 } }; //set it to the identity matrix as default
};

//A 4x4 dimensional matrix with floating-point precision
typedef Matrix4x4<float> mat44f;
//A 4x4 dimensional matrix with double precision
typedef Matrix4x4<double> mat44d;

//Small utility class to help deal with matricies
class Mat44Util
{
public:

	//Creates a rotation matrix around the x-axis
	template<typename T>
	static Matrix4x4<T> rotate_x(float rad)
	{

		Matrix4x4<T> ret;
		ret[1][1] = (T)roundf(cos(rad));
		ret[1][2] = (T)roundf(sin(rad));
		ret[2][1] = (T)roundf(-sin(rad));
		ret[2][2] = (T)roundf(cos(rad));
		return ret;
	}

	//Creates a rotation matrix around the y-axis
	template<typename T>
	static Matrix4x4<T> rotate_y(float rad)
	{
		rad = roundf(rad);
		Matrix4x4<T> ret;
		ret[0][0] = (T)roundf(cos(rad));
		ret[2][0] = (T)roundf(sin(rad));
		ret[0][2] = (T)roundf(-sin(rad));
		ret[2][2] = (T)roundf(cos(rad));
		return ret;
	}

	//Creates a rotation matrix around the z-axis
	template<typename T>
	static Matrix4x4<T> rotate_z(float rad)
	{
		rad = roundf(rad);
		Matrix4x4<T> ret;
		ret[0][0] = (T)roundf(cos(rad));
		ret[0][1] = (T)roundf(sin(rad));
		ret[1][0] = (T)roundf(-sin(rad));
		ret[1][1] = (T)roundf(cos(rad));
		return ret;
	}

	//Creates a look-at matrix (camToWorld), asumes (0,1,0) represents up
	template<typename T>
	static Matrix4x4<T> look_at(const Vec3<T>& pos, const Vec3<T>& lookAt)
	{
		Vec3<T> forward = Vec3Util::normalize(from - t)o;
		Vec3<T> temp = Vec3(0, 1, 0);
		Vec3<T> right = forward.cross(temp);
		Vec3<T> up = forward.cross(right);

		Matrix4x4<T> ret;
		ret[0][0] = right.x;
		ret[0][1] = right.y;
		ret[0][2] = right.z;
		ret[1][0] = up.x;
		ret[1][1] = up.y;
		ret[1][2] = up.z;
		ret[2][0] = forward.x;
		ret[2][1] = forward.y;
		ret[2][2] = forward.z;

		ret[3][0] = from.x;
		ret[3][1] = from.y;
		ret[3][2] = from.z;

		return ret;
	}

	template<typename T>
	static void debug_print(const Matrix4x4<T>& mat)
	{
		std::stringstream ss;
		ss << "{" << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << "\n";
		ss << " " << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << "\n";
		ss << " " << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << "\n";
		ss << " " << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << "}";
		std::cout << ss.str() << std::endl;
	}
};

#endif
#ifndef __MATHS_H__
#define __MATHS_H__
#include <math.h>
#include <iostream>


template<typename T> class Vector2
{
public:
	T x, y;
	Vector2():x(0),y(0){}
	Vector2(T _x, T _y) :x(_x), y(_y) {};
	~Vector2() {};
	T dot(const Vector2<T>& v){ return x * v.x + y * v.y; }
	Vector2<T> operator +(const Vector2<T>& rhs){ return Vector2<T>(x + rhs.x, y + rhs.y); }
	Vector2<T> operator -(const Vector2<T>& rhs){ return Vector2<T>(x - rhs.x, y - rhs.y); }
	Vector2<T> operator *(T t){ return Vector2<T>(x * t, y * t); }
	template<typename U> friend std::ostream& operator <<(std::ostream& out, const Vector2<U>& v);
	
};
template<typename T> std::ostream& operator <<(std::ostream& out, const Vector2<T>& v)
{
	out << "x: " << v.x << ", y: " << v.y;
	return out;
}
template<typename T> class Vector4;
template<typename T> class Vector3
{
public:
	T x, y, z;
	Vector3():x(0),y(0),z(0){}
	Vector3(T _x, T _y, T _z) :x(_x), y(_y), z(_z) {}
	~Vector3(){}
	T dot(const Vector3<T>& v){ return x * v.x + y * v.y + z * v.z; }
	Vector3<T> cross(const Vector3<T>& v){ return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	T len(){ return std::sqrt(x * x + y * y + z * z); }
	Vector3<T> normalize()
	{
		*this = (*this) * (1.f / this->len());
		return *this;
	}
	Vector4<T> to_homogeneous()
	{
		Vector4<T> h(x, y, z, 1);
		return h;
	}

	Vector3<T> operator +(const Vector3<T>& rhs){ return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3<T> operator -(const Vector3<T>& rhs){ return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3<T> operator *(T t){ return Vector3<T>(x * t, y * t, z * t); }
	template<typename U> friend std::ostream& operator<< (std::ostream& out, const Vector3<U>& v);

};
template<typename T> std::ostream& operator<<(std::ostream& out, const Vector3<T>&v)
{
	out << "x: " << v.x << ", y: " << v.y << ", z: " << v.z;
	return out;
}
template<typename T> class Vector4
{
public:
	T x, y, z, w;
	T arr[4];
	Vector4():x(0),y(0),z(0),w(0){}
	Vector4(T _x, T _y, T _z, T _w) :x(_x), y(_y), z(_z), w(_w) 
	{
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		arr[3] = w;
	}
	~Vector4(){}
   
	T dot(const Vector4<T>& v){ return x * v.x + y * v.y + z * v.z + w * v.w; }
	T* get_vector_arr() { return arr; }


	Vector4<T> operator +(const Vector4<T>& rhs){ return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Vector4<T> operator -(const Vector4<T>& rhs){ return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Vector4<T> operator *(T t){ return Vector4<T>(x * t, y * t, z * t, w * t); }
	template<typename U> friend std::ostream& operator<< (std::ostream& out, const Vector4<U>& v);
};
template<typename T> std::ostream& operator<<(std::ostream& out, const Vector4<T>& v)
{
	out << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << ", w: " << v.w;
	return out;
}

//matrix
template<typename T> class Matrix4
{
public:
	T m[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };

	Matrix4(void){}
	
	~Matrix4(){}

	void set_value(int row, int col, T val)
	{
		m[row][col] = val;
	}
	void set_identity()
	{
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = (T)1;
		m[0][1] = m[0][2] = m[0][3] = (T)0;
		m[1][0] = m[1][2] = m[1][3] = (T)0;
		m[2][0] = m[2][1] = m[2][3] = (T)0;
		m[3][0] = m[3][1] = m[3][2] = (T)0;
	}
	void set_zeros()
	{
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = (T)0;
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = (T)0;
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = (T)0;
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = (T)0;
	}
	Matrix4<T> transpose()
	{
		Matrix4<T> tp;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				tp.m[i][j] = m[j][i];
			}
		}
		return tp;
	}
	//Matrix4<T> inverse();
	//Matrix4<T> translate(T x, T y, T z);
	//Matrix4<T> scale(T x, T y, T z);
	//Matrix4<T> rotate_round_axis(T angle, Vector3<T> axis);
	
	//matrix multiply matrix
	Matrix4<T> operator *(const Matrix4<T>& rhs)
	{
		Matrix4<T> re_matrix;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				re_matrix.m[i][j] = m[i][0] * rhs[0][j] + m[i][1] * rhs[1][j] + m[i][2] * rhs[2][j] + m[i][3] * rhs[3][j];
			}
		}
		return re_matrix;
	}
	
	Vector4<T> operator *( Vector4<T>& v) //matrix multiply vector4
	{
		T* v_arr = v.get_vector_arr();
		T rv[4] = { 0,0,0,0 }; //result vector
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				rv[i] += m[i][j] * v_arr[j];
			}
		}
		
		return Vector4<T>(rv[0], rv[1], rv[2], rv[3]);
	}

	

	template<typename U> friend std::ostream& operator<< (std::ostream& out, const Matrix4<U>& mat);
};
template<typename T> std::ostream& operator<< (std::ostream& out, const Matrix4<T>& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			out << mat.m[i][j] << ",";
		}
		out << std::endl;
	}
	return out;
}

typedef Vector2<float> float2;
typedef Vector2<int> int2;
typedef Vector3<float> float3;
typedef Vector3<int> int3;
typedef Vector4<float> float4;
typedef Vector4<int> int4;

typedef Matrix4<float> float4x4;
typedef Matrix4<int> int4x4;

#endif //__MATHS_H__
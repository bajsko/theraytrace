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
        for(uint8_t i = 0; i < 4; i++) {
            for(uint8_t j = 0; j < 4; j++) {
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
        if(w != 1 && w != 0)
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
        for(uint8_t i = 0; i < 4; i++)
        {
            for(uint8_t j = 0; j < 4; j++)
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
        Matrix4x4<T> t (*this);
        
        // Forward elimination
        for (i = 0; i < 3 ; i++) {
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
    
    T m[4][4] = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}}; //set it to the identity matrix as default
};

typedef Matrix4x4<float> mat44f;
typedef Matrix4x4<double> mat44d;

//Helper class containing utility functions
class Mat44Util
{
public:
    
    template<typename T>
    static Matrix4x4<T> rotate_x(float rad)
    {
        
        Matrix4x4<T> ret;
        ret[1][1] = (T)cos(rad);
        ret[1][2] = (T)sin(rad);
        ret[2][1] = (T)-sin(rad);
        ret[2][2] = (T)cos(rad);
        return ret;
    }
    
    template<typename T>
    static Matrix4x4<T> rotate_y(float rad)
    {
        rad = roundf(rad);
        Matrix4x4<T> ret;
        ret[0][0] = (T)cos(rad);
        ret[2][0] = (T)sin(rad);
        ret[0][2] = (T)-sin(rad);
        ret[2][2] = (T)cos(rad);
        return ret;
    }
    
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

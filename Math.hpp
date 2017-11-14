#ifndef _MATH_H
#define _MATH_H
#include <math.h>

namespace Math{
    typedef float Scalar;
    typedef int Bool;

    class Vector3{
        public:
            Scalar X;
            Scalar Y;
            Scalar Z;

            Vector3();
            Vector3(Scalar x, Scalar y, Scalar z);

            Vector3 operator+(const Vector3& vector) const;
            Vector3 operator-(const Vector3& vector) const;
            Vector3 operator-() const;
            Vector3 operator*(Scalar num) const;
            Vector3 operator/(Scalar num) const;
            Vector3& operator+=(const Vector3 vector) const;
            Vector3& operator-=(const Vector3 vector) const;
            Vector3& operator*=(Scalar num) const;
            Vector3& operator/=(Scalar num) const;

            bool operator==(const Vector3& vector) const;
            bool operator!=(const Vector3& vector) const;
            
    };


    inline Vector3::Vector3(){
    }

    inline Vector3::Vector3(Scalar x, Scalar y, Scalar z) : X(x), Y(y), Z(z){
    }

    Vector3& Vector3::operator+=(const Vector3& vector){
        X += vector.X;
        Y += vector.Y;
        Z += vector.Z;
        return *this;
    }

    inline Vector3& Vector3::operator-=(const Vector3& vector){
        X -= vector.X;
        Y -= vector.Y;
        Z -= vector.Z;
        return *this;
    }

    inline Vector3& Vector3::operator*=(Scalar num){
        X *= num;
        Y *= num;
        Z *= num;
        return *this;
    }

    inline Vector3& Vector3::operator/=(Scalar num){
        X /= num;
        Y /= num;
        Z /= num;
        return *this;
    }


    inline bool Vector3::operator==(const Vector3& vector) const{
        return X == vector.X && Y == vector.Y && Z == vector.Z;
    }
    
    inline bool Vector3::operator!=(const Vector3& vector) const{
        return X != vector.X || Y != vector.Y || Z != vector.Z;
    }

    inline Vector3 Vector3::operator+(const Vector3& vector) const{
        return Vector3(X + vector.X, Y + vector.Y, Z + vector.Z);
    }

    inline Vector3 Vector3::operator-(const Vector3& vector) const{
        return Vector3(X - vector.X, Y - vector.Y, Z - vector.Z);
    }

    inline Vector3 Vector3::operator-() const{
        return Vector3(-X, -Y, -Z);
    }

    inline Vector3 Vector3::operator*(Scalar num) const{
        return Vector3(X * num, Y * num, Z * num);
    }

    inline Vector3 Vector3::operator/(Scalar num) const{
        return Vector3(X / num, Y / num, Z / num);
    }

    Scalar len(const Vector3& vector);
    void Clamp(Vector3& vector, Scalar length);
    void Normalize(Vector3& vector);

    Scalar Dot(const Vector3& vec1, const Vector3& vec2);
    Scalar Angle(Vector3 vec1, Vector3 vec2);

    Vector3 Cross(const Vector3& vec1, const Vector3& vec2);


    /*inline Scalar len(const Vector3& vector){
        return sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);
    }*/

    inline void Normalize(Vector3& vec){
        vec /= len(vec);
    }

    inline Scalar Dot(const Vector3& vec1, const Vector3& vec2){
        return vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z;
    }

    inline Scalar Angle(Vector3 vec1, Vector3 vec2){

    }

    /*inline Vector3 Cross(const Vector3& vec1, const Vector3& vec2){
        return Vector3(
            vec1.Y * vec2.X
        )
    }*/
}

#endif //_MATH_H
#pragma once
#include <unordered_map>
#include "..\..\math.h"

template <typename instanceType>
class VecBase2
{
public:
	VecBase2() {};
	VecBase2(const VecBase2<instanceType>& other)
	{
		x = other.x;
		y = other.y;
	}
	template <typename otherType>
	VecBase2(const VecBase2<otherType>& other) 
	{
		x = (instanceType)other.x;
		y = (instanceType)other.y;
	}

	VecBase2(instanceType x, instanceType y)
	{
		this->x = x;
		this->y = y;
	}

	operator int() const
	{
		return this->x * this->y;
	}

	bool operator==(const VecBase2<instanceType>& other)
	{
		return (this->x == other.x) && (this->y == other.y);
	}

	bool operator!=(const VecBase2<instanceType>& other)
	{
		return !(this == other);
	}

	bool operator>(const VecBase2<instanceType>& other) const
	{
		return (x > other.x) && (y > other.y);
	}

	bool operator<(const VecBase2<instanceType>& other) const
	{
		return (x < other.x) && (y < other.y);
	}

	bool operator>=(const VecBase2<instanceType>& other) const
	{
		return (x >= other.x) && (y >= other.y);
	}

	bool operator<=(const VecBase2<instanceType>& other) const
	{
		return (x <= other.x) && (y <= other.y);
	}


	VecBase2<instanceType> operator+(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x + other.x, this->y + other.y);
	}

	VecBase2<instanceType> operator+=(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x += other.x, this->y += other.y);
	}

	VecBase2<instanceType> operator-(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x - other.x, this->y - other.y);
	}

	VecBase2<instanceType> operator-=(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x -= other.x, this->y -= other.y);
	}

	VecBase2<instanceType> operator*(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x * other.x, this->y * other.y);
	}
	VecBase2<instanceType> operator*(instanceType scalar) const
	{
		return VecBase2<instanceType>(x * scalar, y * scalar);
	}

	VecBase2<instanceType> operator*=(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x *= other.x, this->y *= other.y);
	}
	VecBase2<instanceType>& operator*=(instanceType scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	VecBase2<instanceType> operator/(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x / other.x, this->y / other.y);
	}
	VecBase2<instanceType> operator/(instanceType scalar) const
	{
		return VecBase2<instanceType>(x / scalar, y / scalar);
	}

	VecBase2<instanceType> operator/=(const VecBase2<instanceType>& other)
	{
		return VecBase2<instanceType>(this->x /= other.x, this->y /= other.y);
	}

	VecBase2<instanceType> operator^(const VecBase2<instanceType>& other)
	{
		this->x^ other.x;
		this->y^ other.y;
		return VecBase2<instanceType>(x, y);
	}

	VecBase2<instanceType>& operator++()
	{
		++x;
		++y;
		return *this;
	}

	VecBase2<instanceType> operator++(int)
	{
		VecBase2<instanceType> temp(*this);
		++(*this);
		return temp;
	}

	VecBase2<instanceType>& operator--()
	{
		--x;
		--y;
		return *this;
	}

	VecBase2<instanceType> operator--(int)
	{
		VecBase2<instanceType> temp(*this);
		--(*this);
		return temp;
	}

	VecBase2<instanceType> operator+(instanceType scalar) const
	{
		return VecBase2<instanceType>(x + scalar, y + scalar);
	}

	VecBase2<instanceType> operator-(instanceType scalar) const
	{
		return VecBase2<instanceType>(x - scalar, y - scalar);
	}


	instanceType DotProduct(const VecBase2<instanceType>& v1, const VecBase2<instanceType>& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	instanceType CrossProduct(const VecBase2<instanceType>& v1, const VecBase2<instanceType>& v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	instanceType Magnitude(const VecBase2<instanceType>& vec)
	{
		if constexpr (std::is_same<instanceType, float>::value)
		{
			return SqrtSIMD<float>(this->x * this->x + this->y * this->y);
		}
		else if constexpr (std::is_same<instanceType, double>::value)
		{
			return SqrtSIMD<double>(this->x * this->x + this->y * this->y);
		}
		else if constexpr (std::is_same<instanceType, int>::value)
		{
			return SqrtSIMD<int>(this->x * this->x + this->y * this->y);
		}
		else
		{
			return (instanceType)SqrtSIMD<double>((double)this->x * (double)this->x + (double)this->y * (double)this->y);
		}
	}

	VecBase2<instanceType> Normalize() const
	{
		instanceType mag = Magnitude();
		if (mag != 0)
		{
			return *this / mag;
		}
		else
		{
			// Handle the case where the vector is a zero vector.
			return VecBase2<instanceType>(0, 0);
		}
	}

	instanceType Distance(const VecBase2<instanceType>& other) const
	{
		instanceType dx = x - other.x;
		instanceType dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	instanceType Angle(const VecBase2<instanceType>& other) const
	{
		instanceType dot = DotProduct(*this, other);
		instanceType mag1 = Magnitude(*this);
		instanceType mag2 = Magnitude(other);
		return AcosSIMD(dot / (mag1 * mag2));
	}
	instanceType AngleBetween(const VecBase2<instanceType>& other) const
	{
		instanceType dot = dotProduct(*this, other);
		instanceType magProduct = Magnitude() * other.magnitude();

		// Handle the case where the vectors are parallel or zero vectors.
		if (magProduct == 0)
		{
			return 0;
		}

		return std::acos(dot / magProduct);
	}

	VecBase2<instanceType> Project(const VecBase2<instanceType>& other) const
	{
		instanceType dot = DotProduct(*this, other);
		instanceType magSq = other.x * other.x + other.y * other.y;
		return other * (dot / magSq);
	}

	VecBase2<instanceType> Perpendicular() const
	{
		return VecBase2<instanceType>(-y, x);
	}

	VecBase2<instanceType> abs() const
	{
		return VecBase2<instanceType>(AbsSIMD(x), AbsSIMD(y));
	}

	instanceType magnitudeSquared() const
	{
		return x * x + y * y;
	}

	VecBase2<instanceType> operator-() const
	{
		return VecBase2<instanceType>(-x, -y);
	}

	instanceType distanceSquared(const VecBase2<instanceType>& other) const
	{
		instanceType dx = other.x - x;
		instanceType dy = other.y - y;
		return dx * dx + dy * dy;
	}

	VecBase2<instanceType> clamp(const VecBase2<instanceType>& minVal, const VecBase2<instanceType>& maxVal) const
	{
		instanceType clampedX = math::clamp(x, minVal.x, maxVal.x);
		instanceType clampedY = math::clamp(y, minVal.y, maxVal.y);
		return VecBase2<instanceType>(clampedX, clampedY);
	}
	VecBase2<instanceType> lerp(const VecBase2<instanceType>& end, instanceType t) const
	{
		instanceType interpolatedX = (1 - t) * x + t * end.x;
		instanceType interpolatedY = (1 - t) * y + t * end.y;
		return VecBase2<instanceType>(interpolatedX, interpolatedY);
	}

	VecBase2<instanceType> rotate(instanceType angle) const
	{
		instanceType cosAngle =	CosNonSIMD(angle);
		instanceType sinAngle = SinNonSIMD(angle);
		instanceType rotatedX = cosAngle * x - sinAngle * y;
		instanceType rotatedY = sinAngle * x + cosAngle * y;
		return VecBase2<instanceType>(rotatedX, rotatedY);
	}


	bool isOrthogonal(const VecBase2<instanceType>& other) const
	{
		return (x * other.x + y * other.y) == 0;
	}

	bool isPerpendicular(const VecBase2<instanceType>& other) const
	{
		return dotProduct(*this, other) == 0;
	}



	struct Hash
	{
		std::size_t operator()(const VecBase2<instanceType>& vec) const
		{
			std::size_t xHash = std::hash<instanceType>{}(vec.x);
			std::size_t yHash = std::hash<instanceType>{}(vec.y);
			return xHash ^ yHash;
		}
	};



public:
	instanceType x;
	instanceType y;
};

typedef VecBase2<int> IVec2;
typedef VecBase2<unsigned int> UVec2;
typedef VecBase2<double> DVec2;
typedef VecBase2<float> FVec2;



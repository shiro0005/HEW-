#pragma once
#include <math.h>

#define _OX_EPSILON_	0.000001f	// 誤差

// 2成分float
struct Float2 {
	float x, y;

	Float2() {}
	Float2(float x, float y) : x(x), y(y) {}
	~Float2() {}

	Float2 operator +(const Float2 &r) const {
		return Float2(x + r.x, y + r.y);
	}

	Float2 operator -(const Float2 &r) const {
		return Float2(x - r.x, y - r.y);
	}

	Float2 operator -() const {
		return Float2(x * -1.0f, y * -1.0f);
	}

	Float2 operator *(const Float2 &r) const {
		return Float2(x * r.x, y * r.y);
	}

	Float2 operator /(const Float2 &r) const {
		return Float2(x / r.x, y / r.y);
	}

	Float2 operator *(float r) const {
		return Float2(x * r, y * r);
	}

	Float2 operator /(float r) const {
		return Float2(x / r, y / r);
	}

	friend Float2 operator *(float l, const Float2 &r) {
		return Float2(r.x * l, r.y * l);
	}

	friend Float2 operator /(float l, const Float2 &r) {
		return Float2(r.x / l, r.y / l);
	}

	float dot(const Float2 &r) const {
		return x * r.x + y * r.y;
	}

	float cross(const Float2 &r) const {
		return x * r.y - y * r.x;
	}

	float length() const {
		return sqrtf(lengthSq());
	}

	float lengthSq() const {
		return x * x + y * y;
	}

	void norm() {
		const float len = length();
		if (len > 0.0f) {
			x /= len;
			y /= len;
		}
	}

	Float2 getNorm() const {
		const float len = length();
		if (len > 0.0f) {
			return Float2(x / len, y / len);
		}
		return Float2(0.0f, 0.0f);
	}
};


// 点
typedef Float2 Point2D;


// ベクトル
struct Vec2 : public Float2 {
	Vec2() {}
	Vec2(float x, float y) : Float2(x, y) {}
	Vec2(const Float2 &r) : Float2(r) {}
	~Vec2() {}

	Vec2& operator =(const Float2 &r) {
		x = r.x;
		y = r.y;
		return *this;
	}

	// 標準化
	void norm() {
		const float len = length();
		if (len > 0.0f) {
			x /= len;
			y /= len;
		}
	}

	// 垂直関係にある？
	bool isVertical(const Vec2 &r) const {
		float d = dot(r);
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら垂直と判定
	}

	// 平行関係にある？
	bool isParallel(const Vec2 &r) const {
		float d = cross(r);
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定
	}

	// 鋭角関係？
	bool isSharpAngle(const Vec2 &r) const {
		return (dot(r) >= 0.0f);
	}
};


// 直線
struct Line2D {
	Point2D p;
	Vec2 v;		// 方向ベクトル
	Line2D() : p(0.0f, 0.0f), v(1.0f, 0.0f) {}
	Line2D(const Point2D &p, const Vec2 &v) : p(p), v(v) {}
	~Line2D() {}

	// 点上の座標を取得
	//  ベクトルに掛け算する係数
	Point2D getPoint(float t) const {
		return p + t * v;
	}
};


// 線分
struct Segment2D : public Line2D {

	Segment2D() {}
	Segment2D(const Point2D &p, const Vec2 &v) : Line2D(p, v) {}
	Segment2D(const Point2D &p1, const Point2D &p2) : Line2D(p1, p2 - p1) {}

	// 終点を取得
	Float2 getEndPoint() const {
		return p + v;//ベクトル？
	}
};


// 円
struct Circle2D {
	Point2D p;
	float r;	// 半径
	Circle2D() : p(0.0f, 0.0f), r(0.5f) {}
	Circle2D(const Point2D &p, float r) : p(p), r(r) {}
	~Circle2D() {}
};

// カプセル
struct Capsule2D {
	Segment2D s;//多分線分　始点とベクトルが内蔵されてるのでは？
	float r;	// 半径
	

	//いらない気がする
	Capsule2D() : r(0.5f) {}
	Capsule2D(const Segment2D &s, float r) : s(s), r(r) {}
	Capsule2D(const Point2D &p1, const Point2D &p2, float r) : s(p1, p2), r(r) {}
	~Capsule2D() {}
};


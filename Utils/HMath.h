#pragma once
#define PI (3.1415927f)

#include <math.h>
#include <vector>
#include <exception>

static constexpr float DEG_RAD2 = PI / 360.0f;
static constexpr float DEG_RAD = 180.0f / PI;
static constexpr float RAD_DEG = PI / 180.f;

inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

struct vec2_t {

	union {
		struct {
			float x, y;
		};
		float floatArr[2];
	};
	vec2_t() { x = y = 0; }
	vec2_t(float a, float b) : x(a), y(b) {}
	vec2_t(int a, int b) : x((float)a), y((float)b) {}
	vec2_t &operator=(const vec2_t &copy) {
		x = copy.x;
		y = copy.y;
		return *this;
	}

	static vec2_t fromAngle(float angle){
		return vec2_t((float)-sin(angle), (float)cos(angle));
	}

	bool operator==(const vec2_t &o) const { return x == o.x && y == o.y; }
	bool operator!=(const vec2_t &o) const { return x != o.x || y != o.y; }

	vec2_t sub(float f) {
		return vec2_t(x - f, y - f);
	}
	vec2_t sub(float ox, float oy) {
		return vec2_t(x - ox, y - oy);
	}
	vec2_t div(float f) {
		return vec2_t(x / f, y / f);
	}

	vec2_t div(const vec2_t &o) {
		return vec2_t(x / o.x, y / o.y);
	}
	vec2_t mul(const vec2_t &o) {
		return vec2_t(x * o.x, y * o.y);
	}
	vec2_t mul(float f) {
		return vec2_t(x * f, y * f);
	}
	vec2_t sub(const vec2_t &o) {
		return vec2_t(x - o.x, y - o.y);
	}
	vec2_t add(const vec2_t &o) {
		return vec2_t(x + o.x, y + o.y);
	}
	vec2_t add(float o) {
		return vec2_t(x + o, y + o);
	}
	vec2_t add(float ox, float oy) {
		return vec2_t(x + ox, y + oy);
	}

	vec2_t normalized() {
		return div(magnitude());
	}

	vec2_t cross(){
		return vec2_t(-y, x);
	}

	float dot(float ox, float oy) const { return x * ox + y * oy; }


	float dot(const vec2_t &o) const { return x * o.x + y * o.y; }

	vec2_t normAngles() {
		float x = this->x;
		float y = this->y;
		while (x > 90.f)
			x -= 180.0f;
		while (x < -90.f)
			x += 180.0f;

		while (y > 180.0f)
			y -= 360.0f;
		while (y < -180.0f)
			y += 360.0f;
		return vec2_t(x, y);
	}

	float squaredlen() const { return x * x + y * y; }
	float magnitude() const { return sqrtf(squaredlen()); }
};

struct vec3_t {
	union {
		struct {
			float x, y, z;
		};
		float floatArr[3];
	};

	vec3_t() { x = y = z = 0; }
	vec3_t(int a, int b, int c) : x((float)a), y((float)b), z((float)c) {}
	vec3_t(double a, double b, double c) : x((float)a), y((float)b), z((float)c) {}
	vec3_t(float a, float b, float c) : x(a), y(b), z(c) {}
	vec3_t(float a, float b) : x(a), y(b), z(0) {}
	vec3_t(const vec2_t &copy, float zP) {
		x = copy.x;
		y = copy.y;
		z = zP;
	}
	vec3_t(const vec3_t &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	vec3_t &operator=(const vec3_t &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;

		return *this;
	}

	vec3_t(float *v) : x(v[0]), y(v[1]), z(v[2]) {}

	bool iszero() const { return x == 0 && y == 0 && z == 0; }

	bool operator==(const vec3_t &o) const { return x == o.x && y == o.y && z == o.z; };
	bool operator!=(const vec3_t &o) const { return x != o.x || y != o.y || z != o.z; };
	vec3_t operator-() const { return vec3_t(-x, -y, -z); };
	float operator[](int i) { return getElement(i); }

	vec3_t mul(const vec3_t& o) {
		return vec3_t(x * o.x, y * o.y, z * o.z);
	};
	vec3_t mul(float f) {
		return vec3_t(x * f, y * f, z * f);
	};
	vec3_t mul(float x1, float y1, float z1) {
		return vec3_t(x * x1, y * y1, z * z1);
	};
	vec3_t div(float f) {
		return vec3_t(x / f, y / f, z / f);
	};
	vec3_t div(float x1, float y1, float z1) {
		return vec3_t(x / x1, y / y1, z / z1);
	};
	vec3_t div(const vec3_t& o) {
		return vec3_t(x / o.x, y / o.y, z / o.z);
	};
	vec3_t add(float f) {
		return vec3_t(x + f, y + f, z + f);
	};
	vec3_t add(float x1, float y1, float z1) const{
		return vec3_t(x + x1, y + y1, z + z1);
	};
	vec3_t sub(float f) {
		return vec3_t(x - f, y - f, z - f);
	};

	vec3_t sub(float x1, float y1, float z1) {
		return vec3_t(x - x1, y - y1, z - z1);
	};

	vec3_t floor() {
		return vec3_t(floorf(x), floorf(y), floorf(z));
	};

	vec3_t add(const vec3_t &o) const {
		return vec3_t(x + o.x, y + o.y, z + o.z);
	}
	vec3_t sub(const vec3_t &o) const {
		return vec3_t(x - o.x, y - o.y, z - o.z);
	}


	float squaredlen() const { return x * x + y * y + z * z; }
	float squaredxzlen() const { return x * x + z * z; }

	vec3_t lerp(const vec3_t& other, float tx, float ty, float tz) const {
		vec3_t ne;
		ne.x = x + tx * (other.x - x);
		ne.y = y + ty * (other.y - y);
		ne.z = z + tz * (other.z - z);
		return ne;
	}

	vec3_t lerp(const vec3_t other, float val) const {
		vec3_t ne;
		ne.x = x + val * (other.x - x);
		ne.y = y + val * (other.y - y);
		ne.z = z + val * (other.z - z);
		return ne;
	}

	vec3_t lerp(const vec3_t *other, float val) const {
		vec3_t ne;
		ne.x = x + val * (other->x - x);
		ne.y = y + val * (other->y - y);
		ne.z = z + val * (other->z - z);
		return ne;
	}

	vec2_t flatten() const {
		return vec2_t(x, y);
	}

	float sqrxy() const { return x * x + y * y; }

	float dot(const vec3_t &o) const { return x * o.x + y * o.y + z * o.z; }
	float dotxy(const vec3_t &o) const { return x * o.x + y * o.y; }

	float magnitude() const { return sqrtf(squaredlen()); }

	vec3_t normalize() {
		return div(magnitude());
	}

	float dist(const vec3_t &e) const {
		return sub(e).magnitude();
	}

	float Get2DDist(const vec3_t &e) const {
		float dx = e.x - x, dy = e.y - y;
		return sqrtf(dx * dx + dy * dy);
	}

	float magnitudexy() const { return sqrtf(x * x + y * y); }
	float magnitudexz() const { return sqrtf(x * x + z * z); }

	vec3_t cross(const vec3_t &b) {
		return vec3_t(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
	float cxy(const vec3_t &a) { return x * a.y - y * a.x; }

	vec2_t CalcAngle(vec3_t dst) {
		vec3_t diff = dst.sub(*this);

		diff.y = diff.y / diff.magnitude();
		vec2_t angles;
		angles.x = asinf(diff.y) * -DEG_RAD;
		angles.y = (float)-atan2f(diff.x, diff.z) * DEG_RAD;

		return angles;
	}
	float getElement(int i) const{
		//float* it = nullptr;
		switch (i) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				throw std::exception("Parameter was outside the given range");
		}
	}
	float* getElementPtr(int i) const{
		switch (i) {
			case 0:
				return (float *)&x;
			case 1:
				return (float *)&y;
			case 2:
				return (float*)&z;
			default:
				return nullptr;
		}
	}
};

struct vec3_ti {
	int x, y, z;

	vec3_ti() { x = y = z = 0; }
	vec3_ti(int a, int b, int c) : x(a), y(b), z(c) {}
	vec3_ti(int a, int b) : x(a), y(b), z(0) {}
	vec3_ti(const vec3_ti &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	vec3_ti(const vec3_t &copy) {
		x = (int)copy.x;
		y = (int)copy.y;
		z = (int)copy.z;
	}

	vec3_ti &operator=(const vec3_ti &copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;

		return *this;
	}

	vec3_ti *operator=(const vec3_ti *copy) {
		x = copy->x;
		y = copy->y;
		z = copy->z;

		return this;
	}

	vec3_ti(int *v) : x(v[0]), y(v[1]), z(v[2]) {}
	
	vec3_t toVec3t() const {
		return vec3_t(x, y, z);
	}

	bool iszero() const { return x == 0 && y == 0 && z == 0; }

	bool operator==(const vec3_ti &o) const { return x == o.x && y == o.y && z == o.z; }
	bool operator!=(const vec3_ti &o) const { return x != o.x || y != o.y || z != o.z; }

	vec3_ti add(vec3_ti o) const {
		return vec3_ti(x + o.x, y + o.y, z + o.z);
	}

	vec3_ti add(int f) const {
		return vec3_ti(x + f, y + f, z + f);
	}

	vec3_ti add(int a, int b, int c) const {
		return vec3_ti(x + a, y + b, z + c);
	};

	vec3_ti sub(int ox, int oy, int oz) const {
		return vec3_ti(x - ox, y - oy, z - oz);
	}

	vec3_ti sub(const vec3_ti& o) const {
		return vec3_ti(x - o.x, y - o.y, z - o.z);
	}

	void set(vec3_ti *o) {
		x = o->x;
		y = o->y;
		z = o->z;
	};

	vec3_t toFloatVector() const {
		vec3_t vec;
		vec.x = (float)x;
		vec.y = (float)y;
		vec.z = (float)z;
		return vec;
	}
};
struct vec4_t {
	union {
		struct {
			float x, y, z, w;
		};
		float v[4];
	};
	vec4_t() { x = 0, y = 0, z = 0, w = 0; }
	explicit vec4_t(const vec3_t &p, float w = 0) : x(p.x), y(p.y), z(p.z), w(w){};
	vec4_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){};
	float &operator[](int i) { return v[i]; };
	float operator[](int i) const { return v[i]; };

	__forceinline bool contains(vec2_t *point) {
		if (point->x <= x || point->y <= y)
			return false;

		if (point->x >= z || point->y >= w)
			return false;
		return true;
	};
};

struct glmatrixf {
	union {
		float v[16];
		float v_nested[4][4];
	};

	__forceinline float operator[](int i) const { return v[i]; }
	__forceinline float &operator[](int i) { return v[i]; }

#define MULMAT(row, col) v[col + row] = x[row] * y[col] + x[row + 4] * y[col + 1] + x[row + 8] * y[col + 2] + x[row + 12] * y[col + 3];

	template <class XT, class YT>
	void mul(const XT x[16], const YT y[16]) {
		MULMAT(0, 0);
		MULMAT(1, 0);
		MULMAT(2, 0);
		MULMAT(3, 0);
		MULMAT(0, 4);
		MULMAT(1, 4);
		MULMAT(2, 4);
		MULMAT(3, 4);
		MULMAT(0, 8);
		MULMAT(1, 8);
		MULMAT(2, 8);
		MULMAT(3, 8);
		MULMAT(0, 12);
		MULMAT(1, 12);
		MULMAT(2, 12);
		MULMAT(3, 12);
	}

#undef MULMAT

	glmatrixf *correct() {
		glmatrixf *newMatPtr = new glmatrixf;

		for (int i = 0; i < 4; i++) {
			newMatPtr->v[i * 4 + 0] = v[0 + i];
			newMatPtr->v[i * 4 + 1] = v[4 + i];
			newMatPtr->v[i * 4 + 2] = v[8 + i];
			newMatPtr->v[i * 4 + 3] = v[12 + i];
		}
		return newMatPtr;
	};

	inline bool OWorldToScreen(vec3_t origin, vec3_t pos, vec2_t &screen, vec2_t fov, vec2_t displaySize) {
		pos = pos.sub(origin);

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	inline void mul(const glmatrixf &x, const glmatrixf &y) {
		mul(x.v, y.v);
	}

	inline void translate(float x, float y, float z) {
		v[12] += x;
		v[13] += y;
		v[14] += z;
	}

	inline void translate(const vec3_t &o) {
		translate(o.x, o.y, o.z);
	}

	inline void scale(float x, float y, float z) {
		v[0] *= x;
		v[1] *= x;
		v[2] *= x;
		v[3] *= x;
		v[4] *= y;
		v[5] *= y;
		v[6] *= y;
		v[7] *= y;
		v[8] *= z;
		v[9] *= z;
		v[10] *= z;
		v[11] *= z;
	}

	inline void invertnormal(vec3_t &dir) const {
		vec3_t n(dir);
		dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
		dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
		dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
	}

	inline void invertvertex(vec3_t &pos) const {
		vec3_t p(pos);
		p.x -= v[12];
		p.y -= v[13];
		p.z -= v[14];
		pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
		pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
		pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
	}

	inline void transform(const vec3_t &in, vec4_t &out) const {
		out.x = transformx(in);
		out.y = transformy(in);
		out.z = transformz(in);
		out.w = transformw(in);
	}

	__forceinline float transformx(const vec3_t &p) const {
		return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
	}

	__forceinline float transformy(const vec3_t &p) const {
		return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
	}

	__forceinline float transformz(const vec3_t &p) const {
		return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
	}

	__forceinline float transformw(const vec3_t &p) const {
		return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
	}

	__forceinline vec3_t gettranslation() const {
		return vec3_t(v[12], v[13], v[14]);
	}

	//assault cube world2screen
	vec3_t transform(glmatrixf *matrix, vec3_t &totransform) {
		return vec3_t(matrix->transformx(totransform),
					  matrix->transformy(totransform),
					  matrix->transformz(totransform))
			.div(matrix->transformw(totransform));
	}

	///pos should be the exact center of the enemy model for scaling to work properly
	vec3_t WorldToScreen(vec3_t pos, int width, int height) {
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		vec4_t clipCoords;
		clipCoords.x = pos.x * v[0] + pos.y * v[4] + pos.z * v[8] + v[12];
		clipCoords.y = pos.x * v[1] + pos.y * v[5] + pos.z * v[9] + v[13];
		clipCoords.z = pos.x * v[2] + pos.y * v[6] + pos.z * v[10] + v[14];
		clipCoords.w = pos.x * v[3] + pos.y * v[7] + pos.z * v[11] + v[15];

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		vec3_t NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		//viewport tranform to screenCooords

		vec3_t playerscreen;
		playerscreen.x = (width / 2 * NDC.x) + (NDC.x + width / 2);
		playerscreen.y = -(height / 2 * NDC.y) + (NDC.y + height / 2);

		return playerscreen;
	}
};

struct Ray {
private:
#define NaNvec3 vec3_t{NAN, NAN, NAN};
#define Infvec3 vec3_t{INFINITY, INFINITY, INFINITY};

public:
	vec3_t origin;
	vec3_t dir;
	// Set to NAN for infinte Lenght
	float lenght;
	// dir w/ its actual lenght
	inline vec3_t DirrLenght() {
		if (lenght == NAN)
			return dir.mul(INFINITY);
		else
			return dir.mul(lenght);
		//return NaNvec3;
	}
	// the place at which the ray stops
	inline vec3_t out() {
		//if (lenght == NAN)
		//	return NaNvec3;
		//else
		return origin.add(DirrLenght());
	}
	Ray(vec3_t origin, vec3_t dir) {
		this->lenght = dir.magnitude();
		this->dir = dir.normalize();
		this->origin = origin;
	}
	Ray(vec3_t origin, vec3_t dir, float lenght = NAN) {
		this->origin = origin;
		this->dir = dir.normalize();
		this->lenght = lenght;
	}
	// TODO: Colision-detection
};

struct AABB {
	vec3_t lower;
	vec3_t upper;
	bool isZero = false;
	char padding[3];
#pragma warning(push)
#pragma warning(disable : 26495)
	AABB() {}
	AABB(vec3_t l, vec3_t h) : lower(l), upper(h){};
	AABB(const AABB &aabb) {
		lower = vec3_t(aabb.lower);
		upper = vec3_t(aabb.upper);
	}
	AABB(vec3_t lower, float width, float height) {
		this->lower = lower;
		this->upper = {lower.x + width, lower.y + height, lower.z + width};
	}
	AABB(float width, float height, vec3_t Middle) {
		this->lower = Middle.sub(height / 2, width / 2, height / 2);
		this->upper = Middle.add(height / 2, width / 2, height / 2);
	}
	AABB(vec3_t lower, float width, float height, float eyeHeight) {
		lower = lower.sub(vec3_t(width, eyeHeight * 2, width).div(2));
		this->lower = lower;
		this->upper = {lower.x + width, lower.y + height, lower.z + width};
	}
#pragma warning(pop)
	bool operator==(const AABB &rhs) const {
		return lower == rhs.lower && upper == rhs.upper;
	}

	bool isFullBlock() const {
		auto diff = lower.sub(upper);
		return fabsf(diff.y) == 1 && fabsf(diff.x) == 1 && fabsf(diff.z) == 1;
	}

	AABB expanded(float amount) {
		return AABB(lower.sub(amount), upper.add(amount));
	}

	AABB expandedXZ(float amount) {
		return AABB(lower.sub(amount, 0.f, amount), upper.add(amount, 0.f, amount));
	}

	vec3_t centerPoint() const {
		vec3_t diff = upper.sub(lower);
		return lower.add(diff.mul(0.5f));
	}

	bool intersects(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			   aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			   aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

	bool intersectsXZ(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			   aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

	bool contains(vec3_t p) {
		return this->lower.x < p.x && this->upper.x > p.x &&
			   this->lower.y < p.y && this->upper.y > p.y &&
			   this->lower.z < p.z && this->upper.z > p.z;
	}

	float DistToAABB(vec3_t p) {
		if (contains(p))
			return 0.f;
		return p.dist(ClosestPointAABB(p));
	}

	vec3_t ClosestPointAABB(vec3_t p) {
		vec3_t q;
		// For each coordinate axis, if the point coordinate value is
		// outside box, clamp it to the box, else keep it as is
		float v = p.x;
		if (v < this->lower.x)
			v = this->lower.x;
		else if (v > this->upper.x)
			v = this->upper.x;
		q.x = v;

		v = p.y;
		if (v < this->lower.y)
			v = this->lower.y;
		else if (v > this->upper.y)
			v = this->upper.y;
		q.y = v;

		v = p.z;
		if (v < this->lower.z)
			v = this->lower.z;
		else if (v > this->upper.z)
			v = this->upper.z;
		q.z = v;
		
		return q;
	}

	vec3_t ClosestPointAABB(AABB p) {
		vec3_t q;
		// For each coordinate axis, if the point coordinate value is
		// outside box, clamp it to the box, else keep it as is
		float v = p.lower.x;
		if (v < this->lower.x)
			v = this->lower.x;
		else if (v > this->upper.x)
			v = this->upper.x;
		q.x = v;

		v = p.lower.y;
		if (v < this->lower.y)
			v = this->lower.y;
		else if (v > this->upper.y)
			v = this->upper.y;
		q.y = v;

		v = p.lower.z;
		if (v < this->lower.z)
			v = this->lower.z;
		else if (v > this->upper.z)
			v = this->upper.z;
		q.z = v;
		

		v = p.upper.x;
		if (v < this->lower.x)
			v = this->lower.x;
		else if (v > this->upper.x)
			v = this->upper.x;
		q.x = v;

		v = p.upper.y;
		if (v < this->lower.y)
			v = this->lower.y;
		else if (v > this->upper.y)
			v = this->upper.y;
		q.y = v;

		v = p.upper.z;
		if (v < this->lower.z)
			v = this->lower.z;
		else if (v > this->upper.z)
			v = this->upper.z;
		q.z = v;
		
		return q;
	}
	
	std::vector<vec3_t> getCorners() {
		std::vector<vec3_t> corners;
		corners.push_back({upper.x, upper.y, upper.z});
		corners.push_back({lower.x, upper.y, upper.z});
		corners.push_back({lower.x, upper.y, lower.z});
		corners.push_back({upper.x, upper.y, upper.z});
		corners.push_back({upper.x, lower.y, lower.z});
		corners.push_back({upper.x, lower.y, upper.z});
		corners.push_back({lower.x, lower.y, lower.z});
		corners.push_back({lower.x, lower.y, lower.z});
		return corners;
	}
	std::vector<vec3_t> getSamples() {
		std::vector<vec3_t> corners;
		vec3_t size = upper.sub(lower);
		vec3_t amount = {2*size.x +1, 2*size.y +1, 2*size.y +1};
		vec3_t step = {1 / amount.x, 1 / amount.y, 1 / amount.z};
		for (int x = 0; x < (int)round(amount.x); x++) {
			for (int y = 0; y < (int)round(amount.y); y++) {
				for (int z = 0; z < (int)round(amount.z); z++) {
					vec3_t diff = step.mul(vec3_ti{x, y, z}.toVec3t());
					corners.push_back(lower.add(diff));
				}
			}
		}
		return corners;
	}
	AABB sort() {
		vec3_t min = {std::min(lower.x, upper.x), std::min(lower.y, upper.y), std::min(lower.z, upper.z)};
		vec3_t max = {std::max(lower.x, upper.x), std::max(lower.y, upper.y), std::max(lower.z, upper.z)};
		return AABB(min, max);
	}
	void Sort() {
		lower = {std::min(lower.x, upper.x), std::min(lower.y, upper.y), std::min(lower.z, upper.z)};
		upper = {std::max(lower.x, upper.x), std::max(lower.y, upper.y), std::max(lower.z, upper.z)};
	}

#define Vector3D vec3_t
	//Helper function for Line/AABB test.  Tests collision on a single dimension
	//Param:    Start of line, Direction/length of line,
	//          Min value of AABB on plane, Max value of AABB on plane
	//          Enter and Exit "timestamps" of intersection (OUT)
	//Return:   True if there is overlap between Line and AABB, False otherwise
	//Note:     Enter and Exit are used for calculations and are only updated in case of intersection
	bool Line_AABB_1d(float start, float dir, float min, float max, float* enter, float* exit) {
		//If the line segment is more of a point, just check if it's within the segment
		if(fabs(dir) < 1.0E-8)
			return (start >= min && start <= max);

		//Find if the lines overlap
		float ooDir = 1.0f / dir;
		float t0 = (min - start) * ooDir;
		float t1 = (max - start) * ooDir;

		//Make sure t0 is the "first" of the intersections
		if(t0 > t1)
			std::swap(t0, t1);

		//Check if intervals are disjoint
		if(t0 > *exit || t1 < *enter)
			return false;

		//Reduce interval based on intersection
		if(t0 > *enter)
			*enter = t0;
		if(t1 < *exit)
			*exit = t1;

		return true;
	}

	//Check collision between a line segment and an AABB
	//Param:    Start point of line segement, End point of line segment,
	//          One corner of AABB, opposite corner of AABB,
	//          Location where line hits the AABB (OUT)
	//Return:   True if a collision occurs, False otherwise
	//Note:     If no collision occurs, OUT param is not reassigned and is not considered useable
	inline bool Line_AABB(const Vector3D& start, const Vector3D& end, Vector3D* hitPoint = nullptr) {
		bool ret = true;
		float enter = 0.0f;
		float exit = 1.0f;
		Vector3D dir = end.sub(start);
		AABB tmp = this->sort();
		const Vector3D &min = tmp.lower;
		const Vector3D &max = tmp.upper;
		//Check each dimension of Line/AABB for intersection
		if(!Line_AABB_1d(start.x, dir.x, min.x, max.x, &enter, &exit))
			ret = false;
		if(!Line_AABB_1d(start.y, dir.y, min.y, max.y, &enter, &exit))
			ret = false;
		if(!Line_AABB_1d(start.z, dir.z, min.z, max.z, &enter, &exit))
			ret = false;
	
		//If there is intersection on all dimensions, report that point
		if (hitPoint != nullptr)
			*hitPoint = dir.mul(enter).add(start);
		return ret;
	}
	bool slabs(vec3_t rayOrigin, vec3_t Raydir) {
		AABB sanitze = this->sort();
		vec3_t t0 = sanitze.lower.sub(rayOrigin).div(Raydir);
		vec3_t t1 = sanitze.upper.sub(rayOrigin).div(Raydir);
		vec3_t tmin = AABB(t0, t1).sort().lower, tmax = AABB(t0, t1).sort().upper;
		return std::max(std::max(tmin.x, tmin.y),tmin.z) <= std::min(std::min(tmax.x,tmax.y),tmax.z);
	}//						tmin.max_component()						tmax.min_component()
	
#ifdef Vector3D
#undef Vector3D
#endif // Vector3D

	// http://www.codercorner.com/RayAABB.cpp
	//
	//typedef unsigned int udword;
	//
	//! Integer representation of a floating-point value.
	//#define IR(x) ((udword&)x)
	//
	//#define RAYAABB_EPSILON 0.00001f
	//#define Point vec3_t
	
//////////////////////////////////////////////////////////////////////////////////////*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to compute a ray-AABB intersection.
 *	Original code by Andrew Woo, from "Graphics Gems", Academic Press, 1990
 *	Optimized code by Pierre Terdiman, 2000 (~20-30% faster on my Celeron 500)
 *	Epsilon value added by Klaus Hartmann. (discarding it saves a few cycles only)
 *
 *	Hence this version is faster as well as more robust than the original one.
 *
 *	Should work provided:
 *	1) the integer representation of 0.0f is 0x00000000
 *	2) the sign bit of the float is the most significant one
 *
 *	Report bugs: p.terdiman@codercorner.com
 *
 *	\param		aabb		[in] the axis-aligned bounding box
 *	\param		origin		[in] ray origin
 *	\param		dir			[in] ray direction
 *	\param		coord		[out] impact coordinates
 *	\return		true if ray intersects AABB
 */
//////////////////////////////////////////////////////////////////////////////////////*////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*bool RayAABB(const vec3_t &origin, const vec3_t &dir, vec3_t* &coord) {
		
		bool Inside = true;
		vec3_t MinB = this->lower;
		vec3_t MaxB = this->upper;
		vec3_t MaxT;
		MaxT.x = MaxT.y = MaxT.z = -1.0f;

		// Find candidate planes.
		for (udword i = 0; i < 3; i++) {
			if (origin.getElement(i) < MinB.getElement(i)) {
				*coord->getElementPtr(i) = MinB.getElement(i);
				Inside = false;
				float curDir = dir.getElement(i);
				// Calculate T distances to candidate planes
				if (IR(curDir)) *MaxT.getElementPtr(i) = (MinB.getElement(i) - origin.getElement(i)) / dir.getElement(i);
			} else if (origin.getElement(i) > MaxB.getElement(i)) {
				*coord->getElementPtr(i) = MaxB.getElement(i);
				Inside = false;
				float curDir = dir.getElement(i);
				// Calculate T distances to candidate planes
				if (IR(curDir)) *MaxT.getElementPtr(i) = (MaxB.getElement(i) - origin.getElement(i)) / dir.getElement(i);
			}
		}
		// Ray origin inside bounding box
		if (Inside) {
			*coord = origin;
			return true;
		}
		// Get largest of the maxT's for final choice of intersection
		udword WhichPlane = 0;
		if (MaxT.getElement(1) > MaxT.getElement(WhichPlane)) WhichPlane = 1;
		if (MaxT.getElement(2) > MaxT.getElement(WhichPlane)) WhichPlane = 2;
		// Check final candidate actually inside box
		float curDir = MaxT.getElement(WhichPlane);
		if (IR(curDir) & 0x80000000) return false;

		for (udword i = 0; i < 3; i++) {
			if (i != WhichPlane) {
				*coord->getElementPtr(i) = origin.getElement(i) + MaxT.getElement(WhichPlane) * dir.getElement(i);
#ifdef RAYAABB_EPSILON
				if (coord->getElement(i) < MinB.getElement(i) - RAYAABB_EPSILON || coord->getElement(i) > MaxB.getElement(i) + RAYAABB_EPSILON) return false;
#else
				if (coord->getElement(i) < MinB.getElement(i) || coord->getElement(i) > MaxB.getElement(i)) return false;
#endif
			}
		}
		return true;  // ray hits box
	}*/
	/*bool RayAABB(const vec3_t &origin, const vec3_t &dir) {
		bool Inside = true;
		vec3_t MinB = this->lower;
		vec3_t MaxB = this->upper;
		vec3_t MaxT;
		vec3_t tempCoord;
		MaxT.x = MaxT.y = MaxT.z = -1.0f;

		// Find candidate planes.
		for (udword i = 0; i < 3; i++) {
			if (origin.getElement(i) < MinB.getElement(i)) {
				*tempCoord.getElementPtr(i) = MinB.getElement(i);
				Inside = false;
				float curDir = dir.getElement(i);
				// Calculate T distances to candidate planes
				if (IR(curDir)) *MaxT.getElementPtr(i) = (MinB.getElement(i) - origin.getElement(i)) / dir.getElement(i);
			} else if (origin.getElement(i) > MaxB.getElement(i)) {
				*tempCoord.getElementPtr(i) = MaxB.getElement(i);
				Inside = false;
				float curDir = dir.getElement(i);
				// Calculate T distances to candidate planes
				if (IR(curDir)) *MaxT.getElementPtr(i) = (MaxB.getElement(i) - origin.getElement(i)) / dir.getElement(i);
			}
		}
		// Ray origin inside bounding box
		if (Inside) {
			tempCoord = origin;
			return true;
		}
		// Get largest of the maxT's for final choice of intersection
		udword WhichPlane = 0;
		if (MaxT.getElement(1) > MaxT.getElement(WhichPlane)) WhichPlane = 1;
		if (MaxT.getElement(2) > MaxT.getElement(WhichPlane)) WhichPlane = 2;
		// Check final candidate actually inside box
		float curDir = MaxT.getElement(WhichPlane);
		if (IR(curDir) & 0x80000000) return false;

		for (udword i = 0; i < 3; i++) {
			if (i != WhichPlane) {
				*tempCoord.getElementPtr(i) = origin.getElement(i) + MaxT.getElement(WhichPlane) * dir.getElement(i);
#ifdef RAYAABB_EPSILON
				if (tempCoord.getElement(i) < MinB.getElement(i) - RAYAABB_EPSILON || tempCoord.getElement(i) > MaxB.getElement(i) + RAYAABB_EPSILON) return false;
#else
				if (tempCoord.getElement(i) < MinB.getElement(i) || tempCoord.getElement(i) > MaxB.getElement(i)) return false;
#endif
			}
		}
		return true;  // ray hits box
	}*/

	/*
Updated October, 5, 2001 :
	- Below is an alternative version, up to ~25% faster than the one above on my Celeron.
	- Not fully tested so handle with care. It's not that much different anyway.

Updated October, 9, 2001:
	- Fixed a slight bug......
	- Compiles for (Min, Max) or (Center, Extents) boxes
	- I did some tests with Adam Moravanszky, and the fastest version apparently depends on the machine. See for yourself.
	- You should also take a look at Tim Schröder's version in GDMag. It's sometimes faster thanks to early exits, sometimes
	slower (at least on my machine!).
*/

#define USE_MINMAX
	
	#ifndef USE_MINMAX
	// Unroll loop, do the div early to let it pair with CPU code
	#define FIND_CANDIDATE_PLANE(i)                                   \
		if (origin[i] < MinB[i]) {                                    \
			/* Calculate T distances to candidate planes */           \
			if (IR(dir[i])) MaxT[i] = (MinB[i] - origin[i]) / dir[i]; \
																	  \
			Inside = FALSE;                                           \
			coord[i] = MinB[i];                                       \
		} else if (origin[i] > MaxB[i]) {                             \
			/* Calculate T distances to candidate planes */           \
			if (IR(dir[i])) MaxT[i] = (MaxB[i] - origin[i]) / dir[i]; \
																	  \
			Inside = FALSE;                                           \
			coord[i] = MaxB[i];                                       \
		}
	#else
	#define FIND_CANDIDATE_PLANE(i)                                          \
		if (origin[i] < aabb.GetMin(i)) {                                    \
			/* Calculate T distances to candidate planes */                  \
			if (IR(dir[i])) MaxT[i] = (aabb.GetMin(i) - origin[i]) / dir[i]; \
																			 \
			Inside = false;                                                  \
			coord[i] = aabb.GetMin(i);                                       \
		} else if (origin[i] > aabb.GetMax(i)) {                             \
			/* Calculate T distances to candidate planes */                  \
			if (IR(dir[i])) MaxT[i] = (aabb.GetMax(i) - origin[i]) / dir[i]; \
																			 \
			Inside = false;                                                  \
			coord[i] = aabb.GetMax(i);                                       \
		}
	#endif
	
	// Unroll loop
	#ifndef USE_MINMAX
	
	#ifndef RAYAABB_EPSILON
	#define COMPUTE_INTERSECT(i)                                        \
		if (i != WhichPlane) {                                          \
			coord[i] = origin[i] + MaxT[WhichPlane] * dir[i];           \
			if (coord[i] < MinB[i] || coord[i] > MaxB[i]) return false; \
		}
	#else
	#define COMPUTE_INTERSECT(i)                                                                            \
		if (i != WhichPlane) {                                                                              \
			coord[i] = origin[i] + MaxT[WhichPlane] * dir[i];                                               \
			if (coord[i] < MinB[i] - RAYAABB_EPSILON || coord[i] > MaxB[i] + RAYAABB_EPSILON) return false; \
		}
	#endif
	
	#else
	
	#ifndef RAYAABB_EPSILON
	#define COMPUTE_INTERSECT(i)                                                      \
		if (i != WhichPlane) {                                                        \
			coord[i] = origin[i] + MaxT[WhichPlane] * dir[i];                         \
			if (coord[i] < aabb.GetMin(i) || coord[i] > aabb.GetMax(i)) return false; \
		}
	#else
	#define COMPUTE_INTERSECT(i)                                                                                          \
		if (i != WhichPlane) {                                                                                            \
			coord[i] = origin[i] + MaxT[WhichPlane] * dir[i];                                                             \
			if (coord[i] < aabb.GetMin(i) - RAYAABB_EPSILON || coord[i] > aabb.GetMax(i) + RAYAABB_EPSILON) return false; \
		}
	#endif // RayAABB_Epsilon
	
	#endif

	/*bool RayAABB4(const AABB &aabb, const vec3_t &origin, const vec3_t &dir, vec3_t &coord) {
		bool Inside = true;
	#ifndef USE_MINMAX
		vec3_t MinB;
		aabb.GetMin(MinB);
		vec3_t MaxB;
		aabb.GetMax(MaxB);
	#endif
		vec3_t MaxT;
		MaxT.x = MaxT.y = MaxT.z = -1.0f;
	
		// Find candidate planes.
		FIND_CANDIDATE_PLANE(0)
		FIND_CANDIDATE_PLANE(1)
		FIND_CANDIDATE_PLANE(2)
	
		// Ray origin inside bounding box
		if (Inside) {
			coord = origin;
			return true;
		}
	
		// Get largest of the MaxT's for final choice of intersection
		// - this version without FPU compares
		// - but branch prediction might suffer
		// - a bit faster on my Celeron, duno how it behaves on something like a P4
		udword WhichPlane;
		if (IR(MaxT[0]) & 0x80000000) {
			// T[0]<0
			if (IR(MaxT[1]) & 0x80000000) {
				// T[0]<0
				// T[1]<0
				if (IR(MaxT[2]) & 0x80000000) {
					// T[0]<0
					// T[1]<0
					// T[2]<0
					return false;
				} else {
					WhichPlane = 2;
				}
			} else if (IR(MaxT[2]) & 0x80000000) {
				// T[0]<0
				// T[1]>0
				// T[2]<0
				WhichPlane = 1;
			} else {
				// T[0]<0
				// T[1]>0
				// T[2]>0
				if (IR(MaxT[2]) > IR(MaxT[1])) {
					WhichPlane = 2;
				} else {
					WhichPlane = 1;
				}
			}
		} else {
			// T[0]>0
			if (IR(MaxT[1]) & 0x80000000) {
				// T[0]>0
				// T[1]<0
				if (IR(MaxT[2]) & 0x80000000) {
					// T[0]>0
					// T[1]<0
					// T[2]<0
					WhichPlane = 0;
				} else {
					// T[0]>0
					// T[1]<0
					// T[2]>0
					if (IR(MaxT[2]) > IR(MaxT[0])) {
						WhichPlane = 2;
					} else {
						WhichPlane = 0;
					}
				}
			} else if (IR(MaxT[2]) & 0x80000000) {
				// T[0]>0
				// T[1]>0
				// T[2]<0
				if (IR(MaxT[1]) > IR(MaxT[0])) {
					WhichPlane = 1;
				} else {
					WhichPlane = 0;
				}
			} else {
				// T[0]>0
				// T[1]>0
				// T[2]>0
				WhichPlane = 0;
				if (IR(MaxT[1]) > IR(MaxT[WhichPlane])) WhichPlane = 1;
				if (IR(MaxT[2]) > IR(MaxT[WhichPlane])) WhichPlane = 2;
			}
		}
	
		// Old code below:
		// Get largest of the maxT's for final choice of intersection
		udword WhichPlane = 0;
		if(MaxT[1] > MaxT[WhichPlane])	WhichPlane = 1;
		if(MaxT[2] > MaxT[WhichPlane])	WhichPlane = 2;
	
		// Check final candidate actually inside box
		if(IR(MaxT[WhichPlane])&0x80000000) return false;
	
		COMPUTE_INTERSECT(0)
		COMPUTE_INTERSECT(1)
		COMPUTE_INTERSECT(2)
	
		return true;  // ray hits box
	}*/
	
	// Versions usint the SAT. Original code for OBBs from MAGIC.
	// Rewritten for AABBs and reorganized for early exits.
	/*
#define Point vec3_t
	// For a ray
	bool RayAABB(const Ray& ray, const AABB& aabb) {
		Point BoxExtents, Diff;
	
		Diff.x = ray.mOrig.x - aabb.centerPoint(0);
		BoxExtents.x = aabb.GetExtents(0);
		if (fabsf(Diff.x) > BoxExtents.x && Diff.x * ray.mDir.x >= 0.0f) return false;
	
		Diff.y = ray.mOrig.y - aabb.GetCenter(1);
		BoxExtents.y = aabb.GetExtents(1);
		if (fabsf(Diff.y) > BoxExtents.y && Diff.y * ray.mDir.y >= 0.0f) return false;
	
		Diff.z = ray.mOrig.z - aabb.GetCenter(2);
		BoxExtents.z = aabb.GetExtents(2);
		if (fabsf(Diff.z) > BoxExtents.z && Diff.z * ray.mDir.z >= 0.0f) return false;
	
		float fAWdU[3];
		fAWdU[0] = fabsf(ray.mDir.x);
		fAWdU[1] = fabsf(ray.mDir.y);
		fAWdU[2] = fabsf(ray.mDir.z);
	
		float f;
		f = ray.mDir.y * Diff.z - ray.mDir.z * Diff.y;
		if (fabsf(f) > BoxExtents.y * fAWdU[2] + BoxExtents.z * fAWdU[1]) return false;
		f = ray.mDir.z * Diff.x - ray.mDir.x * Diff.z;
		if (fabsf(f) > BoxExtents.x * fAWdU[2] + BoxExtents.z * fAWdU[0]) return false;
		f = ray.mDir.x * Diff.y - ray.mDir.y * Diff.x;
		if (fabsf(f) > BoxExtents.x * fAWdU[1] + BoxExtents.y * fAWdU[0]) return false;
	
		return true;
	}*/
};

inline float randomf(float start, float end) {
	return (start + (float)rand() / RAND_MAX) * (end - start);
}
inline int random(int start, int end) {
#pragma warning(suppress : 4244)
	return (int)roundf(randomf(start, end));
}
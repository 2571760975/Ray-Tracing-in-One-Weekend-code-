#pragma once
#include <iostream>

class vec3 {
	public:
		vec3() : e{ 0,0,0 } {}
		vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		double operator[](int i) const { return e[i]; }
		double& operator[](int i) { return e[i]; }

		vec3& operator+=(const vec3 &v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}

		vec3& operator*=(const double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		vec3& operator/=(const double t) {
			return *this *= 1 / t;
		}

		double length() const {
			return sqrt(length_squared());
		}

		double length_squared() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

		void write_color(std::ostream &out ,int samples_per_pixel) {//输出颜色，多重采样
			auto scale = 1.0 / samples_per_pixel;
			auto r = sqrt(scale * e[0]);
			auto g = sqrt(scale * e[1]);
			auto b = sqrt(scale * e[2]);

			out << static_cast<int>(256 * clamp(r,0.0,0.999)) << ' '
				<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
		}

		inline static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}

		inline static vec3 random(double min, double max) {
			return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

	public:
		double e[3];
};

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {//点乘
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {//叉乘
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

vec3 random_in_unit_sphere() {//用于强制控制漫反射的向量在球体范围内
	while (true)
	{
		auto p = vec3::random(-1, 1);//静态函数用::，返回-1，1的值
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector() {//这里矫正了cos的系数
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r*cos(a), r*sin(a), z);//返回的是极坐标的形式
}

vec3 random_in_hemisphere(const vec3& normal) {//半球判定法，从入射点开始选取一个随机的方向，再判断是否在法向量所在的那个半球
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n) {//计算反射方向
	return v - 2 * dot(v, n) *n;
}

//计算折射
vec3 refract(const vec3& uv, const vec3&n, double etai_over_etat) {//uv是入射光线，n是入射介质面法向量,最后一个是透射比
	auto cos_theta = dot(-uv, n);
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);//折射光线的平行向量
	vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;//折射光线的垂直向量
	return r_out_parallel + r_out_perp;//合成真正的折射光线
}

double schlick(double cosine, double ref_idx) {//模拟折射概率随入射角改变
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

vec3 random_int_unit_disk() {//从一个单位小圆盘射出光线
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1)continue;
		return p;
	}
}
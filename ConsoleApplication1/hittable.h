#pragma once
#include "ray.h"
#include "rtweekend.h"
class material;

//这个类是检测相交的基类
struct hit_record //记录相交，同时也负责集体传参
{
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;//使用智能指针装载材质
	double t;//时间变量
	bool front_face;//光线的射入面


	inline void set_face_normal(const ray& r, const vec3& outward_normal) {//判断入射面是内侧还是外侧
		//这里是使用的法相永远与入射方向相反的做法
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
};


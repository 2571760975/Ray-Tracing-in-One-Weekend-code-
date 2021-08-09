#pragma once
#include "ray.h"
#include "rtweekend.h"
class material;

//������Ǽ���ཻ�Ļ���
struct hit_record //��¼�ཻ��ͬʱҲ�����崫��
{
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;//ʹ������ָ��װ�ز���
	double t;//ʱ�����
	bool front_face;//���ߵ�������


	inline void set_face_normal(const ray& r, const vec3& outward_normal) {//�ж����������ڲ໹�����
		//������ʹ�õķ�����Զ�����䷽���෴������
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
};


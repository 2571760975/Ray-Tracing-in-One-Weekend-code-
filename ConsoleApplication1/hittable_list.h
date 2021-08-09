#pragma once
#include "hittable.h"
#include<memory>
#include<vector>

using std::shared_ptr;//这里用的是智能指针，避免每一次都std::shared_ptr
using std::make_shared;

//物体列表
class hittable_list : public hittable
{
	public:
		hittable_list() {}
		hittable_list(shared_ptr<hittable> object) { add(object); }

		void clear() { objects.clear(); }
		void add(shared_ptr<hittable> object) { objects.push_back(object); }

		virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	public:
		std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;//是否与物体相交
	auto closest_so_far = t_max;//最近的交点

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {//调用每一个物体自己的击中判定函数，虚函数作用的体现
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}


#pragma once

#include "rtweekend.h"

//这里就是相机类了
class camera
{
	public:
		camera(
			vec3 lookfrom,vec3 lookat,vec3 vup,
			double vfov,//视野角度
			double aspect,double aperture,double focus_dist
		) {//初始化设置
			origin = lookfrom;//起始点
			lens_radius = aperture / 2;

			auto theta = degrees_to_radians(vfov);
			auto half_height = tan(theta / 2);
			auto half_width = aspect * half_height;

			w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			lower_left_corner = origin - half_width * focus_dist*u - half_height * focus_dist*v -focus_dist*w;

			horizontal = 2 * half_width*focus_dist*u;
			vertical = 2 * half_height*focus_dist*v;
		}

		ray get_ray(double s, double t) {//从这里得到每一个像素射出的光线
			vec3 rd = lens_radius * random_int_unit_disk();//得到真实半径，并模拟圆盘射出光线
			vec3 offset = u * rd.x() + v * rd.y();//调整圆盘旋向，半径*旋向得到偏移

			return ray(
				origin + offset,
				lower_left_corner + s*horizontal + t*vertical - origin - offset//需要减去来控制超出像素界限
			);
		}

	public:
		vec3 origin;//相机原点
		vec3 lower_left_corner;//相机视野左下角
		vec3 horizontal;//定义了视野大小
		vec3 vertical;
		vec3 u, v, w;//用三轴决定旋向
		double lens_radius;//模拟透镜的半径
};


#include"rtweekend.h"

#include "hittable_list.h"
#include"sphere.h"
#include"camera.h"
#include"material.h"

#include<iostream>

#pragma region "random_scene"
hittable_list random_scene() {//建立一个随机场景
	hittable_list world;

	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
				}
				else {
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(
		make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

	world.add(
		make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return world;
}
#pragma endregion

vec3 ray_color(const ray& r,const hittable& world,int depth) {//使用层数来限制递归次数，避免保栈
	hit_record rec;

	if (depth <= 0)
		return vec3(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec)){//这里的0.001是为了避免反射碰撞回自己
		//vec3 target = rec.p + rec.normal + random_unit_vector();//得到漫反射球
		ray scattered;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//这里其实用albedo给attenuation赋值了
			return attenuation * ray_color(scattered, world, depth - 1);//进一步计算散射光线
		return vec3(0, 0, 0);
		
	}
	
	vec3 unit_direction = unit_vector(r.direction());//标准化向量
	auto t = 0.5*(unit_direction.y() + 1.0);//计算y轴上的像素位置
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);//蓝白线性插值
}



int main() {
	//定义图片大小，像素多少
	const int image_width = 1280;
	const int image_height = 720;
	const int samples_per_pixel = 100;//添加采样数
	const int max_depth = 50;//递归次数
	const auto aspect_ratio = double(image_width) / image_height;
	const vec3 vup(0, 1, 0);
	vec3 lookfrom(13, 2, 3);//摄像机平面
	vec3 lookat(0, 0, 0);//看向方向
	auto dist_to_focus = 10.0;//距离
	auto aperture = 0.1;//光圈大小

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	auto R = cos(pi / 4);
	hittable_list world = random_scene();//得到随机场景

	//auto material_ground = make_shared<lambertian>(vec3(0.8, 0.8, 0.0));//漫反射材质
	//auto material_center = make_shared<lambertian>(vec3(0.1, 0.2, 0.5));//漫反射材质
	//auto material_left = make_shared<dielectric>(1.5);//绝缘体材质，这里直接给透射比了
	//auto material_right = make_shared<metal>(vec3(0.8, 0.6, 0.2),0.0);//金属材质

	//world.add(make_shared<sphere>(vec3(0.0, -100.5, -1.0), 100.0, material_ground));//位置，半径，材质
	//world.add(make_shared<sphere>(vec3(0.0, 0.0, -1.0), 0.5, material_center));
	//world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	//world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), -0.45, material_left));//直接让半径反向的这种操作，得到完全相反的法向量
	//world.add(make_shared<sphere>(vec3(1.0, 0.0, -1.0), 0.5, material_right));

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);//加入摄像机

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining" << j << ' ' << std::flush;//使用错误输出流+刷新
		for (int i = 0; i < image_width; ++i) {
			vec3 color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {//这里的做法是在像素内随机区域继续打出光线
				auto u = (i + random_double()) / image_width;
				auto v = (j + random_double()) / image_height;
				ray r = cam.get_ray(u, v);
				color += ray_color(r, world,max_depth);
			}
			color.write_color(std::cout, samples_per_pixel);//在输出流中做处理
		}
	}
	std::cerr << "\nDone.\n";
}



//光追的整个渲染流程
//使用255.999的原因是因为颜色值取不到256
//关于ray类
//ppm格式的文件使用极速看图打开
//线性插值的一般形式
//使用3D的计算方式，放入2D的结果
//球体求交公式推导
//求根公式系数消去？为什么
//为什么需要t_max和t_min判断相交是否有效,为了计算第一个点的颜色
//智能指针Share_ptr
//采样点抗锯齿...光追居然是这种方法实现的吗
//大球表现为绿色原因估计是因为法向量和采样射线的交角差不多，所以都是绿的，看小球的球顶可以看出来
//漫反射是根据切线建立球体选择随机点来进行反射模拟的
//声明成inline的函数可以跨文件使用吗？ double_random()
//控制漫反射光线的否定法
//伽马矫正
//shadow ance
//lambertian分布律 cos系数矫正
//为什么使用半球矫正法不需要加上法向量？
//反射向量的计算方法
//材质scatter的具体做法
//使用反射随机球半径来模拟模糊程度，半径越大，整体也就越模糊
//别忘了if里面的语句也是会运算的，albeo赋值给了attunation成为了颜色
//绝缘体材质的折射与反射策略
//Snell定律计算，拆解成平行光和垂直光的叠加，公式要记住
//全内反射的发生情况，（从高折射率介质射入低折射率介质，玻璃到空气会形成全内反射，Snell法则会无解）
//摄像机的旋向计算
//散焦模糊，即模仿现实相机透镜
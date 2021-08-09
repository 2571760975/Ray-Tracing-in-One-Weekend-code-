#pragma once

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>

using std::shared_ptr;
using std::make_shared;


//这个文件用来定义一些常数和函数


//常数
const double infinity = std::numeric_limits<double>::infinity();//取极限
const double pi = 3.1415926535897932385;//ps:这里我能背到70位，这里后两位46约到了5

//函数
inline double degrees_to_radians(double degress) {//角度转弧度
	return degress * pi / 180;
}

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);//返回【0，1）的随机数，注意左闭右开
}

inline double random_double(double min, double max) {//返回指定区间的数
	return min + (max - min)*random_double();
}

inline double clamp(double x, double min, double max) {//限制X的区间函数
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

//返回较小值和较大值
inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

//工具头文件

#include "ray.h"
#include "vec3.h"
#include "hittable.h"


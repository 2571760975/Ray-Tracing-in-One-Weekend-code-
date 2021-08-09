#pragma once

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>

using std::shared_ptr;
using std::make_shared;


//����ļ���������һЩ�����ͺ���


//����
const double infinity = std::numeric_limits<double>::infinity();//ȡ����
const double pi = 3.1415926535897932385;//ps:�������ܱ���70λ���������λ46Լ����5

//����
inline double degrees_to_radians(double degress) {//�Ƕ�ת����
	return degress * pi / 180;
}

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);//���ء�0��1�����������ע������ҿ�
}

inline double random_double(double min, double max) {//����ָ���������
	return min + (max - min)*random_double();
}

inline double clamp(double x, double min, double max) {//����X�����亯��
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

//���ؽ�Сֵ�ͽϴ�ֵ
inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

//����ͷ�ļ�

#include "ray.h"
#include "vec3.h"
#include "hittable.h"


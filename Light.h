#pragma once

#include <Eigen/Dense>

class Light {
public:
	Eigen::Vector3f Position;
	Eigen::Vector3f Diffuse;
	Eigen::Vector3f Specular;
	Eigen::Vector3f Ambient;
	float Shininess;

	Light();
	Light(Eigen::Vector3f position, Eigen::Vector3f diffuse, 
			Eigen::Vector3f specular, Eigen::Vector3f ambient, float shininess);
};


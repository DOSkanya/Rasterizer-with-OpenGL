#include "Light.h"

Light::Light() {
	Position = Eigen::Vector3f(500.0f, 500.0f, 1600.0f);
	Specular = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	Diffuse = 0.8 * Specular;
	Ambient = 0.1 * Specular;
	Shininess = 32.0f;
}

Light::Light(Eigen::Vector3f position, Eigen::Vector3f diffuse, 
				Eigen::Vector3f specular, Eigen::Vector3f ambient, float shininess) {
	Position = position;
	Diffuse = diffuse;
	Specular = specular;
	Ambient = ambient;
	Shininess = shininess;
}
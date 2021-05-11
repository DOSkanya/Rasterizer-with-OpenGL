#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/opencv.hpp>

class Texture {
public:
	cv::Mat Texture_Image;

	Texture();
	Texture(std::string path);
	void Load_Texture(std::string path);

	cv::Vec3b Lookup_Color(float tx, float ty);
};


#include "Texture.h"

Texture::Texture() {

}

Texture::Texture(std::string path) {
	Texture_Image = cv::imread(path, cv::IMREAD_COLOR);
	if (Texture_Image.empty()) {
		std::cout << "ERROR:Failed to load texture" << std::endl;
	}
}

void Texture::Load_Texture(std::string path) {
	Texture_Image = cv::imread(path, cv::IMREAD_COLOR);
	if (Texture_Image.empty()) {
		std::cout << "ERROR:Failed to load texture" << std::endl;
	}
}

//This function below, i don't know how, but it works.
//So don't change it, just keep it in that way
cv::Vec3b Texture::Lookup_Color(float tx, float ty) {
	int x = (int)(tx * Texture_Image.rows);
	int y = Texture_Image.cols - (int)(ty * Texture_Image.cols);
	cv::Vec3b color = Texture_Image.at<cv::Vec3b>(y, x);
	return color;
}

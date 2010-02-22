/**
 * @file
 * @brief Load Binaly Core
 * @author project.kuto
 */

#include "kuto_load_texture_core.h"
#include "kuto_file.h"
#include "kuto_png_loader.h"
#include "kuto_xyz_loader.h"
#include "kuto_bmp_loader.h"
#include "kuto_image_loader.h"
#include "kuto_graphics_device.h"

#include <cstdlib>


namespace kuto {

LoadTextureCore::LoadTextureCore(const std::string& filename, const char* subname)
: LoadBinaryCore(filename, subname, File::getExtension(filename) == "png")
, name_(NULL), data_(NULL), width_(0), height_(0), orgWidth_(0), orgHeight_(0), format_(GL_RGB)
{
	if (File::getExtension(filename) == "png") {
		PngLoader pngLoader;
		pngLoader.createTexture(getBytes(), *this, useAlphaPalette(), hue());
	} else if (File::getExtension(filename) == "xyz") {
		XyzLoader xyzLoader;
		xyzLoader.createTexture(getBytes(), *this, useAlphaPalette(), hue());
	} else if (File::getExtension(filename) == "bmp") {
		BmpLoader bmpLoader;
		bmpLoader.createTexture(getBytes(), *this, useAlphaPalette(), hue());
	} else {
		ImageLoader imageLoader;
		imageLoader.createTexture(filename_.c_str(), *this);
	}
}

LoadTextureCore::~LoadTextureCore() 
{
	if (name_) {
		glDeleteTextures(1, &name_);
		name_ = NULL;
	}
	if (data_ && data_ != getBytes()) {
		delete[] data_;
		data_ = NULL;
	}
	releaseBytes();
}

bool LoadTextureCore::createTexture(char* data, int width, int height, int orgWidth, int orgHeight, GLenum format)
{
	data_ = data;
	width_ = width;
	height_ = height;
	orgWidth_ = orgWidth;
	orgHeight_ = orgHeight;
	format_ = format;
	
	GraphicsDevice* device = GraphicsDevice::instance();
	glGenTextures(1, &name_);
	device->setTexture2D(true, name_);
	glTexImage2D(GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_, GL_UNSIGNED_BYTE, data_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return true;
}

bool LoadTextureCore::useAlphaPalette() const
{
	return getSubname().find("$ay") != std::string::npos;
}

int LoadTextureCore::hue() const
{
	std::string::size_type i = getSubname().find("$h");
	if (i != std::string::npos)
		return atoi(&getSubname().c_str()[i+2]);
	return 0;
}


}	// namespace kuto

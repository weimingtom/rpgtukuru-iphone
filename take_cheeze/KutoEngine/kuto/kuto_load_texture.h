/**
 * @file
 * @brief Load Texture Handle
 * @author project.kuto
 */
#pragma once

#if defined KR_IPHONE
	#include <OpenGLES/ES1/gl.h>
#else
	#include <GL/gl.h>
#endif

#include "kuto_load_handle.h"


namespace kuto {

class LoadTextureHandle : public LoadHandle
{
public:
	bool load(const std::string& filename, bool useAlphaPalette, int hue);

	GLuint glTexture() const;
	char* getData();
	int getWidth() const;
	int getHeight() const;
	int getOrgWidth() const;
	int getOrgHeight() const;
	GLenum getFormat() const;
	
private:
	virtual LoadCore* createCore(const std::string& filename, const char* subname);
};

}	// namespace kuto
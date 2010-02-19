/**
 * @file
 * @brief Graphics Device
 * @author project.kuto
 */

#include "kuto_graphics_device.h"
#if defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE==1)
	#include <OpenGLES/ES1/glext.h>
#else
	#include <GL/glext.h>
#endif


namespace kuto {

GraphicsDevice* GraphicsDevice::instance_ = NULL;

GraphicsDevice::GraphicsDevice()
: width_(0), height_(0)
, viewRenderbuffer_(NULL), viewFramebuffer_(NULL), depthRenderbuffer_(NULL)
{
}

GraphicsDevice::~GraphicsDevice()
{
}

bool GraphicsDevice::initialize(GLuint viewRenderbuffer, GLuint viewFramebuffer, GLuint depthRenderbuffer,
								int width, int height)
{
	viewRenderbuffer_ = viewRenderbuffer;
	viewFramebuffer_ = viewFramebuffer;
	depthRenderbuffer_ = depthRenderbuffer;
	width_ = width;
	height_ = height;
	return true;
}

void GraphicsDevice::setProjectionMatrix(const Matrix& matrix)
{
	if (matrixMode_ != GL_PROJECTION) {
		matrixMode_ = GL_PROJECTION;
		glMatrixMode(matrixMode_);
	}
	glLoadMatrixf(matrix.pointer());
}

void GraphicsDevice::setModelMatrix(const Matrix& matrix)
{
	if (matrixMode_ != GL_MODELVIEW) {
		matrixMode_ = GL_MODELVIEW;
		glMatrixMode(matrixMode_);
	}
	glLoadMatrixf(matrix.pointer());
}

void GraphicsDevice::setViewport(const Viewport& viewport)
{
    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void GraphicsDevice::beginRender()
{
#if defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE==1)
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer_);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer_);
#else
#endif
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	matrixMode_ = GL_MODELVIEW;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	enableVertex_ = true; setGLClientState(GL_VERTEX_ARRAY, enableVertex_);
	enableNormal_ = false; setGLClientState(GL_NORMAL_ARRAY, enableNormal_);
	enableTexcoord_ = true; setGLClientState(GL_TEXTURE_COORD_ARRAY, enableTexcoord_);
	enableColor_ = false; setGLClientState(GL_COLOR_ARRAY, enableColor_);
	enableBlend_ = false; setGLEnable(GL_BLEND, enableBlend_);
	blendSrcFactor_ = GL_SRC_ALPHA;
	blendDestFactor_ = GL_ONE_MINUS_SRC_ALPHA;
	glBlendFunc(blendSrcFactor_, blendDestFactor_);
	enableTexture2D_ = false;
	bindTexture2D_ = 0;
	setGLEnable(GL_TEXTURE_2D, enableTexture2D_);
	glBindTexture(GL_TEXTURE_2D, bindTexture2D_);
	vertexPointerInfo_.set(0, 0, 0, 0);
	texcoordPointerInfo_.set(0, 0, 0, 0);
	colorPointerInfo_.set(0, 0, 0, 0);
}

void GraphicsDevice::endRender()
{
#if defined(TARGET_OS_IPHONE) && defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE==1)
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer_);
#else
#endif
}

void GraphicsDevice::setVertexState(bool enableVertex, bool enableNormal, bool enableTexcoord, bool enableColor)
{
	if (enableVertex_ != enableVertex) {
		enableVertex_ = enableVertex;
		setGLClientState(GL_VERTEX_ARRAY, enableVertex_);
	}
	if (enableNormal_ != enableNormal) {
		enableNormal_ = enableNormal;
		setGLClientState(GL_NORMAL_ARRAY, enableNormal_);
	}
	if (enableTexcoord_ != enableTexcoord) {
		enableTexcoord_ = enableTexcoord;
		setGLClientState(GL_TEXTURE_COORD_ARRAY, enableTexcoord_);
	}
	if (enableColor_ != enableColor) {
		enableColor_ = enableColor;
		setGLClientState(GL_COLOR_ARRAY, enableColor_);
	}
}

void GraphicsDevice::setBlendState(bool enableBlend, GLenum srcFactor, GLenum destFactor)
{
	if (enableBlend_ != enableBlend) {
		enableBlend_ = enableBlend;
		setGLEnable(GL_BLEND, enableBlend_);
	}
	if (blendSrcFactor_ != srcFactor || blendDestFactor_ != destFactor) {
		blendSrcFactor_ = srcFactor;
		blendDestFactor_ = destFactor;
		glBlendFunc(blendSrcFactor_, blendDestFactor_);
	}
}

void GraphicsDevice::setTexture2D(bool enable, GLuint texture)
{
	if (enableTexture2D_ != enable) {
		enableTexture2D_ = enable;
		setGLEnable(GL_TEXTURE_2D, enableTexture2D_);
	}
	if (bindTexture2D_ != texture) {
		bindTexture2D_ = texture;
		glBindTexture(GL_TEXTURE_2D, bindTexture2D_);
	}
}

void GraphicsDevice::setColor(const Color& color)
{
	if (color_ != color) {
		color_ = color;
		glColor4f(color_.r, color_.g, color_.b, color_.a);
	}
}

void GraphicsDevice::setVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	if (!vertexPointerInfo_.equals(size, type, stride, pointer)) {
		vertexPointerInfo_.set(size, type, stride, pointer);
		glVertexPointer(size, type, stride, pointer);
	}
}

void GraphicsDevice::setTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	if (!texcoordPointerInfo_.equals(size, type, stride, pointer)) {
		texcoordPointerInfo_.set(size, type, stride, pointer);
		glTexCoordPointer(size, type, stride, pointer);
	}
}

void GraphicsDevice::setColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	if (!colorPointerInfo_.equals(size, type, stride, pointer)) {
		colorPointerInfo_.set(size, type, stride, pointer);
		glColorPointer(size, type, stride, pointer);
	}
}


}	// namespace kuto

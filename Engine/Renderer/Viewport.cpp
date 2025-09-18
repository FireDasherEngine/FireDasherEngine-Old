#include "Viewport.hpp"

Viewport::Viewport() {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenRenderbuffers(1, &RenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);

	// Depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderBuffer);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Viewport::~Viewport() {
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &texture);
}

void Viewport::ResizeFB(GLsizei wwidth, GLsizei hheight) {
	if (width == wwidth && height == hheight) return;

	width = wwidth;
	height = hheight;

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void Viewport::Render() const {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	Camera*& camera = renderer->overrideCamera ? renderer->overrideCamera : renderer->scene->mainCamera;

	camera->viewportWidth = width;
	camera->viewportHeight = height;
	renderer->Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
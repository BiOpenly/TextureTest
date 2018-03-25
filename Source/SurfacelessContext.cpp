#include <stdio.h>
#include <EGL/egl.h>

#include "Context.h"

namespace Context
{
	EGLSurface egl_surf;
	EGLContext egl_ctx;
	EGLDisplay egl_dpy;

	void Create()
	{
		egl_surf = EGL_NO_SURFACE;
		egl_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		EGLConfig config;
		EGLint num_configs;

		int attribs[] = {EGL_RENDERABLE_TYPE,
			EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE, 0,
			EGL_NONE};

		eglInitialize(egl_dpy, nullptr, nullptr);

		if (!eglChooseConfig(egl_dpy, attribs, &config, 1, &num_configs))
		{
			printf("Couldn't get a configuration!\n");
			return;
		}

		eglBindAPI(EGL_OPENGL_ES_API);

		EGLint ctx_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
		egl_ctx =
		eglCreateContext(egl_dpy, config, EGL_NO_CONTEXT, ctx_attribs);
		if (!egl_ctx)
		{
			printf("Failed to eglCreateContext\n");
			return;
		}

		eglMakeCurrent(egl_dpy, egl_surf, egl_surf, egl_ctx);
	}

	void Shutdown()
	{
		eglMakeCurrent(egl_dpy, egl_surf, egl_surf, egl_ctx);
		eglMakeCurrent(egl_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(egl_dpy, egl_ctx);
		eglTerminate(egl_dpy);
	}

	void Swap()
	{
	}

	void *LoadPointer(const char* name)
	{
		void *ptr = (void*)eglGetProcAddress(name);
		if (!ptr)
		{
			printf("Couldn't get address for %s\n", name);
		}
		return ptr;
	}
}

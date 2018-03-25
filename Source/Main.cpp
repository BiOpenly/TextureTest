#include <ctime>
#include <stdio.h>
#include <vector>

#include "BasicLoader.h"
#include "Context.h"

class GLTexture
{
public:
	GLTexture(int32_t width, int32_t height, void* data)
	{
		glGenTextures(1, &tex);
		// Decoded image
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	GLuint GetTex() { return tex; }
private:
	GLuint tex;
};

class GLFramebuffer
{
public:
	GLFramebuffer(int32_t width, int32_t height)
		: TextureBacking(width, height, nullptr)
		, Width{width}
		, Height{height}
	{
		glBindTexture(GL_TEXTURE_2D, TextureBacking.GetTex());
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureBacking.GetTex(), 0);

		glViewport(0, 0, width, height);
		glScissor(0, 0, width, height);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClearDepthf(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}

	void Dump(void *data)
	{
		Bind();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  // 4-byte pixel alignment
		glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	int32_t GetWidth() { return Width; }
	int32_t GetHeight() { return Height; }
	GLuint GetFB() { return framebuffer; }
	GLTexture &GetTex() { return TextureBacking; }
private:
	int32_t Width, Height;
	GLTexture TextureBacking;
	GLuint framebuffer;
};

void DrawQuad()
{
	const char* fs_test =
	"#version 300 es\n"
	"precision highp float;\n\n"

	"in vec4 vert;\n"

	"uniform sampler2D tex;\n"

	"out vec4 ocol;\n"
	"void main() {\n"
		"\tvec4 out_col = texture(tex, vert.xy);\n"
		"\tocol = out_col;\n"
	"}\n";

	const char* vs_test =
	"#version 300 es\n"

	"in vec4 pos;\n"
	"out vec4 vert;\n"

	"void main() {\n"
		"\tgl_Position = pos;\n"
		"\tvert = (pos + 1.0) * 0.5;\n"
	"}\n";

	GLuint fs, vs, pgm;
	GLint stat, attr_pos, attr_tex;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	vs = glCreateShader(GL_VERTEX_SHADER);
	pgm = glCreateProgram();

	glShaderSource(fs, 1, &fs_test, NULL);
	glShaderSource(vs, 1, &vs_test, NULL);

	glCompileShader(fs);
	glCompileShader(vs);

	//GLUtils::CheckShaderStatus(fs, "fs", fs_test);
	//GLUtils::CheckShaderStatus(vs, "vs", vs_test);

	glAttachShader(pgm, fs);
	glAttachShader(pgm, vs);
	glLinkProgram(pgm);

	//GLUtils::CheckProgramLinkStatus(pgm);

	glUseProgram(pgm);

	// Get attribute locations
	attr_pos = glGetAttribLocation(pgm, "pos");

	glEnableVertexAttribArray(attr_pos);

	glClearColor(0.4, 0.4, 0.4, 0.0);

	const GLfloat verts[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	const GLfloat tex_coords[] = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
	};
	const GLfloat colors[] = {
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		1, 1, 0, 1,
	};

	uint32_t TexData[]=
	{
		0xff00ff00,
		0xff0000ff,
		0xffff0000,
		0xffff00ff,
	};

	GLTexture MyTexture(2, 2, TexData);
	GLFramebuffer Framebuffer(2,2);
	glUseProgram(pgm);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, MyTexture.GetTex());

	glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, verts);

	Framebuffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, MyTexture.GetTex());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// If you want to see the output on a real windowed system, enable this
	if (0)
	{
		for (;;)
		{
			// Blit it out to FB 0
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, Framebuffer.GetTex().GetTex());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			Context::Swap();
		}
	}

	// And now dump it to make sure it rendered correctly
	std::vector<uint8_t> Data;
	Data.resize(Framebuffer.GetWidth() * Framebuffer.GetHeight() * 4);

	Framebuffer.Dump(&Data[0]);

	FILE* fp;
	fp = fopen("./test.bin", "wb");
	fwrite(&Data[0], 1, Data.size(), fp);
	fclose(fp);
}

int main()
{
	Context::Create();
	BL::Load();
	printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
	printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
	printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
	printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));

	DrawQuad();

	Context::Shutdown();
}

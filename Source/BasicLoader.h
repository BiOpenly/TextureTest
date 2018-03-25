#pragma once
#define GL_GLES_PROTOTYPES 0
#include "GLES3/gl3.h"
namespace BL
{
	void Load();
}
#define GL_LIST \
_(LINKPROGRAM, LinkProgram) \
_(GETSTRING, GetString) \
_(GENTEXTURES, GenTextures) \
_(DRAWARRAYS             , DrawArrays) \
_(CLEAR                  , Clear) \
_(VERTEXATTRIBPOINTER    , VertexAttribPointer) \
_(BINDTEXTURE            , BindTexture) \
_(ACTIVETEXTURE          , ActiveTexture) \
_(CLEARCOLOR             , ClearColor) \
_(ENABLEVERTEXATTRIBARRAY, EnableVertexAttribArray) \
_(GETATTRIBLOCATION      , GetAttribLocation) \
_(SHADERSOURCE           , ShaderSource) \
_(COMPILESHADER          , CompileShader) \
_(ATTACHSHADER           , AttachShader) \
_(USEPROGRAM             , UseProgram) \
_(CREATEPROGRAM          , CreateProgram) \
_(READPIXELS             , ReadPixels) \
_(PIXELSTOREI            , PixelStorei) \
_(TEXPARAMETERI       , TexParameteri) \
_(TEXIMAGE2D          , TexImage2D) \
_(GENFRAMEBUFFERS     , GenFramebuffers) \
_(BINDFRAMEBUFFER     , BindFramebuffer) \
_(FRAMEBUFFERTEXTURE2D, FramebufferTexture2D) \
_(VIEWPORT            , Viewport) \
_(SCISSOR             , Scissor) \
_(CLEARDEPTHF         , ClearDepthf) \
_(CREATESHADER        , CreateShader) \

#define _(type, name) extern PFNGL##type##PROC gl##name;
GL_LIST
#undef _

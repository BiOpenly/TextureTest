#include "BasicLoader.h"
#include "Context.h"

#define _(type, name) PFNGL##type##PROC gl##name;
GL_LIST
#undef _
namespace BL
{
void Load()
{
#define _(type, name) gl##name = (PFNGL##type##PROC) Context::LoadPointer("gl" #name);
GL_LIST
#undef _
}
}

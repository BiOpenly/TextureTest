#pragma once

namespace Context
{
	void Create();
	void Shutdown();
	void Swap();
	void *LoadPointer(const char* name);
}

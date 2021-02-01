#include <windows.h>
void* VTableHook(void* lpClass, int offset, void* hook)
{
	void* vtable = *(void**)lpClass;
	void** field = (void**)((DWORD64)vtable + offset);
	size_t size = (((DWORD64)vtable) + (offset + sizeof(void*)) - ((DWORD64)vtable));
	DWORD oldProtect;
	DWORD tempProtect;

	VirtualProtect(vtable, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	void* original = *field;
	*field = hook;
	VirtualProtect(vtable, size, oldProtect, &tempProtect);

	return original;
}
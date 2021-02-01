#include "IATinject.h"
#include <tlhelp32.h>
#include <string>
extern IATtargetLib IATtargets[];
extern int IATtargetsCount;
bool LoadLibPtr()
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	DWORD dwPID = GetCurrentProcessId();

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (!Module32First(hModuleSnap, &me32))
	{
		CloseHandle(hModuleSnap);           // clean the snapshot object
		return(FALSE);
	}

	OutputDebugString(L"[DLL]strat listing");
	do
	{
		OutputDebugString((std::wstring(L"[DLL]") + me32.szModule).c_str());
		OutputDebugString((std::wstring(L"[DLL]") + me32.szExePath).c_str());
		
		// loop through and check
		for (int i = 0; i != IATtargetsCount; ++i)
		{
			if (std::wstring(IATtargets[i].libname.begin(), IATtargets[i].libname.end()) == std::wstring(me32.szModule))
			{
				HMODULE libptr;
				std::wstring libpath(me32.szExePath);
				libptr = LoadLibraryA(std::string(libpath.begin(), libpath.end()).c_str());  //load original

				if (!libptr)
					return false;

				for (size_t j = 0; j < IATtargets[i].targetFuncsCount; j++)
				{
					auto funcptr = GetProcAddress(libptr, IATtargets[i].targetFuncs[j].fuc_name.c_str());
					if (!funcptr)
					{
						std::string debugMsg = (std::string("[DLL] encounter error while loading ") + IATtargets[i].targetFuncs[j].fuc_name);
						OutputDebugString(std::wstring(debugMsg.begin(), debugMsg.end()).c_str());
						return false;
					}
					*(IATtargets[i].targetFuncs[j].pOriFuc) = (DWORD_PTR)funcptr;
				}
				break;
			}
		}
	} while (Module32Next(hModuleSnap, &me32));
	OutputDebugString(L"[DLL]end listing");
	return true;
}


void IAT_Hijacking()
{
	OutputDebugString(L"[DLL]start Hooking");
	LPVOID imageBase = GetModuleHandleA(NULL);
	PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)imageBase;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)imageBase + dosHeaders->e_lfanew);

	PIMAGE_IMPORT_DESCRIPTOR importDescriptor = NULL;
	IMAGE_DATA_DIRECTORY importsDirectory = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(importsDirectory.VirtualAddress + (DWORD_PTR)imageBase);
	LPCSTR libraryName = NULL;
	HMODULE library = NULL;
	PIMAGE_IMPORT_BY_NAME functionName = NULL;

	while (importDescriptor->Name != NULL)
	{
		libraryName = (LPCSTR)importDescriptor->Name + (DWORD_PTR)imageBase;
		
		// loop through and check
		for (int i = 0; i != IATtargetsCount; ++i)
		{
			if (IATtargets[i].libname == libraryName)
			{

				library = LoadLibraryA(libraryName);

				if (library)
				{
					PIMAGE_THUNK_DATA originalFirstThunk = NULL, firstThunk = NULL;
					originalFirstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)imageBase + importDescriptor->OriginalFirstThunk);
					firstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)imageBase + importDescriptor->FirstThunk);

					while (originalFirstThunk->u1.AddressOfData != NULL)
					{
						functionName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)imageBase + originalFirstThunk->u1.AddressOfData);

						// find address
						for (int j = 0; j != IATtargets[i].targetFuncsCount; ++j)
						{
							if (std::string(functionName->Name) == IATtargets[i].targetFuncs[j].fuc_name)
							{
								SIZE_T bytesWritten = 0;
								DWORD oldProtect = 0;
								VirtualProtect((LPVOID)(&firstThunk->u1.Function), 8, PAGE_READWRITE, &oldProtect);

								// swap address with address of hooked
								firstThunk->u1.Function = IATtargets[i].targetFuncs[j].pNewFuc;
								break;
							}
						}
						++originalFirstThunk;
						++firstThunk;
					}
				}
			}
		}
		importDescriptor++;
	}

	// message box after IAT hooking
	OutputDebugString(L"[DLL]finishing Hooking");
}
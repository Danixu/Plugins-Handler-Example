#ifndef _SHARED_EXPORTS_H__
#define _SHARED_EXPORTS_H__

#ifdef _WIN32
#include <windows.h>
#include <string>

#define LIBLOAD(x) LoadLibrary((LPCSTR)x)
#define dlclose(x) FreeLibrary((HMODULE)x)
#define dlsym(x, y) GetProcAddress((HINSTANCE)x, y)
char *dlerror();

#ifdef BUILD_LIB
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#include <dlfcn.h>
#define SHARED_EXPORT
#define LIBLOAD(x) dlopen(x, RTLD_LAZY)
#endif

#endif /* _SHARED_EXPORTS_H__ */
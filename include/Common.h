#ifndef COMMON_H
#define COMMON_H

#if _MSC_VER && !__INTEL_COMPILER
#define DllExport __declspec ( dllexport )
#else
#define DllExport
#endif // #if _MSC_VER && !__INTEL_COMPILER

#endif // #ifndef COMMON_H

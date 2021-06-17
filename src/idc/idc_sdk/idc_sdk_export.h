#ifndef _IDC_SDK_EXPORT_H_0394512124509
#define _IDC_SDK_EXPORT_H_0394512124509

#ifdef WIN32
#ifdef DECLARE_IDC_SDK_EXPORT
#define IDC_SDK_EXPORT __declspec(dllexport)
#else
#define IDC_SDK_EXPORT __declspec(dllimport)
#endif
#else
#ifdef __GNUC__
#define IDC_SDK_EXPORT __attribute__ ((visibility("default")))
#else
#define IDC_SDK_EXPORT
#endif
#endif


#endif // !_IDC_SDK_EXPORT_H_0394512124509

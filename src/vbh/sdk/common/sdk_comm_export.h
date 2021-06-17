#ifndef SDK_COMM_EXPORT_H_0F2187C0999E11E9B33E60F18A3A20D1
#define SDK_COMM_EXPORT_H_0F2187C0999E11E9B33E60F18A3A20D1

#ifdef WIN32
	#ifdef DECLARE_BLOCKCHAIN_COMMON_SDK_EXPORT
		#define VBH_COMM_SDK_EXPORT __declspec(dllexport)
	#else
		#define VBH_COMM_SDK_EXPORT __declspec(dllimport)
	#endif
#else
	 #ifdef __GNUC__
		#define VBH_COMM_SDK_EXPORT __attribute__ ((visibility("default")))
	#else
		#define VBH_COMM_SDK_EXPORT
	#endif
#endif

#endif

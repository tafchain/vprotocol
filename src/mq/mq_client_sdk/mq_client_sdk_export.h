#ifndef MQ_CLIENT_SDK_EXPORT_H_3428792347893262137213523157
#define MQ_CLIENT_SDK_EXPORT_H_3428792347893262137213523157

#ifdef WIN32
	#ifdef DECLARE_MQ_CLIENT_SDK_EXPORT
		#define MQ_CLIENT_SDK_EXPORT __declspec(dllexport)
	#else
		#define MQ_CLIENT_SDK_EXPORT __declspec(dllimport)
	#endif
#else
	 #ifdef __GNUC__
		#define MQ_CLIENT_SDK_EXPORT __attribute__ ((visibility("default")))
	#else
		#define MQ_CLIENT_SDK_EXPORT
	#endif
#endif

#endif

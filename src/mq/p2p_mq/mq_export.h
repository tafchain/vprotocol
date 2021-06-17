#ifndef MQ_EXPORT_H_3428792347893262137213523157
#define MQ_EXPORT_H_3428792347893262137213523157

#ifdef WIN32
	#ifdef DECLARE_P2P_MQ_EXPORT
		#define P2P_MQ_EXPORT __declspec(dllexport)
	#else
		#define P2P_MQ_EXPORT __declspec(dllimport)
	#endif
#else
	 #ifdef __GNUC__
		#define P2P_MQ_EXPORT __attribute__ ((visibility("default")))
	#else
		#define P2P_MQ_EXPORT
	#endif
#endif

#endif

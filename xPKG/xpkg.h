#ifndef XPKG_H
#define XPKG_H

#ifdef __cplusplus
extern "C" {
#endif
	const extern unsigned char xpkg_min_version;
	const extern unsigned char xpkg_maj_version;

	typedef struct {
		unsigned char 
	} classInfo;

	typedef struct {
		unsigned char min_ver;
		unsigned char max_ver;

		size_t handle;
	} package;

#ifdef __cplusplus
}
#endif

#endif
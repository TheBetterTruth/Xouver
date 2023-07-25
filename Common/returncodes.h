#ifndef RETURN_CODES_H

#define XVR_SUCCESS					0
#define XVR_NULLPTR					1
#define XVR_MEM_ALLOC_FAIL			2
#define XPKG_INVALID_PACKAGE		3
#define XVR_INDEX_OUT_OF_BOUNDS		4
#define XPKG_CLASS_NOT_FOUND		5
#define XPKG_FUNCTION_NOT_FOUND		6

#define TRY(RESULT)					if (RESULT != XVR_SUCCESS) return RESULT;
#define ALLOC(PTR, SIZE)			{ void** _ptr = (void**)&PTR; *_ptr = malloc(SIZE); if (*_ptr == nullptr) return XVR_MEM_ALLOC_FAIL; }

#endif
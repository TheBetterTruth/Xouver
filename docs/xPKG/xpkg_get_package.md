# xpkg_get_package<br>function

## Declaration
```cpp
unsigned char xpkg_get_package(
	[in] const unsigned char* _Bytes,
	[out] package_info* _PackageInfo
	);
```

## Description
Creates [package_info] representation of the given ``unsigned char`` array, if it's a valid xouver package.

## Parameters
### const unsigned char* _Bytes
The ``unsigned char`` array that constist of valid xouver bytecode to be represented as [package_info].

### package_info* _PackageInfo
A pointer to where the [package_info] representation should be written to.

[package_info]: package_info.md
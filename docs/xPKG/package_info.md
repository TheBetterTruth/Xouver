# package_info<br>struct

## Declaration
```cpp
typedef struct {
	unsigned char minor_version;
	unsigned char major_version;

	unsigned char package_type;

	uint64_t handle;

	uint32_t class_count;

	char** class_names;
	uint64_t* class_ptrs;
} package_info;
```

## Description
If loaded via [xpkg_get_package] this struct represents the given ``unsigned char`` array as xouver package.
<br>Otherwise it's values may specify a xouve package representation to be serialized as ``unsigned char`` using [xpkg_pack].

## Members
|Type|Name|Description|
|----|----|-----------|
|unsigned char|minor_version|The minor vm version the bytecode has been compiled for.|
|unsigned char|The major vm version the bytecode has been compiled for.|
|unsigned char|package_type|The type of this package.<br><br>**0** - This package should be treated as executable.<br>**1** - This package should e treated as library (Must have descriptors).|
|uint64_t|handle|The address of the `` unsigned char`` bytecode array of this package.<br><br>**Note** If this instance is used to __create__ bytecode from a package, then the handle be used. Otherwise the array may __**NOT**__ be deleted and or free'd *before* this package won't be worked with anymore.|
|uint32_t|class_count|The amount of classes in this package.|
|char**|class_names|An array containing all full class names (eg. ``Xouver.Lang.Int``) of this package.|
|uint64_t*|class_ptr|An array containing all positions in ``handle`` of each class.|

## Notes
This object should be kept as long as any of it's children (eg. classes, functions, constants, instructions, descriptors, etc.) are to be worked with.

[xpkg_get_package]: xpkg_get_package.md
[xpkg_pack]: xpkg_pack.md
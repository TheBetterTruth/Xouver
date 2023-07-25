# constant_info<br>struct
## Declaration
```c
typedef struct {
	unsigned char type;
	uint32_t size;
	unsigned char* bytes;
} constant_info;
```

## Members
|Type|Name|Description|
|----|----|-----------|
|unsigned char|type|The type of this constant specifiying how it should be interpreted.<br><br>**0** - int<br>**1** - float<br>**2** - short<br>**3** - long<br>**4** - double<br>**5** - char<br>**6** - byte<br>**7** - bool<br>**8** - string|
|uint32_t|size|The size in bytes of this constant.|
|unsigned char*|bytes|The bytes this constant consists of.|
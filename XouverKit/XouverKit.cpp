#include <XNI.h>

#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

#define BYTE_INT(bytes, pos)		(bytes[*pos] << 24) | (bytes[*pos + 1] << 16) | (bytes[*pos + 2] << 8) | (bytes[*pos + 3]); *pos += 4;

int main(int argc, char** argv) {
	/*if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " application.xvr" << std::endl;
		return 1;
	}*/
	//FILE* file = fopen(argv[1], "rb");

	FILE* file = fopen("test.xvr", "rb");
	if (file == NULL) throw std::exception();

	fseek(file, 0L, SEEK_END);
	long numbytes = ftell(file);

	fseek(file, 0L, SEEK_SET);

	unsigned char* buffer = new unsigned char[numbytes];

	fread(buffer, sizeof(unsigned char), numbytes, file);
	fclose(file);


	XNI_Error err;
	void* rt = createRuntime(&err, buffer, numbytes);

	if (err != NO_XNI_ERROR) {
		std::cout << "Error: " << resolveXNIError(err);
		return -1;
	}

	err = createClass(rt, buffer, numbytes);

	if (err != NO_XNI_ERROR) {
		std::cout << "Error: " << resolveXNIError(err);
		return -1;
	}
	runRuntime(rt, "Xouver.Test", "void Xouver.Test:Main()");

	/*if (getXrtError(rt)) {
		int size = getExpceptionMessageSize(rt);
		char* msg = new char[size];
		getExceptionMessage(rt, &msg);

		std::string str = "";

		for (int i = 0; i < size; i++) {
			str += msg[i];
		}

#ifdef WIN32
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO info;
		WORD oldAttr;
		DWORD bytesWritten = 0;
		if (!GetConsoleScreenBufferInfo(handle, &info)) {
			std::cout << msg;
		}
		else {
			oldAttr = info.wAttributes;
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			WriteFile(handle, msg, size, &bytesWritten, NULL);


			SetConsoleTextAttribute(handle, oldAttr);
		}
#else
		std::cout << str;
#endif

		delete[] msg;
	}*/

	freeRuntime(rt);
	delete[] buffer;
	return 0;
}

int num = 4;
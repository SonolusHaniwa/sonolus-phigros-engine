using namespace std;

bool fileExists(string path) {
	ifstream fin(path.c_str());
	return fin.is_open();
}

#include"json.h"
#include"encrypt.h"
#include"buffer.h"
#include"gzip.h"
#include"error.h"
#include"MaxRects.h"
#include"MaxRects.cpp"
#include"png.h"
#include"zip.h"
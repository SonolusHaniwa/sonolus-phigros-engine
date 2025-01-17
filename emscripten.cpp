// 将 convert.h 中的代码转为 js 用于 PhiStore
#include<jsoncpp/json/json.h>
#include<iostream>
#include<iomanip>
#include<vector>
#include<map>
#include<cstring>
#include<set>
#include<queue>
#include<assert.h>
#include<sstream>
#include<algorithm>
using namespace std;
void throwWarning(string s) { cerr << "Warning: " << s << endl; }
#include"sonolus/items/LevelData.h"
#include"sonolus/modules/json.h"
#include"convert.h"

#ifndef EM_PORT_API
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype
#		else
#			define EM_PORT_API(rettype) rettype
#		endif
#	endif
#endif

extern "C" EMSCRIPTEN_KEEPALIVE char* json2data(char* input, double bgmOffset) {
	string data; data = string(input, strlen(input));
	string res = fromPGS(data, bgmOffset);
	char* result = const_cast<char*>(res.c_str());
	return result;
}

extern "C" EMSCRIPTEN_KEEPALIVE char* pec2json(char* input, double bgmOffset) {
	string data; data = string(input, strlen(input));
	string res = fromPEC(data, bgmOffset);
	char* result = const_cast<char*>(res.c_str());
	return result;
}

extern "C" EMSCRIPTEN_KEEPALIVE char* rpe2json(char* input, double bgmOffset) {
	string data; data = string(input, strlen(input));
	string res = fromRPE(data, bgmOffset);
	char* result = const_cast<char*>(res.c_str());
	return result;
}
#pragma once

// class CustomClass {
	#define CLASSBEGIN public: \
	int classSize = 0; \
	vector<function<void(FuncNode)> > deserializiers; \
	vector<function<FuncNode()> > serializiers; \
	template<typename T> FuncNode operator == (T a) { \
		if (a.classSize != classSize) return false; \
		vector<FuncNode> args; \
		for (int i = 0; i < a.classSize; i++) args.push_back(FuncNode( \
			RuntimeFunction.Equal, {a.serializiers[i](), serializiers[i]()} \
		)); return FuncNode(RuntimeFunction.And, args); \
	} \
	vector<FuncNode> serialize() { \
		vector<FuncNode> res; \
		for (int i = 0; i < classSize; i++) res.push_back(serializiers[i]()); \
		return res; \
	} \
	void deserialize(vector<FuncNode> args) { \
		assert(args.size() == classSize); \
		for (int i = 0; i < classSize; i++) deserializiers[i](args[i]); \
	}

	#define defineVar(T, valName, value) T valName = value; \
	    bool unused_##valName##_unused = [&](){ \
    		for (int i = 0; i < valName.classSize; i++) \
    			deserializiers.push_back(valName.deserializiers[i]), \
    			serializiers.push_back(valName.serializiers[i]); \
    		classSize += valName.classSize; \
	    	return true; \
	    }();
// };
// 
// #define customClass(name) class name: public CustomClass

int allocatorSize[10001];
int allocatorSizeBackup[11][10001];
int allocatorSizeBackupCounter = 0;

void createAllocatorBackup() {
	memcpy(allocatorSizeBackup[allocatorSizeBackupCounter++], allocatorSize, sizeof allocatorSize);
}
void restoreAllocatorBackup() {
	memcpy(allocatorSize, allocatorSizeBackup[allocatorSizeBackupCounter - 1], sizeof allocatorSize);
}
void deleteAllocatorBackup() {
	allocatorSizeBackupCounter--;
}
// vector<FuncNode> getFixedMemory(FuncNode containerId, FuncNode offset, int len) {
// 	vector<FuncNode> args;
// 	for (int i = 0; i < len; i++) args.push_back(FuncNode(RuntimeFunction.Get, {containerId, offset + i});
// 	return args;
// }
// 
// FuncNode setFixedMemory(FuncNode containerId, FuncNode offset, int len, vector<FuncNode> list) {
// 	assert(len == list.size()); vector<FuncNode> args;
// 	for (int i = 0; i < len; i++) args.push_back(FuncNode(containerId, offset + i, list[i]));
// 	return Execute(args);
// }
// 
// int allocateFixedMemory(int containerId, int len) {
// 	int offset = allocatorSize[containerId];
// 	allocatorSize[containerId] += len;
// 	return offset;
// }

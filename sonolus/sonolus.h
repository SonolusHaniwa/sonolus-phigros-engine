using namespace std;

#define Constructor
#define Destructor
#define Blocked
#define NonType
#define CppLoop

// 解释器部分内容
int ForBlockCount = 0;
string callFromFile = "";
int callFromLine = 0;
int callFromColumn = 0;
string callFromFunc = "";
string callFromFuncParam = "";
string archetypeName = "";
string callbackName = "";

struct CallStack {
    string file;
    int line, column;
    string func, param;
};
vector<CallStack> callStacks;
void printStack() {
    cerr << "Call Stack: " << endl;
    for (int i = int(callStacks.size()) - 2; i >= 0; i--)
        cerr << "#" << int(callStacks.size()) - 2 - i
             << " in " << (callStacks[i].func == "" ? "???" : callStacks[i].func)
             << " (" << callStacks[i].param
             << ") at " << callStacks[i].file
             << ":"
             << callStacks[i].line << ":" << callStacks[i].column << endl;
}

#if false
// 用宏定义定义一个虚假函数，让解释器识别到这个函数并自动加到 UserDefined 函数列表中
void SonolusAssert() {}
#endif
// SonolusAssert 宏定义一定要禁止解释器解析，不然解释器会识别到两个同名的宏定义然后出现错误
#define DISABLE_INTERPRETER
#ifndef COMPILE_RUNTIME
// 供 IDE 识别的 SonolusAssert 函数
#define SonolusAssert(expr, ...) (void)( \
    (!!(expr)) || \
    (__builtin_SonolusAssert("", "", 0, 0, "", #expr, __VA_ARGS__), 0) \
)
#else
// 供编译器识别的 SonolusAssert 函数
#define SonolusAssert(arg1, arg2, arg3, arg4, arg5, expr, ...) (void)( \
    (!!(expr)) || \
    (__builtin_SonolusAssert(arg1, arg2, arg3, arg4, arg5, #expr, __VA_ARGS__), 0) \
)
#endif
#undef DISABLE_INTERPRETER
Blocked void __builtin_SonolusAssert(
    string callFromFuncParam, string callFromFunc, int callFromLine, int callFromColumn, string callFromFile,
    string exprString, const char* format, ...
) {
    callStacks.push_back({callFromFile, callFromLine, callFromColumn, callFromFunc, callFromFuncParam});

    const int size = 128 * 1024;
    va_list args;
    va_start(args, format);
    char buf[size];
    memset(buf, '\0', sizeof buf);
    int n = vsnprintf(buf, size, format, args);
    va_end(args);

    cerr << "Build Failed!" << endl << endl;
    cerr << "Error: " << buf << endl;
    cerr << "Assertion `" << exprString << "` failed!" << endl << endl;
    printStack();
    callStacks.pop_back();
    abort();
}

// 库部分内容

/*** Start of inlined file: json.h ***/
#ifndef JSON_H
#define JSON_H

#include<jsoncpp/json/json.h>

string json_encode(Json::Value val) {
    Json::FastWriter writer;
    return writer.write(val);
}

string json_pretty_encode(Json::Value val) {
    Json::StyledWriter writer;
    return writer.write(val);
}

bool json_decode(string json, Json::Value& res) {
    Json::Reader reader;
    return reader.parse(json, res);
}

Json::Value json_decode(string json) {
    Json::Reader reader; Json::Value res;
    reader.parse(json, res);
    return res;
}

#endif
/*** End of inlined file: json.h ***/



/*** Start of inlined file: gzip.h ***/
#ifndef GZIP_H
#define GZIP_H
#include<zlib.h>

#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

string compress_gzip(string str, int compressionlevel = Z_BEST_COMPRESSION) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit2(&zs,
        compressionlevel,
        Z_DEFLATED,
        MOD_GZIP_ZLIB_WINDOWSIZE + 16,
        MOD_GZIP_ZLIB_CFACTOR,
        Z_DEFAULT_STRATEGY) != Z_OK
    ) throw(runtime_error("deflateInit2 failed while compressing."));
    char* x = const_cast<char*>(str.c_str());
    zs.next_in = (Bytef*)x;
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    string outstring;
    while(true) {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out) outstring.append(outbuffer, zs.total_out - outstring.size());
        if (ret != Z_OK) break;
    }
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(runtime_error(oss.str()));
    }
    return outstring;
}

string compress_deflate(string str, int compressionlevel = Z_BEST_COMPRESSION) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(runtime_error("deflateInit failed while compressing."));
    zs.next_in = (Bytef*)(str.c_str());
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    string outstring;
    while(true) {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
        if (ret != Z_OK) break;
    }
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(runtime_error(oss.str()));
    }
	return outstring;
}

string decompress_deflate(string str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit(&zs) != Z_OK)
        throw(runtime_error("inflateInit failed while decompressing."));
    zs.next_in = (Bytef*)(str.c_str());
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    string outstring;
    while(true) {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
        if (ret != Z_OK) break;
    }
    inflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(runtime_error(oss.str()));
    }
    return outstring;
}

string decompress_gzip(string str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit2(&zs, MOD_GZIP_ZLIB_WINDOWSIZE + 16) != Z_OK)
        throw(runtime_error("inflateInit failed while decompressing."));
    char* x = const_cast<char*>(str.c_str());
    zs.next_in = (Bytef*)x;
    zs.avail_in = str.size();
    int ret;
    char outbuffer[100010];
    string outstring;
    while (true) {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
        if (ret != Z_OK) break;
    }
    inflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(runtime_error(oss.str()));
    }
    return outstring;
}

#endif

/*** End of inlined file: gzip.h ***/

string dist = "./dist";
map<int, set<string> > readable, writable;
map<int, string> blockName;


/*** Start of inlined file: FuncNode.h ***/
#ifndef FuncNode_H
#define FuncNode_H

class DataNode {
    public:

    bool isValue = true;
    double value = 0;
    string name = "";
    vector<int> param = {};

	bool operator < (const DataNode& a) const {
        if (a.isValue != isValue) return isValue < a.isValue;
        if (a.isValue) return value < a.value;
        else if (param == a.param) return name < a.name;
        else return param < a.param;
    }

	bool operator == (const DataNode& a) const {
		if (a.isValue != isValue) return false;
		if (isValue) return value == a.value;
		else return name == a.name && param == a.param;
	}

	Json::Value toJsonObject() {
		Json::Value res;
		if (isValue) res["value"] = value;
		else {
			res["func"] = name;
			res["args"].resize(0);
			for (int i = 0; i < param.size(); i++) res["args"].append(param[i]);
		}
		return res;
	}
};

map<DataNode, int> hashMap;
vector<DataNode> dataContainer;

class FuncNode {
	private:
	int nodeId = 0;

	int allocate(DataNode tmp) {
		if (hashMap.find(tmp) != hashMap.end()) return hashMap[tmp];
		hashMap[tmp] = dataContainer.size(); dataContainer.push_back(tmp);
		return hashMap[tmp];
	}

	public:
	Constructor FuncNode() {
		DataNode tmp = {
			isValue: true,
			value: 0
		};
		nodeId = allocate(tmp);
	}
	Constructor FuncNode(double value) {
		DataNode tmp = {
			isValue: true,
			value: value
		};
		nodeId = allocate(tmp);
	}
	Constructor FuncNode(string func, vector<FuncNode> args) {
		vector<int> nodes;
		for (int i = 0; i < args.size(); i++) nodes.push_back(args[i].nodeId);
		DataNode tmp = {
			isValue: false,
			name: func,
			param: nodes
		};
		nodeId = allocate(tmp);
	}

	string toString(int tabLength = 0) {
		stringstream ss; string pre = "";
		for (int i = 0; i < tabLength; i++) pre += "  ";
		DataNode node = dataContainer[nodeId];
		if (node.isValue) return ss << pre << node.value, ss.str();
		bool allValue = true;
		for (int i = 0; i < node.param.size(); i++) allValue &= dataContainer[node.param[i]].isValue;
		if (allValue) {
			ss << pre << node.name << "(";
			for (int i = 0; i < node.param.size(); i++)
				ss << dataContainer[node.param[i]].value << (i != node.param.size() - 1 ? ", " : "");
			ss << ")";
		} else {
			ss << pre << node.name << "(" << endl;
			for (int i = 0; i < node.param.size(); i++) {
				FuncNode newNode = 0;
				newNode.nodeId = node.param[i];
				ss << newNode.toString(tabLength + 1) << (i != node.param.size() - 1 ? ", " : "") << endl;
			} ss << pre << ")";
		}
		return ss.str();
	}

	DataNode getNode() {
		return dataContainer[nodeId];
	}
	int getNodeId() {
		return nodeId;
	}

	#ifndef COMPILE_RUNTIME
	operator bool() {
		return false;
	}
	#endif
};

ostream& operator << (ostream& fout, FuncNode node) {
	return fout << node.toString();
}

stack<vector<FuncNode> > nodesContainer;
stack<int> blockCounter;
stack<bool> addBlockCounter;
void createNodeContainer(bool add = true) {
	nodesContainer.push({});
	if (add) blockCounter.push(1);
	else blockCounter.top() += 1;
	addBlockCounter.push(add);
}
void SonolusRun(FuncNode code) {
	SonolusAssert(nodesContainer.size(), "You shouldn't do anything outside Archetypes!");
	nodesContainer.top().push_back(code);
}
FuncNode mergeNodeContainer() {
	assert(nodesContainer.size());
	auto c = nodesContainer.top(); nodesContainer.pop();
	if (addBlockCounter.top()) blockCounter.pop();
	else blockCounter.top() -= 1;
	addBlockCounter.pop();
	// SonolusAssert(c.size(), "Your Sonolus will boom if I accept your empty function.");
	if (c.size() == 0) c.push_back(0);
	FuncNode res = FuncNode("Block", { FuncNode("Execute", c) });
	return res;
}

#endif
/*** End of inlined file: FuncNode.h ***/

#define Get __builtin_Get
#define Set __builtin_Set
#define Spawn __builtin_Spawn

/*** Start of inlined file: Functions.h ***/
#ifndef Functions_H
#define Functions_H

#define default default_value

FuncNode Abs(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Abs", args);
	return code;
}

FuncNode Add(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Add\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Add", args);
	return code;
}

FuncNode And(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"And\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("And", args);
	return code;
}

FuncNode Arccos(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Arccos", args);
	return code;
}

FuncNode Arcsin(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Arcsin", args);
	return code;
}

FuncNode Arctan(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Arctan", args);
	return code;
}

FuncNode Arctan2(FuncNode y, FuncNode x) {
	vector<FuncNode> args;
	args.push_back(y);
	args.push_back(x);
	FuncNode code = FuncNode("Arctan2", args);
	return code;
}

FuncNode BeatToBPM(FuncNode beat) {
	vector<FuncNode> args;
	args.push_back(beat);
	FuncNode code = FuncNode("BeatToBPM", args);
	return code;
}

FuncNode BeatToStartingBeat(FuncNode beat) {
	vector<FuncNode> args;
	args.push_back(beat);
	FuncNode code = FuncNode("BeatToStartingBeat", args);
	return code;
}

FuncNode BeatToStartingTime(FuncNode beat) {
	vector<FuncNode> args;
	args.push_back(beat);
	FuncNode code = FuncNode("BeatToStartingTime", args);
	return code;
}

FuncNode BeatToTime(FuncNode beat) {
	vector<FuncNode> args;
	args.push_back(beat);
	FuncNode code = FuncNode("BeatToTime", args);
	return code;
}

FuncNode Block(FuncNode body) {
	vector<FuncNode> args;
	args.push_back(body);
	FuncNode code = FuncNode("Block", args);
	return code;
}

FuncNode Break(FuncNode count, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(count);
	args.push_back(value);
	FuncNode code = FuncNode("Break", args);
	SonolusRun(code);
	return code;
}

FuncNode Ceil(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Ceil", args);
	return code;
}

FuncNode Clamp(FuncNode x, FuncNode a, FuncNode b) {
	vector<FuncNode> args;
	args.push_back(x);
	args.push_back(a);
	args.push_back(b);
	FuncNode code = FuncNode("Clamp", args);
	return code;
}

FuncNode Copy(FuncNode srcId, FuncNode srcIndex, FuncNode dstId, FuncNode dstIndex, FuncNode count) {
	vector<FuncNode> args;
	args.push_back(srcId);
	args.push_back(srcIndex);
	args.push_back(dstId);
	args.push_back(dstIndex);
	args.push_back(count);
	FuncNode code = FuncNode("Copy", args);
	SonolusRun(code);
	return code;
}

FuncNode Cos(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Cos", args);
	return code;
}

FuncNode Cosh(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Cosh", args);
	return code;
}

FuncNode DebugLog(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("DebugLog", args);
	SonolusRun(code);
	return code;
}

FuncNode DebugPause() {
	vector<FuncNode> args;
	FuncNode code = FuncNode("DebugPause", args);
	SonolusRun(code);
	return code;
}

FuncNode DecrementPost(FuncNode id, FuncNode index) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	FuncNode code = FuncNode("DecrementPost", args);
	SonolusRun(code);
	return code;
}

FuncNode DecrementPostPointed(FuncNode id, FuncNode index, FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	FuncNode code = FuncNode("DecrementPostPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode DecrementPostShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("DecrementPostShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode DecrementPre(FuncNode id, FuncNode index) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	FuncNode code = FuncNode("DecrementPre", args);
	SonolusRun(code);
	return code;
}

FuncNode DecrementPrePointed(FuncNode id, FuncNode index, FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	FuncNode code = FuncNode("DecrementPrePointed", args);
	SonolusRun(code);
	return code;
}

FuncNode DecrementPreShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("DecrementPreShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode Degree(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Degree", args);
	return code;
}

FuncNode DestroyParticleEffect(FuncNode particleId) {
	vector<FuncNode> args;
	args.push_back(particleId);
	FuncNode code = FuncNode("DestroyParticleEffect", args);
	SonolusRun(code);
	return code;
}

FuncNode Divide(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Divide\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Divide", args);
	return code;
}

FuncNode DoWhile(FuncNode body, FuncNode test) {
	vector<FuncNode> args;
	args.push_back(body);
	args.push_back(test);
	FuncNode code = FuncNode("DoWhile", args);
	return code;
}

struct Draw_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode Draw(FuncNode id, Draw_Group_x_y x_y0, Draw_Group_x_y x_y1, Draw_Group_x_y x_y2, Draw_Group_x_y x_y3, FuncNode z, FuncNode a) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	FuncNode code = FuncNode("Draw", args);
	SonolusRun(code);
	return code;
}

struct DrawCurvedB_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode DrawCurvedB(FuncNode id, DrawCurvedB_Group_x_y x_y0, DrawCurvedB_Group_x_y x_y1, DrawCurvedB_Group_x_y x_y2, DrawCurvedB_Group_x_y x_y3, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	args.push_back(n);
	args.push_back(p);
	args.push_back(q);
	FuncNode code = FuncNode("DrawCurvedB", args);
	SonolusRun(code);
	return code;
}

struct DrawCurvedBT_Group_x_y{
	FuncNode x;
	FuncNode y;
};
struct DrawCurvedBT_Group_p_q{
	FuncNode p;
	FuncNode q;
};
FuncNode DrawCurvedBT(FuncNode id, DrawCurvedBT_Group_x_y x_y0, DrawCurvedBT_Group_x_y x_y1, DrawCurvedBT_Group_x_y x_y2, DrawCurvedBT_Group_x_y x_y3, FuncNode z, FuncNode a, FuncNode n, DrawCurvedBT_Group_p_q p_q0, DrawCurvedBT_Group_p_q p_q1) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	args.push_back(n);
	args.push_back(p_q0.p);
	args.push_back(p_q0.q);
	args.push_back(p_q1.p);
	args.push_back(p_q1.q);
	FuncNode code = FuncNode("DrawCurvedBT", args);
	SonolusRun(code);
	return code;
}

struct DrawCurvedL_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode DrawCurvedL(FuncNode id, DrawCurvedL_Group_x_y x_y0, DrawCurvedL_Group_x_y x_y1, DrawCurvedL_Group_x_y x_y2, DrawCurvedL_Group_x_y x_y3, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	args.push_back(n);
	args.push_back(p);
	args.push_back(q);
	FuncNode code = FuncNode("DrawCurvedL", args);
	SonolusRun(code);
	return code;
}

struct DrawCurvedLR_Group_x_y{
	FuncNode x;
	FuncNode y;
};
struct DrawCurvedLR_Group_p_q{
	FuncNode p;
	FuncNode q;
};
FuncNode DrawCurvedLR(FuncNode id, DrawCurvedLR_Group_x_y x_y0, DrawCurvedLR_Group_x_y x_y1, DrawCurvedLR_Group_x_y x_y2, DrawCurvedLR_Group_x_y x_y3, FuncNode z, FuncNode a, FuncNode n, DrawCurvedLR_Group_p_q p_q0, DrawCurvedLR_Group_p_q p_q1) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	args.push_back(n);
	args.push_back(p_q0.p);
	args.push_back(p_q0.q);
	args.push_back(p_q1.p);
	args.push_back(p_q1.q);
	FuncNode code = FuncNode("DrawCurvedLR", args);
	SonolusRun(code);
	return code;
}

struct DrawCurvedR_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode DrawCurvedR(FuncNode id, DrawCurvedR_Group_x_y x_y0, DrawCurvedR_Group_x_y x_y1, DrawCurvedR_Group_x_y x_y2, DrawCurvedR_Group_x_y x_y3, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	args.push_back(n);
	args.push_back(p);
	args.push_back(q);
	FuncNode code = FuncNode("DrawCurvedR", args);
	SonolusRun(code);
	return code;
}

struct DrawCurvedT_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode DrawCurvedT(FuncNode id, DrawCurvedT_Group_x_y x_y0, DrawCurvedT_Group_x_y x_y1, DrawCurvedT_Group_x_y x_y2, DrawCurvedT_Group_x_y x_y3, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(z);
	args.push_back(a);
	args.push_back(n);
	args.push_back(p);
	args.push_back(q);
	FuncNode code = FuncNode("DrawCurvedT", args);
	SonolusRun(code);
	return code;
}

FuncNode EaseInBack(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInBack", args);
	return code;
}

FuncNode EaseInCirc(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInCirc", args);
	return code;
}

FuncNode EaseInCubic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInCubic", args);
	return code;
}

FuncNode EaseInElastic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInElastic", args);
	return code;
}

FuncNode EaseInExpo(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInExpo", args);
	return code;
}

FuncNode EaseInOutBack(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutBack", args);
	return code;
}

FuncNode EaseInOutCirc(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutCirc", args);
	return code;
}

FuncNode EaseInOutCubic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutCubic", args);
	return code;
}

FuncNode EaseInOutElastic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutElastic", args);
	return code;
}

FuncNode EaseInOutExpo(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutExpo", args);
	return code;
}

FuncNode EaseInOutQuad(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutQuad", args);
	return code;
}

FuncNode EaseInOutQuart(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutQuart", args);
	return code;
}

FuncNode EaseInOutQuint(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutQuint", args);
	return code;
}

FuncNode EaseInOutSine(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInOutSine", args);
	return code;
}

FuncNode EaseInQuad(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInQuad", args);
	return code;
}

FuncNode EaseInQuart(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInQuart", args);
	return code;
}

FuncNode EaseInQuint(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInQuint", args);
	return code;
}

FuncNode EaseInSine(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseInSine", args);
	return code;
}

FuncNode EaseOutBack(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutBack", args);
	return code;
}

FuncNode EaseOutCirc(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutCirc", args);
	return code;
}

FuncNode EaseOutCubic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutCubic", args);
	return code;
}

FuncNode EaseOutElastic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutElastic", args);
	return code;
}

FuncNode EaseOutExpo(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutExpo", args);
	return code;
}

FuncNode EaseOutInBack(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInBack", args);
	return code;
}

FuncNode EaseOutInCirc(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInCirc", args);
	return code;
}

FuncNode EaseOutInCubic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInCubic", args);
	return code;
}

FuncNode EaseOutInElastic(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInElastic", args);
	return code;
}

FuncNode EaseOutInExpo(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInExpo", args);
	return code;
}

FuncNode EaseOutInQuad(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInQuad", args);
	return code;
}

FuncNode EaseOutInQuart(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInQuart", args);
	return code;
}

FuncNode EaseOutInQuint(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInQuint", args);
	return code;
}

FuncNode EaseOutInSine(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutInSine", args);
	return code;
}

FuncNode EaseOutQuad(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutQuad", args);
	return code;
}

FuncNode EaseOutQuart(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutQuart", args);
	return code;
}

FuncNode EaseOutQuint(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutQuint", args);
	return code;
}

FuncNode EaseOutSine(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("EaseOutSine", args);
	return code;
}

FuncNode Equal(FuncNode lhs, FuncNode rhs) {
	vector<FuncNode> args;
	args.push_back(lhs);
	args.push_back(rhs);
	FuncNode code = FuncNode("Equal", args);
	return code;
}

FuncNode Execute(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Execute\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Execute", args);
	return code;
}

FuncNode Execute0(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Execute0\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Execute0", args);
	return code;
}

FuncNode ExportValue(FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("ExportValue", args);
	SonolusRun(code);
	return code;
}

FuncNode Floor(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Floor", args);
	return code;
}

FuncNode Frac(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Frac", args);
	return code;
}

FuncNode Get(FuncNode id, FuncNode index) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	FuncNode code = FuncNode("Get", args);
	return code;
}

FuncNode GetPointed(FuncNode id, FuncNode index, FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	FuncNode code = FuncNode("GetPointed", args);
	return code;
}

FuncNode GetShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("GetShifted", args);
	return code;
}

FuncNode Greater(FuncNode lhs, FuncNode rhs) {
	vector<FuncNode> args;
	args.push_back(lhs);
	args.push_back(rhs);
	FuncNode code = FuncNode("Greater", args);
	return code;
}

FuncNode GreaterOr(FuncNode lhs, FuncNode rhs) {
	vector<FuncNode> args;
	args.push_back(lhs);
	args.push_back(rhs);
	FuncNode code = FuncNode("GreaterOr", args);
	return code;
}

FuncNode HasEffectClip(FuncNode id) {
	vector<FuncNode> args;
	args.push_back(id);
	FuncNode code = FuncNode("HasEffectClip", args);
	return code;
}

FuncNode HasParticleEffect(FuncNode id) {
	vector<FuncNode> args;
	args.push_back(id);
	FuncNode code = FuncNode("HasParticleEffect", args);
	return code;
}

FuncNode HasSkinSprite(FuncNode id) {
	vector<FuncNode> args;
	args.push_back(id);
	FuncNode code = FuncNode("HasSkinSprite", args);
	return code;
}

FuncNode If(FuncNode test, FuncNode consequent, FuncNode alternate) {
	vector<FuncNode> args;
	args.push_back(test);
	args.push_back(consequent);
	args.push_back(alternate);
	FuncNode code = FuncNode("If", args);
	return code;
}

FuncNode IncrementPost(FuncNode id, FuncNode index) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	FuncNode code = FuncNode("IncrementPost", args);
	SonolusRun(code);
	return code;
}

FuncNode IncrementPostPointed(FuncNode id, FuncNode index, FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	FuncNode code = FuncNode("IncrementPostPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode IncrementPostShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("IncrementPostShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode IncrementPre(FuncNode id, FuncNode index) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	FuncNode code = FuncNode("IncrementPre", args);
	SonolusRun(code);
	return code;
}

FuncNode IncrementPrePointed(FuncNode id, FuncNode index, FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	FuncNode code = FuncNode("IncrementPrePointed", args);
	SonolusRun(code);
	return code;
}

FuncNode IncrementPreShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("IncrementPreShifted", args);
	SonolusRun(code);
	return code;
}

struct Judge_Group_min_max{
	FuncNode min;
	FuncNode max;
};
FuncNode Judge(FuncNode source, FuncNode target, Judge_Group_min_max perfect, Judge_Group_min_max great, Judge_Group_min_max good) {
	vector<FuncNode> args;
	args.push_back(source);
	args.push_back(target);
	args.push_back(perfect.min);
	args.push_back(perfect.max);
	args.push_back(great.min);
	args.push_back(great.max);
	args.push_back(good.min);
	args.push_back(good.max);
	FuncNode code = FuncNode("Judge", args);
	return code;
}

FuncNode JudgeSimple(FuncNode source, FuncNode target, FuncNode perfect, FuncNode great, FuncNode good) {
	vector<FuncNode> args;
	args.push_back(source);
	args.push_back(target);
	args.push_back(perfect);
	args.push_back(great);
	args.push_back(good);
	FuncNode code = FuncNode("JudgeSimple", args);
	return code;
}

FuncNode JumpLoop(vector<FuncNode> branch) {
	vector<FuncNode> args;
	if(branch.size() < 1) {
		cout << "Error: The argument \"branch\" in function \"JumpLoop\" expect as least 1 values, but only " << branch.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < branch.size(); i++) {
		args.push_back(branch[i]);
	}
	FuncNode code = FuncNode("JumpLoop", args);
	return code;
}

FuncNode Lerp(FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("Lerp", args);
	return code;
}

FuncNode LerpClamped(FuncNode x, FuncNode y, FuncNode s) {
	vector<FuncNode> args;
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	FuncNode code = FuncNode("LerpClamped", args);
	return code;
}

FuncNode Less(FuncNode lhs, FuncNode rhs) {
	vector<FuncNode> args;
	args.push_back(lhs);
	args.push_back(rhs);
	FuncNode code = FuncNode("Less", args);
	return code;
}

FuncNode LessOr(FuncNode lhs, FuncNode rhs) {
	vector<FuncNode> args;
	args.push_back(lhs);
	args.push_back(rhs);
	FuncNode code = FuncNode("LessOr", args);
	return code;
}

FuncNode Log(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Log", args);
	return code;
}

FuncNode Max(FuncNode x, FuncNode y) {
	vector<FuncNode> args;
	args.push_back(x);
	args.push_back(y);
	FuncNode code = FuncNode("Max", args);
	return code;
}

FuncNode Min(FuncNode x, FuncNode y) {
	vector<FuncNode> args;
	args.push_back(x);
	args.push_back(y);
	FuncNode code = FuncNode("Min", args);
	return code;
}

FuncNode Mod(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Mod\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Mod", args);
	return code;
}

struct MoveParticleEffect_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode MoveParticleEffect(FuncNode particleId, MoveParticleEffect_Group_x_y x_y0, MoveParticleEffect_Group_x_y x_y1, MoveParticleEffect_Group_x_y x_y2, MoveParticleEffect_Group_x_y x_y3) {
	vector<FuncNode> args;
	args.push_back(particleId);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	FuncNode code = FuncNode("MoveParticleEffect", args);
	SonolusRun(code);
	return code;
}

FuncNode Multiply(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Multiply\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Multiply", args);
	return code;
}

FuncNode Negate(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Negate", args);
	return code;
}

FuncNode Not(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Not", args);
	return code;
}

FuncNode NotEqual(FuncNode lhs, FuncNode rhs) {
	vector<FuncNode> args;
	args.push_back(lhs);
	args.push_back(rhs);
	FuncNode code = FuncNode("NotEqual", args);
	return code;
}

FuncNode Or(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Or\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Or", args);
	return code;
}

FuncNode Paint(FuncNode id, FuncNode x, FuncNode y, FuncNode size, FuncNode rotation, FuncNode z, FuncNode a) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(size);
	args.push_back(rotation);
	args.push_back(z);
	args.push_back(a);
	FuncNode code = FuncNode("Paint", args);
	SonolusRun(code);
	return code;
}

FuncNode Play(FuncNode id, FuncNode distance) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(distance);
	FuncNode code = FuncNode("Play", args);
	SonolusRun(code);
	return code;
}

FuncNode PlayLooped(FuncNode id) {
	vector<FuncNode> args;
	args.push_back(id);
	FuncNode code = FuncNode("PlayLooped", args);
	// SonolusRun(code);
	return code;
}

FuncNode PlayLoopedScheduled(FuncNode id, FuncNode startTime) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(startTime);
	FuncNode code = FuncNode("PlayLoopedScheduled", args);
	// SonolusRun(code);
	return code;
}

FuncNode PlayScheduled(FuncNode id, FuncNode time, FuncNode distance) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(time);
	args.push_back(distance);
	FuncNode code = FuncNode("PlayScheduled", args);
	SonolusRun(code);
	return code;
}

FuncNode Power(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Power\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Power", args);
	return code;
}

FuncNode Print(FuncNode value, FuncNode format, FuncNode decimalPlaces, FuncNode anchorX, FuncNode anchorY, FuncNode pivotX, FuncNode pivotY, FuncNode width, FuncNode height, FuncNode rotation, FuncNode color, FuncNode alpha, FuncNode horizontalAlign, FuncNode background) {
	vector<FuncNode> args;
	args.push_back(value);
	args.push_back(format);
	args.push_back(decimalPlaces);
	args.push_back(anchorX);
	args.push_back(anchorY);
	args.push_back(pivotX);
	args.push_back(pivotY);
	args.push_back(width);
	args.push_back(height);
	args.push_back(rotation);
	args.push_back(color);
	args.push_back(alpha);
	args.push_back(horizontalAlign);
	args.push_back(background);
	FuncNode code = FuncNode("Print", args);
	SonolusRun(code);
	return code;
}

FuncNode Radian(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Radian", args);
	return code;
}

FuncNode Random(FuncNode min, FuncNode max) {
	vector<FuncNode> args;
	args.push_back(min);
	args.push_back(max);
	FuncNode code = FuncNode("Random", args);
	return code;
}

FuncNode RandomInteger(FuncNode min, FuncNode max) {
	vector<FuncNode> args;
	args.push_back(min);
	args.push_back(max);
	FuncNode code = FuncNode("RandomInteger", args);
	return code;
}

FuncNode Rem(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Rem\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Rem", args);
	return code;
}

FuncNode Remap(FuncNode a, FuncNode b, FuncNode c, FuncNode d, FuncNode x) {
	vector<FuncNode> args;
	args.push_back(a);
	args.push_back(b);
	args.push_back(c);
	args.push_back(d);
	args.push_back(x);
	FuncNode code = FuncNode("Remap", args);
	return code;
}

FuncNode RemapClamped(FuncNode a, FuncNode b, FuncNode c, FuncNode d, FuncNode x) {
	vector<FuncNode> args;
	args.push_back(a);
	args.push_back(b);
	args.push_back(c);
	args.push_back(d);
	args.push_back(x);
	FuncNode code = FuncNode("RemapClamped", args);
	return code;
}

FuncNode Round(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Round", args);
	return code;
}

FuncNode Set(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("Set", args);
	SonolusRun(code);
	return code;
}

FuncNode SetAdd(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetAdd", args);
	SonolusRun(code);
	return code;
}

FuncNode SetAddPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetAddPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetAddShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetAddShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetDivide(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetDivide", args);
	SonolusRun(code);
	return code;
}

FuncNode SetDividePointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetDividePointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetDivideShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetDivideShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetMod(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetMod", args);
	SonolusRun(code);
	return code;
}

FuncNode SetModPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetModPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetModShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetModShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetMultiply(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetMultiply", args);
	SonolusRun(code);
	return code;
}

FuncNode SetMultiplyPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetMultiplyPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetMultiplyShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetMultiplyShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetPower(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetPower", args);
	SonolusRun(code);
	return code;
}

FuncNode SetPowerPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetPowerPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetPowerShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetPowerShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetRem(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetRem", args);
	SonolusRun(code);
	return code;
}

FuncNode SetRemPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetRemPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetRemShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetRemShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode SetSubtract(FuncNode id, FuncNode index, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(value);
	FuncNode code = FuncNode("SetSubtract", args);
	SonolusRun(code);
	return code;
}

FuncNode SetSubtractPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(index);
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("SetSubtractPointed", args);
	SonolusRun(code);
	return code;
}

FuncNode SetSubtractShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x);
	args.push_back(y);
	args.push_back(s);
	args.push_back(value);
	FuncNode code = FuncNode("SetSubtractShifted", args);
	SonolusRun(code);
	return code;
}

FuncNode Sign(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Sign", args);
	return code;
}

FuncNode Sin(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Sin", args);
	return code;
}

FuncNode Sinh(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Sinh", args);
	return code;
}

FuncNode Spawn(FuncNode id, vector<FuncNode> memory) {
	vector<FuncNode> args;
	args.push_back(id);
	if(memory.size() < 0) {
		cout << "Error: The argument \"memory\" in function \"Spawn\" expect as least 0 values, but only " << memory.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < memory.size(); i++) {
		args.push_back(memory[i]);
	}
	FuncNode code = FuncNode("Spawn", args);
	SonolusRun(code);
	return code;
}

struct SpawnParticleEffect_Group_x_y{
	FuncNode x;
	FuncNode y;
};
FuncNode SpawnParticleEffect(FuncNode id, SpawnParticleEffect_Group_x_y x_y0, SpawnParticleEffect_Group_x_y x_y1, SpawnParticleEffect_Group_x_y x_y2, SpawnParticleEffect_Group_x_y x_y3, FuncNode duration, FuncNode isLooped) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(duration);
	args.push_back(isLooped);
	FuncNode code = FuncNode("SpawnParticleEffect", args);
	SonolusRun(code);
	return code;
}

FuncNode SpawnParticleEffect2(FuncNode id, SpawnParticleEffect_Group_x_y x_y0, SpawnParticleEffect_Group_x_y x_y1, SpawnParticleEffect_Group_x_y x_y2, SpawnParticleEffect_Group_x_y x_y3, FuncNode duration, FuncNode isLooped) {
	vector<FuncNode> args;
	args.push_back(id);
	args.push_back(x_y0.x);
	args.push_back(x_y0.y);
	args.push_back(x_y1.x);
	args.push_back(x_y1.y);
	args.push_back(x_y2.x);
	args.push_back(x_y2.y);
	args.push_back(x_y3.x);
	args.push_back(x_y3.y);
	args.push_back(duration);
	args.push_back(isLooped);
	FuncNode code = FuncNode("SpawnParticleEffect", args);
	return code;
}

FuncNode StackEnter(FuncNode size) {
	vector<FuncNode> args;
	args.push_back(size);
	FuncNode code = FuncNode("StackEnter", args);
	SonolusRun(code);
	return code;
}

FuncNode StackGet(FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(offset);
	FuncNode code = FuncNode("StackGet", args);
	return code;
}

FuncNode StackGetFrame(FuncNode offset) {
	vector<FuncNode> args;
	args.push_back(offset);
	FuncNode code = FuncNode("StackGetFrame", args);
	return code;
}

FuncNode StackGetFramePointer() {
	vector<FuncNode> args;
	FuncNode code = FuncNode("StackGetFramePointer", args);
	return code;
}

FuncNode StackGetPointer() {
	vector<FuncNode> args;
	FuncNode code = FuncNode("StackGetPointer", args);
	return code;
}

FuncNode StackGrow(FuncNode size) {
	vector<FuncNode> args;
	args.push_back(size);
	FuncNode code = FuncNode("StackGrow", args);
	SonolusRun(code);
	return code;
}

FuncNode StackInit() {
	vector<FuncNode> args;
	FuncNode code = FuncNode("StackInit", args);
	SonolusRun(code);
	return code;
}

FuncNode StackLeave() {
	vector<FuncNode> args;
	FuncNode code = FuncNode("StackLeave", args);
	SonolusRun(code);
	return code;
}

FuncNode StackPop() {
	vector<FuncNode> args;
	FuncNode code = FuncNode("StackPop", args);
	SonolusRun(code);
	return code;
}

FuncNode StackPush(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("StackPush", args);
	SonolusRun(code);
	return code;
}

FuncNode StackSet(FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("StackSet", args);
	SonolusRun(code);
	return code;
}

FuncNode StackSetFrame(FuncNode offset, FuncNode value) {
	vector<FuncNode> args;
	args.push_back(offset);
	args.push_back(value);
	FuncNode code = FuncNode("StackSetFrame", args);
	SonolusRun(code);
	return code;
}

FuncNode StackSetFramePointer(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("StackSetFramePointer", args);
	SonolusRun(code);
	return code;
}

FuncNode StackSetPointer(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("StackSetPointer", args);
	SonolusRun(code);
	return code;
}

FuncNode StopLooped(FuncNode loopId) {
	vector<FuncNode> args;
	args.push_back(loopId);
	FuncNode code = FuncNode("StopLooped", args);
	SonolusRun(code);
	return code;
}

FuncNode StopLoopedScheduled(FuncNode loopId, FuncNode endTime) {
	vector<FuncNode> args;
	args.push_back(loopId);
	args.push_back(endTime);
	FuncNode code = FuncNode("StopLoopedScheduled", args);
	SonolusRun(code);
	return code;
}

FuncNode Subtract(vector<FuncNode> value) {
	vector<FuncNode> args;
	if(value.size() < 1) {
		cout << "Error: The argument \"value\" in function \"Subtract\" expect as least 1 values, but only " << value.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < value.size(); i++) {
		args.push_back(value[i]);
	}
	FuncNode code = FuncNode("Subtract", args);
	return code;
}

struct Switch_Group_test_consequent{
	FuncNode test;
	FuncNode consequent;
};
FuncNode Switch(FuncNode discriminant, vector<Switch_Group_test_consequent> test_consequent) {
	vector<FuncNode> args;
	args.push_back(discriminant);
	if(test_consequent.size() < 0) {
		cout << "Error: The argument \"test_consequent\" in function \"Switch\" expect as least 0 values, but only " << test_consequent.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < test_consequent.size(); i++) {
		args.push_back(test_consequent[i].test);
		args.push_back(test_consequent[i].consequent);
	}
	FuncNode code = FuncNode("Switch", args);
	return code;
}

FuncNode SwitchInteger(FuncNode discriminant, vector<FuncNode> consequent) {
	vector<FuncNode> args;
	args.push_back(discriminant);
	if(consequent.size() < 0) {
		cout << "Error: The argument \"consequent\" in function \"SwitchInteger\" expect as least 0 values, but only " << consequent.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < consequent.size(); i++) {
		args.push_back(consequent[i]);
	}
	FuncNode code = FuncNode("SwitchInteger", args);
	return code;
}

FuncNode SwitchIntegerWithDefault(FuncNode discriminant, vector<FuncNode> consequent, FuncNode default) {
	vector<FuncNode> args;
	args.push_back(discriminant);
	if(consequent.size() < 0) {
		cout << "Error: The argument \"consequent\" in function \"SwitchIntegerWithDefault\" expect as least 0 values, but only " << consequent.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < consequent.size(); i++) {
		args.push_back(consequent[i]);
	}
	args.push_back(default);
	FuncNode code = FuncNode("SwitchIntegerWithDefault", args);
	return code;
}

struct SwitchWithDefault_Group_test_consequent{
	FuncNode test;
	FuncNode consequent;
};
FuncNode SwitchWithDefault(FuncNode discriminant, vector<SwitchWithDefault_Group_test_consequent> test_consequent, FuncNode default) {
	vector<FuncNode> args;
	args.push_back(discriminant);
	if(test_consequent.size() < 0) {
		cout << "Error: The argument \"test_consequent\" in function \"SwitchWithDefault\" expect as least 0 values, but only " << test_consequent.size() << " was provided." << endl;
		cout << "    Error occurred in " << callFromFunc << " in " << callFromFile << ":" << callFromLine << ":" << callFromColumn << endl;
	}
	for (int i = 0; i < test_consequent.size(); i++) {
		args.push_back(test_consequent[i].test);
		args.push_back(test_consequent[i].consequent);
	}
	args.push_back(default);
	FuncNode code = FuncNode("SwitchWithDefault", args);
	return code;
}

FuncNode Tan(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Tan", args);
	return code;
}

FuncNode Tanh(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Tanh", args);
	return code;
}

FuncNode TimeToScaledTime(FuncNode time) {
	vector<FuncNode> args;
	args.push_back(time);
	FuncNode code = FuncNode("TimeToScaledTime", args);
	return code;
}

FuncNode TimeToStartingScaledTime(FuncNode time) {
	vector<FuncNode> args;
	args.push_back(time);
	FuncNode code = FuncNode("TimeToStartingScaledTime", args);
	return code;
}

FuncNode TimeToStartingTime(FuncNode time) {
	vector<FuncNode> args;
	args.push_back(time);
	FuncNode code = FuncNode("TimeToStartingTime", args);
	return code;
}

FuncNode TimeToTimeScale(FuncNode time) {
	vector<FuncNode> args;
	args.push_back(time);
	FuncNode code = FuncNode("TimeToTimeScale", args);
	return code;
}

FuncNode Trunc(FuncNode value) {
	vector<FuncNode> args;
	args.push_back(value);
	FuncNode code = FuncNode("Trunc", args);
	return code;
}

FuncNode Unlerp(FuncNode a, FuncNode b, FuncNode x) {
	vector<FuncNode> args;
	args.push_back(a);
	args.push_back(b);
	args.push_back(x);
	FuncNode code = FuncNode("Unlerp", args);
	return code;
}

FuncNode UnlerpClamped(FuncNode a, FuncNode b, FuncNode x) {
	vector<FuncNode> args;
	args.push_back(a);
	args.push_back(b);
	args.push_back(x);
	FuncNode code = FuncNode("UnlerpClamped", args);
	return code;
}

FuncNode While(FuncNode test, FuncNode body) {
	vector<FuncNode> args;
	args.push_back(test);
	args.push_back(body);
	FuncNode code = FuncNode("While", args);
	return code;
}

#undef default

#endif

/*** End of inlined file: Functions.h ***/


#undef Get
#undef Set
#undef Spawn
FuncNode Get(FuncNode id, FuncNode index) {
    SonolusAssert(
        !id.getNode().isValue || readable[id.getNode().value].count(callbackName),
        "`%s` block is not readable in `%s` callback at `%s` archetype!",
        blockName[id.getNode().value].c_str(),
        callbackName.c_str(),
        archetypeName.c_str()
    );
    return __builtin_Get(id, index);
}
FuncNode Set(FuncNode id, FuncNode index, FuncNode value) {
    // if (id.getNode().value == 4102 && index.getNode().value == 63) cout << id << " " << index << " " << value << endl;
    // if (id.getNode().value == 4002 && index.getNode().value == 31) cout << id << " " << index << " " << value << endl;
    SonolusAssert(
        !id.getNode().isValue || writable[id.getNode().value].count(callbackName),
        "`%s` block is not writable in `%s` callback at `%s` archetype!",
        blockName[id.getNode().value].c_str(),
        callbackName.c_str(),
        archetypeName.c_str()
    );
    return __builtin_Set(id, index, value);
}
FuncNode Spawn(FuncNode id, vector<FuncNode> memory) {
    memory.insert(memory.begin(), 0);
	return __builtin_Spawn(id, memory);
}

/*** Start of inlined file: Operators.h ***/
#ifndef Operators_H
#define Operators_H

#ifdef COMPILE_RUNTIME
FuncNode operator + (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value + b.getNode().value;
	return FuncNode("Add", { a, b });
}

FuncNode operator - (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value - b.getNode().value;
	return FuncNode("Subtract", { a, b });
}

FuncNode operator - (FuncNode a) {
    if (a.getNode().isValue) return -a.getNode().value;
	return FuncNode("Multiply", { -1, a });
}

FuncNode operator * (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value * b.getNode().value;
	return FuncNode("Multiply", { a, b });
}

FuncNode operator / (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value / b.getNode().value;
	return FuncNode("Divide", { a, b });
}

FuncNode operator % (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value - b.getNode().value * int(a.getNode().value / b.getNode().value);
	return FuncNode("Mod", { a, b });
}

FuncNode operator == (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value == b.getNode().value;
	return FuncNode("Equal", { a, b });
}

FuncNode operator != (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value != b.getNode().value;
	return FuncNode("NotEqual", { a, b });
}

FuncNode operator < (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value < b.getNode().value;
	return FuncNode("Less", { a, b });
}

FuncNode operator <= (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value <= b.getNode().value;
	return FuncNode("LessOr", { a, b });
}

FuncNode operator > (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value > b.getNode().value;
	return FuncNode("Greater", { a, b });
}

FuncNode operator >= (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value >= b.getNode().value;
	return FuncNode("GreaterOr", { a, b });
}

FuncNode operator || (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value || b.getNode().value;
	return FuncNode("Or", { a, b });
}

FuncNode operator && (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return a.getNode().value && b.getNode().value;
	return FuncNode("And", { a, b });
}

FuncNode operator ! (FuncNode a) {
    if (a.getNode().isValue) return !a.getNode().value;
    return FuncNode("Not", { a });
}

FuncNode operator | (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return int(a.getNode().value) | int(b.getNode().value);
	return FuncNode("Or", { a, b });
}

FuncNode operator & (FuncNode a, FuncNode b) {
    if (a.getNode().isValue && b.getNode().isValue) return int(a.getNode().value) & int(b.getNode().value);
	return FuncNode("And", { a, b });
}

FuncNode operator ~ (FuncNode a) {
    if (a.getNode().isValue) return ~int(a.getNode().value);
    return FuncNode("Not", { a });
}
#endif

#endif
/*** End of inlined file: Operators.h ***/



/*** Start of inlined file: Variable.h ***/
#ifndef Variable_H
#define Variable_H

#define ALLOCATOR_SIZE 10001

map<int, int> SonolusMemorySize;
map<int, FuncNode> SonolusMemoryIndex;
map<int, int> SonolusMemoryDelta;
set<int> allowAllocate;

template<typename T>
map<int, int> SizeOf() {
    createNodeContainer(false);
    auto backupSonolusMemoryDelta = SonolusMemoryDelta;
    SonolusMemoryDelta.clear();
    T x;
    map<int, int> result = SonolusMemoryDelta;
    SonolusMemoryDelta = backupSonolusMemoryDelta;
    mergeNodeContainer();
    return result;
}

// Tips: 当 index.getNodeId().isValue 为 0 时，说明是初始化，否则是引用
int defaultAllocatorId = 0;
class Variable {
    public:
    int allocatorId = defaultAllocatorId;
    FuncNode index = 0;

    FuncNode allocate() {
        // cout << allocatorId << " " << SonolusMemoryIndex[allocatorId] + SonolusMemoryDelta[allocatorId] << endl;
        if (allowAllocate.count(allocatorId) == 0) {
            // cerr << "You are not allowed to allocate pointer dynamically!" << endl;
            return -1;
        }
        FuncNode index = SonolusMemoryIndex[allocatorId] + SonolusMemoryDelta[allocatorId];
        int val1 = index.getNode().value, val2 = SonolusMemorySize[allocatorId];
        SonolusAssert(
            index.getNode().isValue == 0 || index.getNode().value < SonolusMemorySize[allocatorId],
            "Memory is full!(id: %d, allocated %d / %d)",
            allocatorId,
            val1,
            val2
        );
        SonolusMemoryDelta[allocatorId]++;
        return index;
    }

    public:

    // 基础构造函数
    Constructor Variable() {
        index = allocate();
    }
    Constructor Variable(FuncNode value) {
        index = allocate();
        if (index.getNode().isValue) Set(allocatorId, index, value);
    }
    Constructor Variable(double value) {
        index = allocate();
        if (index.getNode().isValue) Set(allocatorId, index, value);
    }
    Constructor Variable(int allocatorId, int index) { // 构造指针类型
        this->allocatorId = allocatorId;
        this->index = index;
    }
    Constructor Variable(int allocatorId, FuncNode index) { // 构造指针类型
        this->allocatorId = allocatorId;
        this->index = index;
    }
    // 拷贝函数
    Constructor Variable(const Variable &value) {
        index = allocate();
        if (index.getNode().isValue) Set(allocatorId, index, FuncNode("Get", { value.allocatorId, value.index }));
    }
    Constructor Variable(const Variable &value, int offset) {
        #ifndef COMPILE_RUNTIME
        this->index = FuncNode(value.index) + offset;
        #else
        this->index = value.index + offset;
        #endif
    }
    // 类型转换函数
    operator FuncNode() {
        return FuncNode("Get", { allocatorId, index });
    }
    operator FuncNode() const {
        return FuncNode("Get", { allocatorId, index });
    }
    #ifndef COMPILE_RUNTIME
    operator int() {
        return false;
    }
    #endif

    // 拷贝赋值
    Variable& operator = (FuncNode a) {
        Set(allocatorId, index, a);
        return *this;
    }
    Variable& operator = (double a) {
        Set(allocatorId, index, a);
        return *this;
    }
    Variable& operator = (const Variable &a) {
        Set(allocatorId, index, FuncNode("Get", { a.allocatorId, a.index }));
        return *this;
    }

    void reallocate(FuncNode index) {
        this->index = index;
    }
};

Variable& operator ++ (Variable &a) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) + 1);
    return a;
}

Variable& operator ++ (Variable &a, int) {
    auto res = a;
    ++a;
    return res;
}

Variable& operator -- (Variable &a) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) - 1);
    return a;
}

Variable& operator -- (Variable &a, int) {
    auto res = a;
    --a;
    return res;
}

Variable& operator += (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) + b);
    return a;
}

Variable& operator -= (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) - b);
    return a;
}

Variable& operator *= (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) * b);
    return a;
}

Variable& operator /= (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) / b);
    return a;
}

Variable& operator %= (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) % b);
    return a;
}

Variable& operator &= (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) & b);
    return a;
}

Variable& operator |= (Variable &a, FuncNode b) {
    Set(a.allocatorId, a.index, FuncNode("Get", { a.allocatorId, a.index }) | b);
    return a;
}

FuncNode GetValue(const Variable &v) {
    return FuncNode(v);
}

Variable GetPointer(int allocatorId, FuncNode index) {
    return Variable(allocatorId, index);
}

#endif
/*** End of inlined file: Variable.h ***/


/*** Start of inlined file: Blocks.h ***/
#ifndef Blocks_H
#define Blocks_H

template<typename T>
class BlocksArray {
    public:

    FuncNode arraySize = 0;
    Constructor BlocksArray(){}
    Constructor BlocksArray(FuncNode arraySize): arraySize(arraySize){}

    T operator [] (FuncNode index) {
        return T(index * arraySize);
    }
};

template<int allocatorId>
class VariablesArray {
    public:

    FuncNode offset = 0;
    FuncNode siz = 0;
    Constructor VariablesArray(){}
    Constructor VariablesArray(FuncNode offset, FuncNode siz): offset(offset), siz(siz){}

    Variable operator [] (FuncNode index) {
        return Variable(allocatorId, offset + index);
    }
};

#endif
/*** End of inlined file: Blocks.h ***/

using var = Variable;
#define SonolusApi var

/*** Start of inlined file: EngineLiterals.h ***/
#ifndef EngineLiterals_H
#define EngineLiterals_H

class Text {
    public:

    /** en: Custom Server */
    string CustomServer = "#CUSTOM_SERVER";
    /** en: Collection */
    string Collection = "#COLLECTION";
    /** en: Server */
    string Server = "#SERVER";
    /** en: Address */
    string Address = "#ADDRESS";
    /** en: Expiration */
    string Expiration = "#EXPIRATION";
    /** en: Storage */
    string Storage = "#STORAGE";
    /** en: Log */
    string Log = "#LOG";
    /** en: Inquiry */
    string Inquiry = "#INQUIRY";
    /** en: Banner */
    string Banner = "#BANNER";
    /** en: Post */
    string Post = "#POST";
    /** en: Playlist */
    string Playlist = "#PLAYLIST";
    /** en: Level */
    string Level = "#LEVEL";
    /** en: Skin */
    string Skin = "#SKIN";
    /** en: Background */
    string Background = "#BACKGROUND";
    /** en: SFX */
    string Effect = "#EFFECT";
    /** en: Particle */
    string Particle = "#PARTICLE";
    /** en: Engine */
    string Engine = "#ENGINE";
    /** en: Replay */
    string Replay = "#REPLAY";
    /** en: Room */
    string Room = "#ROOM";
    /** en: Thumbnail */
    string PostThumbnail = "#POST_THUMBNAIL";
    /** en: Thumbnail */
    string PlaylistThumbnail = "#PLAYLIST_THUMBNAIL";
    /** en: Cover */
    string LevelCover = "#LEVEL_COVER";
    /** en: BGM */
    string LevelBgm = "#LEVEL_BGM";
    /** en: Preview */
    string LevelPreview = "#LEVEL_PREVIEW";
    /** en: Data */
    string LevelData = "#LEVEL_DATA";
    /** en: Thumbnail */
    string SkinThumbnail = "#SKIN_THUMBNAIL";
    /** en: Data */
    string SkinData = "#SKIN_DATA";
    /** en: Texture */
    string SkinTexture = "#SKIN_TEXTURE";
    /** en: Thumbnail */
    string BackgroundThumbnail = "#BACKGROUND_THUMBNAIL";
    /** en: Image */
    string BackgroundImage = "#BACKGROUND_IMAGE";
    /** en: Data */
    string BackgroundData = "#BACKGROUND_DATA";
    /** en: Configuration */
    string BackgroundConfiguration = "#BACKGROUND_CONFIGURATION";
    /** en: Thumbnail */
    string EffectThumbnail = "#EFFECT_THUMBNAIL";
    /** en: Data */
    string EffectData = "#EFFECT_DATA";
    /** en: Audio */
    string EffectAudio = "#EFFECT_AUDIO";
    /** en: Thumbnail */
    string ParticleThumbnail = "#PARTICLE_THUMBNAIL";
    /** en: Data */
    string ParticleData = "#PARTICLE_DATA";
    /** en: Texture */
    string ParticleTexture = "#PARTICLE_TEXTURE";
    /** en: Thumbnail */
    string EngineThumbnail = "#ENGINE_THUMBNAIL";
    /** en: Play Data */
    string EnginePlaydata = "#ENGINE_PLAYDATA";
    /** en: Watch Data */
    string EngineWatchdata = "#ENGINE_WATCHDATA";
    /** en: Preview Data */
    string EnginePreviewdata = "#ENGINE_PREVIEWDATA";
    /** en: Tutorial Data */
    string EngineTutorialdata = "#ENGINE_TUTORIALDATA";
    /** en: ROM */
    string EngineRom = "#ENGINE_ROM";
    /** en: Configuration */
    string EngineConfiguration = "#ENGINE_CONFIGURATION";
    /** en: Data */
    string ReplayData = "#REPLAY_DATA";
    /** en: Configuration */
    string ReplayConfiguration = "#REPLAY_CONFIGURATION";
    /** en: Cover */
    string RoomCover = "#ROOM_COVER";
    /** en: BGM */
    string RoomBgm = "#ROOM_BGM";
    /** en: Preview */
    string RoomPreview = "#ROOM_PREVIEW";
    /** en: Grade */
    string Grade = "#GRADE";
    /** en: Arcade Score */
    string ArcadeScore = "#ARCADE_SCORE";
    /** en: Accuracy Score */
    string AccuracyScore = "#ACCURACY_SCORE";
    /** en: Combo */
    string Combo = "#COMBO";
    /** en: Perfect */
    string Perfect = "#PERFECT";
    /** en: Great */
    string Great = "#GREAT";
    /** en: Good */
    string Good = "#GOOD";
    /** en: Miss */
    string Miss = "#MISS";
    /** en: Filter */
    string Filter = "#FILTER";
    /** en: Sort */
    string Sort = "#SORT";
    /** en: Keywords */
    string Keywords = "#KEYWORDS";
    /** en: Name */
    string Name = "#NAME";
    /** en: Rating */
    string Rating = "#RATING";
    /** en: Minimum Rating */
    string RatingMinimum = "#RATING_MINIMUM";
    /** en: Maximum Rating */
    string RatingMaximum = "#RATING_MAXIMUM";
    /** en: Title */
    string Title = "#TITLE";
    /** en: Subtitle */
    string Subtitle = "#SUBTITLE";
    /** en: Artists */
    string Artists = "#ARTISTS";
    /** en: Time */
    string Time = "#TIME";
    /** en: Author */
    string Author = "#AUTHOR";
    /** en: Description */
    string Description = "#DESCRIPTION";
    /** en: Genre */
    string Genre = "#GENRE";
    /** en: Type */
    string Type = "#TYPE";
    /** en: Category */
    string Category = "#CATEGORY";
    /** en: Status */
    string Status = "#STATUS";
    /** en: Language */
    string Language = "#LANGUAGE";
    /** en: Difficulty */
    string Difficulty = "#DIFFICULTY";
    /** en: Version */
    string Version = "#VERSION";
    /** en: Length */
    string Length = "#LENGTH";
    /** en: Minimum Length */
    string LengthMinimum = "#LENGTH_MINIMUM";
    /** en: Maximum Length */
    string LengthMaximum = "#LENGTH_MAXIMUM";
    /** en: Additional Information */
    string AdditionalInformation = "#ADDITIONAL_INFORMATION";
    /** en: Timezone */
    string Timezone = "#TIMEZONE";
    /** en: Region */
    string Region = "#REGION";
    /** en: Content */
    string Content = "#CONTENT";
    /** en: Comment */
    string Comment = "#COMMENT";
    /** en: Message */
    string Message = "#MESSAGE";
    /** en: Role */
    string Role = "#ROLE";
    /** en: Permission */
    string Permission = "#PERMISSION";
    /** en: User */
    string User = "#USER";
    /** en: Level Speed */
    string Speed = "#SPEED";
    /** en: Mirror Level */
    string Mirror = "#MIRROR";
    /** en: Random */
    string Random = "#RANDOM";
    /** en: Hidden */
    string Hidden = "#HIDDEN";
    /** en: Strict Judgment */
    string JudgmentStrict = "#JUDGMENT_STRICT";
    /** en: Loose Judgment */
    string JudgmentLoose = "#JUDGMENT_LOOSE";
    /** en: Auto SFX */
    string EffectAuto = "#EFFECT_AUTO";
    /** en: Stage */
    string Stage = "#STAGE";
    /** en: Stage Position */
    string StagePosition = "#STAGE_POSITION";
    /** en: Stage Size */
    string StageSize = "#STAGE_SIZE";
    /** en: Stage Rotation */
    string StageRotation = "#STAGE_ROTATION";
    /** en: Stage Direction */
    string StageDirection = "#STAGE_DIRECTION";
    /** en: Stage Transparency */
    string StageAlpha = "#STAGE_ALPHA";
    /** en: Stage Animation */
    string StageAnimation = "#STAGE_ANIMATION";
    /** en: Stage Tilt */
    string StageTilt = "#STAGE_TILT";
    /** en: Vertical Stage Cover */
    string StageCoverVertical = "#STAGE_COVER_VERTICAL";
    /** en: Horizontal Stage Cover */
    string StageCoverHorizontal = "#STAGE_COVER_HORIZONTAL";
    /** en: Stage Cover Transparency */
    string StageCoverAlpha = "#STAGE_COVER_ALPHA";
    /** en: Lock Stage Aspect Ratio */
    string StageAspectratioLock = "#STAGE_ASPECTRATIO_LOCK";
    /** en: Stage Effect */
    string StageEffect = "#STAGE_EFFECT";
    /** en: Stage Effect Position */
    string StageEffectPosition = "#STAGE_EFFECT_POSITION";
    /** en: Stage Effect Size */
    string StageEffectSize = "#STAGE_EFFECT_SIZE";
    /** en: Stage Effect Rotation */
    string StageEffectRotation = "#STAGE_EFFECT_ROTATION";
    /** en: Stage Effect Direction */
    string StageEffectDirection = "#STAGE_EFFECT_DIRECTION";
    /** en: Stage Effect Transparency */
    string StageEffectAlpha = "#STAGE_EFFECT_ALPHA";
    /** en: Lane */
    string Lane = "#LANE";
    /** en: Lane Position */
    string LanePosition = "#LANE_POSITION";
    /** en: Lane Size */
    string LaneSize = "#LANE_SIZE";
    /** en: Lane Rotation */
    string LaneRotation = "#LANE_ROTATION";
    /** en: Lane Direction */
    string LaneDirection = "#LANE_DIRECTION";
    /** en: Lane Transparency */
    string LaneAlpha = "#LANE_ALPHA";
    /** en: Lane Animation */
    string LaneAnimation = "#LANE_ANIMATION";
    /** en: Lane Effect */
    string LaneEffect = "#LANE_EFFECT";
    /** en: Lane Effect Position */
    string LaneEffectPosition = "#LANE_EFFECT_POSITION";
    /** en: Lane Effect Size */
    string LaneEffectSize = "#LANE_EFFECT_SIZE";
    /** en: Lane Effect Rotation */
    string LaneEffectRotation = "#LANE_EFFECT_ROTATION";
    /** en: Lane Effect Direction */
    string LaneEffectDirection = "#LANE_EFFECT_DIRECTION";
    /** en: Lane Effect Transparency */
    string LaneEffectAlpha = "#LANE_EFFECT_ALPHA";
    /** en: Judgment Line */
    string Judgeline = "#JUDGELINE";
    /** en: Judgment Line Position */
    string JudgelinePosition = "#JUDGELINE_POSITION";
    /** en: Judgment Line Size */
    string JudgelineSize = "#JUDGELINE_SIZE";
    /** en: Judgment Line Rotation */
    string JudgelineRotation = "#JUDGELINE_ROTATION";
    /** en: Judgment Line Direction */
    string JudgelineDirection = "#JUDGELINE_DIRECTION";
    /** en: Judgment Line Transparency */
    string JudgelineAlpha = "#JUDGELINE_ALPHA";
    /** en: Judgment Line Animation */
    string JudgelineAnimation = "#JUDGELINE_ANIMATION";
    /** en: Judgment Line Effect */
    string JudgelineEffect = "#JUDGELINE_EFFECT";
    /** en: Judgment Line Effect Position */
    string JudgelineEffectPosition = "#JUDGELINE_EFFECT_POSITION";
    /** en: Judgment Line Effect Size */
    string JudgelineEffectSize = "#JUDGELINE_EFFECT_SIZE";
    /** en: Judgment Line Effect Rotation */
    string JudgelineEffectRotation = "#JUDGELINE_EFFECT_ROTATION";
    /** en: Judgment Line Effect Direction */
    string JudgelineEffectDirection = "#JUDGELINE_EFFECT_DIRECTION";
    /** en: Judgment Line Effect Transparency */
    string JudgelineEffectAlpha = "#JUDGELINE_EFFECT_ALPHA";
    /** en: Slot */
    string Slot = "#SLOT";
    /** en: Slot Position */
    string SlotPosition = "#SLOT_POSITION";
    /** en: Slot Size */
    string SlotSize = "#SLOT_SIZE";
    /** en: Slot Rotation */
    string SlotRotation = "#SLOT_ROTATION";
    /** en: Slot Direction */
    string SlotDirection = "#SLOT_DIRECTION";
    /** en: Slot Transparency */
    string SlotAlpha = "#SLOT_ALPHA";
    /** en: Slot Animation */
    string SlotAnimation = "#SLOT_ANIMATION";
    /** en: Slot Effect */
    string SlotEffect = "#SLOT_EFFECT";
    /** en: Slot Effect Position */
    string SlotEffectPosition = "#SLOT_EFFECT_POSITION";
    /** en: Slot Effect Size */
    string SlotEffectSize = "#SLOT_EFFECT_SIZE";
    /** en: Slot Effect Rotation */
    string SlotEffectRotation = "#SLOT_EFFECT_ROTATION";
    /** en: Slot Effect Direction */
    string SlotEffectDirection = "#SLOT_EFFECT_DIRECTION";
    /** en: Slot Effect Transparency */
    string SlotEffectAlpha = "#SLOT_EFFECT_ALPHA";
    /** en: Note */
    string Note = "#NOTE";
    /** en: Note Speed */
    string NoteSpeed = "#NOTE_SPEED";
    /** en: Random Note Speed */
    string NoteSpeedRandom = "#NOTE_SPEED_RANDOM";
    /** en: Note Position */
    string NotePosition = "#NOTE_POSITION";
    /** en: Note Size */
    string NoteSize = "#NOTE_SIZE";
    /** en: Note Rotation */
    string NoteRotation = "#NOTE_ROTATION";
    /** en: Note Direction */
    string NoteDirection = "#NOTE_DIRECTION";
    /** en: Note Color */
    string NoteColor = "#NOTE_COLOR";
    /** en: Note Transparency */
    string NoteAlpha = "#NOTE_ALPHA";
    /** en: Note Animation */
    string NoteAnimation = "#NOTE_ANIMATION";
    /** en: Note Effect */
    string NoteEffect = "#NOTE_EFFECT";
    /** en: Note Effect Position */
    string NoteEffectPosition = "#NOTE_EFFECT_POSITION";
    /** en: Note Effect Size */
    string NoteEffectSize = "#NOTE_EFFECT_SIZE";
    /** en: Note Effect Rotation */
    string NoteEffectRotation = "#NOTE_EFFECT_ROTATION";
    /** en: Note Effect Direction */
    string NoteEffectDirection = "#NOTE_EFFECT_DIRECTION";
    /** en: Note Effect Color */
    string NoteEffectColor = "#NOTE_EFFECT_COLOR";
    /** en: Note Effect Transparency */
    string NoteEffectAlpha = "#NOTE_EFFECT_ALPHA";
    /** en: Marker */
    string Marker = "#MARKER";
    /** en: Marker Position */
    string MarkerPosition = "#MARKER_POSITION";
    /** en: Marker Size */
    string MarkerSize = "#MARKER_SIZE";
    /** en: Marker Rotation */
    string MarkerRotation = "#MARKER_ROTATION";
    /** en: Marker Direction */
    string MarkerDirection = "#MARKER_DIRECTION";
    /** en: Marker Color */
    string MarkerColor = "#MARKER_COLOR";
    /** en: Marker Transparency */
    string MarkerAlpha = "#MARKER_ALPHA";
    /** en: Marker Animation */
    string MarkerAnimation = "#MARKER_ANIMATION";
    /** en: Connector */
    string Connector = "#CONNECTOR";
    /** en: Connector Position */
    string ConnectorPosition = "#CONNECTOR_POSITION";
    /** en: Connector Size */
    string ConnectorSize = "#CONNECTOR_SIZE";
    /** en: Connector Rotation */
    string ConnectorRotation = "#CONNECTOR_ROTATION";
    /** en: Connector Direction */
    string ConnectorDirection = "#CONNECTOR_DIRECTION";
    /** en: Connector Color */
    string ConnectorColor = "#CONNECTOR_COLOR";
    /** en: Connector Transparency */
    string ConnectorAlpha = "#CONNECTOR_ALPHA";
    /** en: Connector Animation */
    string ConnectorAnimation = "#CONNECTOR_ANIMATION";
    /** en: Simultaneous Line */
    string Simline = "#SIMLINE";
    /** en: Simultaneous Line Position */
    string SimlinePosition = "#SIMLINE_POSITION";
    /** en: Simultaneous Line Size */
    string SimlineSize = "#SIMLINE_SIZE";
    /** en: Simultaneous Line Rotation */
    string SimlineRotation = "#SIMLINE_ROTATION";
    /** en: Simultaneous Line Direction */
    string SimlineDirection = "#SIMLINE_DIRECTION";
    /** en: Simultaneous Line Color */
    string SimlineColor = "#SIMLINE_COLOR";
    /** en: Simultaneous Line Transparency */
    string SimlineAlpha = "#SIMLINE_ALPHA";
    /** en: Simultaneous Line Animation */
    string SimlineAnimation = "#SIMLINE_ANIMATION";
    /** en: Preview Vertical Scale */
    string PreviewScaleVertical = "#PREVIEW_SCALE_VERTICAL";
    /** en: Preview Horizontal Scale */
    string PreviewScaleHorizontal = "#PREVIEW_SCALE_HORIZONTAL";
    /** en: Preview Time */
    string PreviewTime = "#PREVIEW_TIME";
    /** en: Preview Score */
    string PreviewScore = "#PREVIEW_SCORE";
    /** en: Preview BPM */
    string PreviewBpm = "#PREVIEW_BPM";
    /** en: Preview Time Scale */
    string PreviewTimescale = "#PREVIEW_TIMESCALE";
    /** en: Preview Beat */
    string PreviewBeat = "#PREVIEW_BEAT";
    /** en: Preview Measure */
    string PreviewMeasure = "#PREVIEW_MEASURE";
    /** en: Preview Combo */
    string PreviewCombo = "#PREVIEW_COMBO";
    /** en: None */
    string None = "#NONE";
    /** en: Any */
    string Any = "#ANY";
    /** en: All */
    string All = "#ALL";
    /** en: Others */
    string Others = "#OTHERS";
    /** en: Short */
    string Short = "#SHORT";
    /** en: Long */
    string Long = "#LONG";
    /** en: High */
    string High = "#HIGH";
    /** en: Mid */
    string Mid = "#MID";
    /** en: Low */
    string Low = "#LOW";
    /** en: Small */
    string Small = "#SMALL";
    /** en: Medium */
    string Medium = "#MEDIUM";
    /** en: Large */
    string Large = "#LARGE";
    /** en: Left */
    string Left = "#LEFT";
    /** en: Right */
    string Right = "#RIGHT";
    /** en: Up */
    string Up = "#UP";
    /** en: Down */
    string Down = "#DOWN";
    /** en: Front */
    string Front = "#FRONT";
    /** en: Back */
    string Back = "#BACK";
    /** en: Center */
    string Center = "#CENTER";
    /** en: Top */
    string Top = "#TOP";
    /** en: Bottom */
    string Bottom = "#BOTTOM";
    /** en: Top Left */
    string TopLeft = "#TOP_LEFT";
    /** en: Top Center */
    string TopCenter = "#TOP_CENTER";
    /** en: Top Right */
    string TopRight = "#TOP_RIGHT";
    /** en: Center Left */
    string CenterLeft = "#CENTER_LEFT";
    /** en: Center Right */
    string CenterRight = "#CENTER_RIGHT";
    /** en: Bottom Left */
    string BottomLeft = "#BOTTOM_LEFT";
    /** en: Bottom Center */
    string BottomCenter = "#BOTTOM_CENTER";
    /** en: Bottom Right */
    string BottomRight = "#BOTTOM_RIGHT";
    /** en: Clockwise */
    string Clockwise = "#CLOCKWISE";
    /** en: Counterclockwise */
    string Counterclockwise = "#COUNTERCLOCKWISE";
    /** en: Forward */
    string Forward = "#FORWARD";
    /** en: Backward */
    string Backward = "#BACKWARD";
    /** en: Default */
    string Default = "#DEFAULT";
    /** en: Neutral */
    string Neutral = "#NEUTRAL";
    /** en: Red */
    string Red = "#RED";
    /** en: Green */
    string Green = "#GREEN";
    /** en: Blue */
    string Blue = "#BLUE";
    /** en: Yellow */
    string Yellow = "#YELLOW";
    /** en: Purple */
    string Purple = "#PURPLE";
    /** en: Cyan */
    string Cyan = "#CYAN";
    /** en: Simple */
    string Simple = "#SIMPLE";
    /** en: Easy */
    string Easy = "#EASY";
    /** en: Normal */
    string Normal = "#NORMAL";
    /** en: Hard */
    string Hard = "#HARD";
    /** en: Expert */
    string Expert = "#EXPERT";
    /** en: Master */
    string Master = "#MASTER";
    /** en: Pro */
    string Pro = "#PRO";
    /** en: Technical */
    string Technical = "#TECHNICAL";
    /** en: Special */
    string Special = "#SPECIAL";
    /** en: Enter keywords... */
    string KeywordsPlaceholder = "#KEYWORDS_PLACEHOLDER";
    /** en: Enter name... */
    string NamePlaceholder = "#NAME_PLACEHOLDER";
    /** en: Enter rating... */
    string RatingPlaceholder = "#RATING_PLACEHOLDER";
    /** en: Enter minimum rating... */
    string RatingMinimumPlaceholder = "#RATING_MINIMUM_PLACEHOLDER";
    /** en: Enter maximum rating... */
    string RatingMaximumPlaceholder = "#RATING_MAXIMUM_PLACEHOLDER";
    /** en: Enter title... */
    string TitlePlaceholder = "#TITLE_PLACEHOLDER";
    /** en: Enter subtitle... */
    string SubtitlePlaceholder = "#SUBTITLE_PLACEHOLDER";
    /** en: Enter artists... */
    string ArtistsPlaceholder = "#ARTISTS_PLACEHOLDER";
    /** en: Enter time... */
    string TimePlaceholder = "#TIME_PLACEHOLDER";
    /** en: Enter author... */
    string AuthorPlaceholder = "#AUTHOR_PLACEHOLDER";
    /** en: Enter description... */
    string DescriptionPlaceholder = "#DESCRIPTION_PLACEHOLDER";
    /** en: Enter genre... */
    string GenrePlaceholder = "#GENRE_PLACEHOLDER";
    /** en: Enter type... */
    string TypePlaceholder = "#TYPE_PLACEHOLDER";
    /** en: Enter category... */
    string CategoryPlaceholder = "#CATEGORY_PLACEHOLDER";
    /** en: Enter language... */
    string LanguagePlaceholder = "#LANGUAGE_PLACEHOLDER";
    /** en: Enter difficulty... */
    string DifficultyPlaceholder = "#DIFFICULTY_PLACEHOLDER";
    /** en: Enter length... */
    string LengthPlaceholder = "#LENGTH_PLACEHOLDER";
    /** en: Enter minimum length... */
    string LengthMinimumPlaceholder = "#LENGTH_MINIMUM_PLACEHOLDER";
    /** en: Enter maximum length... */
    string LengthMaximumPlaceholder = "#LENGTH_MAXIMUM_PLACEHOLDER";
    /** en: Enter additional information... */
    string AdditionalInformationPlaceholder = "#ADDITIONAL_INFORMATION_PLACEHOLDER";
    /** en: Enter timezone... */
    string TimezonePlaceholder = "#TIMEZONE_PLACEHOLDER";
    /** en: Enter region... */
    string RegionPlaceholder = "#REGION_PLACEHOLDER";
    /** en: Enter content... */
    string ContentPlaceholder = "#CONTENT_PLACEHOLDER";
    /** en: Enter comment... */
    string CommentPlaceholder = "#COMMENT_PLACEHOLDER";
    /** en: Enter review... */
    string ReviewPlaceholder = "#REVIEW_PLACEHOLDER";
    /** en: Enter reply... */
    string ReplyPlaceholder = "#REPLY_PLACEHOLDER";
    /** en: Enter message... */
    string MessagePlaceholder = "#MESSAGE_PLACEHOLDER";
    /** en: Enter role... */
    string RolePlaceholder = "#ROLE_PLACEHOLDER";
    /** en: Enter permission... */
    string PermissionPlaceholder = "#PERMISSION_PLACEHOLDER";
    /** en: Enter user... */
    string UserPlaceholder = "#USER_PLACEHOLDER";
    /** en: {0}% */
    string PercentageUnit = "#PERCENTAGE_UNIT";
    /** en: {0} yr */
    string YearUnit = "#YEAR_UNIT";
    /** en: {0} mo */
    string MonthUnit = "#MONTH_UNIT";
    /** en: {0} d */
    string DayUnit = "#DAY_UNIT";
    /** en: {0} h */
    string HourUnit = "#HOUR_UNIT";
    /** en: {0} m */
    string MinuteUnit = "#MINUTE_UNIT";
    /** en: {0} s */
    string SecondUnit = "#SECOND_UNIT";
    /** en: {0} ms */
    string MillisecondUnit = "#MILLISECOND_UNIT";
    /** en: {0} yr ago */
    string YearPast = "#YEAR_PAST";
    /** en: {0} mo ago */
    string MonthPast = "#MONTH_PAST";
    /** en: {0} d ago */
    string DayPast = "#DAY_PAST";
    /** en: {0} h ago */
    string HourPast = "#HOUR_PAST";
    /** en: {0} m ago */
    string MinutePast = "#MINUTE_PAST";
    /** en: {0} s ago */
    string SecondPast = "#SECOND_PAST";
    /** en: {0} ms ago */
    string MillisecondPast = "#MILLISECOND_PAST";
    /** en: In {0} yr */
    string YearFuture = "#YEAR_FUTURE";
    /** en: In {0} mo */
    string MonthFuture = "#MONTH_FUTURE";
    /** en: In {0} d */
    string DayFuture = "#DAY_FUTURE";
    /** en: In {0} h */
    string HourFuture = "#HOUR_FUTURE";
    /** en: In {0} m */
    string MinuteFuture = "#MINUTE_FUTURE";
    /** en: In {0} s */
    string SecondFuture = "#SECOND_FUTURE";
    /** en: In {0} ms */
    string MillisecondFuture = "#MILLISECOND_FUTURE";
    /** en: Tap */
    string Tap = "#TAP";
    /** en: Tap and Hold */
    string TapHold = "#TAP_HOLD";
    /** en: Tap and Release */
    string TapRelease = "#TAP_RELEASE";
    /** en: Tap and Flick */
    string TapFlick = "#TAP_FLICK";
    /** en: Tap and Slide */
    string TapSlide = "#TAP_SLIDE";
    /** en: Hold */
    string Hold = "#HOLD";
    /** en: Hold and Slide */
    string HoldSlide = "#HOLD_SLIDE";
    /** en: Hold and Follow */
    string HoldFollow = "#HOLD_FOLLOW";
    /** en: Release */
    string Release = "#RELEASE";
    /** en: Flick */
    string Flick = "#FLICK";
    /** en: Slide */
    string Slide = "#SLIDE";
    /** en: Slide and Flick */
    string SlideFlick = "#SLIDE_FLICK";
    /** en: Avoid */
    string Avoid = "#AVOID";
    /** en: Jiggle */
    string Jiggle = "#JIGGLE";
    /** en: Newest */
    string Newest = "#NEWEST";
    /** en: Oldest */
    string Oldest = "#OLDEST";
    /** en: Recommended */
    string Recommended = "#RECOMMENDED";
    /** en: Popular */
    string Popular = "#POPULAR";
    /** en: Featured */
    string Featured = "#FEATURED";
    /** en: Competitive */
    string Competitive = "#COMPETITIVE";
    /** en: Holiday */
    string Holiday = "#HOLIDAY";
    /** en: Limited */
    string Limited = "#LIMITED";
    /** en: Announcement */
    string Announcement = "#ANNOUNCEMENT";
    /** en: Information */
    string Information = "#INFORMATION";
    /** en: Help */
    string Help = "#HELP";
    /** en: Maintenance */
    string Maintenance = "#MAINTENANCE";
    /** en: Event */
    string Event = "#EVENT";
    /** en: Update */
    string Update = "#UPDATE";
    /** en: Search */
    string Search = "#SEARCH";
    /** en: Advanced */
    string Advanced = "#ADVANCED";
    /** en: Related */
    string Related = "#RELATED";
    /** en: Same Author */
    string SameAuthor = "#SAME_AUTHOR";
    /** en: Same Artists */
    string SameArtists = "#SAME_ARTISTS";
    /** en: Same Rating */
    string SameRating = "#SAME_RATING";
    /** en: Same Category */
    string SameCategory = "#SAME_CATEGORY";
    /** en: Same Difficulty */
    string SameDifficulty = "#SAME_DIFFICULTY";
    /** en: Same Genre */
    string SameGenre = "#SAME_GENRE";
    /** en: Same Version */
    string SameVersion = "#SAME_VERSION";
    /** en: Other Authors */
    string OtherAuthors = "#OTHER_AUTHORS";
    /** en: Other Artists */
    string OtherArtists = "#OTHER_ARTISTS";
    /** en: Other Ratings */
    string OtherRatings = "#OTHER_RATINGS";
    /** en: Other Categories */
    string OtherCategories = "#OTHER_CATEGORIES";
    /** en: Other Difficulties */
    string OtherDifficulties = "#OTHER_DIFFICULTIES";
    /** en: Other Genres */
    string OtherGenres = "#OTHER_GENRES";
    /** en: Other Versions */
    string OtherVersions = "#OTHER_VERSIONS";
    /** en: Draft */
    string Draft = "#DRAFT";
    /** en: Public */
    string Public = "#PUBLIC";
    /** en: Private */
    string Private = "#PRIVATE";
    /** en: Pop */
    string Pop = "#POP";
    /** en: Rock */
    string Rock = "#ROCK";
    /** en: Hip Hop */
    string Hiphop = "#HIPHOP";
    /** en: Country */
    string Country = "#COUNTRY";
    /** en: Electronic */
    string Electronic = "#ELECTRONIC";
    /** en: Metal */
    string Metal = "#METAL";
    /** en: Classical */
    string Classical = "#CLASSICAL";
    /** en: Folk */
    string Folk = "#FOLK";
    /** en: Indie */
    string Indie = "#INDIE";
    /** en: Anime */
    string Anime = "#ANIME";
    /** en: Vocaloid */
    string Vocaloid = "#VOCALOID";
    /** en: Remix */
    string Remix = "#REMIX";
    /** en: Instrumental */
    string Instrumental = "#INSTRUMENTAL";
    /** en: Short Version */
    string ShortVersion = "#SHORT_VERSION";
    /** en: Long Version */
    string LongVersion = "#LONG_VERSION";
    /** en: Live Version */
    string LiveVersion = "#LIVE_VERSION";
    /** en: Report */
    string Report = "#REPORT";
    /** en: Reason */
    string Reason = "#REASON";
    /** en: Illegal Activities */
    string IllegalActivities = "#ILLEGAL_ACTIVITIES";
    /** en: Cheating */
    string Cheating = "#CHEATING";
    /** en: AFK */
    string Afk = "#AFK";
    /** en: Spamming */
    string Spamming = "#SPAMMING";
    /** en: Verbal Abuse */
    string VerbalAbuse = "#VERBAL_ABUSE";
    /** en: Inappropriate Language */
    string InappropriateLanguage = "#INAPPROPRIATE_LANGUAGE";
    /** en: Negative Attitude */
    string NegativeAttitude = "#NEGATIVE_ATTITUDE";
    /** en: DNF */
    string Dnf = "#DNF";
    /** en: Suggestions */
    string Suggestions = "#SUGGESTIONS";
    /** en: Suggestions per Player */
    string SuggestionsPerPlayer = "#SUGGESTIONS_PER_PLAYER";
    /** en: Match Scoring */
    string MatchScoring = "#MATCH_SCORING";
    /** en: Match Tiebreaker */
    string MatchTiebreaker = "#MATCH_TIEBREAKER";
    /** en: Match Count */
    string MatchCount = "#MATCH_COUNT";
    /** en: Match Limit */
    string MatchLimit = "#MATCH_LIMIT";
    /** en: Round Scoring */
    string RoundScoring = "#ROUND_SCORING";
    /** en: Round Tiebreaker */
    string RoundTiebreaker = "#ROUND_TIEBREAKER";
    /** en: Round Count */
    string RoundCount = "#ROUND_COUNT";
    /** en: Round Limit */
    string RoundLimit = "#ROUND_LIMIT";
    /** en: Team Scoring */
    string TeamScoring = "#TEAM_SCORING";
    /** en: Team Tiebreaker */
    string TeamTiebreaker = "#TEAM_TIEBREAKER";
    /** en: Team Count */
    string TeamCount = "#TEAM_COUNT";
    /** en: Team Limit */
    string TeamLimit = "#TEAM_LIMIT";
    /** en: Qualified */
    string Qualified = "#QUALIFIED";
    /** en: Disqualified */
    string Disqualified = "#DISQUALIFIED";
    /** en: Ranking */
    string Ranking = "#RANKING";
    /** en: Score */
    string Score = "#SCORE";
    /** en: Owner */
    string Owner = "#OWNER";
    /** en: Admin */
    string Admin = "#ADMIN";
    /** en: Moderator */
    string Moderator = "#MODERATOR";
    /** en: Reviewer */
    string Reviewer = "#REVIEWER";
    /** en: Banned */
    string Banned = "#BANNED";
    /** en: Player */
    string Player = "#PLAYER";
    /** en: Spectator */
    string Spectator = "#SPECTATOR";
    /** en: Referee */
    string Referee = "#REFEREE";
    /** en: Eliminated */
    string Eliminated = "#ELIMINATED";
    /** en: Finalist */
    string Finalist = "#FINALIST";
    /** en: Finished */
    string Finished = "#FINISHED";
    /** en: Winner */
    string Winner = "#WINNER";
    /** en: Gold Medal */
    string GoldMedal = "#GOLD_MEDAL";
    /** en: Silver Medal */
    string SilverMedal = "#SILVER_MEDAL";
    /** en: Bronze Medal */
    string BronzeMedal = "#BRONZE_MEDAL";
    /** en: Team 1 */
    string Team1 = "#TEAM_1";
    /** en: Team 2 */
    string Team2 = "#TEAM_2";
    /** en: Team 3 */
    string Team3 = "#TEAM_3";
    /** en: Team 4 */
    string Team4 = "#TEAM_4";
    /** en: Team 5 */
    string Team5 = "#TEAM_5";
    /** en: Team 6 */
    string Team6 = "#TEAM_6";
    /** en: Team 7 */
    string Team7 = "#TEAM_7";
    /** en: Team 8 */
    string Team8 = "#TEAM_8";
    /** en: Team Red */
    string TeamRed = "#TEAM_RED";
    /** en: Team Green */
    string TeamGreen = "#TEAM_GREEN";
    /** en: Team Blue */
    string TeamBlue = "#TEAM_BLUE";
    /** en: Team Yellow */
    string TeamYellow = "#TEAM_YELLOW";
    /** en: Team Purple */
    string TeamPurple = "#TEAM_PURPLE";
    /** en: Team Cyan */
    string TeamCyan = "#TEAM_CYAN";
    /** en: Team White */
    string TeamWhite = "#TEAM_WHITE";
    /** en: Team Black */
    string TeamBlack = "#TEAM_BLACK";
    /** en: Reply */
    string Reply = "#REPLY";
    /** en: Replied */
    string Replied = "#REPLIED";
    /** en: Review */
    string Review = "#REVIEW";
    /** en: Reviewing */
    string Reviewing = "#REVIEWING";
    /** en: Reviewed */
    string Reviewed = "#REVIEWED";
    /** en: Verify */
    string Verify = "#VERIFY";
    /** en: Verifying */
    string Verifying = "#VERIFYING";
    /** en: Verified */
    string Verified = "#VERIFIED";
    /** en: Upload */
    string Upload = "#UPLOAD";
    /** en: Uploading */
    string Uploading = "#UPLOADING";
    /** en: Uploaded */
    string Uploaded = "#UPLOADED";
    /** en: Submit */
    string Submit = "#SUBMIT";
    /** en: Submitting */
    string Submitting = "#SUBMITTING";
    /** en: Submitted */
    string Submitted = "#SUBMITTED";
    /** en: Edit */
    string Edit = "#EDIT";
    /** en: Editing */
    string Editing = "#EDITING";
    /** en: Edited */
    string Edited = "#EDITED";
    /** en: Like */
    string Like = "#LIKE";
    /** en: Liked */
    string Liked = "#LIKED";
    /** en: Dislike */
    string Dislike = "#DISLIKE";
    /** en: Disliked */
    string Disliked = "#DISLIKED";
    /** en: Bookmark */
    string Bookmark = "#BOOKMARK";
    /** en: Bookmarked */
    string Bookmarked = "#BOOKMARKED";
    /** en: Delete */
    string Delete = "#DELETE";
    /** en: Deleting */
    string Deleting = "#DELETING";
    /** en: Deleted */
    string Deleted = "#DELETED";
    /** en: Remove */
    string Remove = "#REMOVE";
    /** en: Removing */
    string Removing = "#REMOVING";
    /** en: Removed */
    string Removed = "#REMOVED";
    /** en: Restore */
    string Restore = "#RESTORE";
    /** en: Restoring */
    string Restoring = "#RESTORING";
    /** en: Restored */
    string Restored = "#RESTORED";
    /** en: Confirm */
    string Confirm = "#CONFIRM";
    /** en: Confirmed */
    string Confirmed = "#CONFIRMED";
    /** en: Cancel */
    string Cancel = "#CANCEL";
    /** en: Canceled */
    string Canceled = "#CANCELED";
    /** en: Increase */
    string Increase = "#INCREASE";
    /** en: Decrease */
    string Decrease = "#DECREASE";
    /** en: Upvote */
    string Upvote = "#UPVOTE";
    /** en: Upvoted */
    string Upvoted = "#UPVOTED";
    /** en: Downvote */
    string Downvote = "#DOWNVOTE";
    /** en: Downvoted */
    string Downvoted = "#DOWNVOTED";
    /** en: Agree */
    string Agree = "#AGREE";
    /** en: Agreed */
    string Agreed = "#AGREED";
    /** en: Disagree */
    string Disagree = "#DISAGREE";
    /** en: Disagreed */
    string Disagreed = "#DISAGREED";
    /** en: Lock */
    string Lock = "#LOCK";
    /** en: Locked */
    string Locked = "#LOCKED";
    /** en: Unlock */
    string Unlock = "#UNLOCK";
    /** en: Unlocked */
    string Unlocked = "#UNLOCKED";
    /** en: Pin */
    string Pin = "#PIN";
    /** en: Pinned */
    string Pinned = "#PINNED";
    /** en: Unpin */
    string Unpin = "#UNPIN";
    /** en: Unpinned */
    string Unpinned = "#UNPINNED";
    /** en: Follow */
    string Follow = "#FOLLOW";
    /** en: Following */
    string Following = "#FOLLOWING";
    /** en: Followed */
    string Followed = "#FOLLOWED";
    /** en: Unfollow */
    string Unfollow = "#UNFOLLOW";
    /** en: Subscribe */
    string Subscribe = "#SUBSCRIBE";
    /** en: Subscribing */
    string Subscribing = "#SUBSCRIBING";
    /** en: Subscribed */
    string Subscribed = "#SUBSCRIBED";
    /** en: Unsubscribe */
    string Unsubscribe = "#UNSUBSCRIBE";
    /** en: Publish */
    string Publish = "#PUBLISH";
    /** en: Publishing */
    string Publishing = "#PUBLISHING";
    /** en: Published */
    string Published = "#PUBLISHED";
    /** en: Unpublish */
    string Unpublish = "#UNPUBLISH";
    /** en: Show */
    string Show = "#SHOW";
    /** en: Hide */
    string Hide = "#HIDE";
    /** en: Allow */
    string Allow = "#ALLOW";
    /** en: Allowed */
    string Allowed = "#ALLOWED";
    /** en: Disallow */
    string Disallow = "#DISALLOW";
    /** en: Disallowed */
    string Disallowed = "#DISALLOWED";
    /** en: Approve */
    string Approve = "#APPROVE";
    /** en: Approved */
    string Approved = "#APPROVED";
    /** en: Deny */
    string Deny = "#DENY";
    /** en: Denied */
    string Denied = "#DENIED";
    /** en: Accept */
    string Accept = "#ACCEPT";
    /** en: Accepted */
    string Accepted = "#ACCEPTED";
    /** en: Reject */
    string Reject = "#REJECT";
    /** en: Rejected */
    string Rejected = "#REJECTED";
    /** en: Star */
    string Star = "#STAR";
    /** en: Starred */
    string Starred = "#STARRED";
}Text;

class Icon {
    public:

    string Advanced = "advanced";
    string AngleDown = "angleDown";
    string AngleLeft = "angleLeft";
    string AngleRight = "angleRight";
    string AnglesDown = "anglesDown";
    string AnglesLeft = "anglesLeft";
    string AnglesRight = "anglesRight";
    string AnglesUp = "anglesUp";
    string AngleUp = "angleUp";
    string ArrowDown = "arrowDown";
    string ArrowLeft = "arrowLeft";
    string ArrowRight = "arrowRight";
    string ArrowUp = "arrowUp";
    string Award = "award";
    string Background = "background";
    string Bell = "bell";
    string BellSlash = "bellSlash";
    string Bookmark = "bookmark";
    string BookmarkHollow = "bookmarkHollow";
    string Check = "check";
    string Clock = "clock";
    string Comment = "comment";
    string Crown = "crown";
    string CustomServer = "customServer";
    string Delete = "delete";
    string Edit = "edit";
    string Effect = "effect";
    string Engine = "engine";
    string Envelope = "envelope";
    string EnvelopeOpen = "envelopeOpen";
    string Globe = "globe";
    string Heart = "heart";
    string HeartHollow = "heartHollow";
    string Hide = "hide";
    string Information = "information";
    string Level = "level";
    string Lock = "lock";
    string Medal = "medal";
    string Message = "message";
    string Minus = "minus";
    string Options = "options";
    string Particle = "particle";
    string Pin = "pin";
    string Player = "player";
    string Playlist = "playlist";
    string Plus = "plus";
    string Post = "post";
    string Question = "question";
    string Ranking = "ranking";
    string Replay = "replay";
    string Reply = "reply";
    string Restore = "restore";
    string Room = "room";
    string Search = "search";
    string Settings = "settings";
    string Show = "show";
    string Shuffle = "shuffle";
    string Skin = "skin";
    string Star = "star";
    string StarHalf = "starHalf";
    string StarHollow = "starHollow";
    string Stopwatch = "stopwatch";
    string Tag = "tag";
    string ThumbsDown = "thumbsDown";
    string ThumbsDownHollow = "thumbsDownHollow";
    string ThumbsUp = "thumbsUp";
    string ThumbsUpHollow = "thumbsUpHollow";
    string Trophy = "trophy";
    string Unlock = "unlock";
    string XMark = "xMark";
}Icon;

class EngineArchetypeDataName {
    public:

    string Beat = "#BEAT";
    string Bpm = "#BPM";
    string TimeScale = "#TIMESCALE";
    string Judgment = "#JUDGMENT";
    string Accuracy = "#ACCURACY";
}EngineArchetypeDataName;

class EngineArchetypeName {
    public:

    string BpmChange = "#BPM_CHANGE";
    string TimeScaleChange = "#TIMESCALE_CHANGE";
}EngineArchetypeName;

class EngineConfigurationMetric {
    public:

    string Arcade = "arcade";
    string Accuracy = "accuracy";
    string Life = "life";
    string PerfectRate = "perfectRate";
    string ErrorHeatMap = "errorHeatMap";
}EngineConfigurationMetric;
set<string> EngineConfigurationMetricMembers = {
    EngineConfigurationMetric.Arcade,
    EngineConfigurationMetric.Accuracy,
    EngineConfigurationMetric.Life,
    EngineConfigurationMetric.PerfectRate,
    EngineConfigurationMetric.ErrorHeatMap
};

class EngineConfigurationAnimationTweenEase {
    public:

    string Linear = "linear";
    string InSine = "inSine";
    string InQuad = "inQuad";
    string InCubic = "inCubic";
    string InQuart = "inQuart";
    string InQuint = "inQuint";
    string InExpo = "inExpo";
    string InCirc = "inCirc";
    string InBack = "inBack";
    string InElastic = "inElastic";
    string OutSine = "outSine";
    string OutQuad = "outQuad";
    string OutCubic = "outCubic";
    string OutQuart = "outQuart";
    string OutQuint = "outQuint";
    string OutExpo = "outExpo";
    string OutCirc = "outCirc";
    string OutBack = "outBack";
    string OutElastic = "outElastic";
    string InOutSine = "inOutSine";
    string InOutQuad = "inOutQuad";
    string InOutCubic = "inOutCubic";
    string InOutQuart = "inOutQuart";
    string InOutQuint = "inOutQuint";
    string InOutExpo = "inOutExpo";
    string InOutCirc = "inOutCirc";
    string InOutBack = "inOutBack";
    string InOutElastic = "inOutElastic";
    string OutInSine = "outInSine";
    string OutInQuad = "outInQuad";
    string OutInCubic = "outInCubic";
    string OutInQuart = "outInQuart";
    string OutInQuint = "outInQuint";
    string OutInExpo = "outInExpo";
    string OutInCirc = "outInCirc";
    string OutInBack = "outInBack";
    string OutInElastic = "outInElastic";
    string None = "none";
}EngineConfigurationAnimationTweenEase;
set<string> EngineConfigurationAnimationTweenEaseMembers = {
    EngineConfigurationAnimationTweenEase.Linear,
    EngineConfigurationAnimationTweenEase.InSine,
    EngineConfigurationAnimationTweenEase.InQuad,
    EngineConfigurationAnimationTweenEase.InCubic,
    EngineConfigurationAnimationTweenEase.InQuart,
    EngineConfigurationAnimationTweenEase.InQuint,
    EngineConfigurationAnimationTweenEase.InExpo,
    EngineConfigurationAnimationTweenEase.InCirc,
    EngineConfigurationAnimationTweenEase.InBack,
    EngineConfigurationAnimationTweenEase.InElastic,
    EngineConfigurationAnimationTweenEase.OutSine,
    EngineConfigurationAnimationTweenEase.OutQuad,
    EngineConfigurationAnimationTweenEase.OutCubic,
    EngineConfigurationAnimationTweenEase.OutQuart,
    EngineConfigurationAnimationTweenEase.OutQuint,
    EngineConfigurationAnimationTweenEase.OutExpo,
    EngineConfigurationAnimationTweenEase.OutCirc,
    EngineConfigurationAnimationTweenEase.OutBack,
    EngineConfigurationAnimationTweenEase.OutElastic,
    EngineConfigurationAnimationTweenEase.InOutSine,
    EngineConfigurationAnimationTweenEase.InOutQuad,
    EngineConfigurationAnimationTweenEase.InOutCubic,
    EngineConfigurationAnimationTweenEase.InOutQuart,
    EngineConfigurationAnimationTweenEase.InOutQuint,
    EngineConfigurationAnimationTweenEase.InOutExpo,
    EngineConfigurationAnimationTweenEase.InOutCirc,
    EngineConfigurationAnimationTweenEase.InOutBack,
    EngineConfigurationAnimationTweenEase.InOutElastic,
    EngineConfigurationAnimationTweenEase.OutInSine,
    EngineConfigurationAnimationTweenEase.OutInQuad,
    EngineConfigurationAnimationTweenEase.OutInCubic,
    EngineConfigurationAnimationTweenEase.OutInQuart,
    EngineConfigurationAnimationTweenEase.OutInQuint,
    EngineConfigurationAnimationTweenEase.OutInExpo,
    EngineConfigurationAnimationTweenEase.OutInCirc,
    EngineConfigurationAnimationTweenEase.OutInBack,
    EngineConfigurationAnimationTweenEase.OutInElastic,
    EngineConfigurationAnimationTweenEase.None
};

class EngineConfigurationJudgmentErrorStyle {
    public:

    string None = "none";
    string Late = "late";
    string Early = "early";
    string Plus = "plus";
    string Minus = "minus";
    string ArrowUp = "arrowUp";
    string ArrowDown = "arrowDown";
    string ArrowLeft = "arrowLeft";
    string ArrowRight = "arrowRight";
    string TriangleUp = "triangleUp";
    string TriangleDown = "triangleDown";
    string TriangleLeft = "triangleLeft";
    string TriangleRight = "triangleRight";
}EngineConfigurationJudgmentErrorStyle;
set<string> EngineConfigurationJudgmentErrorStyleMembers = {
    EngineConfigurationJudgmentErrorStyle.None,
    EngineConfigurationJudgmentErrorStyle.Late,
    EngineConfigurationJudgmentErrorStyle.Early,
    EngineConfigurationJudgmentErrorStyle.Plus,
    EngineConfigurationJudgmentErrorStyle.Minus,
    EngineConfigurationJudgmentErrorStyle.ArrowUp,
    EngineConfigurationJudgmentErrorStyle.ArrowDown,
    EngineConfigurationJudgmentErrorStyle.ArrowLeft,
    EngineConfigurationJudgmentErrorStyle.ArrowRight,
    EngineConfigurationJudgmentErrorStyle.TriangleUp,
    EngineConfigurationJudgmentErrorStyle.TriangleDown,
    EngineConfigurationJudgmentErrorStyle.TriangleLeft,
    EngineConfigurationJudgmentErrorStyle.TriangleRight
};

class EngineConfigurationJudgmentErrorPlacement {
    public:

    string Left = "left";
    string Right = "right";
    string LeftRight = "leftRight";
    string Top = "top";
    string Bottom = "bottom";
    string TopBottom = "topBottom";
    string Center = "center";
}EngineConfigurationJudgmentErrorPlacement;
set<string> EngineConfigurationJudgmentErrorPlacementMembers = {
    EngineConfigurationJudgmentErrorPlacement.Left,
    EngineConfigurationJudgmentErrorPlacement.Right,
    EngineConfigurationJudgmentErrorPlacement.LeftRight,
    EngineConfigurationJudgmentErrorPlacement.Top,
    EngineConfigurationJudgmentErrorPlacement.Bottom,
    EngineConfigurationJudgmentErrorPlacement.TopBottom,
    EngineConfigurationJudgmentErrorPlacement.Center
};

class HorizontalAlign{
    public:

    int Left = -1;
    int Center = 0;
    int Right = 1;
}HorizontalAlign;

class InstructionIconName {
    public:

    string Hand = "#HAND";
    string Arrow = "#ARROW";
}InstructionIconName;

class CanvasScroll {
    public:

    int LeftToRight = 0;
    int TopToBottom = 1;
    int RightToLeft = 2;
    int BottomToTop = 3;
}CanvasScroll;

class PrintFormat {
    public:

    int Number = 0;
    int Percentage = 1;

    int Time = 10;
    int Score = 11;

    int BPM = 20;
    int TimeScale = 21;

    int BeatCount = 30;
    int MeasureCount = 31;
    int EntityCount = 32;
}PrintFormat;

class PrintColor {
    public:

    int Theme = -1;

    int Neutral = 0;
    int Red = 1;
    int Green = 2;
    int Blue = 3;
    int Yellow = 4;
    int Purple = 5;
    int Cyan = 6;
}PrintColor;

#endif
/*** End of inlined file: EngineLiterals.h ***/



/*** Start of inlined file: EngineDataBucket.h ***/
#ifndef EngineDataBucket_H
#define EngineDataBucket_H

class EngineDataBucketSprite {
    public:

    int id = 0;
    int fallbackId = 0;
    double x = 0;
    double y = 0;
    double w = 0;
    double h = 0;
    double rotation = 0;

    Constructor EngineDataBucketSprite(){}
    Constructor EngineDataBucketSprite(
        int id,
        int fallbackId,
        double x, double y,
        double w, double h,
        double rotation
    ): id(id), fallbackId(fallbackId), x(x), y(y), w(w), h(h), rotation(rotation){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["id"] = id;
        if (fallbackId != 0) res["fallbackId"] = fallbackId;
        res["x"] = x;
        res["y"] = y;
        res["w"] = w;
        res["h"] = h;
        res["rotation"] = rotation;
        return res;
    }
};

class EngineDataBucket {
    public:

    vector<EngineDataBucketSprite> sprites;
    string unit = "";

    Json::Value toJsonObject() {
        Json::Value res;
        res["sprites"].resize(0);
        for (int i = 0; i < sprites.size(); i++) res["sprites"].append(sprites[i].toJsonObject());
        if (unit != "") res["unit"] = unit;
        return res;
    }
};

class Bucket {
    public:

    int id;

    operator int() {
        return id;
    }

    operator FuncNode() {
        return id;
    }

    operator Variable() {
        return id;
    }
};

vector<EngineDataBucket> data_buckets;
Bucket defineBucket(vector<EngineDataBucketSprite> sprites, string unit) {
    EngineDataBucket bucket;
    bucket.unit = unit;
    bucket.sprites = sprites;
    data_buckets.push_back(bucket);
    return { int(data_buckets.size()) - 1 };
}

#endif
/*** End of inlined file: EngineDataBucket.h ***/


/*** Start of inlined file: EngineInstruction.h ***/
#ifndef EngineInstruction_H
#define Engineinstruction_H

class InstructionText {
    public:

    int id;
    string name;

    operator int() {
        return id;
    }

    operator FuncNode() {
        return id;
    }

    operator Variable() {
        return id;
    }
};

vector<string> instruction_texts;
InstructionText defineInstructionText(string name) {
    instruction_texts.push_back(name);
    return { int(instruction_texts.size()) - 1 };
}

class InstructionIcon {
    public:

    int id;
    string name;

    operator int() {
        return id;
    }

    operator FuncNode() {
        return id;
    }

    operator Variable() {
        return id;
    }
};

vector<string> instruction_icons;
InstructionIcon defineInstructionIcon(string name) {
    instruction_icons.push_back(name);
    return { int(instruction_icons.size()) - 1 };
}

#endif
/*** End of inlined file: EngineInstruction.h ***/

vector<pair<string, string> > skin_sprites;
vector<pair<string, string> > effect_clips;
vector<pair<string, string> > particle_effects;
#ifdef play

/*** Start of inlined file: Blocks.h ***/
#ifndef PlayBlocks_H
#define PlayBlocks_H

const int RuntimeEnvironmentId = 1000;
const int RuntimeUpdateId = 1001;
const int RuntimeTouchArrayId = 1002;
const int RuntimeSkinTransformId = 1003;
const int RuntimeParticleTransformId = 1004;
const int RuntimeBackgroundId = 1005;
const int RuntimeUIId = 1006;
const int RuntimeUIConfigurationId = 1007;
const int LevelMemoryId = 2000;
const int LevelDataId = 2001;
const int LevelOptionId = 2002;
const int LevelBucketId = 2003;
const int LevelScoreId = 2004;
const int LevelLifeId = 2005;
const int EngineRomId = 3000;
const int EntityMemoryId = 4000;
const int EntityDataId = 4001;
const int EntitySharedMemoryId = 4002;
const int EntityInfoId = 4003;
const int EntityDespawnId = 4004;
const int EntityInputId = 4005;
const int EntityDataArrayId = 4101;
const int EntitySharedMemoryArrayId = 4102;
const int EntityInfoArrayId = 4103;
const int ArchetypeLifeId = 5000;
const int TemporaryMemoryId = 10000;

bool unused_readable_unused = [](){
	readable = {
		{ RuntimeEnvironmentId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeUpdateId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeTouchArrayId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeSkinTransformId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeParticleTransformId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeBackgroundId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeUIId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ RuntimeUIConfigurationId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ LevelMemoryId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ LevelDataId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ LevelOptionId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ LevelBucketId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ LevelScoreId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ LevelLifeId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EngineRomId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityMemoryId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityDataId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntitySharedMemoryId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityInfoId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityDespawnId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityInputId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityDataArrayId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntitySharedMemoryArrayId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityInfoArrayId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ ArchetypeLifeId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ TemporaryMemoryId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } }
	};
	return true;
}();

bool unused_writable_unused = [](){
	writable = {
		{ RuntimeEnvironmentId, { "preprocess" } },
		{ RuntimeUpdateId, {  } },
		{ RuntimeTouchArrayId, {  } },
		{ RuntimeSkinTransformId, { "preprocess", "updateSequential", "touch" } },
		{ RuntimeParticleTransformId, { "preprocess", "updateSequential", "touch" } },
		{ RuntimeBackgroundId, { "preprocess", "updateSequential", "touch" } },
		{ RuntimeUIId, { "preprocess" } },
		{ RuntimeUIConfigurationId, { "preprocess" } },
		{ LevelMemoryId, { "preprocess", "updateSequential", "touch" } },
		{ LevelDataId, { "preprocess" } },
		{ LevelOptionId, {  } },
		{ LevelBucketId, { "preprocess" } },
		{ LevelScoreId, { "preprocess" } },
		{ LevelLifeId, { "preprocess" } },
		{ EngineRomId, {  } },
		{ EntityMemoryId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityDataId, { "preprocess" } },
		{ EntitySharedMemoryId, { "preprocess", "updateSequential", "touch" } },
		{ EntityInfoId, {  } },
		{ EntityDespawnId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityInputId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } },
		{ EntityDataArrayId, { "preprocess" } },
		{ EntitySharedMemoryArrayId, { "preprocess", "updateSequential", "touch" } },
		{ EntityInfoArrayId, {  } },
		{ ArchetypeLifeId, { "preprocess" } },
		{ TemporaryMemoryId, { "preprocess", "spawnOrder", "shouldSpawn", "initialize", "updateSequential", "touch", "updateParallel", "terminate" } }
	};
	return true;
}();

bool unused_blockName_unused = [](){
	blockName = {
		{ RuntimeEnvironmentId, "RuntimeEnvironment" },
		{ RuntimeUpdateId, "RuntimeUpdate" },
		{ RuntimeTouchArrayId, "RuntimeTouchArray" },
		{ RuntimeSkinTransformId, "RuntimeSkinTransform" },
		{ RuntimeParticleTransformId, "RuntimeParticleTransform" },
		{ RuntimeBackgroundId, "RuntimeBackground" },
		{ RuntimeUIId, "RuntimeUI" },
		{ RuntimeUIConfigurationId, "RuntimeUIConfiguration" },
		{ LevelMemoryId, "LevelMemory" },
		{ LevelDataId, "LevelData" },
		{ LevelOptionId, "LevelOption" },
		{ LevelBucketId, "LevelBucket" },
		{ LevelScoreId, "LevelScore" },
		{ LevelLifeId, "LevelLife" },
		{ EngineRomId, "EngineRom" },
		{ EntityMemoryId, "EntityMemory" },
		{ EntityDataId, "EntityData" },
		{ EntitySharedMemoryId, "EntitySharedMemory" },
		{ EntityInfoId, "EntityInfo" },
		{ EntityDespawnId, "EntityDespawn" },
		{ EntityInputId, "EntityInput" },
		{ EntityDataArrayId, "EntityDataArray" },
		{ EntitySharedMemoryArrayId, "EntitySharedMemoryArray" },
		{ EntityInfoArrayId, "EntityInfoArray" },
		{ ArchetypeLifeId, "ArchetypeLife" },
		{ TemporaryMemoryId, "TemporaryMemory" }
	};
	return true;
}();

bool unused_writeBlockInfo_unused = [](){
	SonolusMemorySize[LevelMemoryId] = 4096;
	SonolusMemoryDelta[LevelMemoryId] = 1;
	allowAllocate.insert(LevelMemoryId);
	SonolusMemorySize[LevelDataId] = 4096;
	SonolusMemoryDelta[LevelDataId] = 1;
	allowAllocate.insert(LevelDataId);
	SonolusMemorySize[EntityMemoryId] = 64;
	SonolusMemoryDelta[EntityMemoryId] = 1;
	allowAllocate.insert(EntityMemoryId);
	SonolusMemorySize[EntityDataId] = 32;
	SonolusMemoryDelta[EntityDataId] = 1;
	allowAllocate.insert(EntityDataId);
	SonolusMemorySize[EntitySharedMemoryId] = 32;
	SonolusMemoryDelta[EntitySharedMemoryId] = 1;
	allowAllocate.insert(EntitySharedMemoryId);
	SonolusMemorySize[TemporaryMemoryId] = 4096;
	SonolusMemoryDelta[TemporaryMemoryId] = 1;
	allowAllocate.insert(TemporaryMemoryId);
	return true;
}();

struct RuntimeEnvironmentGroup{
	Variable debugMode = Variable(RuntimeEnvironmentId, 0);
	Variable screenAspectRatio = Variable(RuntimeEnvironmentId, 0);
	Variable audioOffset = Variable(RuntimeEnvironmentId, 0);
	Variable inputOffset = Variable(RuntimeEnvironmentId, 0);
	Variable multiplayer = Variable(RuntimeEnvironmentId, 0);

	Constructor RuntimeEnvironmentGroup(FuncNode offset) {
		debugMode.reallocate(offset + 0);
		screenAspectRatio.reallocate(offset + 1);
		audioOffset.reallocate(offset + 2);
		inputOffset.reallocate(offset + 3);
		multiplayer.reallocate(offset + 4);
	}
};
RuntimeEnvironmentGroup RuntimeEnvironment = RuntimeEnvironmentGroup(0);

struct RuntimeUpdateGroup{
	Variable time = Variable(RuntimeUpdateId, 0);
	Variable deltaTime = Variable(RuntimeUpdateId, 0);
	Variable scaledTime = Variable(RuntimeUpdateId, 0);
	Variable touchCount = Variable(RuntimeUpdateId, 0);

	Constructor RuntimeUpdateGroup(FuncNode offset) {
		time.reallocate(offset + 0);
		deltaTime.reallocate(offset + 1);
		scaledTime.reallocate(offset + 2);
		touchCount.reallocate(offset + 3);
	}
};
RuntimeUpdateGroup RuntimeUpdate = RuntimeUpdateGroup(0);

struct RuntimeTouchArrayGroup{
	Variable id = Variable(RuntimeTouchArrayId, 0);
	Variable started = Variable(RuntimeTouchArrayId, 0);
	Variable ended = Variable(RuntimeTouchArrayId, 0);
	Variable t = Variable(RuntimeTouchArrayId, 0);
	Variable st = Variable(RuntimeTouchArrayId, 0);
	Variable x = Variable(RuntimeTouchArrayId, 0);
	Variable y = Variable(RuntimeTouchArrayId, 0);
	Variable sx = Variable(RuntimeTouchArrayId, 0);
	Variable sy = Variable(RuntimeTouchArrayId, 0);
	Variable dx = Variable(RuntimeTouchArrayId, 0);
	Variable dy = Variable(RuntimeTouchArrayId, 0);
	Variable vx = Variable(RuntimeTouchArrayId, 0);
	Variable vy = Variable(RuntimeTouchArrayId, 0);
	Variable vr = Variable(RuntimeTouchArrayId, 0);
	Variable vw = Variable(RuntimeTouchArrayId, 0);

	Constructor RuntimeTouchArrayGroup(FuncNode offset) {
		id.reallocate(offset + 0);
		started.reallocate(offset + 1);
		ended.reallocate(offset + 2);
		t.reallocate(offset + 3);
		st.reallocate(offset + 4);
		x.reallocate(offset + 5);
		y.reallocate(offset + 6);
		sx.reallocate(offset + 7);
		sy.reallocate(offset + 8);
		dx.reallocate(offset + 9);
		dy.reallocate(offset + 10);
		vx.reallocate(offset + 11);
		vy.reallocate(offset + 12);
		vr.reallocate(offset + 13);
		vw.reallocate(offset + 14);
	}
};
BlocksArray<RuntimeTouchArrayGroup> RuntimeTouchArray = BlocksArray<RuntimeTouchArrayGroup>(15);

VariablesArray<RuntimeSkinTransformId> RuntimeSkinTransform = VariablesArray<RuntimeSkinTransformId>(0, 1 << 16);

VariablesArray<RuntimeParticleTransformId> RuntimeParticleTransform = VariablesArray<RuntimeParticleTransformId>(0, 1 << 16);

struct RuntimeBackgroundGroup{
	Variable x = Variable(RuntimeBackgroundId, 0);
	Variable y = Variable(RuntimeBackgroundId, 0);

	Constructor RuntimeBackgroundGroup(FuncNode offset) {
		x.reallocate(offset + 0);
		y.reallocate(offset + 1);
	}
};
BlocksArray<RuntimeBackgroundGroup> RuntimeBackground = BlocksArray<RuntimeBackgroundGroup>(2);

struct RuntimeUIGroup{
	struct Group_MYQveg5DpKye902Z{
		Variable anchorX = Variable(RuntimeUIId, 0);
		Variable anchorY = Variable(RuntimeUIId, 0);
		Variable pivotX = Variable(RuntimeUIId, 0);
		Variable pivotY = Variable(RuntimeUIId, 0);
		Variable width = Variable(RuntimeUIId, 0);
		Variable height = Variable(RuntimeUIId, 0);
		Variable rotation = Variable(RuntimeUIId, 0);
		Variable alpha = Variable(RuntimeUIId, 0);
		Variable horizontalAlign = Variable(RuntimeUIId, 0);
		Variable background = Variable(RuntimeUIId, 0);

		void reallocate(FuncNode offset) {
			anchorX.reallocate(offset + 0);
			anchorY.reallocate(offset + 1);
			pivotX.reallocate(offset + 2);
			pivotY.reallocate(offset + 3);
			width.reallocate(offset + 4);
			height.reallocate(offset + 5);
			rotation.reallocate(offset + 6);
			alpha.reallocate(offset + 7);
			horizontalAlign.reallocate(offset + 8);
			background.reallocate(offset + 9);
		}
	};
	Group_MYQveg5DpKye902Z menu;
	Group_MYQveg5DpKye902Z judgment;
	Group_MYQveg5DpKye902Z comboValue;
	Group_MYQveg5DpKye902Z comboText;
	Group_MYQveg5DpKye902Z primaryMetricBar;
	Group_MYQveg5DpKye902Z primaryMetricValue;
	Group_MYQveg5DpKye902Z secondaryMetricBar;
	Group_MYQveg5DpKye902Z secondaryMetricValue;

	Constructor RuntimeUIGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		judgment.reallocate(offset + 10);
		comboValue.reallocate(offset + 20);
		comboText.reallocate(offset + 30);
		primaryMetricBar.reallocate(offset + 40);
		primaryMetricValue.reallocate(offset + 50);
		secondaryMetricBar.reallocate(offset + 60);
		secondaryMetricValue.reallocate(offset + 70);
	}
};
RuntimeUIGroup RuntimeUI = RuntimeUIGroup(0);

struct RuntimeUIConfigurationGroup{
	struct Group_s7jSpZv7FXeEyerI{
		Variable scale = Variable(RuntimeUIConfigurationId, 0);
		Variable alpha = Variable(RuntimeUIConfigurationId, 0);

		void reallocate(FuncNode offset) {
			scale.reallocate(offset + 0);
			alpha.reallocate(offset + 1);
		}
	};
	Group_s7jSpZv7FXeEyerI menu;
	Group_s7jSpZv7FXeEyerI judgment;
	Group_s7jSpZv7FXeEyerI combo;
	Group_s7jSpZv7FXeEyerI primaryMetric;
	Group_s7jSpZv7FXeEyerI secondaryMetric;

	Constructor RuntimeUIConfigurationGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		judgment.reallocate(offset + 2);
		combo.reallocate(offset + 4);
		primaryMetric.reallocate(offset + 6);
		secondaryMetric.reallocate(offset + 8);
	}
};
RuntimeUIConfigurationGroup RuntimeUIConfiguration = RuntimeUIConfigurationGroup(0);

VariablesArray<LevelMemoryId> LevelMemory = VariablesArray<LevelMemoryId>(0, 1 << 16);

VariablesArray<LevelDataId> LevelData = VariablesArray<LevelDataId>(0, 1 << 16);

VariablesArray<LevelOptionId> LevelOption = VariablesArray<LevelOptionId>(0, 1 << 16);

struct LevelBucketGroup{
	struct Group_AIEowH1mZxOgyQFq{
		Variable min = Variable(LevelBucketId, 0);
		Variable max = Variable(LevelBucketId, 0);

		void reallocate(FuncNode offset) {
			min.reallocate(offset + 0);
			max.reallocate(offset + 1);
		}
	};
	Group_AIEowH1mZxOgyQFq perfect;
	Group_AIEowH1mZxOgyQFq great;
	Group_AIEowH1mZxOgyQFq good;

	Constructor LevelBucketGroup(FuncNode offset) {
		perfect.reallocate(offset + 0);
		great.reallocate(offset + 2);
		good.reallocate(offset + 4);
	}
};
BlocksArray<LevelBucketGroup> LevelBucket = BlocksArray<LevelBucketGroup>(6);

struct LevelScoreGroup{
	Variable perfect_multiplier = Variable(LevelScoreId, 0);
	Variable great_multiplier = Variable(LevelScoreId, 0);
	Variable good_multiplier = Variable(LevelScoreId, 0);
	struct Group_VxHKUCPih3wdhlwr{
		Variable consecutiveMultiplier = Variable(LevelScoreId, 0);
		Variable consecutiveStep = Variable(LevelScoreId, 0);
		Variable consecutiveCap = Variable(LevelScoreId, 0);

		void reallocate(FuncNode offset) {
			consecutiveMultiplier.reallocate(offset + 0);
			consecutiveStep.reallocate(offset + 1);
			consecutiveCap.reallocate(offset + 2);
		}
	};
	Group_VxHKUCPih3wdhlwr perfect;
	Group_VxHKUCPih3wdhlwr great;
	Group_VxHKUCPih3wdhlwr good;

	Constructor LevelScoreGroup(FuncNode offset) {
		perfect_multiplier.reallocate(offset + 0);
		great_multiplier.reallocate(offset + 1);
		good_multiplier.reallocate(offset + 2);
		perfect.reallocate(offset + 3);
		great.reallocate(offset + 6);
		good.reallocate(offset + 9);
	}
};
LevelScoreGroup LevelScore = LevelScoreGroup(0);

struct LevelLifeGroup{
	struct Group_18dyPckwa6dWWqKz{
		Variable consecutiveIncrement = Variable(LevelLifeId, 0);
		Variable consecutiveStep = Variable(LevelLifeId, 0);

		void reallocate(FuncNode offset) {
			consecutiveIncrement.reallocate(offset + 0);
			consecutiveStep.reallocate(offset + 1);
		}
	};
	Group_18dyPckwa6dWWqKz perfect;
	Group_18dyPckwa6dWWqKz great;
	Group_18dyPckwa6dWWqKz good;

	Constructor LevelLifeGroup(FuncNode offset) {
		perfect.reallocate(offset + 0);
		great.reallocate(offset + 2);
		good.reallocate(offset + 4);
	}
};
LevelLifeGroup LevelLife = LevelLifeGroup(0);

VariablesArray<EngineRomId> EngineRom = VariablesArray<EngineRomId>(0, 1 << 16);

VariablesArray<EntityMemoryId> EntityMemory = VariablesArray<EntityMemoryId>(0, 1 << 16);

VariablesArray<EntityDataId> EntityData = VariablesArray<EntityDataId>(0, 1 << 16);

VariablesArray<EntitySharedMemoryId> EntitySharedMemory = VariablesArray<EntitySharedMemoryId>(0, 1 << 16);

struct EntityInfoGroup{
	Variable index = Variable(EntityInfoId, 0);
	Variable archetype = Variable(EntityInfoId, 0);
	Variable state = Variable(EntityInfoId, 0);

	Constructor EntityInfoGroup(FuncNode offset) {
		index.reallocate(offset + 0);
		archetype.reallocate(offset + 1);
		state.reallocate(offset + 2);
	}
};
EntityInfoGroup EntityInfo = EntityInfoGroup(0);

struct EntityDespawnGroup{
	Variable despawn = Variable(EntityDespawnId, 0);

	Constructor EntityDespawnGroup(FuncNode offset) {
		despawn.reallocate(offset + 0);
	}
};
EntityDespawnGroup EntityDespawn = EntityDespawnGroup(0);

struct EntityInputGroup{
	Variable judgment = Variable(EntityInputId, 0);
	Variable accuracy = Variable(EntityInputId, 0);
	Variable bucketIndex = Variable(EntityInputId, 0);
	Variable bucketValue = Variable(EntityInputId, 0);

	Constructor EntityInputGroup(FuncNode offset) {
		judgment.reallocate(offset + 0);
		accuracy.reallocate(offset + 1);
		bucketIndex.reallocate(offset + 2);
		bucketValue.reallocate(offset + 3);
	}
};
EntityInputGroup EntityInput = EntityInputGroup(0);

struct EntityDataArrayGroup{
	VariablesArray<EntityDataArrayId> generic = VariablesArray<EntityDataArrayId>(0, 32);

	Constructor EntityDataArrayGroup(FuncNode offset) {
		generic = VariablesArray<EntityDataArrayId>(offset + 0, 32);
	}
};
BlocksArray<EntityDataArrayGroup> EntityDataArray = BlocksArray<EntityDataArrayGroup>(32);

struct EntitySharedMemoryArrayGroup{
	VariablesArray<EntitySharedMemoryArrayId> generic = VariablesArray<EntitySharedMemoryArrayId>(0, 32);

	Constructor EntitySharedMemoryArrayGroup(FuncNode offset) {
		generic = VariablesArray<EntitySharedMemoryArrayId>(offset + 0, 32);
	}
};
BlocksArray<EntitySharedMemoryArrayGroup> EntitySharedMemoryArray = BlocksArray<EntitySharedMemoryArrayGroup>(32);

struct EntityInfoArrayGroup{
	Variable index = Variable(EntityInfoArrayId, 0);
	Variable archetype = Variable(EntityInfoArrayId, 0);
	Variable state = Variable(EntityInfoArrayId, 0);

	Constructor EntityInfoArrayGroup(FuncNode offset) {
		index.reallocate(offset + 0);
		archetype.reallocate(offset + 1);
		state.reallocate(offset + 2);
	}
};
BlocksArray<EntityInfoArrayGroup> EntityInfoArray = BlocksArray<EntityInfoArrayGroup>(3);

struct ArchetypeLifeGroup{
	Variable perfect_increment = Variable(ArchetypeLifeId, 0);
	Variable great_increment = Variable(ArchetypeLifeId, 0);
	Variable good_increment = Variable(ArchetypeLifeId, 0);
	Variable miss_increment = Variable(ArchetypeLifeId, 0);

	Constructor ArchetypeLifeGroup(FuncNode offset) {
		perfect_increment.reallocate(offset + 0);
		great_increment.reallocate(offset + 1);
		good_increment.reallocate(offset + 2);
		miss_increment.reallocate(offset + 3);
	}
};
BlocksArray<ArchetypeLifeGroup> ArchetypeLife = BlocksArray<ArchetypeLifeGroup>(4);

VariablesArray<TemporaryMemoryId> TemporaryMemory = VariablesArray<TemporaryMemoryId>(0, 1 << 16);

#endif

/*** End of inlined file: Blocks.h ***/



/*** Start of inlined file: Runtime.h ***/
#ifndef PlayRuntime_H
#define PlayRuntime_H

const int OptionId = LevelOptionId;
auto unused_defaultAllocatorId_unused = [](){
    defaultAllocatorId = LevelMemoryId;
    createNodeContainer();
    return true;
}();

Variable &debugMode = RuntimeEnvironment.debugMode;
Variable &aspectRatio = RuntimeEnvironment.screenAspectRatio;
struct offset {
    Variable &audio = RuntimeEnvironment.audioOffset;
    Variable &input = RuntimeEnvironment.inputOffset;
}offsets;
Variable &multiplayer = RuntimeEnvironment.multiplayer;
struct times {
    Variable &now = RuntimeUpdate.time;
    Variable &delta = RuntimeUpdate.deltaTime;
    Variable &scaled = RuntimeUpdate.scaledTime;
}times;
Variable &touchCount = RuntimeUpdate.touchCount;
BlocksArray<RuntimeTouchArrayGroup> &touches = RuntimeTouchArray;
struct transforms {
    VariablesArray<RuntimeSkinTransformId> &skin = RuntimeSkinTransform;
    VariablesArray<RuntimeParticleTransformId> &particle = RuntimeParticleTransform;
}transforms;
RuntimeUIGroup &ui = RuntimeUI;
RuntimeUIConfigurationGroup &configuration = RuntimeUIConfiguration;
BlocksArray<LevelBucketGroup> &buckets = LevelBucket;
LevelScoreGroup &scores = LevelScore;
LevelLifeGroup &lifes = LevelLife;

template<typename T>
SonolusApi setUI(T &ui, var anchorX, var anchorY, var pivotX, var pivotY, var width, var height, var rotation, var alpha, var horizontalAlign, var background) {
    ui.anchorX = anchorX; ui.anchorY = anchorY;
    ui.pivotX = pivotX; ui.pivotY = pivotY;
    ui.width = width; ui.height = height;
    ui.rotation = rotation;
    ui.alpha = alpha;
    ui.horizontalAlign = horizontalAlign;
    ui.background = background;
}

SonolusApi setBucket(var bucketId, var perfectMin, var perfectMax, var greatMin, var greatMax, var goodMin, var goodMax) {
    buckets[bucketId].perfect.min = perfectMin;
    buckets[bucketId].perfect.max = perfectMax;
    buckets[bucketId].great.min = greatMin;
    buckets[bucketId].great.max = greatMax;
    buckets[bucketId].good.min = goodMin;
    buckets[bucketId].good.max = goodMax;
}

SonolusApi setScore(var perfect, var great, var good) {
    scores.perfect_multiplier = perfect;
    scores.great_multiplier = great;
    scores.good_multiplier = good;
}

#endif
/*** End of inlined file: Runtime.h ***/


/*** Start of inlined file: Archetype.h ***/
#ifndef PlayArchetype_H
#define PlayArchetype_H

class Archetype {
    public:
    // Archetype id
    int id = 0;

    // Entity Memory block is used as generic memory storage.
    VariablesArray<EntityMemoryId> &memory = EntityMemory;
    // Entity Shared Memory block is used as generic memory storage that are accessible by other entities.
    VariablesArray<EntitySharedMemoryId> &sharedMemory = EntitySharedMemory;
    // Entity Info block contains state information of the entity.
    EntityInfoGroup &info = EntityInfo;
    // Entity Despawn block is used by entities to pass despawn information back to Sonolus.
    EntityDespawnGroup &despawn = EntityDespawn;
    // Entity Input block is used by entities to pass input information back to Sonolus.
    EntityInputGroup &input = EntityInput;

    public:
    // Archetype Life block contains life configuration of each archetype.
    ArchetypeLifeGroup life = ArchetypeLife[id];

    // Name of this archetype, which will be referenced by entities in level data.
    // Special archetype names will have its effect applied automatically:
    // `#BPM_CHANGE`: Signals a BPM change and will affect BPM change related functions. Entity with this archetype must also provide data named `#BEAT` and `#BPM`.
    // `#TIMESCALE_CHANGE`: Signals a time scale change and will affect time scale change related functions. Entity with this archetype must also provide data named `#BEAT` and `#TIMESCALE`.
    string name = "Default Archetype";
    // If true, Sonolus will treat entities with this archetype to be playable entities, and will contribute to judgment, combo, life, score, etc.
    bool hasInput = false;
    // Imported data to be inject into Entity Data block.
    // Imported data with matching `name` will be injected at `index` of Entity Data block.
    vector<pair<string, int> > imports;
    // Exported data to be written into replay.
    vector<string> exports;

    double preprocessOrder = 0;
    SonolusApi preprocess() { return 0; }

    double spawnOrderOrder = 0;
    SonolusApi spawnOrder() { return 0; }

    double shouldSpawnOrder = 0;
    SonolusApi shouldSpawn() { return 0; }

    double initializeOrder = 0;
    SonolusApi initialize() { return 0; }

    double updateSequentialOrder = 0;
    SonolusApi updateSequential() { return 0; }

    double touchOrder = 0;
    SonolusApi touch() { return 0; }

    double updateParallelOrder = 0;
    SonolusApi updateParallel() { return 0; }

    double terminateOrder = 0;
    SonolusApi terminate() { return 0; }
};

#define defineImport(name) bool unused_##name##_unused = [&](){ \
        FuncNode id = imports.size(); \
        imports.push_back({ #name, imports.size() }); \
        return true; \
    }(); \
    Variable name = Variable(EntityDataId, imports.size() - 1)
#define defineImportDetailed(varName, jsonName) bool unused_##varName##_unused = [&](){ \
        FuncNode id = imports.size(); \
		imports.push_back({ jsonName, imports.size() }); \
        return true; \
    }(); \
    Variable varName = Variable(EntityDataId, imports.size() - 1)
#define defineExport(name) int name = [&](){ \
		exports.push_back(#name); \
        return exports.size() - 1; \
    }()
#define defineExportDetailed(varName, jsonName) int varName = [&](){ \
		exports.push_back(jsonName); \
        return exports.size() - 1; \
    }()

#endif
/*** End of inlined file: Archetype.h ***/


/*** Start of inlined file: Build.h ***/
#ifndef PlayBuild_H
#define PlayBuild_H

class EnginePlayDataArchetype {
    public:

    string name = "";
    bool hasInput = false;
    pair<int, double> preprocess = {0, 0};
    pair<int, double> spawnOrder = {0, 0};
    pair<int, double> shouldSpawn = {0, 0};
    pair<int, double> initialize = {0, 0};
    pair<int, double> updateSequential = {0, 0};
    pair<int, double> touch = {0, 0};
    pair<int, double> updateParallel = {0, 0};
    pair<int, double> terminate = {0, 0};
    vector<pair<string, int>> imports = {};
    vector<string> exports = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["hasInput"] = hasInput;
        res["preprocess"]["index"] = preprocess.first;
        res["preprocess"]["order"] = preprocess.second;
        res["spawnOrder"]["index"] = spawnOrder.first;
        res["spawnOrder"]["order"] = spawnOrder.second;
        res["shouldSpawn"]["index"] = shouldSpawn.first;
        res["shouldSpawn"]["order"] = shouldSpawn.second;
        res["initialize"]["index"] = initialize.first;
        res["initialize"]["order"] = initialize.second;
        res["updateSequential"]["index"] = updateSequential.first;
        res["updateSequential"]["order"] = updateSequential.second;
        res["touch"]["index"] = touch.first;
        res["touch"]["order"] = touch.second;
        res["updateParallel"]["index"] = updateParallel.first;
        res["updateParallel"]["order"] = updateParallel.second;
        res["terminate"]["index"] = terminate.first;
        res["terminate"]["order"] = terminate.second;
        res["imports"].resize(0);
        for (int i = 0; i < imports.size(); i++)
        {
            Json::Value single;
            single["name"] = imports[i].first;
            single["index"] = imports[i].second;
            res["imports"].append(single);
        }
        res["exports"].resize(0);
        for (int i = 0; i < exports.size(); i++)
            res["exports"].append(exports[i]);
        return res;
    }
};

class EnginePlayData {
    public:

    string skinRenderMode = "default";
    vector<pair<string, int> > skinSprites = {};
    vector<pair<string, int> > effectClips = {};
    vector<pair<string, int> > particleEffects = {};
    vector<EngineDataBucket> buckets = {};
    vector<EnginePlayDataArchetype> archetypes = {};
    vector<DataNode> nodes = {};

    Json::Value toJsonObject() {
        Json::Value res;
        SonolusAssert(
            set<string>({"default", "standard", "lightweight"}).count(skinRenderMode),
            "`skinRenderMode` must be `default`, `standard` or `lightweight`!"
        );
        res["skin"]["renderMode"] = skinRenderMode;
        res["skin"]["sprites"].resize(0);
        for (int i = 0; i < skinSprites.size(); i++) {
            Json::Value single;
            single["name"] = skinSprites[i].first;
            single["id"] = skinSprites[i].second;
            res["skin"]["sprites"].append(single);
        }
        res["effect"]["clips"].resize(0);
        for (int i = 0; i < effectClips.size(); i++) {
            Json::Value single;
            single["name"] = effectClips[i].first;
            single["id"] = effectClips[i].second;
            res["effect"]["clips"].append(single);
        }
        res["particle"]["effects"].resize(0);
        for (int i = 0; i < particleEffects.size(); i++) {
            Json::Value single;
            single["name"] = particleEffects[i].first;
            single["id"] = particleEffects[i].second;
            res["particle"]["effects"].append(single);
        }
        res["buckets"].resize(0);
        for (int i = 0; i < buckets.size(); i++)
            res["buckets"].append(buckets[i].toJsonObject());
        res["archetypes"].resize(0);
        for (int i = 0; i < archetypes.size(); i++)
            res["archetypes"].append(archetypes[i].toJsonObject());
        res["nodes"].resize(0);
        for (int i = 0; i < nodes.size(); i++)
            res["nodes"].append(nodes[i].toJsonObject());
        return res;
    }
}enginePlayData;

#if false
// 用宏定义定义一个虚假函数，让解释器识别到这个函数并自动加到 UserDefined 函数列表中
void BuildData() {}
#endif
// BuildData 宏定义一定要禁止解释器解析，不然解释器会识别到两个同名的宏定义然后出现错误
#define DISABLE_INTERPRETER
#ifndef COMPILE_RUNTIME
// 供 IDE 识别的 BuildData 函数
#define BuildData(...) [&]() {\
    allocateArchetypeId(#__VA_ARGS__); \
    BuildArchetypes<__VA_ARGS__>(); \
    BuildPlayData(); }()
#else
// 供编译器识别的 BuildData 函数
#define BuildData(arg1, arg2, arg3, arg4, arg5, ...) [&]() {\
    allocateArchetypeId(#__VA_ARGS__); \
    BuildArchetypes<__VA_ARGS__>(arg1, arg2, arg3, arg4, arg5); \
    BuildPlayData(arg1, arg2, arg3, arg4, arg5); }()
#endif
#undef DISABLE_INTERPRETER

map<string, int> archetypeId;
int archetypeNumber = 0;
Blocked void allocateArchetypeId(string archetypeList) {
    string tmp = "";
    int id = 0;
    for (int i = 0; i < archetypeList.size(); i++) {
        if (archetypeList[i] == ',') archetypeId[tmp] = id++, tmp = "";
        else if (isalnum(archetypeList[i])) tmp += archetypeList[i];
    }
    if (tmp != "") archetypeId[tmp] = id++;
    archetypeNumber = id;
}

#define getAid(archetype) (archetypeId.find(#archetype) == archetypeId.end() ? -1 : archetypeId[#archetype])
#define buildCallback(name) [&]() { \
    createNodeContainer(); \
    defaultAllocatorId = TemporaryMemoryId; \
    callbackName = #name; \
    archetype.name(); \
    dataArchetype.name.first = mergeNodeContainer().getNodeId(); \
    dataArchetype.name.second = archetype.name##Order; }()

template <typename T>
void BuildArchetype(T &archetype) {
    EnginePlayDataArchetype dataArchetype;
    dataArchetype.name = archetype.name;
    dataArchetype.hasInput = archetype.hasInput;
    dataArchetype.imports = archetype.imports;
    dataArchetype.exports = archetype.exports;

    buildCallback(preprocess);
    buildCallback(spawnOrder);
    buildCallback(shouldSpawn);
    buildCallback(initialize);
    buildCallback(updateSequential);
    buildCallback(touch);
    buildCallback(updateParallel);
    buildCallback(terminate);

    enginePlayData.archetypes.push_back(dataArchetype);
}

template <typename T, typename... Args>
void BuildArchetypes(int aid = 0) {
    defaultAllocatorId = EntityMemoryId;
    SonolusMemoryDelta[EntityMemoryId] = 1;
    T archetype = T();
    archetypeName = archetype.name;
    archetype.id = aid;
    archetype.life.perfect_increment.reallocate(4 * aid + 0);
    archetype.life.great_increment.reallocate(4 * aid + 1);
    archetype.life.good_increment.reallocate(4 * aid + 2);
    archetype.life.miss_increment.reallocate(4 * aid + 3);
    BuildArchetype(archetype);

#define DISABLE_INTERPRETER
#ifdef COMPILE_RUNTIME
    if constexpr (sizeof...(Args))
        BuildArchetypes<Args...>(callFromFuncParam, callFromFunc, callFromLine, callFromColumn, callFromFile, aid + 1);
#else
    if constexpr (sizeof...(Args))
        BuildArchetypes<Args...>();
#endif
#undef DISABLE_INTERPRETER
}

void BuildPlayData() {
    enginePlayData.nodes = dataContainer;
    enginePlayData.skinSprites.clear();
    for (int i = 0; i < skin_sprites.size(); i++)
        enginePlayData.skinSprites.push_back({ skin_sprites[i].first, i });
    enginePlayData.effectClips.clear();
    for (int i = 0; i < effect_clips.size(); i++)
        enginePlayData.effectClips.push_back({ effect_clips[i].first, i });
    enginePlayData.particleEffects.clear();
    for (int i = 0; i < particle_effects.size(); i++)
        enginePlayData.particleEffects.push_back({ particle_effects[i].first, i });
    enginePlayData.buckets.clear();
    for (int i = 0; i < data_buckets.size(); i++)
        enginePlayData.buckets.push_back(data_buckets[i]);

    string EnginePlayData = compress_gzip(json_encode(enginePlayData.toJsonObject()));
    ofstream fout = ofstream(dist + "/EnginePlayData");
    fout.write(EnginePlayData.c_str(), EnginePlayData.size());
}

#endif
/*** End of inlined file: Build.h ***/

#endif
#ifdef tutorial

/*** Start of inlined file: Blocks.h ***/
#ifndef TutorialBlocks_H
#define TutorialBlocks_H

const int RuntimeEnvironmentId = 1000;
const int RuntimeUpdateId = 1001;
const int RuntimeSkinTransformId = 1002;
const int RuntimeParticleTransformId = 1003;
const int RuntimeBackgroundId = 1004;
const int RuntimeUIId = 1005;
const int RuntimeUIConfigurationId = 1006;
const int TutorialMemoryId = 2000;
const int TutorialDataId = 2001;
const int TutorialInstructionId = 2002;
const int EngineRomId = 3000;
const int TemporaryMemoryId = 10000;

bool unused_readable_unused = [](){
	readable = {
		{ RuntimeEnvironmentId, { "preprocess", "navigate", "update" } },
		{ RuntimeUpdateId, { "preprocess", "navigate", "update" } },
		{ RuntimeSkinTransformId, { "preprocess", "navigate", "update" } },
		{ RuntimeParticleTransformId, { "preprocess", "navigate", "update" } },
		{ RuntimeBackgroundId, { "preprocess", "navigate", "update" } },
		{ RuntimeUIId, { "preprocess", "navigate", "update" } },
		{ RuntimeUIConfigurationId, { "preprocess", "navigate", "update" } },
		{ TutorialMemoryId, { "preprocess", "navigate", "update" } },
		{ TutorialDataId, { "preprocess", "navigate", "update" } },
		{ TutorialInstructionId, { "preprocess", "navigate", "update" } },
		{ EngineRomId, { "preprocess", "navigate", "update" } },
		{ TemporaryMemoryId, { "preprocess", "navigate", "update" } }
	};
	return true;
}();

bool unused_writable_unused = [](){
	writable = {
		{ RuntimeEnvironmentId, { "preprocess" } },
		{ RuntimeUpdateId, {  } },
		{ RuntimeSkinTransformId, { "preprocess", "navigate", "update" } },
		{ RuntimeParticleTransformId, { "preprocess", "navigate", "update" } },
		{ RuntimeBackgroundId, { "preprocess", "navigate", "update" } },
		{ RuntimeUIId, { "preprocess" } },
		{ RuntimeUIConfigurationId, { "preprocess" } },
		{ TutorialMemoryId, { "preprocess", "navigate", "update" } },
		{ TutorialDataId, { "preprocess" } },
		{ TutorialInstructionId, { "preprocess", "navigate", "update" } },
		{ EngineRomId, {  } },
		{ TemporaryMemoryId, { "preprocess", "navigate", "update" } }
	};
	return true;
}();

bool unused_blockName_unused = [](){
	blockName = {
		{ RuntimeEnvironmentId, "RuntimeEnvironment" },
		{ RuntimeUpdateId, "RuntimeUpdate" },
		{ RuntimeSkinTransformId, "RuntimeSkinTransform" },
		{ RuntimeParticleTransformId, "RuntimeParticleTransform" },
		{ RuntimeBackgroundId, "RuntimeBackground" },
		{ RuntimeUIId, "RuntimeUI" },
		{ RuntimeUIConfigurationId, "RuntimeUIConfiguration" },
		{ TutorialMemoryId, "TutorialMemory" },
		{ TutorialDataId, "TutorialData" },
		{ TutorialInstructionId, "TutorialInstruction" },
		{ EngineRomId, "EngineRom" },
		{ TemporaryMemoryId, "TemporaryMemory" }
	};
	return true;
}();

bool unused_writeBlockInfo_unused = [](){
	SonolusMemorySize[TutorialMemoryId] = 4096;
	SonolusMemoryDelta[TutorialMemoryId] = 1;
	allowAllocate.insert(TutorialMemoryId);
	SonolusMemorySize[TutorialDataId] = 4096;
	SonolusMemoryDelta[TutorialDataId] = 1;
	allowAllocate.insert(TutorialDataId);
	SonolusMemorySize[TemporaryMemoryId] = 4096;
	SonolusMemoryDelta[TemporaryMemoryId] = 1;
	allowAllocate.insert(TemporaryMemoryId);
	return true;
}();

struct RuntimeEnvironmentGroup{
	Variable debugMode = Variable(RuntimeEnvironmentId, 0);
	Variable screenAspectRatio = Variable(RuntimeEnvironmentId, 0);
	Variable audioOffset = Variable(RuntimeEnvironmentId, 0);

	Constructor RuntimeEnvironmentGroup(FuncNode offset) {
		debugMode.reallocate(offset + 0);
		screenAspectRatio.reallocate(offset + 1);
		audioOffset.reallocate(offset + 2);
	}
};
RuntimeEnvironmentGroup RuntimeEnvironment = RuntimeEnvironmentGroup(0);

struct RuntimeUpdateGroup{
	Variable time = Variable(RuntimeUpdateId, 0);
	Variable deltaTime = Variable(RuntimeUpdateId, 0);
	Variable navigationDirection = Variable(RuntimeUpdateId, 0);

	Constructor RuntimeUpdateGroup(FuncNode offset) {
		time.reallocate(offset + 0);
		deltaTime.reallocate(offset + 1);
		navigationDirection.reallocate(offset + 2);
	}
};
RuntimeUpdateGroup RuntimeUpdate = RuntimeUpdateGroup(0);

VariablesArray<RuntimeSkinTransformId> RuntimeSkinTransform = VariablesArray<RuntimeSkinTransformId>(0, 1 << 16);

VariablesArray<RuntimeParticleTransformId> RuntimeParticleTransform = VariablesArray<RuntimeParticleTransformId>(0, 1 << 16);

struct RuntimeBackgroundGroup{
	Variable x = Variable(RuntimeBackgroundId, 0);
	Variable y = Variable(RuntimeBackgroundId, 0);

	Constructor RuntimeBackgroundGroup(FuncNode offset) {
		x.reallocate(offset + 0);
		y.reallocate(offset + 1);
	}
};
BlocksArray<RuntimeBackgroundGroup> RuntimeBackground = BlocksArray<RuntimeBackgroundGroup>(2);

struct RuntimeUIGroup{
	struct Group_PZSBzYvZkjxrz1hh{
		Variable anchorX = Variable(RuntimeUIId, 0);
		Variable anchorY = Variable(RuntimeUIId, 0);
		Variable pivotX = Variable(RuntimeUIId, 0);
		Variable pivotY = Variable(RuntimeUIId, 0);
		Variable width = Variable(RuntimeUIId, 0);
		Variable height = Variable(RuntimeUIId, 0);
		Variable rotation = Variable(RuntimeUIId, 0);
		Variable alpha = Variable(RuntimeUIId, 0);
		Variable background = Variable(RuntimeUIId, 0);

		void reallocate(FuncNode offset) {
			anchorX.reallocate(offset + 0);
			anchorY.reallocate(offset + 1);
			pivotX.reallocate(offset + 2);
			pivotY.reallocate(offset + 3);
			width.reallocate(offset + 4);
			height.reallocate(offset + 5);
			rotation.reallocate(offset + 6);
			alpha.reallocate(offset + 7);
			background.reallocate(offset + 8);
		}
	};
	Group_PZSBzYvZkjxrz1hh menu;
	Group_PZSBzYvZkjxrz1hh navigationPrevious;
	Group_PZSBzYvZkjxrz1hh navigationNext;
	Group_PZSBzYvZkjxrz1hh instruction;

	Constructor RuntimeUIGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		navigationPrevious.reallocate(offset + 9);
		navigationNext.reallocate(offset + 18);
		instruction.reallocate(offset + 27);
	}
};
RuntimeUIGroup RuntimeUI = RuntimeUIGroup(0);

struct RuntimeUIConfigurationGroup{
	struct Group_5LHKJ8civYrNsseF{
		Variable scale = Variable(RuntimeUIConfigurationId, 0);
		Variable alpha = Variable(RuntimeUIConfigurationId, 0);

		void reallocate(FuncNode offset) {
			scale.reallocate(offset + 0);
			alpha.reallocate(offset + 1);
		}
	};
	Group_5LHKJ8civYrNsseF menu;
	Group_5LHKJ8civYrNsseF navigation;
	Group_5LHKJ8civYrNsseF instruction;

	Constructor RuntimeUIConfigurationGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		navigation.reallocate(offset + 2);
		instruction.reallocate(offset + 4);
	}
};
RuntimeUIConfigurationGroup RuntimeUIConfiguration = RuntimeUIConfigurationGroup(0);

VariablesArray<TutorialMemoryId> TutorialMemory = VariablesArray<TutorialMemoryId>(0, 1 << 16);

VariablesArray<TutorialDataId> TutorialData = VariablesArray<TutorialDataId>(0, 1 << 16);

struct TutorialInstructionGroup{
	Variable text = Variable(TutorialInstructionId, 0);

	Constructor TutorialInstructionGroup(FuncNode offset) {
		text.reallocate(offset + 0);
	}
};
TutorialInstructionGroup TutorialInstruction = TutorialInstructionGroup(0);

VariablesArray<EngineRomId> EngineRom = VariablesArray<EngineRomId>(0, 1 << 16);

VariablesArray<TemporaryMemoryId> TemporaryMemory = VariablesArray<TemporaryMemoryId>(0, 1 << 16);

#endif

/*** End of inlined file: Blocks.h ***/



/*** Start of inlined file: Runtime.h ***/
#ifndef TutorialRuntime_H
#define TutorialRuntime_H

const int OptionId = 0;
auto unused_defaultAllocatorId_unused = [](){
    defaultAllocatorId = TutorialMemoryId;
    createNodeContainer();
    return true;
}();

Variable &debugMode = RuntimeEnvironment.debugMode;
Variable &aspectRatio = RuntimeEnvironment.screenAspectRatio;
struct offset {
    Variable &audio = RuntimeEnvironment.audioOffset;
}offsets;
struct times {
    Variable &now = RuntimeUpdate.time;
    Variable &delta = RuntimeUpdate.deltaTime;
    Variable &scaled = RuntimeUpdate.time;
}times;
Variable &navigation = RuntimeUpdate.navigationDirection;
struct transforms {
    VariablesArray<RuntimeSkinTransformId> &skin = RuntimeSkinTransform;
    VariablesArray<RuntimeParticleTransformId> &particle = RuntimeParticleTransform;
}transforms;
RuntimeUIGroup &ui = RuntimeUI;
RuntimeUIConfigurationGroup &configuration = RuntimeUIConfiguration;
Variable &instruction = TutorialInstruction.text;

template<typename T>
SonolusApi setUI(T &ui, var anchorX, var anchorY, var pivotX, var pivotY, var width, var height, var rotation, var alpha, var background) {
    ui.anchorX = anchorX; ui.anchorY = anchorY;
    ui.pivotX = pivotX; ui.pivotY = pivotY;
    ui.width = width; ui.height = height;
    ui.rotation = rotation;
    ui.alpha = alpha;
    ui.background = background;
}

#endif
/*** End of inlined file: Runtime.h ***/


/*** Start of inlined file: Build.h ***/
#ifndef TutorialBuild_H
#define TutorialBuild_H

class EngineTutorialData {
    public:

    string skinRenderMode = "default";
    vector<pair<string, int> > skinSprites = {};
    vector<pair<string, int> > effectClips = {};
    vector<pair<string, int> > particleEffects = {};
    vector<pair<string, int> > instructionTexts = {};
    vector<pair<string, int> > instructionIcons = {};
    int preprocess = 0;
    int navigate = 0;
    int update = 0;
    vector<DataNode> nodes = {};

    Json::Value toJsonObject() {
        Json::Value res;
        SonolusAssert(
            set<string>({"default", "standard", "lightweight"}).count(skinRenderMode),
            "`skinRenderMode` must be `default`, `standard` or `lightweight`!"
        );
        res["skin"]["renderMode"] = skinRenderMode;
        res["skin"]["sprites"].resize(0);
        for (int i = 0; i < skinSprites.size(); i++) {
            Json::Value single;
            single["name"] = skinSprites[i].first;
            single["id"] = skinSprites[i].second;
            res["skin"]["sprites"].append(single);
        }
        res["effect"]["clips"].resize(0);
        for (int i = 0; i < effectClips.size(); i++) {
            Json::Value single;
            single["name"] = effectClips[i].first;
            single["id"] = effectClips[i].second;
            res["effect"]["clips"].append(single);
        }
        res["particle"]["effects"].resize(0);
        for (int i = 0; i < particleEffects.size(); i++) {
            Json::Value single;
            single["name"] = particleEffects[i].first;
            single["id"] = particleEffects[i].second;
            res["particle"]["effects"].append(single);
        }
        res["instruction"]["texts"].resize(0);
        for (int i = 0; i < instructionTexts.size(); i++) {
            Json::Value single;
            single["name"] = instructionTexts[i].first;
            single["id"] = instructionTexts[i].second;
            res["instruction"]["texts"].append(single);
        }
        res["instruction"]["icons"].resize(0);
        for (int i = 0; i < instructionIcons.size(); i++) {
            Json::Value single;
            single["name"] = instructionIcons[i].first;
            single["id"] = instructionIcons[i].second;
            res["instruction"]["icons"].append(single);
        }
        res["preprocess"] = preprocess;
        res["navigate"] = navigate;
        res["update"] = update;
        res["nodes"].resize(0);
        for (int i = 0; i < nodes.size(); i++)
            res["nodes"].append(nodes[i].toJsonObject());
        return res;
    }
}engineTutorialData;

vector<function<SonolusApi()> > Preprocesses = {};
vector<pair<function<SonolusApi(var, var)>, double> > TemporarySegments = {};
vector<vector<pair<function<SonolusApi(var, var)>, double> > > Segments = {};
vector<function<SonolusApi(var, var)> > Statics = {};

vector<SwitchWithDefault_Group_test_consequent> Duration, BaseTime, NextSegment;
int segmentNumber = 0, groupNumber = 0; double sumTime = 0;
void TutorialBuildPreprocess() {
    int id = 0;
    for (int i = 0; i < Segments.size(); i++) {
        auto segmentGroup = Segments[i];
        int tmpid = id; groupNumber++;
        for (int j = 0; j < segmentGroup.size(); j++) {
            auto segment = segmentGroup[j];
            segmentNumber++;
            Duration.push_back({ id, segment.second }),
            BaseTime.push_back({ id, sumTime }),
            sumTime += segment.second; id++;
        }
        for (int j = tmpid; j < id; j++) NextSegment.push_back({ j, id });
    }
};

var tutorialNavigation, tutorialStartTime;
SonolusApi TutorialPreprocess() {
    tutorialStartTime = 0;
    tutorialNavigation = 0;
    for (CppLoop int i = 0; i < Preprocesses.size(); i++) Preprocesses[i]();
}

SonolusApi TutorialNavigate() {
    for (var i = 0; i < If(navigation > 0, 1, groupNumber - 1); i++) {
        tutorialNavigation = SwitchWithDefault(tutorialNavigation, NextSegment, 0);
        tutorialNavigation = tutorialNavigation % segmentNumber;
    }
    tutorialStartTime = times.now;
    instruction = -1;
}

FuncNode TutorialUpdateContainer(function<SonolusApi(var, var)> func, var arg1, var arg2) {
    int tmp = SonolusMemoryDelta[TemporaryMemoryId];
    createNodeContainer();
    func(arg1, arg2);
    FuncNode node = mergeNodeContainer();
    SonolusMemoryDelta[TemporaryMemoryId] = tmp;
    return node;
}
SonolusApi TutorialUpdate() {
    if (times.now - tutorialStartTime > SwitchWithDefault(tutorialNavigation, Duration, 1)) {
        tutorialNavigation = tutorialNavigation + 1;
        tutorialNavigation = tutorialNavigation % segmentNumber;
        tutorialStartTime = times.now;
    }

    for (CppLoop int i = 0; i < Statics.size(); i++) {
        auto Static = Statics[i];
        Static(SwitchWithDefault(tutorialNavigation, BaseTime, 1) + times.now - tutorialStartTime, sumTime);
    }

    vector<SwitchWithDefault_Group_test_consequent> funcs; int id = 0;
    for (CppLoop int i = 0; i < Segments.size(); i++) {
        auto group = Segments[i];
        for (CppLoop int j = 0; j < group.size(); j++) {
            auto segment = group[j];
            funcs.push_back({
                id++,
                TutorialUpdateContainer(segment.first, times.now - tutorialStartTime, segment.second)
            });
        }
    }

    SonolusRun(SwitchWithDefault(tutorialNavigation, funcs, 0));
}

void BuildData() {
    TutorialBuildPreprocess();

    archetypeName = "Tutorial Archetype";
    defaultAllocatorId = TemporaryMemoryId;

    createNodeContainer();
    callbackName = "preprocess";
    TutorialPreprocess();
    engineTutorialData.preprocess = mergeNodeContainer().getNodeId();

    createNodeContainer();
    callbackName = "navigate";
    TutorialNavigate();
    engineTutorialData.navigate = mergeNodeContainer().getNodeId();

    createNodeContainer();
    callbackName = "update";
    TutorialUpdate();
    engineTutorialData.update = mergeNodeContainer().getNodeId();

    engineTutorialData.nodes = dataContainer;
    engineTutorialData.skinSprites.clear();
    for (int i = 0; i < skin_sprites.size(); i++)
        engineTutorialData.skinSprites.push_back({ skin_sprites[i].first, i });
    engineTutorialData.effectClips.clear();
    for (int i = 0; i < effect_clips.size(); i++)
        engineTutorialData.effectClips.push_back({ effect_clips[i].first, i });
    engineTutorialData.particleEffects.clear();
    for (int i = 0; i < particle_effects.size(); i++)
        engineTutorialData.particleEffects.push_back({ particle_effects[i].first, i });
    engineTutorialData.instructionTexts.clear();
    for (int i = 0; i < instruction_texts.size(); i++)
        engineTutorialData.instructionTexts.push_back({ instruction_texts[i], i });
    engineTutorialData.instructionIcons.clear();
    for (int i = 0; i < instruction_icons.size(); i++)
        engineTutorialData.instructionIcons.push_back({ instruction_icons[i], i });

    string EngineTutorialData = compress_gzip(json_encode(engineTutorialData.toJsonObject()));
    ofstream fout = ofstream(dist + "/EngineTutorialData");
    fout.write(EngineTutorialData.c_str(), EngineTutorialData.size());
}

#define defineTutorialPreprocess(func) bool unused_##func##_unused = [](){ Preprocesses.push_back(func); return true; }()
#define defineTutorialSegment(func, duration) bool unused_##func##_unused = [](){ TemporarySegments.push_back({ func, duration }); return true; }()
#define defineTutorialSegmentsGroup(name) bool unused_##name##_unused = [](){ Segments.push_back(TemporarySegments); TemporarySegments.clear(); return true; }()
#define defineTutorialStatic(func) bool unused_##func##_unused = [](){ Statics.push_back(func); return true; }()

#endif
/*** End of inlined file: Build.h ***/

#endif
#ifdef preview

/*** Start of inlined file: Blocks.h ***/
#ifndef PreviewBlocks_H
#define PreviewBlocks_H

const int RuntimeEnvironmentId = 1000;
const int RuntimeCanvasId = 1001;
const int RuntimeSkinTransformId = 1002;
const int RuntimeUIId = 1003;
const int RuntimeUIConfigurationId = 1004;
const int PreviewDataId = 2000;
const int PreviewOptionId = 2001;
const int EngineRomId = 3000;
const int EntityDataId = 4000;
const int EntitySharedMemoryId = 4001;
const int EntityInfoId = 4002;
const int EntityDataArrayId = 4100;
const int EntitySharedMemoryArrayId = 4101;
const int EntityInfoArrayId = 4102;
const int TemporaryMemoryId = 10000;

bool unused_readable_unused = [](){
	readable = {
		{ RuntimeEnvironmentId, { "preprocess", "render" } },
		{ RuntimeCanvasId, { "preprocess", "render" } },
		{ RuntimeSkinTransformId, { "preprocess", "render" } },
		{ RuntimeUIId, { "preprocess", "render" } },
		{ RuntimeUIConfigurationId, { "preprocess", "render" } },
		{ PreviewDataId, { "preprocess", "render" } },
		{ PreviewOptionId, { "preprocess", "render" } },
		{ EngineRomId, { "preprocess", "render" } },
		{ EntityDataId, { "preprocess", "render" } },
		{ EntitySharedMemoryId, { "preprocess", "render" } },
		{ EntityInfoId, { "preprocess", "render" } },
		{ EntityDataArrayId, { "preprocess", "render" } },
		{ EntitySharedMemoryArrayId, { "preprocess", "render" } },
		{ EntityInfoArrayId, { "preprocess", "render" } },
		{ TemporaryMemoryId, { "preprocess", "render" } }
	};
	return true;
}();

bool unused_writable_unused = [](){
	writable = {
		{ RuntimeEnvironmentId, { "preprocess" } },
		{ RuntimeCanvasId, { "preprocess" } },
		{ RuntimeSkinTransformId, { "preprocess" } },
		{ RuntimeUIId, { "preprocess" } },
		{ RuntimeUIConfigurationId, { "preprocess" } },
		{ PreviewDataId, { "preprocess" } },
		{ PreviewOptionId, {  } },
		{ EngineRomId, {  } },
		{ EntityDataId, { "preprocess" } },
		{ EntitySharedMemoryId, { "preprocess" } },
		{ EntityInfoId, {  } },
		{ EntityDataArrayId, { "preprocess" } },
		{ EntitySharedMemoryArrayId, { "preprocess" } },
		{ EntityInfoArrayId, {  } },
		{ TemporaryMemoryId, { "preprocess", "render" } }
	};
	return true;
}();

bool unused_blockName_unused = [](){
	blockName = {
		{ RuntimeEnvironmentId, "RuntimeEnvironment" },
		{ RuntimeCanvasId, "RuntimeCanvas" },
		{ RuntimeSkinTransformId, "RuntimeSkinTransform" },
		{ RuntimeUIId, "RuntimeUI" },
		{ RuntimeUIConfigurationId, "RuntimeUIConfiguration" },
		{ PreviewDataId, "PreviewData" },
		{ PreviewOptionId, "PreviewOption" },
		{ EngineRomId, "EngineRom" },
		{ EntityDataId, "EntityData" },
		{ EntitySharedMemoryId, "EntitySharedMemory" },
		{ EntityInfoId, "EntityInfo" },
		{ EntityDataArrayId, "EntityDataArray" },
		{ EntitySharedMemoryArrayId, "EntitySharedMemoryArray" },
		{ EntityInfoArrayId, "EntityInfoArray" },
		{ TemporaryMemoryId, "TemporaryMemory" }
	};
	return true;
}();

bool unused_writeBlockInfo_unused = [](){
	SonolusMemorySize[PreviewDataId] = 4096;
	SonolusMemoryDelta[PreviewDataId] = 1;
	allowAllocate.insert(PreviewDataId);
	SonolusMemorySize[EntityDataId] = 32;
	SonolusMemoryDelta[EntityDataId] = 1;
	allowAllocate.insert(EntityDataId);
	SonolusMemorySize[EntitySharedMemoryId] = 32;
	SonolusMemoryDelta[EntitySharedMemoryId] = 1;
	allowAllocate.insert(EntitySharedMemoryId);
	SonolusMemorySize[TemporaryMemoryId] = 4096;
	SonolusMemoryDelta[TemporaryMemoryId] = 1;
	allowAllocate.insert(TemporaryMemoryId);
	return true;
}();

struct RuntimeEnvironmentGroup{
	Variable debugMode = Variable(RuntimeEnvironmentId, 0);
	Variable screenAspectRatio = Variable(RuntimeEnvironmentId, 0);

	Constructor RuntimeEnvironmentGroup(FuncNode offset) {
		debugMode.reallocate(offset + 0);
		screenAspectRatio.reallocate(offset + 1);
	}
};
RuntimeEnvironmentGroup RuntimeEnvironment = RuntimeEnvironmentGroup(0);

struct RuntimeCanvasGroup{
	Variable scroll = Variable(RuntimeCanvasId, 0);
	Variable size = Variable(RuntimeCanvasId, 0);

	Constructor RuntimeCanvasGroup(FuncNode offset) {
		scroll.reallocate(offset + 0);
		size.reallocate(offset + 1);
	}
};
RuntimeCanvasGroup RuntimeCanvas = RuntimeCanvasGroup(0);

VariablesArray<RuntimeSkinTransformId> RuntimeSkinTransform = VariablesArray<RuntimeSkinTransformId>(0, 1 << 16);

struct RuntimeUIGroup{
	struct Group_r7lLuofqbV4aIOsl{
		Variable anchorX = Variable(RuntimeUIId, 0);
		Variable anchorY = Variable(RuntimeUIId, 0);
		Variable pivotX = Variable(RuntimeUIId, 0);
		Variable pivotY = Variable(RuntimeUIId, 0);
		Variable width = Variable(RuntimeUIId, 0);
		Variable height = Variable(RuntimeUIId, 0);
		Variable rotation = Variable(RuntimeUIId, 0);
		Variable alpha = Variable(RuntimeUIId, 0);
		Variable background = Variable(RuntimeUIId, 0);

		void reallocate(FuncNode offset) {
			anchorX.reallocate(offset + 0);
			anchorY.reallocate(offset + 1);
			pivotX.reallocate(offset + 2);
			pivotY.reallocate(offset + 3);
			width.reallocate(offset + 4);
			height.reallocate(offset + 5);
			rotation.reallocate(offset + 6);
			alpha.reallocate(offset + 7);
			background.reallocate(offset + 8);
		}
	};
	Group_r7lLuofqbV4aIOsl menu;
	Group_r7lLuofqbV4aIOsl progress;

	Constructor RuntimeUIGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		progress.reallocate(offset + 9);
	}
};
RuntimeUIGroup RuntimeUI = RuntimeUIGroup(0);

struct RuntimeUIConfigurationGroup{
	struct Group_2r2W6GSKyZAfOsND{
		Variable scale = Variable(RuntimeUIConfigurationId, 0);
		Variable alpha = Variable(RuntimeUIConfigurationId, 0);

		void reallocate(FuncNode offset) {
			scale.reallocate(offset + 0);
			alpha.reallocate(offset + 1);
		}
	};
	Group_2r2W6GSKyZAfOsND menu;
	Group_2r2W6GSKyZAfOsND progress;

	Constructor RuntimeUIConfigurationGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		progress.reallocate(offset + 2);
	}
};
RuntimeUIConfigurationGroup RuntimeUIConfiguration = RuntimeUIConfigurationGroup(0);

VariablesArray<PreviewDataId> PreviewData = VariablesArray<PreviewDataId>(0, 1 << 16);

VariablesArray<PreviewOptionId> PreviewOption = VariablesArray<PreviewOptionId>(0, 1 << 16);

VariablesArray<EngineRomId> EngineRom = VariablesArray<EngineRomId>(0, 1 << 16);

VariablesArray<EntityDataId> EntityData = VariablesArray<EntityDataId>(0, 1 << 16);

VariablesArray<EntitySharedMemoryId> EntitySharedMemory = VariablesArray<EntitySharedMemoryId>(0, 1 << 16);

struct EntityInfoGroup{
	Variable index = Variable(EntityInfoId, 0);
	Variable archetype = Variable(EntityInfoId, 0);

	Constructor EntityInfoGroup(FuncNode offset) {
		index.reallocate(offset + 0);
		archetype.reallocate(offset + 1);
	}
};
EntityInfoGroup EntityInfo = EntityInfoGroup(0);

struct EntityDataArrayGroup{
	VariablesArray<EntityDataArrayId> generic = VariablesArray<EntityDataArrayId>(0, 32);

	Constructor EntityDataArrayGroup(FuncNode offset) {
		generic = VariablesArray<EntityDataArrayId>(offset + 0, 32);
	}
};
BlocksArray<EntityDataArrayGroup> EntityDataArray = BlocksArray<EntityDataArrayGroup>(32);

struct EntitySharedMemoryArrayGroup{
	VariablesArray<EntitySharedMemoryArrayId> generic = VariablesArray<EntitySharedMemoryArrayId>(0, 32);

	Constructor EntitySharedMemoryArrayGroup(FuncNode offset) {
		generic = VariablesArray<EntitySharedMemoryArrayId>(offset + 0, 32);
	}
};
BlocksArray<EntitySharedMemoryArrayGroup> EntitySharedMemoryArray = BlocksArray<EntitySharedMemoryArrayGroup>(32);

struct EntityInfoArrayGroup{
	Variable index = Variable(EntityInfoArrayId, 0);
	Variable archetype = Variable(EntityInfoArrayId, 0);

	Constructor EntityInfoArrayGroup(FuncNode offset) {
		index.reallocate(offset + 0);
		archetype.reallocate(offset + 1);
	}
};
BlocksArray<EntityInfoArrayGroup> EntityInfoArray = BlocksArray<EntityInfoArrayGroup>(2);

VariablesArray<TemporaryMemoryId> TemporaryMemory = VariablesArray<TemporaryMemoryId>(0, 1 << 16);

#endif

/*** End of inlined file: Blocks.h ***/



/*** Start of inlined file: Runtime.h ***/
#ifndef PreviewRuntime_H
#define PreviewRuntime_H

const int OptionId = PreviewOptionId;
auto unused_defaultAllocatorId_unused = [](){
    defaultAllocatorId = PreviewDataId;
    createNodeContainer();
    return true;
}();

Variable &debugMode = RuntimeEnvironment.debugMode;
Variable &aspectRatio = RuntimeEnvironment.screenAspectRatio;
Variable &scroll = RuntimeCanvas.scroll;
Variable &canvasSize = RuntimeCanvas.size;
struct transforms {
    VariablesArray<RuntimeSkinTransformId> &skin = RuntimeSkinTransform;
}transforms;
RuntimeUIGroup &ui = RuntimeUI;
RuntimeUIConfigurationGroup &configuration = RuntimeUIConfiguration;

template<typename T>
SonolusApi setUI(T &ui, var anchorX, var anchorY, var pivotX, var pivotY, var width, var height, var rotation, var alpha, var background) {
    ui.anchorX = anchorX; ui.anchorY = anchorY;
    ui.pivotX = pivotX; ui.pivotY = pivotY;
    ui.width = width; ui.height = height;
    ui.rotation = rotation;
    ui.alpha = alpha;
    ui.background = background;
}

#endif
/*** End of inlined file: Runtime.h ***/


/*** Start of inlined file: Archetype.h ***/
#ifndef PreviewArchetype_H
#define PreviewArchetype_H

class Archetype {
    public:
    // Archetype id
    int id = 0;

    // Entity Shared Memory block is used as generic memory storage that are accessible by other entities.
    VariablesArray<EntitySharedMemoryId> &sharedMemory = EntitySharedMemory;
    // Entity Info block contains state information of the entity.
    EntityInfoGroup &info = EntityInfo;

    public:
    // Name of this archetype, which will be referenced by entities in level data.
    // Special archetype names will have its effect applied automatically:
    // `#BPM_CHANGE`: Signals a BPM change and will affect BPM change related functions. Entity with this archetype must also provide data named `#BEAT` and `#BPM`.
    // `#TIMESCALE_CHANGE`: Signals a time scale change and will affect time scale change related functions. Entity with this archetype must also provide data named `#BEAT` and `#TIMESCALE`.
    string name = "Default Archetype";
    // Imported data to be inject into Entity Data block.
    // Imported data with matching `name` will be injected at `index` of Entity Data block.
    vector<pair<string, int> > imports;

    double preprocessOrder = 0;
    SonolusApi preprocess() { return 0; }

    double renderOrder = 0;
    SonolusApi render() { return 0; }
};

#define defineImport(name) bool unused_##name##_unused = [&](){ \
        FuncNode id = imports.size(); \
        imports.push_back({ #name, imports.size() }); \
        return true; \
    }(); \
    Variable name = Variable(EntityDataId, imports.size() - 1)
#define defineImportDetailed(varName, jsonName) bool unused_##varName##_unused = [&](){ \
        FuncNode id = imports.size(); \
		imports.push_back({ jsonName, imports.size() }); \
        return true; \
    }(); \
    Variable varName = Variable(EntityDataId, imports.size() - 1)

#endif
/*** End of inlined file: Archetype.h ***/


/*** Start of inlined file: Build.h ***/
#ifndef PreviewBuild_H
#define PreviewBuild_H

class EnginePreviewDataArchetype {
    public:

    string name = "";
    pair<int, double> preprocess = {0, 0};
    pair<int, double> render = {0, 0};
    vector<pair<string, int>> imports = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["preprocess"]["index"] = preprocess.first;
        res["preprocess"]["order"] = preprocess.second;
        res["render"]["index"] = render.first;
        res["render"]["order"] = render.second;
        res["imports"].resize(0);
        for (int i = 0; i < imports.size(); i++)
        {
            Json::Value single;
            single["name"] = imports[i].first;
            single["index"] = imports[i].second;
            res["imports"].append(single);
        }
        return res;
    }
};

class EnginePreviewData {
    public:

    string skinRenderMode = "default";
    vector<pair<string, int> > skinSprites = {};
    vector<EnginePreviewDataArchetype> archetypes = {};
    vector<DataNode> nodes = {};

    Json::Value toJsonObject() {
        Json::Value res;
        SonolusAssert(
            set<string>({"default", "standard", "lightweight"}).count(skinRenderMode),
            "`skinRenderMode` must be `default`, `standard` or `lightweight`!"
        );
        res["skin"]["renderMode"] = skinRenderMode;
        res["skin"]["sprites"].resize(0);
        for (int i = 0; i < skinSprites.size(); i++) {
            Json::Value single;
            single["name"] = skinSprites[i].first;
            single["id"] = skinSprites[i].second;
            res["skin"]["sprites"].append(single);
        }
        res["archetypes"].resize(0);
        for (int i = 0; i < archetypes.size(); i++)
            res["archetypes"].append(archetypes[i].toJsonObject());
        res["nodes"].resize(0);
        for (int i = 0; i < nodes.size(); i++)
            res["nodes"].append(nodes[i].toJsonObject());
        return res;
    }
}enginePreviewData;

#if false
// 用宏定义定义一个虚假函数，让解释器识别到这个函数并自动加到 UserDefined 函数列表中
void BuildData() {}
#endif
// BuildData 宏定义一定要禁止解释器解析，不然解释器会识别到两个同名的宏定义然后出现错误
#define DISABLE_INTERPRETER
#ifndef COMPILE_RUNTIME
// 供 IDE 识别的 BuildData 函数
#define BuildData(...) [&]() {\
    allocateArchetypeId(#__VA_ARGS__); \
    BuildArchetypes<__VA_ARGS__>(); \
    BuildPreviewData(); }()
#else
// 供编译器识别的 BuildData 函数
#define BuildData(arg1, arg2, arg3, arg4, arg5, ...) [&]() {\
    allocateArchetypeId(#__VA_ARGS__); \
    BuildArchetypes<__VA_ARGS__>(arg1, arg2, arg3, arg4, arg5); \
    BuildPreviewData(arg1, arg2, arg3, arg4, arg5); }()
#endif
#undef DISABLE_INTERPRETER

map<string, int> archetypeId;
int archetypeNumber = 0;
Blocked void allocateArchetypeId(string archetypeList) {
    string tmp = "";
    int id = 0;
    for (int i = 0; i < archetypeList.size(); i++) {
        if (archetypeList[i] == ',') archetypeId[tmp] = id++, tmp = "";
        else if (isalnum(archetypeList[i])) tmp += archetypeList[i];
    }
    if (tmp != "") archetypeId[tmp] = id++;
    archetypeNumber = id;
}

#define getAid(archetype) (archetypeId.find(#archetype) == archetypeId.end() ? -1 : archetypeId[#archetype])
#define buildCallback(name) [&]() { \
    createNodeContainer(); \
    defaultAllocatorId = TemporaryMemoryId; \
    callbackName = #name; \
    archetype.name(); \
    dataArchetype.name.first = mergeNodeContainer().getNodeId(); \
    dataArchetype.name.second = archetype.name##Order; }()

template <typename T>
void BuildArchetype(T &archetype) {
    EnginePreviewDataArchetype dataArchetype;
    dataArchetype.name = archetype.name;
    dataArchetype.imports = archetype.imports;

    buildCallback(preprocess);
    buildCallback(render);

    enginePreviewData.archetypes.push_back(dataArchetype);
}

template <typename T, typename... Args>
void BuildArchetypes(int aid = 0) {
    defaultAllocatorId = EntitySharedMemoryId;
    SonolusMemoryDelta[EntitySharedMemoryId] = 1;
    T archetype = T();
    archetypeName = archetype.name;
    archetype.id = aid;
    BuildArchetype(archetype);

#define DISABLE_INTERPRETER
#ifdef COMPILE_RUNTIME
    if constexpr (sizeof...(Args))
        BuildArchetypes<Args...>(callFromFuncParam, callFromFunc, callFromLine, callFromColumn, callFromFile, aid + 1);
#else
    if constexpr (sizeof...(Args))
        BuildArchetypes<Args...>();
#endif
#undef DISABLE_INTERPRETER
}

void BuildPreviewData() {
    enginePreviewData.nodes = dataContainer;
    enginePreviewData.skinSprites.clear();
    for (int i = 0; i < skin_sprites.size(); i++)
        enginePreviewData.skinSprites.push_back({ skin_sprites[i].first, i });

    string EnginePreviewData = compress_gzip(json_encode(enginePreviewData.toJsonObject()));
    ofstream fout = ofstream(dist + "/EnginePreviewData");
    fout.write(EnginePreviewData.c_str(), EnginePreviewData.size());
}

#endif
/*** End of inlined file: Build.h ***/

#endif
#ifdef watch

/*** Start of inlined file: Blocks.h ***/
#ifndef WatchBlocks_H
#define WatchBlocks_H

const int RuntimeEnvironmentId = 1000;
const int RuntimeUpdateId = 1001;
const int RuntimeSkinTransformId = 1002;
const int RuntimeParticleTransformId = 1003;
const int RuntimeBackgroundId = 1004;
const int RuntimeUIId = 1005;
const int RuntimeUIConfigurationId = 1006;
const int LevelMemoryId = 2000;
const int LevelDataId = 2001;
const int LevelOptionId = 2002;
const int LevelBucketId = 2003;
const int LevelScoreId = 2004;
const int LevelLifeId = 2005;
const int EngineRomId = 3000;
const int EntityMemoryId = 4000;
const int EntityDataId = 4001;
const int EntitySharedMemoryId = 4002;
const int EntityInfoId = 4003;
const int EntityInputId = 4004;
const int EntityDataArrayId = 4101;
const int EntitySharedMemoryArrayId = 4102;
const int EntityInfoArrayId = 4103;
const int ArchetypeLifeId = 5000;
const int TemporaryMemoryId = 10000;

bool unused_readable_unused = [](){
	readable = {
		{ RuntimeEnvironmentId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ RuntimeUpdateId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ RuntimeSkinTransformId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ RuntimeParticleTransformId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ RuntimeBackgroundId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ RuntimeUIId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ RuntimeUIConfigurationId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ LevelMemoryId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ LevelDataId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ LevelOptionId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ LevelBucketId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ LevelScoreId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ LevelLifeId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ EngineRomId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ EntityMemoryId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate" } },
		{ EntityDataId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate" } },
		{ EntitySharedMemoryId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate" } },
		{ EntityInfoId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate" } },
		{ EntityInputId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate" } },
		{ EntityDataArrayId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ EntitySharedMemoryArrayId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ EntityInfoArrayId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ ArchetypeLifeId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } },
		{ TemporaryMemoryId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } }
	};
	return true;
}();

bool unused_writable_unused = [](){
	writable = {
		{ RuntimeEnvironmentId, { "preprocess" } },
		{ RuntimeUpdateId, {  } },
		{ RuntimeSkinTransformId, { "preprocess", "updateSequential" } },
		{ RuntimeParticleTransformId, { "preprocess", "updateSequential" } },
		{ RuntimeBackgroundId, { "preprocess", "updateSequential" } },
		{ RuntimeUIId, { "preprocess" } },
		{ RuntimeUIConfigurationId, { "preprocess" } },
		{ LevelMemoryId, { "preprocess", "updateSequential" } },
		{ LevelDataId, { "preprocess" } },
		{ LevelOptionId, {  } },
		{ LevelBucketId, { "preprocess" } },
		{ LevelScoreId, { "preprocess" } },
		{ LevelLifeId, { "preprocess" } },
		{ EngineRomId, {  } },
		{ EntityMemoryId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate" } },
		{ EntityDataId, { "preprocess" } },
		{ EntitySharedMemoryId, { "preprocess", "updateSequential" } },
		{ EntityInfoId, {  } },
		{ EntityInputId, { "preprocess" } },
		{ EntityDataArrayId, { "preprocess" } },
		{ EntitySharedMemoryArrayId, { "preprocess", "updateSequential" } },
		{ EntityInfoArrayId, {  } },
		{ ArchetypeLifeId, { "preprocess" } },
		{ TemporaryMemoryId, { "preprocess", "spawnTime", "despawnTime", "initialize", "updateSequential", "updateParallel", "terminate", "updateSpawn" } }
	};
	return true;
}();

bool unused_blockName_unused = [](){
	blockName = {
		{ RuntimeEnvironmentId, "RuntimeEnvironment" },
		{ RuntimeUpdateId, "RuntimeUpdate" },
		{ RuntimeSkinTransformId, "RuntimeSkinTransform" },
		{ RuntimeParticleTransformId, "RuntimeParticleTransform" },
		{ RuntimeBackgroundId, "RuntimeBackground" },
		{ RuntimeUIId, "RuntimeUI" },
		{ RuntimeUIConfigurationId, "RuntimeUIConfiguration" },
		{ LevelMemoryId, "LevelMemory" },
		{ LevelDataId, "LevelData" },
		{ LevelOptionId, "LevelOption" },
		{ LevelBucketId, "LevelBucket" },
		{ LevelScoreId, "LevelScore" },
		{ LevelLifeId, "LevelLife" },
		{ EngineRomId, "EngineRom" },
		{ EntityMemoryId, "EntityMemory" },
		{ EntityDataId, "EntityData" },
		{ EntitySharedMemoryId, "EntitySharedMemory" },
		{ EntityInfoId, "EntityInfo" },
		{ EntityInputId, "EntityInput" },
		{ EntityDataArrayId, "EntityDataArray" },
		{ EntitySharedMemoryArrayId, "EntitySharedMemoryArray" },
		{ EntityInfoArrayId, "EntityInfoArray" },
		{ ArchetypeLifeId, "ArchetypeLife" },
		{ TemporaryMemoryId, "TemporaryMemory" }
	};
	return true;
}();

bool unused_writeBlockInfo_unused = [](){
	SonolusMemorySize[LevelMemoryId] = 4096;
	SonolusMemoryDelta[LevelMemoryId] = 1;
	allowAllocate.insert(LevelMemoryId);
	SonolusMemorySize[LevelDataId] = 4096;
	SonolusMemoryDelta[LevelDataId] = 1;
	allowAllocate.insert(LevelDataId);
	SonolusMemorySize[EntityMemoryId] = 64;
	SonolusMemoryDelta[EntityMemoryId] = 1;
	allowAllocate.insert(EntityMemoryId);
	SonolusMemorySize[EntityDataId] = 32;
	SonolusMemoryDelta[EntityDataId] = 1;
	allowAllocate.insert(EntityDataId);
	SonolusMemorySize[EntitySharedMemoryId] = 32;
	SonolusMemoryDelta[EntitySharedMemoryId] = 1;
	allowAllocate.insert(EntitySharedMemoryId);
	SonolusMemorySize[TemporaryMemoryId] = 4096;
	SonolusMemoryDelta[TemporaryMemoryId] = 1;
	allowAllocate.insert(TemporaryMemoryId);
	return true;
}();

struct RuntimeEnvironmentGroup{
	Variable debugMode = Variable(RuntimeEnvironmentId, 0);
	Variable screenAspectRatio = Variable(RuntimeEnvironmentId, 0);
	Variable audioOffset = Variable(RuntimeEnvironmentId, 0);
	Variable inputOffset = Variable(RuntimeEnvironmentId, 0);
	Variable replay = Variable(RuntimeEnvironmentId, 0);

	Constructor RuntimeEnvironmentGroup(FuncNode offset) {
		debugMode.reallocate(offset + 0);
		screenAspectRatio.reallocate(offset + 1);
		audioOffset.reallocate(offset + 2);
		inputOffset.reallocate(offset + 3);
		replay.reallocate(offset + 4);
	}
};
RuntimeEnvironmentGroup RuntimeEnvironment = RuntimeEnvironmentGroup(0);

struct RuntimeUpdateGroup{
	Variable time = Variable(RuntimeUpdateId, 0);
	Variable deltaTime = Variable(RuntimeUpdateId, 0);
	Variable scaledTime = Variable(RuntimeUpdateId, 0);
	Variable skip = Variable(RuntimeUpdateId, 0);

	Constructor RuntimeUpdateGroup(FuncNode offset) {
		time.reallocate(offset + 0);
		deltaTime.reallocate(offset + 1);
		scaledTime.reallocate(offset + 2);
		skip.reallocate(offset + 3);
	}
};
RuntimeUpdateGroup RuntimeUpdate = RuntimeUpdateGroup(0);

VariablesArray<RuntimeSkinTransformId> RuntimeSkinTransform = VariablesArray<RuntimeSkinTransformId>(0, 1 << 16);

VariablesArray<RuntimeParticleTransformId> RuntimeParticleTransform = VariablesArray<RuntimeParticleTransformId>(0, 1 << 16);

struct RuntimeBackgroundGroup{
	Variable x = Variable(RuntimeBackgroundId, 0);
	Variable y = Variable(RuntimeBackgroundId, 0);

	Constructor RuntimeBackgroundGroup(FuncNode offset) {
		x.reallocate(offset + 0);
		y.reallocate(offset + 1);
	}
};
BlocksArray<RuntimeBackgroundGroup> RuntimeBackground = BlocksArray<RuntimeBackgroundGroup>(2);

struct RuntimeUIGroup{
	struct Group_5jlRR5Aemi1MiGcH{
		Variable anchorX = Variable(RuntimeUIId, 0);
		Variable anchorY = Variable(RuntimeUIId, 0);
		Variable pivotX = Variable(RuntimeUIId, 0);
		Variable pivotY = Variable(RuntimeUIId, 0);
		Variable width = Variable(RuntimeUIId, 0);
		Variable height = Variable(RuntimeUIId, 0);
		Variable rotation = Variable(RuntimeUIId, 0);
		Variable alpha = Variable(RuntimeUIId, 0);
		Variable horizontalAlign = Variable(RuntimeUIId, 0);
		Variable background = Variable(RuntimeUIId, 0);

		void reallocate(FuncNode offset) {
			anchorX.reallocate(offset + 0);
			anchorY.reallocate(offset + 1);
			pivotX.reallocate(offset + 2);
			pivotY.reallocate(offset + 3);
			width.reallocate(offset + 4);
			height.reallocate(offset + 5);
			rotation.reallocate(offset + 6);
			alpha.reallocate(offset + 7);
			horizontalAlign.reallocate(offset + 8);
			background.reallocate(offset + 9);
		}
	};
	Group_5jlRR5Aemi1MiGcH menu;
	Group_5jlRR5Aemi1MiGcH judgment;
	Group_5jlRR5Aemi1MiGcH comboValue;
	Group_5jlRR5Aemi1MiGcH comboText;
	Group_5jlRR5Aemi1MiGcH primaryMetricBar;
	Group_5jlRR5Aemi1MiGcH primaryMetricValue;
	Group_5jlRR5Aemi1MiGcH secondaryMetricBar;
	Group_5jlRR5Aemi1MiGcH secondaryMetricValue;
	Group_5jlRR5Aemi1MiGcH progress;

	Constructor RuntimeUIGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		judgment.reallocate(offset + 10);
		comboValue.reallocate(offset + 20);
		comboText.reallocate(offset + 30);
		primaryMetricBar.reallocate(offset + 40);
		primaryMetricValue.reallocate(offset + 50);
		secondaryMetricBar.reallocate(offset + 60);
		secondaryMetricValue.reallocate(offset + 70);
		progress.reallocate(offset + 80);
	}
};
RuntimeUIGroup RuntimeUI = RuntimeUIGroup(0);

struct RuntimeUIConfigurationGroup{
	struct Group_xA3YUqGE5K1dBrgG{
		Variable scale = Variable(RuntimeUIConfigurationId, 0);
		Variable alpha = Variable(RuntimeUIConfigurationId, 0);

		void reallocate(FuncNode offset) {
			scale.reallocate(offset + 0);
			alpha.reallocate(offset + 1);
		}
	};
	Group_xA3YUqGE5K1dBrgG menu;
	Group_xA3YUqGE5K1dBrgG judgment;
	Group_xA3YUqGE5K1dBrgG combo;
	Group_xA3YUqGE5K1dBrgG primaryMetric;
	Group_xA3YUqGE5K1dBrgG secondaryMetric;
	Group_xA3YUqGE5K1dBrgG progress;

	Constructor RuntimeUIConfigurationGroup(FuncNode offset) {
		menu.reallocate(offset + 0);
		judgment.reallocate(offset + 2);
		combo.reallocate(offset + 4);
		primaryMetric.reallocate(offset + 6);
		secondaryMetric.reallocate(offset + 8);
		progress.reallocate(offset + 10);
	}
};
RuntimeUIConfigurationGroup RuntimeUIConfiguration = RuntimeUIConfigurationGroup(0);

VariablesArray<LevelMemoryId> LevelMemory = VariablesArray<LevelMemoryId>(0, 1 << 16);

VariablesArray<LevelDataId> LevelData = VariablesArray<LevelDataId>(0, 1 << 16);

VariablesArray<LevelOptionId> LevelOption = VariablesArray<LevelOptionId>(0, 1 << 16);

struct LevelBucketGroup{
	struct Group_ZRf0VocHWcbFsBUn{
		Variable min = Variable(LevelBucketId, 0);
		Variable max = Variable(LevelBucketId, 0);

		void reallocate(FuncNode offset) {
			min.reallocate(offset + 0);
			max.reallocate(offset + 1);
		}
	};
	Group_ZRf0VocHWcbFsBUn perfect;
	Group_ZRf0VocHWcbFsBUn great;
	Group_ZRf0VocHWcbFsBUn good;

	Constructor LevelBucketGroup(FuncNode offset) {
		perfect.reallocate(offset + 0);
		great.reallocate(offset + 2);
		good.reallocate(offset + 4);
	}
};
BlocksArray<LevelBucketGroup> LevelBucket = BlocksArray<LevelBucketGroup>(6);

struct LevelScoreGroup{
	Variable perfect_multiplier = Variable(LevelScoreId, 0);
	Variable great_multiplier = Variable(LevelScoreId, 0);
	Variable good_multiplier = Variable(LevelScoreId, 0);
	struct Group_MVKoMY2Rs141siHP{
		Variable consecutiveMultiplier = Variable(LevelScoreId, 0);
		Variable consecutiveStep = Variable(LevelScoreId, 0);
		Variable consecutiveCap = Variable(LevelScoreId, 0);

		void reallocate(FuncNode offset) {
			consecutiveMultiplier.reallocate(offset + 0);
			consecutiveStep.reallocate(offset + 1);
			consecutiveCap.reallocate(offset + 2);
		}
	};
	Group_MVKoMY2Rs141siHP perfect;
	Group_MVKoMY2Rs141siHP great;
	Group_MVKoMY2Rs141siHP good;

	Constructor LevelScoreGroup(FuncNode offset) {
		perfect_multiplier.reallocate(offset + 0);
		great_multiplier.reallocate(offset + 1);
		good_multiplier.reallocate(offset + 2);
		perfect.reallocate(offset + 3);
		great.reallocate(offset + 6);
		good.reallocate(offset + 9);
	}
};
LevelScoreGroup LevelScore = LevelScoreGroup(0);

struct LevelLifeGroup{
	struct Group_9vOej0tDaSSRevCy{
		Variable consecutiveIncrement = Variable(LevelLifeId, 0);
		Variable consecutiveStep = Variable(LevelLifeId, 0);

		void reallocate(FuncNode offset) {
			consecutiveIncrement.reallocate(offset + 0);
			consecutiveStep.reallocate(offset + 1);
		}
	};
	Group_9vOej0tDaSSRevCy perfect;
	Group_9vOej0tDaSSRevCy great;
	Group_9vOej0tDaSSRevCy good;

	Constructor LevelLifeGroup(FuncNode offset) {
		perfect.reallocate(offset + 0);
		great.reallocate(offset + 2);
		good.reallocate(offset + 4);
	}
};
LevelLifeGroup LevelLife = LevelLifeGroup(0);

VariablesArray<EngineRomId> EngineRom = VariablesArray<EngineRomId>(0, 1 << 16);

VariablesArray<EntityMemoryId> EntityMemory = VariablesArray<EntityMemoryId>(0, 1 << 16);

VariablesArray<EntityDataId> EntityData = VariablesArray<EntityDataId>(0, 1 << 16);

VariablesArray<EntitySharedMemoryId> EntitySharedMemory = VariablesArray<EntitySharedMemoryId>(0, 1 << 16);

struct EntityInfoGroup{
	Variable index = Variable(EntityInfoId, 0);
	Variable archetype = Variable(EntityInfoId, 0);
	Variable state = Variable(EntityInfoId, 0);

	Constructor EntityInfoGroup(FuncNode offset) {
		index.reallocate(offset + 0);
		archetype.reallocate(offset + 1);
		state.reallocate(offset + 2);
	}
};
EntityInfoGroup EntityInfo = EntityInfoGroup(0);

struct EntityInputGroup{
	Variable time = Variable(EntityInputId, 0);
	Variable bucketIndex = Variable(EntityInputId, 0);
	Variable bucketValue = Variable(EntityInputId, 0);

	Constructor EntityInputGroup(FuncNode offset) {
		time.reallocate(offset + 0);
		bucketIndex.reallocate(offset + 1);
		bucketValue.reallocate(offset + 2);
	}
};
EntityInputGroup EntityInput = EntityInputGroup(0);

struct EntityDataArrayGroup{
	VariablesArray<EntityDataArrayId> generic = VariablesArray<EntityDataArrayId>(0, 32);

	Constructor EntityDataArrayGroup(FuncNode offset) {
		generic = VariablesArray<EntityDataArrayId>(offset + 0, 32);
	}
};
BlocksArray<EntityDataArrayGroup> EntityDataArray = BlocksArray<EntityDataArrayGroup>(32);

struct EntitySharedMemoryArrayGroup{
	VariablesArray<EntitySharedMemoryArrayId> generic = VariablesArray<EntitySharedMemoryArrayId>(0, 32);

	Constructor EntitySharedMemoryArrayGroup(FuncNode offset) {
		generic = VariablesArray<EntitySharedMemoryArrayId>(offset + 0, 32);
	}
};
BlocksArray<EntitySharedMemoryArrayGroup> EntitySharedMemoryArray = BlocksArray<EntitySharedMemoryArrayGroup>(32);

struct EntityInfoArrayGroup{
	Variable index = Variable(EntityInfoArrayId, 0);
	Variable archetype = Variable(EntityInfoArrayId, 0);
	Variable state = Variable(EntityInfoArrayId, 0);

	Constructor EntityInfoArrayGroup(FuncNode offset) {
		index.reallocate(offset + 0);
		archetype.reallocate(offset + 1);
		state.reallocate(offset + 2);
	}
};
BlocksArray<EntityInfoArrayGroup> EntityInfoArray = BlocksArray<EntityInfoArrayGroup>(3);

struct ArchetypeLifeGroup{
	Variable perfect_increment = Variable(ArchetypeLifeId, 0);
	Variable great_increment = Variable(ArchetypeLifeId, 0);
	Variable good_increment = Variable(ArchetypeLifeId, 0);
	Variable miss_increment = Variable(ArchetypeLifeId, 0);

	Constructor ArchetypeLifeGroup(FuncNode offset) {
		perfect_increment.reallocate(offset + 0);
		great_increment.reallocate(offset + 1);
		good_increment.reallocate(offset + 2);
		miss_increment.reallocate(offset + 3);
	}
};
BlocksArray<ArchetypeLifeGroup> ArchetypeLife = BlocksArray<ArchetypeLifeGroup>(4);

VariablesArray<TemporaryMemoryId> TemporaryMemory = VariablesArray<TemporaryMemoryId>(0, 1 << 16);

#endif

/*** End of inlined file: Blocks.h ***/



/*** Start of inlined file: Runtime.h ***/
#ifndef WatchRuntime_H
#define WatchRuntime_H

const int OptionId = LevelOptionId;
auto unused_defaultAllocatorId_unused = [](){
    defaultAllocatorId = LevelMemoryId;
    createNodeContainer();
    return true;
}();

Variable &debugMode = RuntimeEnvironment.debugMode;
Variable &aspectRatio = RuntimeEnvironment.screenAspectRatio;
struct offset {
    Variable &audio = RuntimeEnvironment.audioOffset;
    Variable &input = RuntimeEnvironment.inputOffset;
}offsets;
Variable &replay = RuntimeEnvironment.replay;
struct times {
    Variable &now = RuntimeUpdate.time;
    Variable &delta = RuntimeUpdate.deltaTime;
    Variable &scaled = RuntimeUpdate.scaledTime;
}times;
Variable &skip = RuntimeUpdate.skip;
struct transforms {
    VariablesArray<RuntimeSkinTransformId> &skin = RuntimeSkinTransform;
    VariablesArray<RuntimeParticleTransformId> &particle = RuntimeParticleTransform;
}transforms;
RuntimeUIGroup &ui = RuntimeUI;
RuntimeUIConfigurationGroup &configuration = RuntimeUIConfiguration;
BlocksArray<LevelBucketGroup> &buckets = LevelBucket;
LevelScoreGroup &scores = LevelScore;
LevelLifeGroup &lifes = LevelLife;

template<typename T>
SonolusApi setUI(T &ui, var anchorX, var anchorY, var pivotX, var pivotY, var width, var height, var rotation, var alpha, var horizontalAlign, var background) {
    ui.anchorX = anchorX; ui.anchorY = anchorY;
    ui.pivotX = pivotX; ui.pivotY = pivotY;
    ui.width = width; ui.height = height;
    ui.rotation = rotation;
    ui.alpha = alpha;
    ui.horizontalAlign = horizontalAlign;
    ui.background = background;
}

SonolusApi setBucket(var bucketId, var perfectMin, var perfectMax, var greatMin, var greatMax, var goodMin, var goodMax) {
    buckets[bucketId].perfect.min = perfectMin;
    buckets[bucketId].perfect.max = perfectMax;
    buckets[bucketId].great.min = greatMin;
    buckets[bucketId].great.max = greatMax;
    buckets[bucketId].good.min = goodMin;
    buckets[bucketId].good.max = goodMax;
}

SonolusApi setScore(var perfect, var great, var good) {
    scores.perfect_multiplier = perfect;
    scores.great_multiplier = great;
    scores.good_multiplier = good;
}

#endif
/*** End of inlined file: Runtime.h ***/


/*** Start of inlined file: Archetype.h ***/
#ifndef WatchArchetype_H
#define WatchArchetype_H

class Archetype {
    public:
    // Archetype id
    int id = 0;

    // Entity Memory block is used as generic memory storage.
    VariablesArray<EntityMemoryId> &memory = EntityMemory;
    // Entity Shared Memory block is used as generic memory storage that are accessible by other entities.
    VariablesArray<EntitySharedMemoryId> &sharedMemory = EntitySharedMemory;
    // Entity Info block contains state information of the entity.
    EntityInfoGroup &info = EntityInfo;
    // Entity Input block is used by entities to pass input information back to Sonolus.
    EntityInputGroup &input = EntityInput;

    public:
    // Archetype Life block contains life configuration of each archetype.
    ArchetypeLifeGroup life = ArchetypeLife[id];

    // Name of this archetype, which will be referenced by entities in level data.
    // Special archetype names will have its effect applied automatically:
    // `#BPM_CHANGE`: Signals a BPM change and will affect BPM change related functions. Entity with this archetype must also provide data named `#BEAT` and `#BPM`.
    // `#TIMESCALE_CHANGE`: Signals a time scale change and will affect time scale change related functions. Entity with this archetype must also provide data named `#BEAT` and `#TIMESCALE`.
    string name = "Default Archetype";
    // If true, Sonolus will treat entities with this archetype to be playable entities, and will contribute to judgment, combo, life, score, etc.
    bool hasInput = false;
    // Imported data to be inject into Entity Data block.
    // Imported data with matching `name` will be injected at `index` of Entity Data block.
    vector<pair<string, int> > imports;

    double preprocessOrder = 0;
    SonolusApi preprocess() { return 0; }

    double spawnTimeOrder = 0;
    SonolusApi spawnTime() { return 0; }

    double despawnTimeOrder = 0;
    SonolusApi despawnTime() { return 0; }

    double initializeOrder = 0;
    SonolusApi initialize() { return 0; }

    double updateSequentialOrder = 0;
    SonolusApi updateSequential() { return 0; }

    double updateParallelOrder = 0;
    SonolusApi updateParallel() { return 0; }

    double terminateOrder = 0;
    SonolusApi terminate() { return 0; }
};

#define defineImport(name) bool unused_##name##_unused = [&](){ \
        FuncNode id = imports.size(); \
        imports.push_back({ #name, imports.size() }); \
        return true; \
    }(); \
    Variable name = Variable(EntityDataId, imports.size() - 1)
#define defineImportDetailed(varName, jsonName) bool unused_##varName##_unused = [&](){ \
        FuncNode id = imports.size(); \
		imports.push_back({ jsonName, imports.size() }); \
        return true; \
    }(); \
    Variable varName = Variable(EntityDataId, imports.size() - 1)

#endif
/*** End of inlined file: Archetype.h ***/


/*** Start of inlined file: Build.h ***/
#ifndef WatchBuild_H
#define WatchBuild_H

class EngineWatchDataArchetype {
    public:

    string name = "";
    bool hasInput = false;
    pair<int, double> preprocess = {0, 0};
    pair<int, double> spawnTime = {0, 0};
    pair<int, double> despawnTime = {0, 0};
    pair<int, double> initialize = {0, 0};
    pair<int, double> updateSequential = {0, 0};
    pair<int, double> updateParallel = {0, 0};
    pair<int, double> terminate = {0, 0};
    vector<pair<string, int>> imports = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["hasInput"] = hasInput;
        res["preprocess"]["index"] = preprocess.first;
        res["preprocess"]["order"] = preprocess.second;
        res["spawnTime"]["index"] = spawnTime.first;
        res["spawnTime"]["order"] = spawnTime.second;
        res["despawnTime"]["index"] = despawnTime.first;
        res["despawnTime"]["order"] = despawnTime.second;
        res["initialize"]["index"] = initialize.first;
        res["initialize"]["order"] = initialize.second;
        res["updateSequential"]["index"] = updateSequential.first;
        res["updateSequential"]["order"] = updateSequential.second;
        res["updateParallel"]["index"] = updateParallel.first;
        res["updateParallel"]["order"] = updateParallel.second;
        res["terminate"]["index"] = terminate.first;
        res["terminate"]["order"] = terminate.second;
        res["imports"].resize(0);
        for (int i = 0; i < imports.size(); i++)
        {
            Json::Value single;
            single["name"] = imports[i].first;
            single["index"] = imports[i].second;
            res["imports"].append(single);
        }
        return res;
    }
};

class EngineWatchData {
    public:

    string skinRenderMode = "default";
    vector<pair<string, int> > skinSprites = {};
    vector<pair<string, int> > effectClips = {};
    vector<pair<string, int> > particleEffects = {};
    vector<EngineDataBucket> buckets = {};
    vector<EngineWatchDataArchetype> archetypes = {};
    vector<DataNode> nodes = {};
    int updateSpawn = 0;

    Json::Value toJsonObject() {
        Json::Value res;
        SonolusAssert(
            set<string>({"default", "standard", "lightweight"}).count(skinRenderMode),
            "`skinRenderMode` must be `default`, `standard` or `lightweight`!"
        );
        res["skin"]["renderMode"] = skinRenderMode;
        res["skin"]["sprites"].resize(0);
        for (int i = 0; i < skinSprites.size(); i++) {
            Json::Value single;
            single["name"] = skinSprites[i].first;
            single["id"] = skinSprites[i].second;
            res["skin"]["sprites"].append(single);
        }
        res["effect"]["clips"].resize(0);
        for (int i = 0; i < effectClips.size(); i++) {
            Json::Value single;
            single["name"] = effectClips[i].first;
            single["id"] = effectClips[i].second;
            res["effect"]["clips"].append(single);
        }
        res["particle"]["effects"].resize(0);
        for (int i = 0; i < particleEffects.size(); i++) {
            Json::Value single;
            single["name"] = particleEffects[i].first;
            single["id"] = particleEffects[i].second;
            res["particle"]["effects"].append(single);
        }
        res["buckets"].resize(0);
        for (int i = 0; i < buckets.size(); i++)
            res["buckets"].append(buckets[i].toJsonObject());
        res["archetypes"].resize(0);
        for (int i = 0; i < archetypes.size(); i++)
            res["archetypes"].append(archetypes[i].toJsonObject());
        res["nodes"].resize(0);
        for (int i = 0; i < nodes.size(); i++)
            res["nodes"].append(nodes[i].toJsonObject());
        SonolusAssert(
            updateSpawn,
            "Sonolus cannot accept a constant value in \"updateSpawn\" function. Did you forget to use \"defineUpdateSpawn\"?"
        );
        res["updateSpawn"] = updateSpawn;
        return res;
    }
}engineWatchData;

#if false
// 用宏定义定义一个虚假函数，让解释器识别到这个函数并自动加到 UserDefined 函数列表中
void BuildData() {}
#endif
// BuildData 宏定义一定要禁止解释器解析，不然解释器会识别到两个同名的宏定义然后出现错误
#define DISABLE_INTERPRETER
#ifndef COMPILE_RUNTIME
// 供 IDE 识别的 BuildData 函数
#define BuildData(...) [&]() {\
    allocateArchetypeId(#__VA_ARGS__); \
    BuildArchetypes<__VA_ARGS__>(); \
    BuildWatchData(); }()
#else
// 供编译器识别的 BuildData 函数
#define BuildData(arg1, arg2, arg3, arg4, arg5, ...) [&]() {\
    allocateArchetypeId(#__VA_ARGS__); \
    BuildArchetypes<__VA_ARGS__>(arg1, arg2, arg3, arg4, arg5); \
    BuildWatchData(arg1, arg2, arg3, arg4, arg5); }()
#endif
#undef DISABLE_INTERPRETER

map<string, int> archetypeId;
int archetypeNumber = 0;
Blocked void allocateArchetypeId(string archetypeList) {
    string tmp = "";
    int id = 0;
    for (int i = 0; i < archetypeList.size(); i++) {
        if (archetypeList[i] == ',') archetypeId[tmp] = id++, tmp = "";
        else if (isalnum(archetypeList[i])) tmp += archetypeList[i];
    }
    if (tmp != "") archetypeId[tmp] = id++;
    archetypeNumber = id;
}

#define getAid(archetype) (archetypeId.find(#archetype) == archetypeId.end() ? -1 : archetypeId[#archetype])
#define buildCallback(name) [&]() { \
    createNodeContainer(); \
    defaultAllocatorId = TemporaryMemoryId; \
    callbackName = #name; \
    archetype.name(); \
    dataArchetype.name.first = mergeNodeContainer().getNodeId(); \
    dataArchetype.name.second = archetype.name##Order; }()

template <typename T>
void BuildArchetype(T &archetype) {
    EngineWatchDataArchetype dataArchetype;
    dataArchetype.name = archetype.name;
    dataArchetype.hasInput = archetype.hasInput;
    dataArchetype.imports = archetype.imports;

    buildCallback(preprocess);
    buildCallback(spawnTime);
    buildCallback(despawnTime);
    buildCallback(initialize);
    buildCallback(updateSequential);
    buildCallback(updateParallel);
    buildCallback(terminate);

    engineWatchData.archetypes.push_back(dataArchetype);
}

template <typename T, typename... Args>
void BuildArchetypes(int aid = 0) {
    defaultAllocatorId = EntityMemoryId;
    SonolusMemoryDelta[EntityMemoryId] = 1;
    T archetype = T();
    archetypeName = archetype.name;
    archetype.id = aid;
    archetype.life.perfect_increment.reallocate(4 * aid + 0);
    archetype.life.great_increment.reallocate(4 * aid + 1);
    archetype.life.good_increment.reallocate(4 * aid + 2);
    archetype.life.miss_increment.reallocate(4 * aid + 3);
    BuildArchetype(archetype);

#define DISABLE_INTERPRETER
#ifdef COMPILE_RUNTIME
    if constexpr (sizeof...(Args))
        BuildArchetypes<Args...>(callFromFuncParam, callFromFunc, callFromLine, callFromColumn, callFromFile, aid + 1);
#else
    if constexpr (sizeof...(Args))
        BuildArchetypes<Args...>();
#endif
#undef DISABLE_INTERPRETER
}

void BuildWatchData() {
    engineWatchData.nodes = dataContainer;
    engineWatchData.skinSprites.clear();
    for (int i = 0; i < skin_sprites.size(); i++)
        engineWatchData.skinSprites.push_back({ skin_sprites[i].first, i });
    engineWatchData.effectClips.clear();
    for (int i = 0; i < effect_clips.size(); i++)
        engineWatchData.effectClips.push_back({ effect_clips[i].first, i });
    engineWatchData.particleEffects.clear();
    for (int i = 0; i < particle_effects.size(); i++)
        engineWatchData.particleEffects.push_back({ particle_effects[i].first, i });
    engineWatchData.buckets.clear();
    for (int i = 0; i < data_buckets.size(); i++)
        engineWatchData.buckets.push_back(data_buckets[i]);

    string EngineWatchData = compress_gzip(json_encode(engineWatchData.toJsonObject()));
    ofstream fout = ofstream(dist + "/EngineWatchData");
    fout.write(EngineWatchData.c_str(), EngineWatchData.size());
}

#define defineUpdateSpawn(func) bool unused_##func##_unused = []() { \
    createNodeContainer(); \
    defaultAllocatorId = TemporaryMemoryId; \
    callbackName = "updateSpawn"; \
    func(); \
    engineWatchData.updateSpawn = mergeNodeContainer().getNodeId(); \
    return true; }()

#endif
/*** End of inlined file: Build.h ***/

#endif

/*** Start of inlined file: Build.h ***/
#ifndef ConfigurationBuild_H
#define ConfigurationBuild_H

vector<Json::Value> engineConfigurationOptions;

/**
 * @brief Engine configuration option is used by Sonolus app to populate options section in level and engine configuration interface.
 *
 * This function can help you to create a `slider option` in Sonolus app, and it will return a proper `Variable` pointer which point to one memory in `LevelOption` or `PreviewOption` block.
 *
 * @param name The name of this option. Standardized option names starts with `#` and will be translated into client language when displayed. Special options names will have its effect applied automatically.
 * @param description Description will be displayed below this option. If you do not specify any description, it will not be displayed.
 * @param standard Whether this option is considered as a standard option. See wiki website for detailed information.
 * @param advanced If `advanced` is `true`, a sentence `This option is for advanced players.` will be displayed below this option.
 * @param scope Option values are automatically saved and shared between all levels. When a saved value matching the scope and name exists, it will be recalled.
 * @param def Default value of this slider option. `def` value must be between `min` value and `max` value.
 * @param min Minimal value range of this slider option. `min` value must be equal to or smaller than `max` value.
 * @param max Maximal value range of this slider option. `max` value must be equal to or greater than `min` value.
 * @param step Step value of this slider option. `step` value must be greater than `0`.
 * @param unit Unit of this slider option. `""` or `Text.PercentageUnit` is recommended.
 * @return `Variable<LevelOptionId>` or `Variable<PreviewOptionId>`, which depends on current build mode.
 *
 * @see <https://wiki.sonolus.com/engine-specs/resources/engine-configuration-option.html>
 */
Variable SliderOption(
    string name = "",
    string description = "",
    bool standard = false,
    bool advanced = false,
    string scope = "",
    double def = 0,
    double min = 0,
    double max = 100,
    double step = 1,
    string unit = ""
) {
    SonolusAssert(
        min <= def && def <= max,
        "`def` value must be equal to or greater than `min` value and be equal to or smaller than `max` value!"
    );
    SonolusAssert(
        min <= max,
        "`max` value must be equal to or greater than `min` value!"
    );
    SonolusAssert(
        step > 0,
        "`step` value must be greater than `0`!"
    );
    Json::Value res;
    res["name"] = name;
    res["description"] = description;
    res["standard"] = standard;
    res["advanced"] = advanced;
    res["scope"] = scope;
    res["type"] = "slider";
    res["def"] = def;
    res["min"] = min;
    res["max"] = max;
    res["step"] = step;
    res["unit"] = unit;
    engineConfigurationOptions.push_back(res);
    return Variable(OptionId, engineConfigurationOptions.size() - 1);
}

/**
 * @brief Engine configuration option is used by Sonolus app to populate options section in level and engine configuration interface.
 *
 * This function can help you to create a `toggle option` in Sonolus app, and it will return a proper `Variable` pointer which point to one memory in `LevelOption` or `PreviewOption` block.
 *
 * @param name The name of this option. Standardized option names starts with `#` and will be translated into client language when displayed. Special options names will have its effect applied automatically.
 * @param description Description will be displayed below this option. If you do not specify any description, it will not be displayed.
 * @param standard Whether this option is considered as a standard option. See wiki website for detailed information.
 * @param advanced If `advanced` is `true`, a sentence `This option is for advanced players.` will be displayed below this option.
 * @param scope Option values are automatically saved and shared between all levels. When a saved value matching the scope and name exists, it will be recalled.
 * @param def Default value of this slider option. Only `0` or `1` was accepted.
 * @return `Variable<LevelOptionId>` or `Variable<PreviewOptionId>`, which depends on current build mode.
 *
 * @see <https://wiki.sonolus.com/engine-specs/resources/engine-configuration-option.html>
 */
Variable ToggleOption(
    string name = "",
    string description = "",
    bool standard = false,
    bool advanced = false,
    string scope = "",
    bool def = false
) {
    Json::Value res;
    res["name"] = name;
    res["description"] = description;
    res["standard"] = standard;
    res["advanced"] = advanced;
    res["scope"] = scope;
    res["type"] = "toggle";
    res["def"] = def ? 1 : 0;
    engineConfigurationOptions.push_back(res);
    return Variable(OptionId, engineConfigurationOptions.size() - 1);
}

/**
 * @brief Engine configuration option is used by Sonolus app to populate options section in level and engine configuration interface.
 *
 * This function can help you to create a `select option` in Sonolus app, and it will return a proper `Variable` pointer which point to one memory in `LevelOption` or `PreviewOption` block.
 *
 * @param name The name of this option. Standardized option names starts with `#` and will be translated into client language when displayed. Special options names will have its effect applied automatically.
 * @param description Description will be displayed below this option. If you do not specify any description, it will not be displayed.
 * @param standard Whether this option is considered as a standard option. See wiki website for detailed information.
 * @param advanced If `advanced` is `true`, a sentence `This option is for advanced players.` will be displayed below this option.
 * @param scope Option values are automatically saved and shared between all levels. When a saved value matching the scope and name exists, it will be recalled.
 * @param def Default value of this slider option. `def` value must be equal to or granter than `0` and be smaller than `values.size()`
 * @param values Option values of this option. Standardized option values starts with `#` and will be translated into client language when displayed.
 * @return `Variable<LevelOptionId>` or `Variable<PreviewOptionId>`, which depends on current build mode.
 *
 * @see <https://wiki.sonolus.com/engine-specs/resources/engine-configuration-option.html>
 */
Variable SelectOption(
    string name = "",
    string description = "",
    bool standard = false,
    bool advanced = false,
    string scope = "",
    int def = 0,
    vector<string> values = {}
) {
    SonolusAssert(
        def >= 0 && def < values.size(),
        "`def` value must be equal to or granter than `0` and be smaller than `values.size()`!"
    );
    Json::Value res;
    res["name"] = name;
    res["description"] = description;
    res["standard"] = standard;
    res["advanced"] = advanced;
    res["scope"] = scope;
    res["type"] = "toggle";
    res["def"] = def;
    res["values"].size();
    for (int i = 0; i < values.size(); i++) res["values"].append(values[i]);
    engineConfigurationOptions.push_back(res);
    return Variable(OptionId, engineConfigurationOptions.size() - 1);
}

class Visibility {
    private:

    double scale = 1;
    double alpha = 0;

    public:

    /**
     * @brief Construct a new Visibility object
     *
     * @param scale The scale size of the item. The `scale` value must be between `0.5` and `2`.
     * @param alpha The alpha value of the item. The `alpha` value must be between `0` and `1`.
     */
    Constructor Visibility(double scale = 1, double alpha = 0) {
        SonolusAssert(
            scale >= 0.5 && scale <= 2,
            "`scale` value must be between `0.5` and `2`!"
        );
        SonolusAssert(
            alpha >= 0 && alpha <= 1,
            "`alpha` value must be between `0` and `1`!"
        );
        this->scale = scale;
        this->alpha = alpha;
    }

    /**
     * @brief Convert Visibility object to json object
     *
     * @return Json::Value
     */
    Json::Value toJsonObject() {
        Json::Value res;
        res["scale"] = scale;
        res["alpha"] = alpha;
        return res;
    }
};

class AnimationTween {
    private:

    double from;
    double to;
    double duration;
    string ease;

    public:

    /**
     * @brief Construct a new AnimationTween object
     *
     * @param from Start value of current animation tween. `from` value must be between `0` and `1`.
     * @param to End value of current animation tween. `to` value must be between `0` and `1`.
     * @param duration Duration of current animation tween. `duration` value must be equal to or greater than `0`.
     * @param ease Easing type of current animation tween. Only members of `EngineConfigurationAnimationTweenEase` are accepted.
     */
    Constructor AnimationTween(
        double from = 0,
        double to = 1,
        double duration = 1,
        string ease = EngineConfigurationAnimationTweenEase.None
    ) {
        SonolusAssert(
            from >= 0 && from <= 1,
            "`from` value must be between `0` and `1`!"
        );
        SonolusAssert(
            to >= 0 && to <= 1,
            "`to` value must be between `0` and `1`!"
        );
        SonolusAssert(
            duration >= 0,
            "`duration` value must be equal to or greater than `0`!"
        );
        SonolusAssert(
            EngineConfigurationAnimationTweenEaseMembers.count(ease),
            "For `ease`, only members of `EngineConfigurationAnimationTweenEase` are accepted!"
        );
        this->from = from;
        this->to = to;
        this->duration = duration;
        this->ease = ease;
    }

    /**
     * @brief Convert AnimationTween object to json object
     *
     * @return Json::Value
     */
    Json::Value toJsonObject() {
        Json::Value res;
        res["from"] = from;
        res["to"] = to;
        res["duration"] = duration;
        res["ease"] = ease;
        return res;
    }
};

class Animation {
    private:

    AnimationTween scale;
    AnimationTween alpha;

    public:

    /**
     * @brief Construct a new Animation object
     *
     * @param scale The scale animation tween of the item
     * @param alpha The alpha animation tween of the item
     */
    Constructor Animation(
        AnimationTween scale = AnimationTween(),
        AnimationTween alpha = AnimationTween()
    ) {
        this->scale = scale;
        this->alpha = alpha;
    }

    /**
     * @brief Convert Animation object to json object
     *
     * @return Json::Value
     */
    Json::Value toJsonObject() {
        Json::Value res;
        res["scale"] = scale.toJsonObject();
        res["alpha"] = alpha.toJsonObject();
        return res;
    }
};

/**
 * @brief Engine configuration UI is used by Sonolus app to populate UI section in level and engine configuration interface.
 *
 */
class UI {
    private:

    string scope = "";
    string primaryMetric = EngineConfigurationMetric.Arcade;
    string secondaryMetric = EngineConfigurationMetric.Life;
    Visibility menuVisibility;
    Visibility judgmentVisibility;
    Visibility comboVisibility;
    Visibility primaryMetricVisibility;
    Visibility secondaryMetricVisibility;
    Visibility progressVisibility;
    Visibility tutorialNavigationVisibility;
    Visibility tutorialInstructionVisibility;
    Animation judgmentAnimation;
    Animation comboAnimation;
    string judgmentErrorStyle = EngineConfigurationJudgmentErrorStyle.None;
    string judgmentErrorPlacement = EngineConfigurationJudgmentErrorPlacement.LeftRight;
    double judgmentErrorMin = 0;

    public:

    /**
     * @brief Set UI scope.
     *
     * @param scope UI values are automatically saved and shared between all engines. When a saved value matching the scope exists, it will be recalled.
     */
    void SetScope(string scope) {
        this->scope = scope;
    }

    /**
     * @brief Set UI primary metric and secondary metric.
     *
     * @param primary Primary metric to show. Only members of `EngineConfigurationMetric` are accepted.
     * @param primaryVisibility Visibility property of primary metric.
     * @param secondary Secondary metric to show. Only members of `EngineConfigurationMetric` are accepted.
     * @param secondaryVisibility Visibility property of secondary metric.
     */
    void SetMetric(
        string primary = EngineConfigurationMetric.Arcade,
        Visibility primaryVisibility = Visibility(),
        string secondary = EngineConfigurationMetric.Life,
        Visibility secondaryVisibility = Visibility()
    ) {
        SonolusAssert(
            EngineConfigurationMetricMembers.count(primary),
            "For `primaryMetric`, only members of `EngineConfigurationMetric` are accepted!"
        );
        SonolusAssert(
            EngineConfigurationMetricMembers.count(secondary),
            "For `secondaryMetric`, only members of `EngineConfigurationMetric` are accepted!"
        );
        primaryMetric = primary;
        primaryMetricVisibility = primaryVisibility;
        secondaryMetric = secondary;
        secondaryMetricVisibility = secondaryVisibility;
    }

    /**
     * @brief Set UI menu.
     *
     * @param visibility Visibility property of menu.
     */
    void SetMenu(Visibility visibility = Visibility()) {
        menuVisibility = visibility;
    }

    /**
     * @brief Set UI judgment.
     *
     * @param visibility Visibility property of judgment.
     * @param animation Animation of judgment.
     * @param errorStyle Style of positive judgment error, while the negative judgment error is opposite to the positive judgment error. Only members of `EngineConfigurationJudgmentErrorStyle` are accepted.
     * @param errorPlacement Placement of judgment error. Only members of `EngineConfigurationJudgmentErrorPlacement` are accepted.
     * @param errorMin Minimum judgment error. Only judgment errors larger than this will be shown. `errorMin` value must be equal to or greater than `0`.
     */
    void SetJudgment(
        Visibility visibility = Visibility(),
        Animation animation = Animation(),
        string errorStyle = EngineConfigurationJudgmentErrorStyle.None,
        string errorPlacement = EngineConfigurationJudgmentErrorPlacement.LeftRight,
        double errorMin = 0
    ) {
        SonolusAssert(
            EngineConfigurationJudgmentErrorStyleMembers.count(errorStyle),
            "For `errorStyle`, only members of `EngineConfigurationJudgmentErrorStyle` are accepted!"
        );
        SonolusAssert(
            EngineConfigurationJudgmentErrorPlacementMembers.count(errorPlacement),
            "For `errorPlacement`, only members of `EngineConfigurationJudgmentErrorPlacement` are accepted!"
        );
        SonolusAssert(
            errorMin >= 0,
            "`errorMin` value must be equal to or greater than `0`!"
        );
        judgmentVisibility = visibility;
        judgmentAnimation = animation;
        judgmentErrorStyle = errorStyle;
        judgmentErrorPlacement = errorPlacement;
        judgmentErrorMin = errorMin;
    }

    /**
     * @brief Set UI combo.
     *
     * @param visibility Visibility property of combo.
     * @param animation Animation of combo.
     */
    void SetCombo(Visibility visibility = Visibility(), Animation animation = Animation()) {
        comboVisibility = visibility;
        comboAnimation = animation;
    }

    /**
     * @brief Set UI progress.
     *
     * @param visibility Visibility property of progress.
     */
    void SetProgress(Visibility visibility = Visibility()) {
        progressVisibility = visibility;
    }

    /**
     * @brief Set UI in tutorial mode.
     *
     * @param navigation Visibility property of navigation in tutorial mode.
     * @param instruction Visibility property of instruction in tutorial mode.
     */
    void SetTutorial(Visibility navigation = Visibility(), Visibility instruction = Visibility()) {
        tutorialNavigationVisibility = navigation;
        tutorialInstructionVisibility = instruction;
    }

    /**
     * @brief Convert UI object to json object
     *
     * @return Json::Value
     */
    Json::Value toJsonObject() {
        Json::Value res;
        res["scope"] = scope;
        res["primaryMetric"] = primaryMetric;
        res["secondaryMetric"] = secondaryMetric;
        res["menuVisibility"] = menuVisibility.toJsonObject();
        res["judgmentVisibility"] = judgmentVisibility.toJsonObject();
        res["comboVisibility"] = comboVisibility.toJsonObject();
        res["primaryMetricVisibility"] = primaryMetricVisibility.toJsonObject();
        res["secondaryMetricVisibility"] = secondaryMetricVisibility.toJsonObject();
        res["progressVisibility"] = progressVisibility.toJsonObject();
        res["tutorialNavigationVisibility"] = tutorialNavigationVisibility.toJsonObject();
        res["tutorialInstructionVisibility"] = tutorialInstructionVisibility.toJsonObject();
        res["judgmentAnimation"] = judgmentAnimation.toJsonObject();
        res["comboAnimation"] = comboAnimation.toJsonObject();
        res["judgmentErrorStyle"] = judgmentErrorStyle;
        res["judgmentErrorPlacement"] = judgmentErrorPlacement;
        res["judgmentErrorMin"] = judgmentErrorMin;
        return res;
    }
}UI;

/**
 * @brief Build Engine Configuration
 *
 * @param path Output path of Engine Configuration.
 */
void BuildConfiguration() {
    Json::Value configuration;
    configuration["ui"] = UI.toJsonObject();
    configuration["options"].resize(0);
    for (int i = 0; i < engineConfigurationOptions.size(); i++)
        configuration["options"].append(engineConfigurationOptions[i]);
    string EngineConfiguration = compress_gzip(json_encode(configuration));
    ofstream fout = ofstream(dist + "/EngineConfiguration");
    fout.write(EngineConfiguration.c_str(), EngineConfiguration.size());
}
#endif
/*** End of inlined file: Build.h ***/



/*** Start of inlined file: Array.h ***/
#ifndef Array_H
#define Array_H

template<typename T, int size>
class Array {
    private:
    map<int, int> Tsize;
    map<int, FuncNode> index;
    int currentDefaultAllocatorId;

    public:
    Constructor Array() {
        currentDefaultAllocatorId = defaultAllocatorId;
        for (auto it = SonolusMemoryDelta.begin(); it != SonolusMemoryDelta.end(); it++)
            index[(*it).first] = SonolusMemoryIndex[(*it).first] + (*it).second;
        for (auto it = SonolusMemoryIndex.begin(); it != SonolusMemoryIndex.end(); it++)
            index[(*it).first] = SonolusMemoryDelta[(*it).first] + (*it).second;
        Tsize = SizeOf<T>();
        for (int i = 0; i < size; i++) T();
    }
    Constructor Array(const Array<T, size>& value) {
        currentDefaultAllocatorId = defaultAllocatorId;
        for (auto it = SonolusMemoryDelta.begin(); it != SonolusMemoryDelta.end(); it++)
            index[(*it).first] = SonolusMemoryIndex[(*it).first] + (*it).second;
        for (auto it = SonolusMemoryIndex.begin(); it != SonolusMemoryIndex.end(); it++)
            index[(*it).first] = SonolusMemoryDelta[(*it).first] + (*it).second;
        Tsize = SizeOf<T>();
        for (int i = 0; i < size; i++) T();
        for (int i = 0; i < size; i++) (*this)[i] = value[i];
    }

    T operator[] (FuncNode i) {
        int backupDefaultAllocatorId = defaultAllocatorId;
        defaultAllocatorId = currentDefaultAllocatorId;
        map<int, FuncNode> offsets;
        for (auto it = Tsize.begin(); it != Tsize.end(); it++) {
            int aId = (*it).first; FuncNode offset = index[aId] + i * (*it).second;
            offsets[aId] = offset;
        }
        auto backupSonolusMemoryIndex = SonolusMemoryIndex;
        auto backupSonolusMemoryDelta = SonolusMemoryDelta;
        SonolusMemoryIndex = offsets;
        SonolusMemoryDelta.clear();
        T res;
        SonolusMemoryIndex = backupSonolusMemoryIndex;
        SonolusMemoryDelta = backupSonolusMemoryDelta;
        defaultAllocatorId = backupDefaultAllocatorId;
        return res;
    }
    T operator[] (FuncNode i) const {
        return (*this)[i];
    }
    Array<T, size>& operator = (const Array<T, size>& value) {
        for (int i = 0; i < size; i++) (*this)[i] = value[i];
        return *this;
    }
};

template<typename T, int size> using Tuple = Array<T, size>;

#endif
/*** End of inlined file: Array.h ***/


/*** Start of inlined file: Collection.h ***/
#ifndef Collection_H
#define Collection_H

template<typename T, int size>
class Collection {
    public:

    Variable count;
    Array<T, size> array = Array<T, size>();

    Constructor Collection(){}

    Blocked T get(FuncNode i) {
        return array[i];
    }
    Blocked SonolusApi add(const T &value) {
        array[count++] = value;
    }
    Blocked SonolusApi has(const T &value) {
        for (Variable i = 0; i < count; i++)
            if (value == array[i]) return true;
        return false;
    }
    Blocked SonolusApi indexOf(const T &value) {
        for (Variable i = 0; i < count; i++)
            if (value == array[i]) return i;
        return -1;
    }
    Blocked SonolusApi clear() {
        count = 0;
    }

    T operator[] (FuncNode i) {
        return array[i];
    }
};

#endif
/*** End of inlined file: Collection.h ***/


/*** Start of inlined file: Dictionary.h ***/
#ifndef Dictionary_H
#define Dictionary_H

template<typename T1, typename T2, int size>
class Dictionary {
    public:

    Variable count;
    Array<T1, size> keys = Array<T1, size>();
    Array<T2, size> values = Array<T2, size>();

    Constructor Dictionary(){}

    Blocked T1 getKey(FuncNode i) {
        return keys[i];
    }
    Blocked T2 getValue(FuncNode i) {
        return values[i];
    }
    Blocked SonolusApi has(const T1 &key) {
        for (Variable i = 0; i < count; i++)
            if (keys[i] == key) return true;
        return false;
    }
    Blocked SonolusApi indexOf(const T1 &key) {
        for (Variable i = 0; i < count; i++)
            if (keys[i] == key) return i;
        return -1;
    }
    Blocked SonolusApi set(const T1 &key, const T2 &value) {
        Variable index = indexOf(key);
        if (index == -1) {
            keys[count] = key;
            values[count] = value;
            count++;
        } else values[index] = value;
    }
    Blocked SonolusApi clear() {
        count = 0;
    }

    T2 operator [] (const T1 &key) {
        Variable id = indexOf(key);
        return getValue(id);
    }
};

template<typename T1, typename T2, int size> using Map = Dictionary<T1, T2, size>;

#endif
/*** End of inlined file: Dictionary.h ***/


/*** Start of inlined file: Mat.h ***/
#ifndef Mat_H
#define Mat_H

class Matrix {
    public:

    Array<Array<Variable, 4>, 4> m;

    Constructor Matrix(){}
    Constructor Matrix(double a) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = a;
            }
        }
    }
    Constructor Matrix(FuncNode a) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = a;
            }
        }
    }
    Constructor Matrix(vector<vector<FuncNode> > a) {
        for (int i = 0; i < min(4, int(a.size())); i++) {
            for (int j = 0; j < min(4, int(a[i].size())); j++) {
                m[i][j] = a[i][j];
            }
        }
    }

    Blocked Matrix mul(const Matrix &mat) {
        Matrix res;
        FuncNode m1[4][4], m2[4][4], m3[4][4];
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m1[i][j] = m[i][j];
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m2[i][j] = mat.m[i][j];
        for (int i = 0; i < 4; i++) for (int k = 0; k < 4; k++)
            for (int j = 0; j < 4; j++) m3[i][j] = m3[i][j] + m1[i][k] * m2[k][j];
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) res.m[i][j] = m3[i][j];
        return res;
    }

    static Blocked Matrix identity() {
        return Matrix({
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        });
    }
    static Blocked Matrix translation(FuncNode x, FuncNode y) {
        return Matrix({
            { 1, 0, 0, x },
            { 0, 1, 0, y },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        });
    }
    static Blocked Matrix scaling(FuncNode x, FuncNode y) {
        return Matrix({
            { x, 0, 0, 0 },
            { 0, y, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        });
    }
    static Blocked Matrix rotation(FuncNode angle) {
        FuncNode c = FuncNode("Cos", { angle });
        FuncNode s = FuncNode("Sin", { angle });
        return Matrix({
            { c, -s, 0, 0 },
            { s, c, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        });
    }
    Blocked Matrix translate(FuncNode x, FuncNode y) {
        return Matrix::translation(x, y).mul(*this);
    }
    Blocked Matrix scale(FuncNode x, FuncNode y) {
        return Matrix::scaling(x, y).mul(*this);
    }
    Blocked Matrix rotate(FuncNode angle) {
        return Matrix::rotation(angle).mul(*this);
    }

    Matrix operator * (const Matrix &a) {
        return mul(a);
    }
    FuncNode operator == (const Matrix &a) {
        FuncNode m1[4][4], m2[4][4];
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m1[i][j] = m[i][j];
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m2[i][j] = a.m[i][j];
        FuncNode res = true;
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) res = FuncNode("And", { res, m1[i][j] == m2[i][j] });
        return res;
    }
};

using Mat = Matrix;

#endif
/*** End of inlined file: Mat.h ***/


/*** Start of inlined file: Vec.h ***/
#ifndef Vec_H
#define Vec_H

class Vector {
    public:

    Variable x;
    Variable y;

    Constructor Vector(){}
    Constructor Vector(FuncNode x, FuncNode y) {
        this->x = x;
        this->y = y;
    }

    Blocked FuncNode length() {
        return FuncNode("Power", { x * x + y * y });
    }
    Blocked Vector add(const Vector &vec) const {
        #ifdef COMPILE_RUNTIME
        return Vector(x + vec.x, y + vec.y);
        #endif
    }
    Blocked Vector sub(const Vector &vec) const {
        #ifdef COMPILE_RUNTIME
        return Vector(x - vec.x, y - vec.y);
        #endif
    }
    Blocked Vector mul(FuncNode val) {
        return Vector(x * val, y * val);
    }
    Blocked Vector div(FuncNode val) {
        return Vector(x / val, y / val);
    }
    Blocked Vector translate(FuncNode x, FuncNode y) {
        return Vector(this->x + x, this->y + y);
    }
    Blocked Vector scale(FuncNode x, FuncNode y) {
        return Vector(this->x * x, this->y * y);
    }
    Blocked Vector rotate(FuncNode angle) {
        FuncNode c = FuncNode("Cos", { angle });
        FuncNode s = FuncNode("Sin", { angle });
        return Vector(c * x - s * y, s * x + c * y);
    }
    Blocked Vector transform(const Matrix &mat) {
        FuncNode m[4][4];
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m[i][j] = mat.m[i][j];
        return Vector(x * m[0][0] + y * m[0][1] + m[0][3], x * m[1][0] + y * m[1][1] + m[1][3]);
    }

    static Blocked Vector zero() {
        return Vector(0, 0);
    }
    static Blocked Vector one() {
        return Vector(1, 1);
    }
    static Blocked Vector L() {
        return Vector(-1, 0);
    }
    static Blocked Vector R() {
        return Vector(1, 0);
    }
    static Blocked Vector B() {
        return Vector(0, -1);
    }
    static Blocked Vector T() {
        return Vector(0, 1);
    }
    static Blocked Vector LB() {
        return Vector(-1, -1);
    }
    static Blocked Vector RB() {
        return Vector(1, -1);
    }
    static Blocked Vector LT() {
        return Vector(-1, 1);
    }
    static Blocked Vector RT() {
        return Vector(1, 1);
    }

    Vector operator + (const Vector &a) const {
        return add(a);
    }
    Vector operator - (const Vector &a) const {
        return sub(a);
    }
    Vector operator - () {
        return Vector::zero().sub(*this);
    }
    Vector operator * (FuncNode a) {
        return mul(a);
    }
    Vector operator / (FuncNode a) {
        return div(a);
    }
    Vector& operator += (const Vector &a) {
        x += a.x; y += a.y;
        return *this;
    }
    Vector& operator -= (const Vector &a) {
        x -= a.x; y -= a.y;
        return *this;
    }
    Vector& operator *= (const Vector &a) {
        x *= a.x; y *= a.y;
        return *this;
    }
    Vector& operator /= (const Vector &a) {
        x /= a.x; y /= a.y;
        return *this;
    }
    Vector& operator ++ () {
        x++; y++;
        return *this;
    }
    Vector& operator ++ (int) {
        auto res = (*this);
        ++(*this);
        return res;
    }
    Vector& operator -- () {
        x--; y--;
        return *this;
    }
    Vector& operator -- (int) {
        auto res = (*this);
        --(*this);
        return res;
    }
    FuncNode operator == (const Vector &a) {
        #ifdef COMPILE_RUNTIME
        return x == a.x && y == a.y;
        #endif
    }

    operator Draw_Group_x_y() { return { x, y }; }
    operator DrawCurvedBT_Group_x_y() { return { x, y }; }
    operator DrawCurvedL_Group_x_y() { return { x, y }; }
    operator DrawCurvedLR_Group_x_y() { return { x, y }; }
    operator DrawCurvedR_Group_x_y() { return { x, y }; }
    operator DrawCurvedT_Group_x_y() { return { x, y }; }
    operator MoveParticleEffect_Group_x_y() { return { x, y }; }
    operator SpawnParticleEffect_Group_x_y() { return { x, y }; }
};

Blocked Vector lerp(const Vector &x, const Vector &y, FuncNode s) {
    return Vector(
        FuncNode("Lerp", { x.x, y.x, s }),
        FuncNode("Lerp", { x.y, y.y, s })
    );
}
Blocked Vector lerpClamped(const Vector &x, const Vector &y, FuncNode s) {
    return Vector(
        FuncNode("LerpClamped", { x.x, y.x, s }),
        FuncNode("LerpClamped", { x.y, y.y, s })
    );
}
Blocked Vector remap(FuncNode a, FuncNode b, const Vector &c, const Vector &d, FuncNode s) {
    return Vector(
        FuncNode("Remap", { a, b, c.x, d.x, s }),
        FuncNode("Remap", { a, b, c.y, d.y, s })
    );
}
Blocked Vector remapClamped(FuncNode a, FuncNode b, const Vector &c, const Vector &d, FuncNode s) {
    return Vector(
        FuncNode("RemapClamped", { a, b, c.x, d.x, s }),
        FuncNode("RemapClamped", { a, b, c.y, d.y, s })
    );
}

using Vec = Vector;

#endif
/*** End of inlined file: Vec.h ***/


/*** Start of inlined file: Range.h ***/
#ifndef Range_H
#define Range_H

class Range {
    public:

    Variable min;
    Variable max;

    Constructor Range(){}
    Constructor Range(FuncNode x, FuncNode y) {
        min = x;
        max = y;
    }

    Blocked FuncNode length() {
        return max - min;
    }
    Blocked Range mirror() {
        return Range(-max, -min);
    }
    Blocked Range add(FuncNode val) {
        return Range(min + val, max + val);
    }
    Blocked Range sub(FuncNode val) {
        return Range(min - val, max - val);
    }
    Blocked Range mul(FuncNode val) {
        return Range(min * val, max * val);
    }
    Blocked Range div(FuncNode val) {
        return Range(min / val, max / val);
    }
    Blocked Range translate(FuncNode val) {
        return Range(min + val, max + val);
    }
    Blocked Range scale(FuncNode val) {
        return Range(min * val, max * val);
    }
    Blocked Range expand(FuncNode val) {
        return Range(min - val, max + val);
    }
    Blocked Range shrink(FuncNode val) {
        return Range(min + val, max - val);
    }
    Blocked FuncNode contains(FuncNode val) {
        return min <= val && val <= max;
    }

    static Blocked Range zero() {
        return Range(0, 0);
    }
    static Blocked Range one() {
        return Range(-1, 1);
    }
    static Blocked Range L() {
        return Range(-1, 0);
    }
    static Blocked Range R() {
        return Range(0, 1);
    }

    Range operator + (FuncNode a) {
        return add(a);
    }
    Range operator - (FuncNode a) {
        return sub(a);
    }
    Range operator - () {
        min = -max;
        max = -min;
        return *this;
    }
    Range operator * (FuncNode a) {
        return mul(a);
    }
    Range operator / (FuncNode a) {
        return div(a);
    }
    Range& operator += (const Range &a) {
        min += a.min; max += a.max;
        return *this;
    }
    // Range& operator -= (const Range &a) {
    //     min -= a.min; max -= a.max;
    //     return *this;
    // }
    Range& operator *= (const Range &a) {
        min *= a.min; max *= a.max;
        return *this;
    }
    // Range& operator /= (const Range &a) {
    //     min /= a.min; max /= a.max;
    //     return *this;
    // }
    Range& operator ++ () {
        min++; max++;
        return *this;
    }
    Range& operator ++ (int) {
        auto res = (*this);
        ++(*this);
        return res;
    }
    Range& operator -- () {
        min--; max--;
        return *this;
    }
    Range& operator -- (int) {
        auto res = (*this);
        --(*this);
        return res;
    }
    FuncNode operator == (const Range &a) {
        #ifdef COMPILE_RUNTIME
        return min == a.min && max == a.max;
        #endif
    }
};

Blocked Range lerp(const Range &x, const Range &y, FuncNode s) {
    return Range(
        FuncNode("Lerp", { x.min, y.min, s }),
        FuncNode("Lerp", { x.max, y.max, s })
    );
}
Blocked Range lerpClamped(const Range &x, const Range &y, FuncNode s) {
    return Range(
        FuncNode("LerpClamped", { x.min, y.min, s }),
        FuncNode("LerpClamped", { x.max, y.max, s })
    );
}
Blocked Range remap(FuncNode a, FuncNode b, const Range &c, const Range &d, FuncNode s) {
    return Range(
        FuncNode("Remap", { a, b, c.min, d.min, s }),
        FuncNode("Remap", { a, b, c.max, d.max, s })
    );
}
Blocked Range remapClamped(FuncNode a, FuncNode b, const Range &c, const Range &d, FuncNode s) {
    return Range(
        FuncNode("RemapClamped", { a, b, c.min, d.min, s }),
        FuncNode("RemapClamped", { a, b, c.max, d.max, s })
    );
}

using Rg = Range;

#endif
/*** End of inlined file: Range.h ***/


/*** Start of inlined file: Rect.h ***/
#ifndef Rect_H
#define Rect_H

class Rectangle {
    public:

    Variable l;
    Variable b;
    Variable r;
    Variable t;

    Constructor Rectangle(){}
    Constructor Rectangle(FuncNode l, FuncNode b, FuncNode r, FuncNode t) {
        this->l = l;
        this->b = b;
        this->r = r;
        this->t = t;
    }

    Blocked FuncNode w() {
        return r - l;
    }
    Blocked FuncNode h() {
        return t - b;
    }
    Blocked FuncNode aspectRatio() {
        return w() / h();
    }
    Blocked Vector c() {
        return Vector((l + r) / 2, (b + t) / 2);
    }
    Blocked Vector cl() {
        return Vector(l, (b + t) / 2);
    }
    Blocked Vector cb() {
        return Vector((l + r) / 2, b);
    }
    Blocked Vector cr() {
        return Vector(r, (b + t) / 2);
    }
    Blocked Vector ct() {
        return Vector((l + r) / 2, t);
    }
    Blocked Vector lb() {
        return Vector(l, b);
    }
    Blocked Vector rb() {
        return Vector(r, b);
    }
    Blocked Vector lt() {
        return Vector(l, t);
    }
    Blocked Vector rt() {
        return Vector(r, t);
    }
    Blocked Rectangle add(const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        return Rectangle(
            l + vec.x, b + vec.y,
            r + vec.x, t + vec.y
        );
        #endif
    }
    Blocked Rectangle sub(const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        return Rectangle(
            l - vec.x, b - vec.y,
            r - vec.x, t - vec.y
        );
        #endif
    }
    Blocked Rectangle mul(FuncNode value) {
        return Rectangle(
            l * value, b * value,
            r * value, t * value
        );
    }
    Blocked Rectangle div(FuncNode value) {
        return Rectangle(
            l / value, b / value,
            r / value, t / value
        );
    }
    Blocked Rectangle translate(FuncNode x, FuncNode y) {
        Vec lbv = Vec(l, b).translate(x, y);
        Vec rtv = Vec(r, t).translate(x, y);
        return Rectangle(lbv.x, lbv.y, rtv.x, rtv.y);
    }
    Blocked Rectangle scale(FuncNode x, FuncNode y) {
        Vec lbv = Vec(l, b).scale(x, y);
        Vec rtv = Vec(r, t).scale(x, y);
        return Rectangle(lbv.x, lbv.y, rtv.x, rtv.y);
    }
    Blocked Rectangle rotate(FuncNode angle) {
        Vec lbv = Vec(l, b).rotate(angle);
        Vec rtv = Vec(r, t).rotate(angle);
        return Rectangle(lbv.x, lbv.y, rtv.x, rtv.y);
    }
    Blocked Rectangle transform(const Matrix &mat) {
        Vec lbv = Vec(l, b).transform(mat);
        Vec rtv = Vec(r, t).transform(mat);
        return Rectangle(lbv.x, lbv.y, rtv.x, rtv.y);
    }
    Blocked Rectangle expand(FuncNode x, FuncNode y) {
        return Rectangle(l - x, b - y, r + x, t + y);
    }
    Blocked Rectangle shrink(FuncNode x, FuncNode y) {
        return Rectangle(l + x, b + y, r - x, t - y);
    }
    Blocked FuncNode contains(const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        return l < vec.x && vec.x < r && b < vec.y && vec.y < t;
        #endif
    }
    Blocked FuncNode overlaps(const Rectangle &rect) {
        #ifdef COMPILE_RUNTIME
        return rect.b < t && b < rect.t && rect.l < r && rect.r < l;
        #endif
    }

    static Blocked Rectangle zero() {
        return Rectangle(0, 0, 0, 0);
    }
    static Blocked Rectangle one() {
        return Rectangle(-1, -1, 1, 1);
    }
    static Blocked Rectangle L() {
        return Rectangle(-1, -1, 0, 1);
    }
    static Blocked Rectangle B() {
        return Rectangle(-1, -1, 1, 0);
    }
    static Blocked Rectangle R() {
        return Rectangle(0, -1, 1, 1);
    }
    static Blocked Rectangle T() {
        return Rectangle(-1, 0, 1, 1);
    }
    static Blocked Rectangle LB() {
        return Rectangle(-1, -1, 0, 0);
    }
    static Blocked Rectangle RB() {
        return Rectangle(0, -1, 1, 0);
    }
    static Blocked Rectangle LT() {
        return Rectangle(-1, 0, 0, 1);
    }
    static Blocked Rectangle RT() {
        return Rectangle(0, 0, 1, 1);
    }

    Rectangle operator + (const Vector &vec) {
        return add(vec);
    }
    Rectangle operator - (const Vector &vec) {
        return sub(vec);
    }
    Rectangle operator * (FuncNode value) {
        return mul(value);
    }
    Rectangle operator / (FuncNode value) {
        return div(value);
    }
    Rectangle& operator += (const Vector &vec) {
        l += vec.x, b += vec.y;
        r += vec.x, t += vec.y;
        return *this;
    }
    Rectangle& operator -= (const Vector &vec) {
        l -= vec.x, b -= vec.y;
        r -= vec.x, t -= vec.y;
        return *this;
    }
    Rectangle& operator *= (FuncNode value) {
        l *= value, b *= value;
        r *= value, t *= value;
        return *this;
    }
    Rectangle& operator /= (FuncNode value) {
        l /= value, b /= value;
        r /= value, t /= value;
        return *this;
    }
    FuncNode operator == (const Rectangle &a) {
        #ifdef COMPILE_RUNTIME
        return l == a.l && r == a.r && b == a.b && t == a.t;
        #endif
    }
};

Blocked Rectangle lerp(const Rectangle &x, const Rectangle &y, FuncNode s) {
    return Rectangle(
        FuncNode("Lerp", { x.l, y.l, s }),
        FuncNode("Lerp", { x.b, y.b, s }),
        FuncNode("Lerp", { x.r, y.r, s }),
        FuncNode("Lerp", { x.t, y.t, s })
    );
}
Blocked Rectangle lerpClamped(const Rectangle &x, const Rectangle &y, FuncNode s) {
    return Rectangle(
        FuncNode("LerpClamped", { x.l, y.l, s }),
        FuncNode("LerpClamped", { x.b, y.b, s }),
        FuncNode("LerpClamped", { x.r, y.r, s }),
        FuncNode("LerpClamped", { x.t, y.t, s })
    );
}
Blocked Rectangle remap(FuncNode a, FuncNode b, const Rectangle &c, const Rectangle &d, FuncNode s) {
    return Rectangle(
        FuncNode("Remap", { a, b, c.l, d.l, s }),
        FuncNode("Remap", { a, b, c.b, d.b, s }),
        FuncNode("Remap", { a, b, c.r, d.r, s }),
        FuncNode("Remap", { a, b, c.t, d.t, s })
    );
}
Blocked Rectangle remapClamped(FuncNode a, FuncNode b, const Rectangle &c, const Rectangle &d, FuncNode s) {
    return Rectangle(
        FuncNode("RemapClamped", { a, b, c.l, d.l, s }),
        FuncNode("RemapClamped", { a, b, c.b, d.b, s }),
        FuncNode("RemapClamped", { a, b, c.r, d.r, s }),
        FuncNode("RemapClamped", { a, b, c.t, d.t, s })
    );
}

using Rect = Rectangle;

#endif
/*** End of inlined file: Rect.h ***/


/*** Start of inlined file: Quad.h ***/
#ifndef Quad_H
#define Quad_H

class Quadratic {
    public:

    Array<Variable, 4> x;
    Array<Variable, 4> y;

    Constructor Quadratic(){}
    Constructor Quadratic(const Rectangle &rect) {
        x[0] = rect.l; y[0] = rect.b;
        x[1] = rect.l; y[1] = rect.t;
        x[2] = rect.r; y[2] = rect.t;
        x[3] = rect.r; y[3] = rect.b;
    }
    Constructor Quadratic(const Vector &p1, const Vector &p2, const Vector &p3, const Vector &p4) {
        x[0] = p1.x, y[0] = p1.y;
        x[1] = p2.x, y[1] = p2.y;
        x[2] = p3.x, y[2] = p3.y;
        x[3] = p4.x, y[3] = p4.y;
    }
    Constructor Quadratic(FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4) {
        x[0] = x1, y[0] = y1;
        x[1] = x2, y[1] = y2;
        x[2] = x3, y[2] = y3;
        x[3] = x4, y[3] = y4;
    }

    Blocked Vector p1() {
        return Vector(x[0], y[0]);
    }
    Blocked Vector p2() {
        return Vector(x[1], y[1]);
    }
    Blocked Vector p3() {
        return Vector(x[2], y[2]);
    }
    Blocked Vector p4() {
        return Vector(x[3], y[3]);
    }
    Blocked Vector c() {
        return (p1() + p2() + p3() + p4()) / 4;
    }
    Blocked Vector c12() {
        return (p1() + p2()) / 2;
    }
    Blocked Vector c13() {
        return (p1() + p3()) / 2;
    }
    Blocked Vector c14() {
        return (p1() + p4()) / 2;
    }
    Blocked Vector c23() {
        return (p2() + p3()) / 2;
    }
    Blocked Vector c24() {
        return (p2() + p4()) / 2;
    }
    Blocked Vector c34() {
        return (p3() + p4()) / 2;
    }
    Blocked Rectangle aabb() {
        return Rectangle(
            FuncNode("Min", { x[0], FuncNode("Min", { x[1], FuncNode("Min", { x[2], x[3] }) }) }),
            FuncNode("Min", { y[0], FuncNode("Min", { y[1], FuncNode("Min", { y[2], y[3] }) }) }),
            FuncNode("Max", { x[0], FuncNode("Max", { x[1], FuncNode("Max", { x[2], x[3] }) }) }),
            FuncNode("Max", { y[0], FuncNode("Max", { y[1], FuncNode("Max", { y[2], y[3] }) }) })
        );
    }
    Blocked Quadratic add(const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        return Quadratic(
            x[0] + vec.x, y[0] + vec.y,
            x[1] + vec.x, y[1] + vec.y,
            x[2] + vec.x, y[2] + vec.y,
            x[3] + vec.x, y[3] + vec.y
        );
        #endif
    }
    Blocked Quadratic sub(const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        return Quadratic(
            x[0] - vec.x, y[0] - vec.y,
            x[1] - vec.x, y[1] - vec.y,
            x[2] - vec.x, y[2] - vec.y,
            x[3] - vec.x, y[3] - vec.y
        );
        #endif
    }
    Blocked Quadratic mul(FuncNode value) {
        return Quadratic(
            x[0] * value, y[0] * value,
            x[1] * value, y[1] * value,
            x[2] * value, y[2] * value,
            x[3] * value, y[3] * value
        );
    }
    Blocked Quadratic div(FuncNode value) {
        return Quadratic(
            x[0] / value, y[0] / value,
            x[1] / value, y[1] / value,
            x[2] / value, y[2] / value,
            x[3] / value, y[3] / value
        );
    }
    Blocked Quadratic translate(FuncNode x, FuncNode y) {
        Vec p0 = Vec(this->x[0], this->y[0]).translate(x, y);
        Vec p1 = Vec(this->x[1], this->y[1]).translate(x, y);
        Vec p2 = Vec(this->x[2], this->y[2]).translate(x, y);
        Vec p3 = Vec(this->x[3], this->y[3]).translate(x, y);
        return Quadratic(p0, p1, p2, p3);
    }
    Blocked Quadratic scale(FuncNode x, FuncNode y) {
        Vec p0 = Vec(this->x[0], this->y[0]).scale(x, y);
        Vec p1 = Vec(this->x[1], this->y[1]).scale(x, y);
        Vec p2 = Vec(this->x[2], this->y[2]).scale(x, y);
        Vec p3 = Vec(this->x[3], this->y[3]).scale(x, y);
        return Quadratic(p0, p1, p2, p3);
    }
    Blocked Quadratic rotate(FuncNode angle) {
        Vec p0 = Vec(x[0], y[0]).rotate(angle);
        Vec p1 = Vec(x[1], y[1]).rotate(angle);
        Vec p2 = Vec(x[2], y[2]).rotate(angle);
        Vec p3 = Vec(x[3], y[3]).rotate(angle);
        return Quadratic(p0, p1, p2, p3);
    }
    Blocked Quadratic transform(const Matrix &mat) {
        Vec p0 = Vec(x[0], y[0]).transform(mat);
        Vec p1 = Vec(x[1], y[1]).transform(mat);
        Vec p2 = Vec(x[2], y[2]).transform(mat);
        Vec p3 = Vec(x[3], y[3]).transform(mat);
        return Quadratic(p0, p1, p2, p3);
    }
    Blocked Quadratic swapRotate90() {
        return Quadratic(x[1], y[1], x[2], y[2], x[3], y[3], x[0], y[0]);
    }
    Blocked Quadratic swapRotate180() {
        return Quadratic(x[2], y[2], x[3], y[3], x[0], y[0], x[1], y[1]);
    }
    Blocked Quadratic swapRotate270() {
        return Quadratic(x[3], y[3], x[0], y[0], x[1], y[1], x[2], y[2]);
    }
    Blocked Quadratic swapMirrorY() {
        return Quadratic(x[3], y[3], x[2], y[2], x[1], y[1], x[0], y[0]);
    }
    Blocked Quadratic swapMirrorX() {
        return Quadratic(x[1], y[1], x[0], y[0], x[3], y[3], x[2], y[2]);
    }

    static Blocked Quadratic zero() {
        return Quadratic(0, 0, 0, 0, 0, 0, 0, 0);
    }
    static Blocked Quadratic one() {
        return Quadratic(-1, -1, -1, 1, 1, 1, 1, -1);
    }
    static Blocked Quadratic L() {
        return Quadratic(-1, -1, -1, 1, 0, 1, 0, -1);
    }
    static Blocked Quadratic B() {
        return Quadratic(-1, -1, -1, 0, 1, 0, 1, -1);
    }
    static Blocked Quadratic R() {
        return Quadratic(0, -1, 0, 1, 1, 1, 1, -1);
    }
    static Blocked Quadratic T() {
        return Quadratic(-1, 0, -1, 1, 1, 1, 1, 0);
    }
    static Blocked Quadratic LB() {
        return Quadratic(-1, -1, -1, 0, 0, 0, 0, -1);
    }
    static Blocked Quadratic RB() {
        return Quadratic(0, -1, 0, 0, 1, 0, 1, -1);
    }
    static Blocked Quadratic LT() {
        return Quadratic(-1, 0, -1, 1, 0, 1, 0, 0);
    }
    static Blocked Quadratic RT() {
        return Quadratic(0, 0, 0, 1, 1, 1, 1, 0);
    }

    Quadratic operator + (const Vector& vec) {
        return add(vec);
    }
    Quadratic operator - (const Vector& vec) {
        return sub(vec);
    }
    Quadratic operator * (FuncNode value) {
        return mul(value);
    }
    Quadratic operator / (FuncNode value) {
        return div(value);
    }
    Quadratic& operator += (const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        x[0] = x[0] + vec.x, y[0] = y[0] + vec.y;
        x[1] = x[1] + vec.x, y[1] = y[1] + vec.y;
        x[2] = x[2] + vec.x, y[2] = y[2] + vec.y;
        x[3] = x[3] + vec.x, y[3] = y[3] + vec.y;
        #endif
        return *this;
    }
    Quadratic& operator -= (const Vector &vec) {
        #ifdef COMPILE_RUNTIME
        x[0] = x[0] - vec.x, y[0] = y[0] - vec.y;
        x[1] = x[1] - vec.x, y[1] = y[1] - vec.y;
        x[2] = x[2] - vec.x, y[2] = y[2] - vec.y;
        x[3] = x[3] - vec.x, y[3] = y[3] - vec.y;
        #endif
        return *this;
    }
    Quadratic& operator *= (FuncNode value) {
        x[0] = x[0] * value, y[0] = y[0] * value;
        x[1] = x[1] * value, y[1] = y[1] * value;
        x[2] = x[2] * value, y[2] = y[2] * value;
        x[3] = x[3] * value, y[3] = y[3] * value;
        return *this;
    }
    Quadratic& operator /= (FuncNode value) {
        x[0] = x[0] / value, y[0] = y[0] / value;
        x[1] = x[1] / value, y[1] = y[1] / value;
        x[2] = x[2] / value, y[2] = y[2] / value;
        x[3] = x[3] / value, y[3] = y[3] / value;
        return *this;
    }
    FuncNode operator == (const Quadratic &a) {
        return x[0] == a.x[0] && y[0] == a.y[0] && x[1] == a.x[1] && y[1] == a.y[1] &&
            x[2] == a.x[2] && y[2] == a.y[2] && x[3] == a.x[3] && y[3] == a.y[3];
    }
};

Blocked Quadratic lerp(const Quadratic &x, const Quadratic &y, FuncNode s) {
    return Quadratic(
        FuncNode("Lerp", { x.x[0], y.x[0], s }),
        FuncNode("Lerp", { x.y[0], y.y[0], s }),
        FuncNode("Lerp", { x.x[1], y.x[1], s }),
        FuncNode("Lerp", { x.y[1], y.y[1], s }),
        FuncNode("Lerp", { x.x[2], y.x[2], s }),
        FuncNode("Lerp", { x.y[2], y.y[2], s }),
        FuncNode("Lerp", { x.x[3], y.x[3], s }),
        FuncNode("Lerp", { x.y[3], y.y[3], s })
    );
}
Blocked Quadratic lerpClamped(const Quadratic &x, const Quadratic &y, FuncNode s) {
    return Quadratic(
        FuncNode("LerpClamped", { x.x[0], y.x[0], s }),
        FuncNode("LerpClamped", { x.y[0], y.y[0], s }),
        FuncNode("LerpClamped", { x.x[1], y.x[1], s }),
        FuncNode("LerpClamped", { x.y[1], y.y[1], s }),
        FuncNode("LerpClamped", { x.x[2], y.x[2], s }),
        FuncNode("LerpClamped", { x.y[2], y.y[2], s }),
        FuncNode("LerpClamped", { x.x[3], y.x[3], s }),
        FuncNode("LerpClamped", { x.y[3], y.y[3], s })
    );
}
Blocked Quadratic remap(FuncNode a, FuncNode b, const Quadratic &c, const Quadratic &d, FuncNode s) {
    return Quadratic(
        FuncNode("Remap", { a, b, c.x[0], d.x[0], s }),
        FuncNode("Remap", { a, b, c.y[0], d.y[0], s }),
        FuncNode("Remap", { a, b, c.x[1], d.x[1], s }),
        FuncNode("Remap", { a, b, c.y[1], d.y[1], s }),
        FuncNode("Remap", { a, b, c.x[2], d.x[2], s }),
        FuncNode("Remap", { a, b, c.y[2], d.y[2], s }),
        FuncNode("Remap", { a, b, c.x[3], d.x[3], s }),
        FuncNode("Remap", { a, b, c.y[3], d.y[3], s })
    );
}
Blocked Quadratic remapClamped(FuncNode a, FuncNode b, const Quadratic &c, const Quadratic &d, FuncNode s) {
    return Quadratic(
        FuncNode("RemapClamped", { a, b, c.x[0], d.x[0], s }),
        FuncNode("RemapClamped", { a, b, c.y[0], d.y[0], s }),
        FuncNode("RemapClamped", { a, b, c.x[1], d.x[1], s }),
        FuncNode("RemapClamped", { a, b, c.y[1], d.y[1], s }),
        FuncNode("RemapClamped", { a, b, c.x[2], d.x[2], s }),
        FuncNode("RemapClamped", { a, b, c.y[2], d.y[2], s }),
        FuncNode("RemapClamped", { a, b, c.x[3], d.x[3], s }),
        FuncNode("RemapClamped", { a, b, c.y[3], d.y[3], s })
    );
}

using Quad = Quadratic;

#endif
/*** End of inlined file: Quad.h ***/


/*** Start of inlined file: Skins.h ***/
#ifndef Skins_H
#define Skins_H

#ifndef DISABLE_PACK_SKIN

/*** Start of inlined file: png.h ***/
#include<libpng/png.h>

struct image {
	int width;
	int height;
	int channel;
	png_bytep* data;

	image(){};
	image(int width, int height) {
		this->width = width, this->height = height;
		this->data = new png_bytep[height];
		for (int i = 0; i < height; i++) this->data[i] = reinterpret_cast<png_bytep>(new Byte[width * 4]);
	}
};

image readImage(string path) {
	FILE *fp = fopen(path.c_str(), "rb");
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	image img;
	img.width = png_get_image_width(png_ptr, info_ptr);
	img.height = png_get_image_height(png_ptr, info_ptr);
	img.data = png_get_rows(png_ptr, info_ptr);
	img.channel = png_get_channels(png_ptr, info_ptr);
	return img;
}

void writeImage(string path, image img) {
	FILE *fp = fopen(path.c_str(), "wb");
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, img.width, img.height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, img.data);
	png_write_end(png_ptr, NULL);
	fclose(fp);
}

/*** End of inlined file: png.h ***/


#define DISABLE_INTERPRETER

/*** Start of inlined file: MaxRects.cpp ***/
#include <limits>
#include <cmath>
#include <cstdlib>
#include <algorithm>


/*** Start of inlined file: MaxRects.h ***/
#include <vector>
#include <cstddef>

namespace rbp
{
	struct RectSize
	{
		int width;
		int height;
	};

	struct Rect
	{
		int x;
		int y;
		int width;
		int height;
	};

	class MaxRects
	{
	public:
		MaxRects(int width, int height, bool rotate = true);

		enum Mode
		{
			ShortSide = 1,
			LongSide,
			BestArea,
			BottomLeft,
			ContactPoint
		};

		size_t insert(int mode, const std::vector<RectSize> &rects, std::vector<size_t> &rects_indices,
			std::vector<Rect> &result, std::vector<size_t> &result_indices);

	private:
		int width_;
		int height_;
		bool rotate_;

		std::vector<Rect> used_;
		std::vector<Rect> free_;

		Rect score_rect(int width, int height, int mode, int &score1, int &score2);
		void place_rect(const Rect &node);
		int score_node_cp(int x, int y, int width, int height);

		Rect find_bl(int width, int height, int &bestY, int &bestX);
		Rect find_ss(int width, int height, int &bestShortSideFit, int &bestLongSideFit);
		Rect find_ls(int width, int height, int &bestShortSideFit, int &bestLongSideFit);
		Rect find_ba(int width, int height, int &bestAreaFit, int &bestShortSideFit);
		Rect find_cp(int width, int height, int &contactScore);

		bool split_free_node(Rect freeNode, const Rect &usedNode);
		void prune_free_list();
	};
}

/*** End of inlined file: MaxRects.h ***/

namespace rbp {

static int common_interval_length(int i1start, int i1end, int i2start, int i2end)
{
	if (i1end < i2start || i2end < i1start)
		return 0;

	return std::min(i1end, i2end) - std::max(i1start, i2start);
}

static bool is_contained_in(const Rect &a, const Rect &b)
{
	return a.x >= b.x && a.y >= b.y
		&& a.x + a.width <= b.x + b.width
		&& a.y + a.height <= b.y + b.height;
}

MaxRects::MaxRects(int width, int height, bool rotate)
{
	width_ = width;
	height_ = height;
	rotate_ = rotate;

	Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;

	free_.push_back(rect);
}

size_t MaxRects::insert(int mode, const std::vector<RectSize> &rects, std::vector<size_t> &rects_indices,
	std::vector<Rect> &result, std::vector<size_t> &result_indices)
{
	result.clear();
	result_indices.clear();

	result.reserve(rects_indices.size());
	result_indices.reserve(rects_indices.size());

	std::vector<size_t> &idx = rects_indices;

	while (idx.size() > 0)
	{
		Rect bestNode;

		int bestScore1 = std::numeric_limits<int>::max();
		int bestScore2 = std::numeric_limits<int>::max();
		int bestRectIndex = -1;

		for (size_t i = 0; i < idx.size(); ++i)
		{
			int score1;
			int score2;

			const RectSize &rect = rects[idx[i]];

			Rect newNode = score_rect(rect.width, rect.height, mode, score1, score2);

			if (score1 < bestScore1 || (score1 == bestScore1 && score2 < bestScore2))
			{
				bestScore1 = score1;
				bestScore2 = score2;
				bestNode = newNode;
				bestRectIndex = i;
			}
		}

		if (bestNode.height == 0 || bestRectIndex == -1)
			break;

		place_rect(bestNode);

		result.push_back(bestNode);
		result_indices.push_back(idx[bestRectIndex]);
		idx.erase(idx.begin() + bestRectIndex);
	}

	return result.size();
}

void MaxRects::place_rect(const Rect &node)
{
	size_t numRectanglesToProcess = free_.size();

	for (size_t i = 0; i < numRectanglesToProcess; ++i)
	{
		if (split_free_node(free_[i], node))
		{
			free_.erase(free_.begin() + i);
			--i;
			--numRectanglesToProcess;
		}
	}

	prune_free_list();
	used_.push_back(node);
}

Rect MaxRects::score_rect(int width, int height, int mode, int &score1, int &score2)
{
	Rect newNode;

	score1 = std::numeric_limits<int>::max();
	score2 = std::numeric_limits<int>::max();

	switch (mode)
	{
		case ShortSide:
			newNode = find_ss(width, height, score1, score2);
			break;

		case BottomLeft:
			newNode = find_bl(width, height, score1, score2);
			break;

		case ContactPoint:
			newNode = find_cp(width, height, score1);
			score1 = -score1;
			break;

		case LongSide:
			newNode = find_ls(width, height, score2, score1);
			break;

		case BestArea:
			newNode = find_ba(width, height, score1, score2);
			break;
	}

	if (newNode.height == 0)
	{
		score1 = std::numeric_limits<int>::max();
		score2 = std::numeric_limits<int>::max();
	}

	return newNode;
}

Rect MaxRects::find_bl(int width, int height, int &bestY, int &bestX)
{
	Rect bestNode = {0};

	bestY = std::numeric_limits<int>::max();

	for (size_t i = 0; i < free_.size(); ++i)
	{
		if (free_[i].width >= width && free_[i].height >= height)
		{
			int topSideY = free_[i].y + height;

			if (topSideY < bestY || (topSideY == bestY && free_[i].x < bestX))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = width;
				bestNode.height = height;
				bestY = topSideY;
				bestX = free_[i].x;
			}
		}

		if (rotate_ && free_[i].width >= height && free_[i].height >= width)
		{
			int topSideY = free_[i].y + width;

			if (topSideY < bestY || (topSideY == bestY && free_[i].x < bestX))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = height;
				bestNode.height = width;
				bestY = topSideY;
				bestX = free_[i].x;
			}
		}
	}

	return bestNode;
}

Rect MaxRects::find_ss(int width, int height, int &bestShortSideFit, int &bestLongSideFit)
{
	Rect bestNode = {0};

	bestShortSideFit = std::numeric_limits<int>::max();

	for (size_t i = 0; i < free_.size(); ++i)
	{
		if (free_[i].width >= width && free_[i].height >= height)
		{
			int leftoverHoriz = std::abs(free_[i].width - width);
			int leftoverVert = std::abs(free_[i].height - height);
			int shortSideFit = std::min(leftoverHoriz, leftoverVert);
			int longSideFit = std::max(leftoverHoriz, leftoverVert);

			if (shortSideFit < bestShortSideFit ||
				(shortSideFit == bestShortSideFit && longSideFit < bestLongSideFit))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = width;
				bestNode.height = height;
				bestShortSideFit = shortSideFit;
				bestLongSideFit = longSideFit;
			}
		}

		if (rotate_ && free_[i].width >= height && free_[i].height >= width)
		{
			int flippedLeftoverHoriz = std::abs(free_[i].width - height);
			int flippedLeftoverVert = std::abs(free_[i].height - width);
			int flippedShortSideFit = std::min(flippedLeftoverHoriz, flippedLeftoverVert);
			int flippedLongSideFit = std::max(flippedLeftoverHoriz, flippedLeftoverVert);

			if (flippedShortSideFit < bestShortSideFit ||
				(flippedShortSideFit == bestShortSideFit && flippedLongSideFit < bestLongSideFit))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = height;
				bestNode.height = width;
				bestShortSideFit = flippedShortSideFit;
				bestLongSideFit = flippedLongSideFit;
			}
		}
	}

	return bestNode;
}

Rect MaxRects::find_ls(int width, int height, int &bestShortSideFit, int &bestLongSideFit)
{
	Rect bestNode = {0};

	bestLongSideFit = std::numeric_limits<int>::max();

	for (size_t i = 0; i < free_.size(); ++i)
	{
		if (free_[i].width >= width && free_[i].height >= height)
		{
			int leftoverHoriz = std::abs(free_[i].width - width);
			int leftoverVert = std::abs(free_[i].height - height);
			int shortSideFit = std::min(leftoverHoriz, leftoverVert);
			int longSideFit = std::max(leftoverHoriz, leftoverVert);

			if (longSideFit < bestLongSideFit ||
				(longSideFit == bestLongSideFit && shortSideFit < bestShortSideFit))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = width;
				bestNode.height = height;
				bestShortSideFit = shortSideFit;
				bestLongSideFit = longSideFit;
			}
		}

		if (rotate_ && free_[i].width >= height && free_[i].height >= width)
		{
			int leftoverHoriz = std::abs(free_[i].width - height);
			int leftoverVert = std::abs(free_[i].height - width);
			int shortSideFit = std::min(leftoverHoriz, leftoverVert);
			int longSideFit = std::max(leftoverHoriz, leftoverVert);

			if (longSideFit < bestLongSideFit ||
				(longSideFit == bestLongSideFit && shortSideFit < bestShortSideFit))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = height;
				bestNode.height = width;
				bestShortSideFit = shortSideFit;
				bestLongSideFit = longSideFit;
			}
		}
	}

	return bestNode;
}

Rect MaxRects::find_ba(int width, int height, int &bestAreaFit, int &bestShortSideFit)
{
	Rect bestNode = {0};

	bestAreaFit = std::numeric_limits<int>::max();

	for (size_t i = 0; i < free_.size(); ++i)
	{
		int areaFit = free_[i].width * free_[i].height - width * height;

		if (free_[i].width >= width && free_[i].height >= height)
		{
			int leftoverHoriz = std::abs(free_[i].width - width);
			int leftoverVert = std::abs(free_[i].height - height);
			int shortSideFit = std::min(leftoverHoriz, leftoverVert);

			if (areaFit < bestAreaFit || (areaFit == bestAreaFit && shortSideFit < bestShortSideFit))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = width;
				bestNode.height = height;
				bestShortSideFit = shortSideFit;
				bestAreaFit = areaFit;
			}
		}

		if (rotate_ && free_[i].width >= height && free_[i].height >= width)
		{
			int leftoverHoriz = std::abs(free_[i].width - height);
			int leftoverVert = std::abs(free_[i].height - width);
			int shortSideFit = std::min(leftoverHoriz, leftoverVert);

			if (areaFit < bestAreaFit || (areaFit == bestAreaFit && shortSideFit < bestShortSideFit))
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = height;
				bestNode.height = width;
				bestShortSideFit = shortSideFit;
				bestAreaFit = areaFit;
			}
		}
	}

	return bestNode;
}

int MaxRects::score_node_cp(int x, int y, int width, int height)
{
	int score = 0;

	if (x == 0 || x + width == width_)
		score += height;

	if (y == 0 || y + height == height_)
		score += width;

	for (size_t i = 0; i < used_.size(); ++i)
	{
		if (used_[i].x == x + width || used_[i].x + used_[i].width == x)
			score += common_interval_length(used_[i].y, used_[i].y + used_[i].height, y, y + height);

		if (used_[i].y == y + height || used_[i].y + used_[i].height == y)
			score += common_interval_length(used_[i].x, used_[i].x + used_[i].width, x, x + width);
	}

	return score;
}

Rect MaxRects::find_cp(int width, int height, int &bestContactScore)
{
	Rect bestNode = {0};

	bestContactScore = -1;

	for (size_t i = 0; i < free_.size(); ++i)
	{
		if (free_[i].width >= width && free_[i].height >= height)
		{
			int score = score_node_cp(free_[i].x, free_[i].y, width, height);

			if (score > bestContactScore)
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = width;
				bestNode.height = height;
				bestContactScore = score;
			}
		}

		if (rotate_ && free_[i].width >= height && free_[i].height >= width)
		{
			int score = score_node_cp(free_[i].x, free_[i].y, height, width);

			if (score > bestContactScore)
			{
				bestNode.x = free_[i].x;
				bestNode.y = free_[i].y;
				bestNode.width = height;
				bestNode.height = width;
				bestContactScore = score;
			}
		}
	}

	return bestNode;
}

bool MaxRects::split_free_node(Rect freeNode, const Rect &usedNode)
{
	if (usedNode.x >= freeNode.x + freeNode.width || usedNode.x + usedNode.width <= freeNode.x ||
		usedNode.y >= freeNode.y + freeNode.height || usedNode.y + usedNode.height <= freeNode.y)
		return false;

	if (usedNode.x < freeNode.x + freeNode.width && usedNode.x + usedNode.width > freeNode.x)
	{
		if (usedNode.y > freeNode.y && usedNode.y < freeNode.y + freeNode.height)
		{
			Rect newNode = freeNode;
			newNode.height = usedNode.y - newNode.y;
			free_.push_back(newNode);
		}

		if (usedNode.y + usedNode.height < freeNode.y + freeNode.height)
		{
			Rect newNode = freeNode;
			newNode.y = usedNode.y + usedNode.height;
			newNode.height = freeNode.y + freeNode.height - (usedNode.y + usedNode.height);
			free_.push_back(newNode);
		}
	}

	if (usedNode.y < freeNode.y + freeNode.height && usedNode.y + usedNode.height > freeNode.y)
	{
		if (usedNode.x > freeNode.x && usedNode.x < freeNode.x + freeNode.width)
		{
			Rect newNode = freeNode;
			newNode.width = usedNode.x - newNode.x;
			free_.push_back(newNode);
		}

		if (usedNode.x + usedNode.width < freeNode.x + freeNode.width)
		{
			Rect newNode = freeNode;
			newNode.x = usedNode.x + usedNode.width;
			newNode.width = freeNode.x + freeNode.width - (usedNode.x + usedNode.width);
			free_.push_back(newNode);
		}
	}

	return true;
}

void MaxRects::prune_free_list()
{
	for (size_t i = 0; i < free_.size(); ++i)
	{
		for (size_t j = i + 1; j < free_.size(); ++j)
		{
			if (is_contained_in(free_[i], free_[j]))
			{
				free_.erase(free_.begin() + i);
				--i;
				break;
			}

			if (is_contained_in(free_[j], free_[i]))
			{
				free_.erase(free_.begin() + j);
				--j;
			}
		}
	}
}

}

/*** End of inlined file: MaxRects.cpp ***/


#undef DISABLE_INTERPRETER

class SkinDataSprite {
	public:
	string name;
	int x, y, w, h;

	Json::Value toJsonObject() {
		Json::Value res;
		res["name"] = name;
		res["x"] = x; res["y"] = y; res["w"] = w; res["h"] = h;
		string x[8] = { "x1", "x2", "x3", "x4", "y1", "y2", "y3", "y4" };
        // string x[8];
        // x[0] = "x1"; x[1] = "x2"; x[2] = "x3"; x[3] = "x4";
        // x[4] = "x5"; x[5] = "x6"; x[6] = "x7"; x[7] = "x8";
		for (int i = 0; i < 8; i++) res["transform"][x[i]][x[i]] = 1;
		return res;
	}
};

class SkinData {
	public:
	int width, height;
	bool interpolation = false;
	vector<SkinDataSprite> sprites;

	Json::Value toJsonObject() {
		Json::Value res;
		res["width"] = width;
		res["height"] = height;
		res["interpolation"] = interpolation;
		res["sprites"].resize(0);
		for (int i = 0; i < sprites.size(); i++) res["sprites"].append(sprites[i].toJsonObject());
		return res;
	}
};

bool fileExists(string path) {
	ifstream fin;
    fin.open(path.c_str());
	return fin.is_open();
}

void PackSkin() {
    string textureDir = dist + "/SkinTexture";
    string dataDir = dist + "/SkinData";
	vector<image> imgs; SkinData data;
	for (int i = 0; i < skin_sprites.size(); i++) {
		string imgPath = skin_sprites[i].second;
        SonolusAssert(
            fileExists(imgPath),
            "Unknown Skin Sprite \"%s\"",
            imgPath.c_str()
        );
		imgs.push_back(readImage(imgPath));
		data.sprites.push_back({ skin_sprites[i].first, 0, 0, 0, 0 });
	}

	using namespace rbp;
	vector<RectSize> rects, rects_back; vector<rbp::Rect> result_rects;
	vector<size_t> rects_id, rects_id_back, result_ids;
	for (int i = 0; i < imgs.size(); i++) rects.push_back({imgs[i].width + 2, imgs[i].height + 2});
	for (int i = 0; i < imgs.size(); i++) rects_id.push_back(i);
	rects_id_back = rects_id; rects_back = rects;
	int width = 512, height = 512;
	while (true) {
		rects = rects_back, rects_id = rects_id_back;
		result_rects.clear(); result_ids.clear();
		auto RectsFactory = MaxRects(width, height, false);
		int res = RectsFactory.insert(1, rects, rects_id, result_rects, result_ids);
		if (res == rects.size()) break;
		width *= 2, height *= 2;
	}

	image outimg = image(width, height);
	outimg.width = width, outimg.height = height;
	data.width = width; data.height = height;
	for (int i = 0; i < result_rects.size(); i++) {
		int id = result_ids[i], x = result_rects[i].x + 1, y = result_rects[i].y + 1;
		data.sprites[id].x = x; data.sprites[id].y = y;
		data.sprites[id].w = result_rects[i].width - 2; data.sprites[id].h = result_rects[i].height - 2;
		for (int j = y; j < y + result_rects[i].height - 2; j++) {
			for (int k = x; k < x + result_rects[i].width - 2; k++) {
				outimg.data[j][k * 4] = imgs[id].data[j - y][(k - x) * imgs[id].channel];
				outimg.data[j][k * 4 + 1] = imgs[id].data[j - y][(k - x) * imgs[id].channel + 1];
				outimg.data[j][k * 4 + 2] = imgs[id].data[j - y][(k - x) * imgs[id].channel + 2];
				outimg.data[j][k * 4 + 3] = imgs[id].channel < 4 ? 255 : imgs[id].data[j - y][(k - x) * imgs[id].channel + 3];
			}
		}
	} writeImage(textureDir, outimg);

	string json = json_encode(data.toJsonObject());
	string dataBuffer = compress_gzip(json);
	ofstream fout; fout.open(dataDir);
    fout.write(dataBuffer.c_str(), dataBuffer.size());
}
#endif

class SkinSprite {
    public:

    int id;
    string spriteName;

    operator int() {
        return id;
    }

    operator FuncNode() {
        return id;
    }

    operator Variable() {
        return id;
    }
};

SkinSprite defineSkinSprite(string name, string path = "") {
    if (path == "") path = "./skins/" + name + ".png";
    skin_sprites.push_back({ name, path });
    return { int(skin_sprites.size()) - 1, name };
}

class SkinSpriteName {
    public:

    string NoteHeadNeutral = "#NOTE_HEAD_NEUTRAL";
    string NoteHeadRed = "#NOTE_HEAD_RED";
    string NoteHeadGreen = "#NOTE_HEAD_GREEN";
    string NoteHeadBlue = "#NOTE_HEAD_BLUE";
    string NoteHeadYellow = "#NOTE_HEAD_YELLOW";
    string NoteHeadPurple = "#NOTE_HEAD_PURPLE";
    string NoteHeadCyan = "#NOTE_HEAD_CYAN";

    string NoteTickNeutral = "#NOTE_TICK_NEUTRAL";
    string NoteTickRed = "#NOTE_TICK_RED";
    string NoteTickGreen = "#NOTE_TICK_GREEN";
    string NoteTickBlue = "#NOTE_TICK_BLUE";
    string NoteTickYellow = "#NOTE_TICK_YELLOW";
    string NoteTickPurple = "#NOTE_TICK_PURPLE";
    string NoteTickCyan = "#NOTE_TICK_CYAN";

    string NoteTailNeutral = "#NOTE_TAIL_NEUTRAL";
    string NoteTailRed = "#NOTE_TAIL_RED";
    string NoteTailGreen = "#NOTE_TAIL_GREEN";
    string NoteTailBlue = "#NOTE_TAIL_BLUE";
    string NoteTailYellow = "#NOTE_TAIL_YELLOW";
    string NoteTailPurple = "#NOTE_TAIL_PURPLE";
    string NoteTailCyan = "#NOTE_TAIL_CYAN";

    string NoteConnectionNeutral = "#NOTE_CONNECTION_NEUTRAL";
    string NoteConnectionRed = "#NOTE_CONNECTION_RED";
    string NoteConnectionGreen = "#NOTE_CONNECTION_GREEN";
    string NoteConnectionBlue = "#NOTE_CONNECTION_BLUE";
    string NoteConnectionYellow = "#NOTE_CONNECTION_YELLOW";
    string NoteConnectionPurple = "#NOTE_CONNECTION_PURPLE";
    string NoteConnectionCyan = "#NOTE_CONNECTION_CYAN";

    string NoteConnectionNeutralSeamless = "#NOTE_CONNECTION_NEUTRAL_SEAMLESS";
    string NoteConnectionRedSeamless = "#NOTE_CONNECTION_RED_SEAMLESS";
    string NoteConnectionGreenSeamless = "#NOTE_CONNECTION_GREEN_SEAMLESS";
    string NoteConnectionBlueSeamless = "#NOTE_CONNECTION_BLUE_SEAMLESS";
    string NoteConnectionYellowSeamless = "#NOTE_CONNECTION_YELLOW_SEAMLESS";
    string NoteConnectionPurpleSeamless = "#NOTE_CONNECTION_PURPLE_SEAMLESS";
    string NoteConnectionCyanSeamless = "#NOTE_CONNECTION_CYAN_SEAMLESS";

    string SimultaneousConnectionNeutral = "#SIMULTANEOUS_CONNECTION_NEUTRAL";
    string SimultaneousConnectionRed = "#SIMULTANEOUS_CONNECTION_RED";
    string SimultaneousConnectionGreen = "#SIMULTANEOUS_CONNECTION_GREEN";
    string SimultaneousConnectionBlue = "#SIMULTANEOUS_CONNECTION_BLUE";
    string SimultaneousConnectionYellow = "#SIMULTANEOUS_CONNECTION_YELLOW";
    string SimultaneousConnectionPurple = "#SIMULTANEOUS_CONNECTION_PURPLE";
    string SimultaneousConnectionCyan = "#SIMULTANEOUS_CONNECTION_CYAN";

    string SimultaneousConnectionNeutralSeamless = "#SIMULTANEOUS_CONNECTION_NEUTRAL_SEAMLESS";
    string SimultaneousConnectionRedSeamless = "#SIMULTANEOUS_CONNECTION_RED_SEAMLESS";
    string SimultaneousConnectionGreenSeamless = "#SIMULTANEOUS_CONNECTION_GREEN_SEAMLESS";
    string SimultaneousConnectionBlueSeamless = "#SIMULTANEOUS_CONNECTION_BLUE_SEAMLESS";
    string SimultaneousConnectionYellowSeamless = "#SIMULTANEOUS_CONNECTION_YELLOW_SEAMLESS";
    string SimultaneousConnectionPurpleSeamless = "#SIMULTANEOUS_CONNECTION_PURPLE_SEAMLESS";
    string SimultaneousConnectionCyanSeamless = "#SIMULTANEOUS_CONNECTION_CYAN_SEAMLESS";

    string DirectionalMarkerNeutral = "#DIRECTIONAL_MARKER_NEUTRAL";
    string DirectionalMarkerRed = "#DIRECTIONAL_MARKER_RED";
    string DirectionalMarkerGreen = "#DIRECTIONAL_MARKER_GREEN";
    string DirectionalMarkerBlue = "#DIRECTIONAL_MARKER_BLUE";
    string DirectionalMarkerYellow = "#DIRECTIONAL_MARKER_YELLOW";
    string DirectionalMarkerPurple = "#DIRECTIONAL_MARKER_PURPLE";
    string DirectionalMarkerCyan = "#DIRECTIONAL_MARKER_CYAN";

    string SimultaneousMarkerNeutral = "#SIMULTANEOUS_MARKER_NEUTRAL";
    string SimultaneousMarkerRed = "#SIMULTANEOUS_MARKER_RED";
    string SimultaneousMarkerGreen = "#SIMULTANEOUS_MARKER_GREEN";
    string SimultaneousMarkerBlue = "#SIMULTANEOUS_MARKER_BLUE";
    string SimultaneousMarkerYellow = "#SIMULTANEOUS_MARKER_YELLOW";
    string SimultaneousMarkerPurple = "#SIMULTANEOUS_MARKER_PURPLE";
    string SimultaneousMarkerCyan = "#SIMULTANEOUS_MARKER_CYAN";

    string StageMiddle = "#STAGE_MIDDLE";

    string StageLeftBorder = "#STAGE_LEFT_BORDER";
    string StageRightBorder = "#STAGE_RIGHT_BORDER";
    string StageTopBorder = "#STAGE_TOP_BORDER";
    string StageBottomBorder = "#STAGE_BOTTOM_BORDER";
    string StageLeftBorderSeamless = "#STAGE_LEFT_BORDER_SEAMLESS";
    string StageRightBorderSeamless = "#STAGE_RIGHT_BORDER_SEAMLESS";
    string StageTopBorderSeamless = "#STAGE_TOP_BORDER_SEAMLESS";
    string StageBottomBorderSeamless = "#STAGE_BOTTOM_BORDER_SEAMLESS";

    string StageTopLeftCorner = "#STAGE_TOP_LEFT_CORNER";
    string StageTopRightCorner = "#STAGE_TOP_RIGHT_CORNER";
    string StageBottomLeftCorner = "#STAGE_BOTTOM_LEFT_CORNER";
    string StageBottomRightCorner = "#STAGE_BOTTOM_RIGHT_CORNER";

    string Lane = "#LANE";
    string LaneSeamless = "#LANE_SEAMLESS";

    string LaneAlternative = "#LANE_ALTERNATIVE";
    string LaneAlternativeSeamless = "#LANE_ALTERNATIVE_SEAMLESS";

    string JudgmentLine = "#JUDGMENT_LINE";
    string NoteSlot = "#NOTE_SLOT";

    string StageCover = "#STAGE_COVER";

    string GridNeutral = "#GRID_NEUTRAL";
    string GridRed = "#GRID_RED";
    string GridGreen = "#GRID_GREEN";
    string GridBlue = "#GRID_BLUE";
    string GridYellow = "#GRID_YELLOW";
    string GridPurple = "#GRID_PURPLE";
    string GridCyan = "#GRID_CYAN";
}SkinSpriteName;

#endif
/*** End of inlined file: Skins.h ***/


/*** Start of inlined file: Effects.h ***/
#ifndef Effects_H
#define Effects_H

#ifndef DISABLE_PACK_EFFECT

/*** Start of inlined file: zip.h ***/
#include<zip.h>
using namespace std;

string getFileFromZip(string zipPath, string filePath) {
    zip* z = zip_open(zipPath.c_str(), 0, NULL);
    if (z == NULL) {
        cerr << "Cannot open zip file." << endl;
        exit(3);
    }
    zip_stat_t st;
    zip_stat_init(&st);
    zip_stat(z, filePath.c_str(), 0, &st);
    char* contents = new char[st.size];
    zip_file_t* f = zip_fopen(z, filePath.c_str(), 0);
    if (f == NULL) {
        cerr << "Cannot open file in zip." << endl;
        exit(3);
    }
    zip_fread(f, contents, st.size);
    zip_fclose(f);
    zip_close(z);
    string ret = string(contents, st.size);
    delete[] contents;
    return ret;
}

void writeFileIntoZip(string zipPath, string filePath, string fileContent) {
    zip* z = zip_open(zipPath.c_str(), ZIP_CREATE, NULL);
    if (z == NULL) {
        cerr << "Cannot open zip file." << endl;
        exit(3);
    }
    zip_source_t* s = zip_source_buffer(z, fileContent.c_str(), fileContent.size(), 0);
    zip_add(z, filePath.c_str(), s);
    zip_close(z);
}
/*** End of inlined file: zip.h ***/


class EffectData {
	public:
    vector<pair<string, string> > clips;

	Json::Value toJsonObject() {
		Json::Value res;
		res["clips"].resize(0);
		for (int i = 0; i < clips.size(); i++) {
            Json::Value single;
            single["name"] = clips[i].first;
            single["filename"] = clips[i].second;
            res["clips"].append(single);
        } return res;
	}
};

void PackEffect() {
    string audioDir = dist + "/EffectAudio";
    string dataDir = dist + "/EffectData";
    remove(audioDir.c_str());

    EffectData data;
    for (int i = 0; i < effect_clips.size(); i++) {
        string name = effect_clips[i].second;
        string filename = to_string(i);
        data.clips.push_back(make_pair(effect_clips[i].first, filename));
        ifstream fin; fin.open(name);
        fin.seekg(0, ios::end); int len = fin.tellg();
        SonolusAssert(
            len != -1,
            "Unknown Effect Clip \"%s\"",
            name.c_str()
        );
        char* ch = new char[len]; fin.seekg(0, ios::beg);
        fin.read(ch, len); fin.close(); string res = string(ch, len); delete[] ch;
        writeFileIntoZip(audioDir, filename, res);
    }

	string json = json_encode(data.toJsonObject());
	string dataBuffer = compress_gzip(json);
	ofstream fout; fout.open(dataDir);
    fout.write(dataBuffer.c_str(), dataBuffer.size());
}
#endif

class EffectClip {
    public:

    int id;
    string clipName;

    operator int() {
        return id;
    }

    operator FuncNode() {
        return id;
    }

    operator Variable() {
        return id;
    }
};

EffectClip defineEffectClip(string name, string path = "") {
    if (path == "") path = "./effects/" + name + ".mp3";
    effect_clips.push_back({ name, path });
    return { int(effect_clips.size()) - 1, name };
}

class EffectClipName {
    public:

    string Miss = "#MISS";
    string Perfect = "#PERFECT";
    string Great = "#GREAT";
    string Good = "#GOOD";

    string Hold = "#HOLD";

    string MissAlternative = "#MISS_ALTERNATIVE";
    string PerfectAlternative = "#PERFECT_ALTERNATIVE";
    string GreatAlternative = "#GREAT_ALTERNATIVE";
    string GoodAlternative = "#GOOD_ALTERNATIVE";

    string HoldAlternative = "#HOLD_ALTERNATIVE";

    string Stage = "#STAGE";
}EffectClipName;

#endif
/*** End of inlined file: Effects.h ***/


/*** Start of inlined file: Particles.h ***/
#ifndef Particles_H
#define Particles_H

class ParticleEffect {
    public:

    int id;
    string effectName;

    operator int() {
        return id;
    }

    operator FuncNode() {
        return id;
    }

    operator Variable() {
        return id;
    }
};

ParticleEffect defineParticleEffect(string name) {
    // if (path == "") path = "./skin/" + name + ".png";
    particle_effects.push_back({ name, name });
    return { int(particle_effects.size()) - 1, name };
}

class ParticleEffectName {
    public:

    string NoteCircularTapNeutral = "#NOTE_CIRCULAR_TAP_NEUTRAL";
    string NoteCircularTapRed = "#NOTE_CIRCULAR_TAP_RED";
    string NoteCircularTapGreen = "#NOTE_CIRCULAR_TAP_GREEN";
    string NoteCircularTapBlue = "#NOTE_CIRCULAR_TAP_BLUE";
    string NoteCircularTapYellow = "#NOTE_CIRCULAR_TAP_YELLOW";
    string NoteCircularTapPurple = "#NOTE_CIRCULAR_TAP_PURPLE";
    string NoteCircularTapCyan = "#NOTE_CIRCULAR_TAP_CYAN";

    string NoteCircularAlternativeNeutral = "#NOTE_CIRCULAR_ALTERNATIVE_NEUTRAL";
    string NoteCircularAlternativeRed = "#NOTE_CIRCULAR_ALTERNATIVE_RED";
    string NoteCircularAlternativeGreen = "#NOTE_CIRCULAR_ALTERNATIVE_GREEN";
    string NoteCircularAlternativeBlue = "#NOTE_CIRCULAR_ALTERNATIVE_BLUE";
    string NoteCircularAlternativeYellow = "#NOTE_CIRCULAR_ALTERNATIVE_YELLOW";
    string NoteCircularAlternativePurple = "#NOTE_CIRCULAR_ALTERNATIVE_PURPLE";
    string NoteCircularAlternativeCyan = "#NOTE_CIRCULAR_ALTERNATIVE_CYAN";

    string NoteCircularHoldNeutral = "#NOTE_CIRCULAR_HOLD_NEUTRAL";
    string NoteCircularHoldRed = "#NOTE_CIRCULAR_HOLD_RED";
    string NoteCircularHoldGreen = "#NOTE_CIRCULAR_HOLD_GREEN";
    string NoteCircularHoldBlue = "#NOTE_CIRCULAR_HOLD_BLUE";
    string NoteCircularHoldYellow = "#NOTE_CIRCULAR_HOLD_YELLOW";
    string NoteCircularHoldPurple = "#NOTE_CIRCULAR_HOLD_PURPLE";
    string NoteCircularHoldCyan = "#NOTE_CIRCULAR_HOLD_CYAN";

    string NoteLinearTapNeutral = "#NOTE_LINEAR_TAP_NEUTRAL";
    string NoteLinearTapRed = "#NOTE_LINEAR_TAP_RED";
    string NoteLinearTapGreen = "#NOTE_LINEAR_TAP_GREEN";
    string NoteLinearTapBlue = "#NOTE_LINEAR_TAP_BLUE";
    string NoteLinearTapYellow = "#NOTE_LINEAR_TAP_YELLOW";
    string NoteLinearTapPurple = "#NOTE_LINEAR_TAP_PURPLE";
    string NoteLinearTapCyan = "#NOTE_LINEAR_TAP_CYAN";

    string NoteLinearAlternativeNeutral = "#NOTE_LINEAR_ALTERNATIVE_NEUTRAL";
    string NoteLinearAlternativeRed = "#NOTE_LINEAR_ALTERNATIVE_RED";
    string NoteLinearAlternativeGreen = "#NOTE_LINEAR_ALTERNATIVE_GREEN";
    string NoteLinearAlternativeBlue = "#NOTE_LINEAR_ALTERNATIVE_BLUE";
    string NoteLinearAlternativeYellow = "#NOTE_LINEAR_ALTERNATIVE_YELLOW";
    string NoteLinearAlternativePurple = "#NOTE_LINEAR_ALTERNATIVE_PURPLE";
    string NoteLinearAlternativeCyan = "#NOTE_LINEAR_ALTERNATIVE_CYAN";

    string NoteLinearHoldNeutral = "#NOTE_LINEAR_HOLD_NEUTRAL";
    string NoteLinearHoldRed = "#NOTE_LINEAR_HOLD_RED";
    string NoteLinearHoldGreen = "#NOTE_LINEAR_HOLD_GREEN";
    string NoteLinearHoldBlue = "#NOTE_LINEAR_HOLD_BLUE";
    string NoteLinearHoldYellow = "#NOTE_LINEAR_HOLD_YELLOW";
    string NoteLinearHoldPurple = "#NOTE_LINEAR_HOLD_PURPLE";
    string NoteLinearHoldCyan = "#NOTE_LINEAR_HOLD_CYAN";

    string LaneCircular = "#LANE_CIRCULAR";
    string LaneLinear = "#LANE_LINEAR";

    string SlotCircular = "#SLOT_CIRCULAR";
    string SlotLinear = "#SLOT_LINEAR";

    string JudgeLineCircular = "#JUDGE_LINE_CIRCULAR";
    string JudgeLineLinear = "#JUDGE_LINE_LINEAR";
}ParticleEffectName;

#endif
/*** End of inlined file: Particles.h ***/


/*** Start of inlined file: Levels.h ***/
#ifndef Levels_H
#define Levels_H

string getRef(int len = 32) {
	string res = "", key = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	for (int i = 0; i < len; i++) res += key[rand() % key.size()];
	return res;
}

class LevelEntity {
	public:

	string archetype = "";
	string name = getRef();

	struct LevelEntityData {
		string name = "";
		bool isValue = true;
		double value = 0;
		string refName = "";

		Constructor LevelEntityData(string name, bool isValue): name(name), isValue(isValue) {};

		LevelEntityData operator = (double val) {
			value = val;
			return *this;
		}
		LevelEntityData operator = (LevelEntity val) {
			refName = val.name;
			return *this;
		}
	};

	vector<LevelEntityData*> data;

	#define defineArchetypeName(name) \
		private: bool archetype_unused = [&](){ \
			archetype = name; return true; \
		}();

	#define defineLevelDataValue(name) \
		public: LevelEntityData name = LevelEntityData(#name, true); \
		private: bool unused_##name##_unused = [&](){ \
			data.push_back(&name); \
			return true; \
		}();

	#define defineLevelDataValueDetailed(varName, dataName) \
		public: LevelEntityData varName = LevelEntityData(dataName, true); \
		private: bool unused_##varName##_unused = [&](){ \
			data.push_back(&varName); \
			return true; \
		}();

	#define defineLevelDataRef(name) \
		public: LevelEntityData name = LevelEntityData(#name, false); \
		private: bool unused_##name##_unused = [&](){ \
			data.push_back(&name); \
			return true; \
		}();

	#define defineLevelDataRefDetailed(varName, dataName) \
		public: LevelEntityData varName = LevelEntityData(dataName, false); \
		private: bool unused_##varName##_unused = [&](){ \
			data.push_back(&varName); \
			return true; \
		}();

	Json::Value toJsonObject() { // Serialize
		Json::Value res;
		res["archetype"] = archetype;
		res["name"] = name;
		res["data"].resize(0);
		for (int i = 0; i < data.size(); i++) {
			auto item = *data[i]; Json::Value single;
			single["name"] = item.name;
			if (item.isValue) single["value"] = item.value;
			else single["ref"] = item.refName;
			res["data"].append(single);
		}
		return res;
	}

	void fromJsonObject(Json::Value res) { // Deserialize
		name = res["name"].asString();
		for (int i = 0; i < data.size(); i++) {
			string varName = (*data[i]).name;
            bool exist = false;
			for (int j = 0; j < res["data"].size(); j++) {
				if (res["data"][j]["name"] != varName) continue;
				if ((*data[i]).isValue) (*data[i]).value = res["data"][j]["value"].asDouble();
				else (*data[i]).refName = res["data"][j]["ref"].asString();
                exist = true;
			}
            SonolusAssert(
                exist,
                "Unsafe conversion from archetype \"%s\" to archetype \"%s\"!",
                res["archetype"].asCString(),
                archetype.c_str()
            );
		}
    }
};

class LevelRawData {
	public:

	double bgmOffset = 0;
	vector<Json::Value> entities;

#define DISABLE_INTERPRETER
	template<typename T>
	void append(T val) {
#undef DISABLE_INTERPRETER
		entities.push_back(val.toJsonObject());
#define DISABLE_INTERPRETER
	}

	template<typename T>
	T get(int id) {
		T res = *(new T);
#undef DISABLE_INTERPRETER
		res.fromJsonObject(entities[id]);
#define DISABLE_INTERPRETER
		return res;
	}

	template<typename T>
	T front() {
		return get<T>(0);
	}

	template<typename T>
	T back() {
		return get<T>(entities.size() - 1);
	}

	int size() { return entities.size(); }
#undef DISABLE_INTERPRETER

	Json::Value toJsonObject() {
		Json::Value res;
		res["bgmOffset"] = bgmOffset;
		res["entities"].resize(0);
		for (int i = 0; i < entities.size(); i++) res["entities"].append(entities[i]);
		return res;
	}
};

template<typename T1, typename T2>
T1 Transform(T2 v) {
    T1 res;
    res.fromJsonObject(v.toJsonObject());
    return res;
}

#endif
/*** End of inlined file: Levels.h ***/


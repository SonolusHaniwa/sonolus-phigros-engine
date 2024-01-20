#ifndef FUNCNODE_H
#define FUNCNODE_H

using namespace std;

/*class FuncNode {
    public:
    bool isValue;
    double value;
    string func;
    vector<FuncNode> args;

    FuncNode(){};
    FuncNode(int value): isValue(true), value(value){};
    FuncNode(double value): isValue(true), value(value){};
	FuncNode(string func, vector<FuncNode> args): isValue(false), func(func), args(args){};
    FuncNode(initializer_list<FuncNode> args): isValue(false), func("Execute"), args(args){};

    string stringify(int tabSize = 2, int maxSize = 100, int tabDepth = 0) {
        string result = "", tab = "", singleTab = "";
        for (int i = 1; i <= tabDepth * tabSize; i++) tab.push_back(' ');
        for (int i = 1; i <= tabSize; i++) singleTab.push_back(' ');
        if (isValue) return tab + "ValueNode { value: " + to_string(value) + " }";
        result += tab + "FuncNode {\n";
        result += tab + singleTab + "func: '" + func + "',\n";
        result += tab + singleTab + "args: [\n";
        for (int i = 0; i < min((int)args.size(), maxSize); i++)
            result += args[i].stringify(tabSize, maxSize, tabDepth + 2) + (i == args.size() - 1 ? "" : ",") + "\n";
        if (args.size() > maxSize)
            result += tab + singleTab + singleTab + "... " + to_string(args.size() - maxSize) + " more items\n";
        result += tab + singleTab + "]\n";
        result += tab + "}";
        return result;
    }
};

ostream& operator << (ostream& out, FuncNode x) {
    out << x.stringify();
    return out;
}*/

map<EngineDataNode, int> hashMap;
vector<EngineDataNode> dataContainer;

class FuncNode {
	public:
	int nodeId = 0;
	
	private:
	bool writeable = true;
	void checkWriteable() {
		// if (!writeable) {
		// 	throwError("You cannot write FuncNode again!");
		// 	exit(1);
		// } writeable = true;
	}
	int allocate(EngineDataNode tmp) {
		globalCounter++;
		if (hashMap.find(tmp) != hashMap.end()) return hashMap[tmp];
		hashMap[tmp] = dataContainer.size(); dataContainer.push_back(tmp);
		return hashMap[tmp];
	}

	public:
	int getNodeId() {
		return nodeId;
	}

	FuncNode(){ checkWriteable(); }
	FuncNode(int value) {
		checkWriteable();
		EngineDataNode tmp = EngineDataValueNode(value);
		nodeId = allocate(tmp);
	}
	FuncNode(double value) {
		checkWriteable();
		EngineDataNode tmp = EngineDataValueNode(value);
		nodeId = allocate(tmp);
	}
	FuncNode(string func, vector<FuncNode> args) {
		checkWriteable();
		vector<int> nodes;
		for (int i = 0; i < args.size(); i++) nodes.push_back(args[i].getNodeId());
		EngineDataNode tmp = EngineDataFunctionNode(func, nodes);
		nodeId = allocate(tmp);
	}
	FuncNode(initializer_list<FuncNode> args) {
		checkWriteable();
		vector<FuncNode> args2 = args; vector<int> nodes;
		if (args2.size() == 1) nodeId = args2[0].getNodeId();
		else {
			for (int i = 0; i < args2.size(); i++) nodes.push_back(args2[i].getNodeId());
			EngineDataNode tmp = EngineDataFunctionNode("Execute", nodes);
			nodeId = allocate(tmp);
		}
	}
	FuncNode(vector<FuncNode> args) {
		checkWriteable(); vector<int> nodes;
		if (args.size() == 1) nodeId = args[0].getNodeId();
		else {
			for (int i = 0; i < args.size(); i++) nodes.push_back(args[i].getNodeId());
			EngineDataNode tmp = EngineDataFunctionNode("Execute", nodes);
			nodeId = allocate(tmp);
		}
	}

	string toString(int tabLength = 0) {
		stringstream ss; string pre = "";
		for (int i = 0; i < tabLength; i++) pre += "  ";
		EngineDataNode node = dataContainer[nodeId];
		if (node.type == "value") return ss << pre << node.value.value << ", ", ss.str();
		ss << pre << node.func.func << "(" << endl;
		for (int i = 0; i < node.func.args.size(); i++) {
			FuncNode newNode = 0;
			newNode.nodeId = node.func.args[i];
			ss << newNode.toString(tabLength + 1) << endl;
		} ss << pre << "),";
		return ss.str();
	}

	// 供 CustomClass 使用的参数，防止 CustomClass 编译报错，实际 FuncNode 用不到
	int classSize = 1;
	vector<function<void(FuncNode)> > deserializiers = {
		[&](FuncNode var){ (*this) = var; }
	};
	vector<function<FuncNode()> > serializiers = {
		[&](){ return *this; }
	};
	vector<FuncNode> serialize() {
		return { *this };
	}
	void deserialize(FuncNode val) {
		(*this) = val;
	}
};

ostream& operator << (ostream& fout, FuncNode node) {
	return fout << node.toString();
}
#endif

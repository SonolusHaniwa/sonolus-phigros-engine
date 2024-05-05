stack<vector<FuncNode> > nodesContainer;
stack<int> blockCounter;
stack<bool> addBlockCounter;
void createNodeContainer(bool);
void R(FuncNode);

template<int identifierId>
class Variable {
    public:

    int offset;
    Variable(){
    	offset = allocatorSize[identifierId]++;
    }
    Variable(int val) {
    	offset = allocatorSize[identifierId]++;
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val}));
    }
    Variable(double val) {
    	offset = allocatorSize[identifierId]++;
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val}));
    }
	Variable(const Variable<identifierId>& val) {
		offset = allocatorSize[identifierId]++;
		// cout << "Copy Constructor: " << identifierId << " " << val.offset << endl;
		// DumpTraceback();
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val.get()}));
	}
	template<int T> Variable(Variable<T> val) {
		// cout << "Constructor: " << identifierId << " " << T << " " << val.offset << endl;
     	offset = allocatorSize[identifierId]++;
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val.get()}));
    }
    Variable(FuncNode val) {
    	offset = allocatorSize[identifierId]++;
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val}));
    }
    Variable(int offset, bool unused) {
    	this->offset = offset;
    }

	FuncNode get() const {
		return FuncNode(RuntimeFunction.Get, {identifierId, offset});
	}
    void set(int val) {
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val}));
    }
    void set(double val) {
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val}));
    }
    void set(FuncNode val) {
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val}));
    }
    template<int T> void set(Variable<T> val) {
    	// cout << T << " " << val.offset << endl; DumpTraceback();
		R(FuncNode(RuntimeFunction.Set, {identifierId, offset, val.get()}));
    }

	// template<int T>
	// Variable<identifierId> operator = (Variable<T> val) { set(val); return *this; }
	Variable<identifierId> operator = (double val) { set(val); return Variable<identifierId>(offset, true); }
	Variable<identifierId> operator = (FuncNode val) { set(val); return Variable<identifierId>(offset, true); }
	// template<typename T>
    // Variable<identifierId> operator = (T val) { set(val); return *this; };

    operator FuncNode() {
    	return get();
    }

    int classSize = 1;
   	vector<function<void(FuncNode)> > deserializiers = {
   		[&](FuncNode var){ set(var); }
   	};
   	vector<function<FuncNode()> > serializiers = {
   		[&](){ return get(); }
   	};
   	vector<FuncNode> serialize() {
   		return { get() };
   	}
   	void deserialize(vector<FuncNode> val) {
   		if (val.size() != classSize) throwError("Failed to deserialize data! Expect len = " +
   			to_string(classSize) + " but read len = " + to_string(val.size()));
   		set(val[0]);
   	}
};

void createNodeContainer(bool add = true) {
	nodesContainer.push(vector<FuncNode>());
	if (add) blockCounter.push(1);
	else blockCounter.top() += 1;
	addBlockCounter.push(add);
}
void R(FuncNode body) {
	if (nodesContainer.size() == 0) throwError("You shouldn't execute anything outside Archetypes!");
	nodesContainer.top().push_back(body);
}
void Run(FuncNode body) {
	if (nodesContainer.size() == 0) throwError("You shouldn't execute anything outside Archetypes!");
	nodesContainer.top().push_back(body);
}
FuncNode mergeNodeContainer() {
	assert(nodesContainer.size());
	auto c = nodesContainer.top(); nodesContainer.pop();
	if (addBlockCounter.top()) blockCounter.pop();
	else blockCounter.top() -= 1;
	addBlockCounter.pop();
	if (c.size() == 0) throwError("Your Sonolus will boom if I accept your empty function.");
	FuncNode res = FuncNode(RuntimeFunction.Block, {
		c.size() == 1 ? c[0] : FuncNode(RuntimeFunction.Execute, c)
	}); return res;
}

bool unused_nodesContainer_unused = [](){ createNodeContainer(); return true; }();

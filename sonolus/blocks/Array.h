template<int identifierId, typename T>
class Array {
	public:

	int capacity = 0, offset = 0, classSize = 0;

	Array(){}
	Array(int siz) {
		offset = allocatorSize[identifierId];
		capacity = siz; classSize = T().classSize;
		allocatorSize[identifierId] = offset;
		allocatorSize[identifierId] += capacity * classSize;
	}
	SonolusApi set(FuncNode index, T obj) {
		FUNCBEGIN
		auto c = obj.serialize();
		// IF (Get(4003, 0) != 1) Debuglog(c[0]); FI
		for (int i = 0; i < classSize; i++)
			Set(identifierId, offset + index * classSize + i, c[i]);
		return VOID;
	}
	T get(FuncNode index) {
		vector<FuncNode> vals;
		for (int i = 0; i < classSize; i++)
			vals.push_back(Get(identifierId, offset + index * classSize + i));
		T res = T(); res.deserialize(vals);
		return res;
	}
	T operator [] (FuncNode id) {
		return get(id);
	}
	// bool outputed = false;
	SonolusApi indexOf(T obj, let en) { // 时间复杂度 O(capacity * classSize)
		FUNCBEGIN
		FOR (i, 0, Min(capacity, en), 1) { 
		// FOR (i, 0, capacity, 1) { // 暂时禁用 en 选项
			// if (!outputed) cout << get(i) << endl << obj << endl, outputed = true;
			IF (get(i) == obj) Return(i); FI
		} DONE
		Return(-1);
		return VAR;
	}
	SonolusApi indexOf(T obj) { // 时间复杂度 O(capacity * classSize)
		FUNCBEGIN
		Return(indexOf(obj, capacity));
		return VAR;
	}
	SonolusApi clear() {
		FUNCBEGIN
		FOR (i, offset, offset + capacity * classSize, 1) {
			Set(identifierId, i, 0);
		} DONE
		return VOID;
	}
};

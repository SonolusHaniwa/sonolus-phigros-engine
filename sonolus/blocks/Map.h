template<int identifierId, typename T1, typename T2> 
class Map {
	public:

	int capacity = 0, offset = 0;
	Variable<identifierId> size;
	Array<identifierId, T1> key; 
	Array<identifierId, T2> val;

	Map(){}
	Map(int siz) {
		capacity = siz;
		key = Array<identifierId, T1>(siz); 
		val = Array<identifierId, T2>(siz);
	}
	SonolusApi add(T1 key, T2 value) {
		FUNCBEGIN
		this->key.set(size, key);
		this->val.set(size, value);
		size = size + 1;
		return VOID;
	}
	T1 getKeyById(FuncNode index) {
		return key[index];
	}
	T2 getValById(FuncNode index) {
		return val[index];
	}
	SonolusApi indexOf(T1 key) { // 时间复杂度 O(size * classSize_key)
		FUNCBEGIN
		Return(this->key.indexOf(key, size));
		return VAR;
	}
	T2 get(T1 key) { // 不安全的函数，使用前请用 indexOf() != -1 判断 key 的存在性
		return getValById(indexOf(key));
	}
	T2 operator [] (T1 key) { // 问题同上
		return get(key);
	}
	SonolusApi set(T1 key, T2 value) {
		FUNCBEGIN
		let id = indexOf(key);
		IF (id == -1) add(key, value); 
		ELSE val.set(id, value); FI
		return VOID;
	}
	SonolusApi clear() {
		FUNCBEGIN
		size = 0;
		key.clear(); val.clear();
		return VOID;
	}
};

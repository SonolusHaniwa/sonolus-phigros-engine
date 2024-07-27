template<int identifierId>
class BlockPointer {
    public:

	FuncNode offset = 0;
	BlockPointer(){}
	BlockPointer(FuncNode offset): offset(offset){}
	
    FuncNode get(FuncNode i) {
//        if (size != -1 && i.isValue == true &&
//            (i.value >= size || i.value < 0)) throwWarning("");
        return FuncNode(RuntimeFunction.Get, {
        	identifierId,
        	FuncNode(RuntimeFunction.Add, {i, offset})
        });
    }

    void set(FuncNode i, FuncNode value) {
//        if (size != -1 && i.isValue == true &&
//            (i.value >= size || i.value < 0)) throwWarning("");
        R(FuncNode(RuntimeFunction.Set, { 
        	identifierId, 
        	FuncNode(RuntimeFunction.Add, {i, offset}),
        	value
        }));
    }
};

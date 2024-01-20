template<int identifierId>
class BlockArray {
    public:

	int blockSize = 1;
    BlockArray(int blockSize) {
    	this->blockSize = blockSize;
    }

    BlockPointer<identifierId> get(FuncNode i) {
    	return BlockPointer<identifierId>(FuncNode(RuntimeFunction.Multiply, {blockSize, i}));
    }

    BlockPointer<identifierId> operator [] (FuncNode i) {
    	return get(i);
    }
};

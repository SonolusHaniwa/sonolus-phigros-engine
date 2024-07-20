class UpdateJudgment: public Archetype {
	public:

	static const bool disableGlobalPreprocess = true;
	static constexpr const char* name = "Phigros Update Judgment";
	Variable<EntityMemoryId> entityId;
	BlockPointer<EntitySharedMemoryArrayId> entity = EntitySharedMemoryArray[entityId];

	SonolusApi spawnTime() { return entity.get(1); }
	SonolusApi despawnTime() { 
		return Max(
			entity.get(1) + 1,
			If(
				EntitySharedMemoryArray[entity.get(0)].get(1) >= entity.get(1),
				EntitySharedMemoryArray[entity.get(0)].get(1),
				999999
			)
		);
	}

 	// int updateSequentialOrder = 1;
	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (lastUpdatedTime > entity.get(6)) Return(0); FI
		combo = entity.get(2);
		maxCombo = entity.get(3);
		judgeStatus = entity.get(4);
		accscore = entity.get(5);
		lastUpdatedTime = entity.get(6);
		return VOID;
	}
};

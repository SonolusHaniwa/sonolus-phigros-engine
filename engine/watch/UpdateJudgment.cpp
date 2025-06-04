class UpdateJudgment: public Archetype {
	public:

	string name = "Phigros Update Judgment";
	var entityId;
	EntitySharedMemoryArrayGroup entity = EntitySharedMemoryArray[entityId];

	SonolusApi spawnTime() { return entity.generic[1]; }
	SonolusApi despawnTime() { 
		return Max(
			entity.generic[1] + 1,
			If(
				EntitySharedMemoryArray[entity.generic[0]].generic[1] >= entity.generic[1],
				EntitySharedMemoryArray[entity.generic[0]].generic[1],
				999999
			)
		);
	}

 	// int updateSequentialOrder = 1;
	SonolusApi updateSequential() {
		// DebugLog(entityId); DebugLog(entity.generic[1]);
		if (lastUpdatedId > entity.generic[6]) return;
		combo = entity.generic[2];
		maxCombo = entity.generic[3];
		judgeStatus = entity.generic[4];
		accscore = entity.generic[5];
		lastUpdatedId = entity.generic[6];
	}
};

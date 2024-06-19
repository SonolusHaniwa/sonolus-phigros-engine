Map<LevelMemoryId, let, let> disallowEmptiesNow(16);
Map<LevelMemoryId, let, let> disallowEmptiesOld(16);

class ClaimManager {
	public:

	Map<LevelMemoryId, let, let> claimed = Map<LevelMemoryId, let, let>(16);

	class ClaimInfo {
		public:

		let cx, cy, rotate; // 中心点位置和判定角度
		let time;
		
		SonolusApi getDis(let x, let y) {
			FUNCBEGIN
			IF (rotate == PI / 2 || rotate == PI / 2 * 3) {
				Return(Abs(x - cx));
			} FI
			let k = Tan(rotate), b = cy - k * cx;
			let dis = Abs(-1 * k * x + y - b) / Power({k * k + 1, 0.5});
			Return(dis);
			return VAR;
		}
		SonolusApi contain(let x, let y) {
			FUNCBEGIN
			Return(getDis(x, y) <= judgeDistanceLimit);
			return VAR;
		}
	};

	ClaimInfo getInfo(let index) {
		auto note = EntityDataArray[index];
		auto judgeline = EntitySharedMemoryArray[note.get(8)];
		let dx = note.get(1) * stage.w * 0.05625;
		let jx = judgeline.get(1), jy = judgeline.get(2);
		let rotate = judgeline.get(3);
		let vecX = dx * Cos(rotate), vecY = dx * Sin(rotate);
		let cx = jx + vecX, cy = jy + vecY;
		return {
			cx: cx,
			cy: cy,
			rotate: rotate + PI / 2,
			time: note.get(0)
		};
	}

	SonolusApi findBestTouchIndex(let index) {
		FUNCBEGIN
		ClaimInfo origin = getInfo(index);
		var res = -1, minDis = 1e9;
		FOR (i, 0, touches.size, 1) {
			IF (touches[i].started == 0) CONTINUE; FI
			IF (origin.contain(touches[i].x, touches[i].y) == 0) CONTINUE; FI

			let dis = origin.getDis(touches[i].x, touches[i].y);
			IF (res != -1 && minDis <= dis) CONTINUE; FI

			let claimIndex = claimed.indexOf(touches[i].id);
			IF (claimIndex == -1) {
				res = touches[i].id; minDis = dis;
				CONTINUE;
			} FI

			ClaimInfo claim = getInfo(claimed.getValById(claimIndex));
			IF (origin.time > claim.time) CONTINUE; FI
			IF (origin.time < claim.time) {
				res = touches[i].id; minDis = dis;
				CONTINUE;
			} FI
         
			IF (dis > claim.getDis(touches[i].x, touches[i].y)) CONTINUE; FI
			res = touches[i].id; minDis = dis;
		} DONE
		Return(res);
		return VAR;
	}

	SonolusApi claim(let index) {
		FUNCBEGIN
		var currentId = index;
		ClaimInfo info = getInfo(currentId);
		WHILE (true) {
			var touchIndex = findBestTouchIndex(currentId);
			IF (touchIndex == -1) BREAK; FI
			disallowEmptiesNow.set(touchIndex, 1);
			
			let claimIndex = claimed.indexOf(touchIndex);
			IF (claimIndex == -1) {
				claimed.set(touchIndex, currentId); 
				BREAK;
			} FI

			var tmp = currentId;
			currentId = claimed.getValById(claimIndex);
			claimed.set(touchIndex, tmp);
		} DONE
		return VOID;
	}

	SonolusApi getClaimedTouchIndex(let index) {
		FUNCBEGIN
		FOR (i, 0, claimed.size, 1) {
			IF (claimed.getValById(i) == index) {
				Return(claimed.getKeyById(i)); 
			} FI
		} DONE 
		Return(-1);
		return VAR;
	}

	SonolusApi clear() {
		FUNCBEGIN
		claimed.clear();
		return VOID;
	}
};

ClaimManager claimStartManager = ClaimManager();
SonolusApi claimStart(let index) {
	FUNCBEGIN
	claimStartManager.claim(index);
	return VOID;
}
SonolusApi getClaimedStart(let index) {
	FUNCBEGIN
	Return(claimStartManager.getClaimedTouchIndex(index));
	return VAR;
}

SonolusApi hasTouch(let index) {
	FUNCBEGIN
	ClaimManager::ClaimInfo info = claimStartManager.getInfo(index);
	FOR (i, 0, touches.size, 1) {
		IF (info.contain(touches[i].x, touches[i].y)) {
			Return(1);
		} FI
	} DONE
	Return(0);
	return VAR;
}

class InputManager: public Archetype {
	public:

	static constexpr const char* name = "Phigros Input Manager";

	SonolusApi spawnOrder() { return 1; }
	SonolusApi shouldSpawn() { return 1; }
	
	SonolusApi updateSequential() {
		FUNCBEGIN
		claimStartManager.clear();
		disallowEmptiesOld.clear();
		FOR (i, 0, disallowEmptiesNow.size, 1) {
			disallowEmptiesOld.set(
				disallowEmptiesNow.getKeyById(i), 
				disallowEmptiesNow.getValById(i)
			);
		} DONE
		disallowEmptiesNow.clear();
		FOR (i, 0, touches.size, 1) {
			IF (disallowEmptiesOld.indexOf(touches[i].id) != -1)
				disallowEmptiesNow.add(touches[i].id, 1); 
			FI
		} DONE
		return VOID;
	}
};

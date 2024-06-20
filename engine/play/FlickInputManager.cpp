Map<LevelMemoryId, let, let> flickDisallowEmptiesNow(16);
Map<LevelMemoryId, let, let> flickDisallowEmptiesOld(16);
Map<LevelMemoryId, let, let> lastdx(16);
Map<LevelMemoryId, let, let> lastdy(16);
Map<LevelMemoryId, let, let> touchDistance(16);
Variable<LevelMemoryId> lastUpdateTime;
Variable<LevelMemoryId> allowClaim;

double minFlickV = 0.5;
double updateTimeLimit = 1.0 / 60.0; // 目标 fps: 60
SonolusApi calcFlickV(let dx, let dy, let deltaTime) {
	FUNCBEGIN
	var d = Power({dx * dx + dy * dy, 0.5});
	var t = deltaTime;
	Return(d / t);
	return VAR;
};

class FlickClaimManager {
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
			IF (calcFlickV(touches[i].dx, touches[i].dy, times.delta) < minFlickV) CONTINUE; FI
			var id = flickDisallowEmptiesNow.indexOf(touches[i].id);
			IF (id != -1) CONTINUE; FI
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
			flickDisallowEmptiesNow.set(touchIndex, 1);
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

FlickClaimManager flickClaimStartManager = FlickClaimManager();
SonolusApi flickClaimStart(let index) {
	FUNCBEGIN
	IF (allowClaim) Return(0); FI
	flickClaimStartManager.claim(index);
	return VOID;
}
SonolusApi flickGetClaimedStart(let index) {
	FUNCBEGIN
	Return(flickClaimStartManager.getClaimedTouchIndex(index));
	return VAR;
}

class FlickInputManager: public Archetype {
	public:

	static constexpr const char* name = "Phigros Flick Input Manager";

	SonolusApi spawnOrder() { return 1; }
	SonolusApi shouldSpawn() { return 1; }
	
	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.now - lastUpdateTime < updateTimeLimit) {
			FOR (i, 0, touches.size, 1) {
				lastdx.set(touches[i].id, touches[i].dx);
				lastdy.set(touches[i].id, touches[i].dy);
			} DONE
			allowClaim = 0;
			Return(0);
		} FI
		flickClaimStartManager.clear();
		flickDisallowEmptiesOld.clear();
		FOR (i, 0, flickDisallowEmptiesNow.size, 1) {
			flickDisallowEmptiesOld.set(
				flickDisallowEmptiesNow.getKeyById(i), 
				flickDisallowEmptiesNow.getValById(i)
			);
		} DONE
		flickDisallowEmptiesNow.clear();
		FOR (i, 0, touches.size, 1) {
			var lastdxIndex = lastdx.indexOf(touches[i].id);
			var lastdyIndex = lastdy.indexOf(touches[i].id);
			var lastdxValue = If(lastdxIndex == -1, 0, lastdx.getValById(lastdxIndex));
			var lastdyValue = If(lastdyIndex == -1, 0, lastdy.getValById(lastdyIndex));
			var dx = lastdxValue + touches[i].dx;
			var dy = lastdyValue + touches[i].dy;
			var timeDis = times.now - lastUpdateTime;
            IF (calcFlickV(dx, dy, timeDis) < minFlickV) CONTINUE; FI
			var id = flickDisallowEmptiesOld.indexOf(touches[i].id);
            IF (id == -1) CONTINUE; FI
			flickDisallowEmptiesNow.add(touches[i].id, 1); 
			// Debuglog(flickDisallowEmptiesNow.size);
		} DONE
		lastUpdateTime = times.now;
		lastdx.clear(); lastdy.clear();
		allowClaim = 1;
		return VOID;
	}
};

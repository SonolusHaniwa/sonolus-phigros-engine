Map<LevelMemoryId, let, let> flickDisallowEmptiesNow(16);
Map<LevelMemoryId, let, let> flickDisallowEmptiesOld(16);
Map<LevelMemoryId, let, let> lastdx(16);
Map<LevelMemoryId, let, let> lastdy(16);
Map<LevelMemoryId, let, let> lastdxOld(16);
Map<LevelMemoryId, let, let> lastdyOld(16);

double minFlickV = 0.2;
SonolusApi calcV(Touch touch) {
	FUNCBEGIN
	Return(Power({touch.dx * touch.dx + touch.dy * touch.dy, 0.5}) / times.delta);
	return VAR;
}

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
            IF (calcV(touches[i]) < minFlickV) CONTINUE; FI
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
			IF (index > claimed.getValById(claimIndex)) CONTINUE; FI // nmd 如果 time 和 dis 完全相等的话会导致一直 claim，然后 Sonolus 死机
			// mlgb 老子在这里调了 6 个小时结果是 nm 这个问题
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
		flickClaimStartManager.clear();

		flickDisallowEmptiesOld.clear();
		FOR (i, 0, flickDisallowEmptiesNow.size, 1) {
			flickDisallowEmptiesOld.set(
				flickDisallowEmptiesNow.getKeyById(i), 
				flickDisallowEmptiesNow.getValById(i)
			);
		} DONE
		flickDisallowEmptiesNow.clear();

		lastdxOld.clear(); lastdyOld.clear();
		FOR (i, 0, lastdx.size, 1) {
			lastdxOld.set(lastdx.getKeyById(i), lastdx.getValById(i));
			lastdyOld.set(lastdy.getKeyById(i), lastdy.getValById(i));
		} DONE
		lastdx.clear(); lastdy.clear();

		FOR (i, 0, touches.size, 1) {
			var id = flickDisallowEmptiesOld.indexOf(touches[i].id);
			lastdx.set(touches[i].id, touches[i].dx); lastdy.set(touches[i].id, touches[i].dy);
            IF (id == -1) CONTINUE; FI
			var lastdxIndex = lastdxOld.indexOf(touches[i].id);
			var lastdxValue = If(lastdxIndex == -1, 0, lastdxOld.getValById(lastdxIndex));
			var lastdyValue = If(lastdxIndex == -1, 0, lastdyOld.getValById(lastdxIndex));
            IF (calcV(touches[i]) < minFlickV) {
				lastdx.set(touches[i].id, lastdxValue); lastdy.set(touches[i].id, lastdxValue);
				flickDisallowEmptiesNow.add(touches[i].id, 1); 
				CONTINUE;
			} FI
			var dotmul = lastdxValue * touches[i].dx + lastdyValue * touches[i].dy;
			IF (dotmul <= 0) CONTINUE; FI
			flickDisallowEmptiesNow.add(touches[i].id, 1); 
		} DONE
		return VOID;
	}
};

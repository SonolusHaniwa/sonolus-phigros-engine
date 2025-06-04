Map<var, var, 16> disallowEmptiesNow;
Map<var, var, 16> disallowEmptiesOld;

class ClaimManager {
	public:

	Map<var, var, 16> claimed;

	class ClaimInfo {
		public:

		var cx, cy, rotate; // 中心点位置和判定角度
		var time;
		
		SonolusApi getDis(var x, var y) {
			if (rotate == PI / 2 || rotate == PI / 2 * 3) return Abs(x - cx);
			var k = Tan(rotate), b = cy - k * cx;
			var dis = Abs(-1 * k * x + y - b) / Power({k * k + 1, 0.5});
			return dis;
		}
		SonolusApi contain(var x, var y) {
			return getDis(x, y) <= judgeDistanceLimit;
		}
	};

	ClaimInfo getInfo(var index) {
		auto note = EntityDataArray[index];
		auto judgeline = EntitySharedMemoryArray[
			note.generic[If(EntityInfoArray[index].archetype == getAid(HoldNote), 9, 8)]
		];
		var dx = note.generic[1] * stage.w * 0.05625;
		var jx = judgeline.generic[1], jy = judgeline.generic[2];
		var rotate = judgeline.generic[3];
		var vecX = dx * Cos(rotate), vecY = dx * Sin(rotate);
		var cx = jx + vecX, cy = jy + vecY;
		return {
			cx: cx,
			cy: cy,
			rotate: rotate + PI / 2,
			time: note.generic[0]
		};
	}

	SonolusApi findBestTouchIndex(var index) {
		ClaimInfo origin = getInfo(index);
		var res = -1, minDis = 1e9;
		for (var i = 0; i < touchCount; i++) {
			if (touches[i].started == 0) continue;
			if (origin.contain(touches[i].x, touches[i].y) == 0) continue;

			var dis = origin.getDis(touches[i].x, touches[i].y);
			if (res != -1 && minDis <= dis) continue;

			var claimIndex = claimed.indexOf(touches[i].id);
			if (claimIndex == -1) {
				res = touches[i].id; minDis = dis;
				continue;
			}

			ClaimInfo claim = getInfo(claimed.getValue(claimIndex));
			if (origin.time > claim.time) continue;
			if (origin.time < claim.time) {
				res = touches[i].id; minDis = dis;
				continue;
			}
         
			if (dis > claim.getDis(touches[i].x, touches[i].y)) continue;
			if (index > claimed.getValue(claimIndex)) continue; // nmd 如果 time 和 dis 完全相等的话会导致一直 claim，然后 Sonolus 死机
			// mlgb 老子在这里调了 6 个小时结果是 nm 这个问题
			res = touches[i].id; minDis = dis;
		}
		return res;
	}

	SonolusApi claim(var index) {
		var currentId = index;
		ClaimInfo info = getInfo(currentId);
		while (true) {
			var touchIndex = findBestTouchIndex(currentId);
			if (touchIndex == -1) break;
			disallowEmptiesNow.set(touchIndex, 1);
			
			var claimIndex = claimed.indexOf(touchIndex);
			if (claimIndex == -1) {
				claimed.set(touchIndex, currentId); 
				break;
			}

			var tmp = currentId;
			currentId = claimed.getValue(claimIndex);
			claimed.set(touchIndex, tmp);
		}
	}

	SonolusApi getClaimedTouchIndex(var index) {
		for (var i = 0; i < claimed.count; i++)
			if (claimed.getValue(i) == index) return claimed.getKey(i);
		return -1;
	}

	Blocked SonolusApi clear() {
		claimed.clear();
	}
};

ClaimManager claimStartManager = ClaimManager();
SonolusApi claimStart(var index) {
	claimStartManager.claim(index);
}
SonolusApi getClaimedStart(var index) {
	return claimStartManager.getClaimedTouchIndex(index);
}

SonolusApi hasTouch(var index) {
	ClaimManager::ClaimInfo info = claimStartManager.getInfo(index);
	for (var i = 0; i < touchCount; i++)
		if (info.contain(touches[i].x, touches[i].y)) return true;
	return false;
}

class InputManager: public Archetype {
	public:

	string name = "Phigros Input Manager";

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }
	
	SonolusApi updateSequential() {
		claimStartManager.clear();
		disallowEmptiesOld.clear();
		for (var i = 0; i < disallowEmptiesNow.count; i++)
			disallowEmptiesOld.set(
				disallowEmptiesNow.getKey(i), 
				disallowEmptiesNow.getValue(i)
			);
		disallowEmptiesNow.clear();
		for (var i = 0; i < touchCount; i++)
			if (disallowEmptiesOld.indexOf(touches[i].id) != -1)
				disallowEmptiesNow.set(touches[i].id, 1);
	}
};

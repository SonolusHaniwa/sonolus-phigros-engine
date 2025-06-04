Map<var, var, 16> flickDisallowEmptiesNow;
Map<var, var, 16> flickDisallowEmptiesOld;
Map<var, var, 16> lastdx;
Map<var, var, 16> lastdy;
Map<var, var, 16> lastdxOld;
Map<var, var, 16> lastdyOld;

double minFlickV = 0.2;
SonolusApi calcV(auto touch) {
	return Power({touch.dx * touch.dx + touch.dy * touch.dy, 0.5}) / times.delta;
}

class FlickClaimManager {
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
		auto judgeline = EntitySharedMemoryArray[note.generic[8]];
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
            if (calcV(touches[i]) < minFlickV) continue;
			var id = flickDisallowEmptiesNow.indexOf(touches[i].id);
			if (id != -1) continue;
			if (origin.contain(touches[i].x, touches[i].y) == 0) continue;

			var dis = Min(
				origin.getDis(touches[i].x, touches[i].y),
				origin.getDis(touches[i].x - touches[i].dx, touches[i].y - touches[i].dy)
			);
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
         
			if (dis > Min(
				claim.getDis(touches[i].x, touches[i].y),
				claim.getDis(touches[i].x - touches[i].dx, touches[i].y - touches[i].dy)
			)) continue;
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
			flickDisallowEmptiesNow.set(touchIndex, 1);
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

FlickClaimManager flickClaimStartManager = FlickClaimManager();
SonolusApi flickClaimStart(var index) {
	flickClaimStartManager.claim(index);
}
SonolusApi flickGetClaimedStart(var index) {
	return flickClaimStartManager.getClaimedTouchIndex(index);
}

class FlickInputManager: public Archetype {
	public:

	string name = "Phigros Flick Input Manager";

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }
	
	SonolusApi updateSequential() {
		flickClaimStartManager.clear();

		flickDisallowEmptiesOld.clear();
		for (var i = 0; i < flickDisallowEmptiesNow.count; i++)
			flickDisallowEmptiesOld.set(
				flickDisallowEmptiesNow.getKey(i), 
				flickDisallowEmptiesNow.getValue(i)
			);
		flickDisallowEmptiesNow.clear();

		lastdxOld.clear(); lastdyOld.clear();
		for (var i = 0; i < lastdx.count; i++) {
			lastdxOld.set(lastdx.getKey(i), lastdx.getValue(i));
			lastdyOld.set(lastdy.getKey(i), lastdy.getValue(i));
		}
		lastdx.clear(); lastdy.clear();

		for (var i = 0; i < touchCount; i++) {
			var id = flickDisallowEmptiesOld.indexOf(touches[i].id);
			lastdx.set(touches[i].id, touches[i].dx); lastdy.set(touches[i].id, touches[i].dy);
            if (id == -1) continue;
			var lastdxIndex = lastdxOld.indexOf(touches[i].id);
			var lastdxValue = If(lastdxIndex == -1, 0, lastdxOld.getValue(lastdxIndex));
			var lastdyValue = If(lastdxIndex == -1, 0, lastdyOld.getValue(lastdxIndex));
            if (calcV(touches[i]) < minFlickV) {
				lastdx.set(touches[i].id, lastdxValue); lastdy.set(touches[i].id, lastdxValue);
				flickDisallowEmptiesNow.set(touches[i].id, 1); 
				continue;
			}
			var dotmul = lastdxValue * touches[i].dx + lastdyValue * touches[i].dy;
			if (dotmul <= 0) continue;
			flickDisallowEmptiesNow.set(touches[i].id, 1); 
		}
	}
};

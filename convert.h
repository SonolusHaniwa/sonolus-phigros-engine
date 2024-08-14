#pragma once
#define INFO if (levelData.size() * 100 / total != last) \
	last = levelData.size() * 100 / total, cout << "[INFO] Processed: " << levelData.size() << "/" << total << "(" << last << "%)" << endl;

const int official_version = 3;
const int optimizer_version = 13;

class InitializationEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Initialization");
};

class StageControllerEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Stage Controller");
};

class InputManagerEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Input Manager");
};

class FlickInputManagerEntity: public LevelEntity {
	public:
	
	defineArchetypeName("Phigros Flick Input Manager");
};

class JudgelineEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Judgeline");
	defineLevelDataRef(speedEvent);
	defineLevelDataRef(moveXEvent);
	defineLevelDataRef(moveYEvent);
	defineLevelDataRef(rotateEvent);
	defineLevelDataRef(disappearEvent);
	defineLevelDataValue(bpm);
};

class SpeedEventEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Judgeline Speed Event");
	defineLevelDataValue(startTime);
	defineLevelDataValue(endTime);
	defineLevelDataValue(start);
	defineLevelDataValue(end);
	defineLevelDataRef(next);
};

class CommonEventEntity: public LevelEntity {
	public:

	defineLevelDataValue(startTime);
	defineLevelDataValue(endTime);
	defineLevelDataValue(start);
	defineLevelDataValue(end);
	defineLevelDataValue(easing);
	defineLevelDataValue(easingLeft);
	defineLevelDataValue(easingRight);
	defineLevelDataValue(bezier);
	defineLevelDataValue(bezierP1);
	defineLevelDataValue(bezierP2);
	defineLevelDataValue(bezierP3);
	defineLevelDataValue(bezierP4);
	defineLevelDataRef(next);
};

class MoveXEventEntity: public CommonEventEntity {
	public:

	defineArchetypeName("Phigros Judgeline Move X Event");
};

class MoveYEventEntity: public CommonEventEntity {
	public:

	defineArchetypeName("Phigros Judgeline Move Y Event");
};

class RotateEventEntity: public CommonEventEntity {
	public:

	defineArchetypeName("Phigros Judgeline Rotate Event");
};

class DisappearEventEntity: public CommonEventEntity {
	public:

	defineArchetypeName("Phigros Judgeline Disappear Event");
};

class CommonNoteEntity: public LevelEntity {
	public:

	defineLevelDataValue(time);
	defineLevelDataValue(positionX);
	defineLevelDataValue(holdTime);
	defineLevelDataValue(speed);
	defineLevelDataValue(floorPosition);
	defineLevelDataValue(isAbove);
	defineLevelDataValue(isFake);
	defineLevelDataValue(isMulti);
	defineLevelDataRef(judgeline);
	defineLevelDataValue(bpm);
	defineLevelDataValue(size);
	defineLevelDataValue(yOffset);
	defineLevelDataValue(visibleTime);
	defineLevelDataValue(alpha);
};

class NormalNoteEntity: public CommonNoteEntity {
	public:

	defineArchetypeName("Phigros Normal Note");
};

class FakeNormalNoteEntity: public NormalNoteEntity {
	public:

	defineArchetypeName("Phigros Fake Normal Note");
};

class DragNoteEntity: public CommonNoteEntity {
	public:

	defineArchetypeName("Phigros Drag Note");
};

class FakeDragNoteEntity: public DragNoteEntity {
	public:

	defineArchetypeName("Phigros Fake Drag Note");
};

class HoldNoteEntity: public CommonNoteEntity {
	public:

	defineArchetypeName("Phigros Hold Note");
	defineLevelDataValue(endFloorPosition);
};

class FakeHoldNoteEntity: public HoldNoteEntity {
	public:

	defineArchetypeName("Phigros Fake Hold Note");
};


class FlickNoteEntity: public CommonNoteEntity {
	public:

	defineArchetypeName("Phigros Flick Note");
};

class FakeFlickNoteEntity: public FlickNoteEntity {
	public:

	defineArchetypeName("Phigros Fake Flick Note");
};


class BpmChangeEntity: public LevelEntity {
	public:

	defineArchetypeName("#BPM_CHANGE");
    defineLevelDataValueDetailed(beat, "#BEAT");
    defineLevelDataValueDetailed(bpm, "#BPM");
};



// ========================================================================================
//
//               Official Phigros Format Chart --> Sonolus Level Data
//
// ========================================================================================



// 结构体定义
const double basicBpm = 120;
struct BPM {
	double startBeat, bpm;
	double basicTime;
};
struct PGRNote {
	int type, lineId;
	double time, endTime, offsetX;
	bool isAbove, isFake;
	double speed = 1, size = 1;
	double yOffset = 0, visibleTime = 999999, alpha = 1;
	double floorPosition = 0;

	Json::Value toJsonObject() {
		Json::Value obj;
		obj["type"] = type;
		obj["judgeline"] = lineId;
		obj["time"] = time;
		obj["holdTime"] = endTime - time;
		obj["positionX"] = offsetX;
		obj["isAbove"] = isAbove;
		obj["isFake"] = isFake;
		obj["speed"] = speed;
		obj["size"] = size;
		obj["yOffset"] = yOffset;
		obj["visibleTime"] = visibleTime;
		obj["alpha"] = alpha;
		obj["floorPosition"] = floorPosition;
		return obj;
	}
};
struct PGRSpeedEvent {
	double startTime, endTime;
	double value, floorPosition;

	Json::Value toJsonObject() {
		Json::Value obj;
		obj["startTime"] = startTime;
		obj["endTime"] = endTime;
		obj["value"] = value;
		obj["floorPosition"] = floorPosition;
		return obj;
	}
};
struct PGREvent {
	double startTime, endTime;
	double start, end;
	int easing; 
	double easingLeft = 0, easingRight = 1;
	bool bezier = 0; 
	double bezierP1 = 0, bezierP2 = 0, bezierP3 = 0, bezierP4 = 0;

	Json::Value toJsonObject() {
		Json::Value obj;
		obj["startTime"] = startTime;
		obj["endTime"] = endTime;
		obj["start"] = start;
		obj["end"] = end;
		obj["easing"] = easing;
		obj["easingLeft"] = easingLeft;
		obj["easingRight"] = easingRight;
		obj["bezier"] = bezier;
		obj["bezierP1"] = bezierP1;
		obj["bezierP2"] = bezierP2;
		obj["bezierP3"] = bezierP3;
		obj["bezierP4"] = bezierP4;
		return obj;
	}
};
struct PGRJudgeline {
	double bpm;
	vector<PGRSpeedEvent> speedEvents;
	vector<PGREvent> rpeSpeedEvents; // rpe 速度事件预留位
	vector<PGREvent> moveXEvents, moveYEvents, rotateEvents, disappearEvents;
	vector<PGRNote> notesAbove, notesBelow;

	Json::Value toJsonObject() {
		Json::Value obj;
		obj["bpm"] = bpm;
		obj["numOfNotes"] = (int)notesAbove.size() + (int)notesBelow.size();
		obj["numOfNotesAbove"] = (int)notesAbove.size();
		obj["numOfNotesBelow"] = (int)notesBelow.size();
		obj["speedEvents"].resize(0);
		for (auto &v: speedEvents) obj["speedEvents"].append(v.toJsonObject());
		obj["judgeLineMoveXEvents"].resize(0);
		for (auto &v: moveXEvents) obj["judgeLineMoveXEvents"].append(v.toJsonObject());
		obj["judgeLineMoveYEvents"].resize(0);
		for (auto &v: moveYEvents) obj["judgeLineMoveYEvents"].append(v.toJsonObject());
		obj["judgeLineRotateEvents"].resize(0);
		for (auto &v: rotateEvents) obj["judgeLineRotateEvents"].append(v.toJsonObject());
		obj["judgeLineDisappearEvents"].resize(0);
		for (auto &v: disappearEvents) obj["judgeLineDisappearEvents"].append(v.toJsonObject());
		obj["notesAbove"].resize(0);
		for (auto &v: notesAbove) obj["notesAbove"].append(v.toJsonObject());
		obj["notesBelow"].resize(0);
		for (auto &v: notesBelow) obj["notesBelow"].append(v.toJsonObject());
		return obj;
	}
};
string fromPGS(string json, double bgmOffset = 0) {
	Json::Value obj; json_decode(json, obj);
	int fmt = obj["formatVersion"].asInt();
	bgmOffset += obj["offset"].asDouble();
	Json::Value entities; int total = 0, current = 0; int last = 0;
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		total++;
		total += item["speedEvents"].size();
		if (fmt == official_version) total += item["judgeLineMoveEvents"].size() * 2;
		else if (fmt == optimizer_version)
			total += item["judgeLineMoveXEvents"].size() + item["judgeLineMoveYEvents"].size();
		else cout << "Unknown format version: " << fmt << endl, exit(1);
		total += item["judgeLineRotateEvents"].size();
		total += item["judgeLineDisappearEvents"].size();
		total += item["notesAbove"].size();
		total += item["notesBelow"].size();
	}
	cout << "[INFO] Total Entities: " << total << endl;
	LevelRawData levelData;
	levelData.bgmOffset = bgmOffset;
	levelData.append(InitializationEntity());
	levelData.append(StageControllerEntity());
	levelData.append(InputManagerEntity());
	levelData.append(FlickInputManagerEntity());
	BpmChangeEntity bpmChange;
	bpmChange.beat = 0; bpmChange.bpm = 60;
	levelData.append(bpmChange);
	// 计算 MultiNote
	map<double, int> noteNumber;
	for (int i = 0; i < obj["judgeLineList"].size(); i++) {
		auto item = obj["judgeLineList"][i];
		double bpm = item["bpm"].asDouble();
		for (int j = 0; j < item["notesAbove"].size(); j++) {
			auto v = item["notesAbove"][j];
			double time = v["time"].asDouble() / bpm * 1.875;
			noteNumber[time]++;
		}
		for (int j = 0; j < item["notesBelow"].size(); j++) {
			auto v = item["notesBelow"][j];
			double time = v["time"].asDouble() / bpm * 1.875;
			noteNumber[time]++;
		}
	}
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		double bpm = item["bpm"].asDouble();
		JudgelineEntity judgeline;

		// 添加 Speed Event
		vector<PGRSpeedEvent> speedEvents;
		LevelRawData speedData;
		int tmpSize = levelData.entities.size();
		for (int j = item["speedEvents"].size() - 1; j >= 0; j--) {
			auto v = item["speedEvents"][j];
			SpeedEventEntity speed;
			speed.startTime = v["startTime"].asDouble();
			speed.endTime = v["endTime"].asDouble();
			if (v.isMember("value")) speed.start = v["value"].asDouble(), speed.end = v["value"].asDouble();
			else speed.start = v["start"].asDouble(), speed.end = v["end"].asDouble();
			speed.next = j != item["speedEvents"].size() - 1 
				? speedData.back<SpeedEventEntity>() 
				: SpeedEventEntity();
			speedData.append(speed); INFO;
			speedEvents.push_back({ v["startTime"].asDouble(), v["endTime"].asDouble(), v["value"].asDouble(), 0 });
		}
		for (int j = speedData.entities.size() - 1; j >= 0; j--) levelData.entities.push_back(speedData.entities[j]);
		judgeline.speedEvent = item["speedEvents"].size()
			? levelData.get<SpeedEventEntity>(tmpSize) 
			: SpeedEventEntity();
		sort(speedEvents.begin(), speedEvents.end(), [&](PGRSpeedEvent a, PGRSpeedEvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		// 哈哈
		// for 条件都是命名的 j 变量，结果语句用了 i 变量
		// 居然导致 LevelRawData 在函数结束释放时报 free(): invalid next size (fast)
		// 调了大半天才发现这个问题
		// 警钟长鸣
		for (int j = 1; j < speedEvents.size(); j++) speedEvents[j].floorPosition = 
			speedEvents[j - 1].floorPosition + (speedEvents[j - 1].endTime - speedEvents[j - 1].startTime) * speedEvents[j - 1].value / bpm * 1.875;

		// 添加 Move Event
		if (fmt == official_version) {
			for (int j = item["judgeLineMoveEvents"].size() - 1; j >= 0; j--) {
				auto v = item["judgeLineMoveEvents"][j];
				
				MoveXEventEntity moveX;
				moveX.startTime = v["startTime"].asDouble();
				moveX.endTime = v["endTime"].asDouble();
				moveX.start = v["start"].asDouble();
				moveX.end = v["end"].asDouble();
				moveX.easing = v["easing"].asInt();
				moveX.easingLeft = v.isMember("easingLeft") ? v["easingLeft"].asInt() : 0;
				moveX.easingRight = v.isMember("easingRight") ? v["easingRight"].asInt() : 1;
				moveX.bezier = v.isMember("bezier") ? v["bezier"].asInt() : 0;
				moveX.bezierP1 = v.isMember("bezierP1") ? v["bezierP1"].asDouble() : 0;
				moveX.bezierP2 = v.isMember("bezierP2") ? v["bezierP2"].asDouble() : 0;
				moveX.bezierP3 = v.isMember("bezierP3") ? v["bezierP3"].asDouble() : 0;
				moveX.bezierP4 = v.isMember("bezierP4") ? v["bezierP4"].asDouble() : 0;
				moveX.next = j != item["judgeLineMoveEvents"].size() - 1
					? levelData.get<MoveXEventEntity>(levelData.size() - 2)
					: MoveXEventEntity();
				levelData.append(moveX); INFO;
				
				MoveYEventEntity moveY;
				moveY.startTime = v["startTime"].asDouble();
				moveY.endTime = v["endTime"].asDouble();
				moveY.start = v["start2"].asDouble();
				moveY.end = v["end2"].asDouble();
				moveY.easing = v["easing"].asInt();
				moveY.easingLeft = v.isMember("easingLeft") ? v["easingLeft"].asInt() : 0;
				moveY.easingRight = v.isMember("easingRight") ? v["easingRight"].asInt() : 1;
				moveY.bezier = v.isMember("bezier") ? v["bezier"].asInt() : 0;
				moveY.bezierP1 = v.isMember("bezierP1") ? v["bezierP1"].asDouble() : 0;
				moveY.bezierP2 = v.isMember("bezierP2") ? v["bezierP2"].asDouble() : 0;
				moveY.bezierP3 = v.isMember("bezierP3") ? v["bezierP3"].asDouble() : 0;
				moveY.bezierP4 = v.isMember("bezierP4") ? v["bezierP4"].asDouble() : 0;
				moveY.next = j != item["judgeLineMoveEvents"].size() - 1
					? levelData.get<MoveYEventEntity>(levelData.size() - 2)
					: MoveYEventEntity();
				levelData.append(moveY); INFO;
			} 
			judgeline.moveXEvent = item["judgeLineMoveEvents"].size()
					? levelData.get<MoveXEventEntity>(levelData.size() - 2) 
					: MoveXEventEntity();
			judgeline.moveYEvent = item["judgeLineMoveEvents"].size()
					? levelData.get<MoveYEventEntity>(levelData.size() - 1)
					: MoveYEventEntity();
		} else if (fmt == optimizer_version) {
			for (int j = item["judgeLineMoveXEvents"].size() - 1; j >= 0; j--) {
				auto v = item["judgeLineMoveXEvents"][j];
				MoveXEventEntity moveX;
				moveX.startTime = v["startTime"].asDouble();
				moveX.endTime = v["endTime"].asDouble();
				moveX.start = v["start"].asDouble();
				moveX.end = v["end"].asDouble();
				moveX.easing = v["easing"].asInt();
				moveX.easingLeft = v.isMember("easingLeft") ? v["easingLeft"].asInt() : 0;
				moveX.easingRight = v.isMember("easingRight") ? v["easingRight"].asInt() : 1;
				moveX.bezier = v.isMember("bezier") ? v["bezier"].asInt() : 0;
				moveX.bezierP1 = v.isMember("bezierP1") ? v["bezierP1"].asDouble() : 0;
				moveX.bezierP2 = v.isMember("bezierP2") ? v["bezierP2"].asDouble() : 0;
				moveX.bezierP3 = v.isMember("bezierP3") ? v["bezierP3"].asDouble() : 0;
				moveX.bezierP4 = v.isMember("bezierP4") ? v["bezierP4"].asDouble() : 0;
				moveX.next = j != item["judgeLineMoveXEvents"].size() - 1
					? levelData.get<MoveXEventEntity>(levelData.size() - 1)
					: MoveXEventEntity();
				levelData.append(moveX); INFO;
			} 
			judgeline.moveXEvent = item["judgeLineMoveXEvents"].size()
				? levelData.back<MoveXEventEntity>() 
				: MoveXEventEntity();
			
			for (int j = item["judgeLineMoveYEvents"].size() - 1; j >= 0; j--) {
				auto v = item["judgeLineMoveYEvents"][j];
				MoveYEventEntity moveY;
				moveY.startTime = v["startTime"].asDouble();
				moveY.endTime = v["endTime"].asDouble();
				moveY.start = v["start"].asDouble();
				moveY.end = v["end"].asDouble();
				moveY.easing = v["easing"].asInt();
				moveY.easingLeft = v.isMember("easingLeft") ? v["easingLeft"].asInt() : 0;
				moveY.easingRight = v.isMember("easingRight") ? v["easingRight"].asInt() : 1;
				moveY.bezier = v.isMember("bezier") ? v["bezier"].asInt() : 0;
				moveY.bezierP1 = v.isMember("bezierP1") ? v["bezierP1"].asDouble() : 0;
				moveY.bezierP2 = v.isMember("bezierP2") ? v["bezierP2"].asDouble() : 0;
				moveY.bezierP3 = v.isMember("bezierP3") ? v["bezierP3"].asDouble() : 0;
				moveY.bezierP4 = v.isMember("bezierP4") ? v["bezierP4"].asDouble() : 0;
				moveY.next = j != item["judgeLineMoveYEvents"].size() - 1
					? levelData.get<MoveYEventEntity>(levelData.size() - 1)
					: MoveYEventEntity();
				levelData.append(moveY); INFO;
			} 
			judgeline.moveYEvent = item["judgeLineMoveYEvents"].size()
				? levelData.back<MoveYEventEntity>() 
				: MoveYEventEntity();
		}

		// 添加 Rotate Event
		for (int j = item["judgeLineRotateEvents"].size() - 1; j >= 0; j--) {
			auto v = item["judgeLineRotateEvents"][j];
			RotateEventEntity rotate;
			rotate.startTime = v["startTime"].asDouble();
			rotate.endTime = v["endTime"].asDouble();
			rotate.start = v["start"].asDouble();
			rotate.end = v["end"].asDouble();
			rotate.easing = v["easing"].asInt();
			rotate.easingLeft = v.isMember("easingLeft") ? v["easingLeft"].asInt() : 0;
			rotate.easingRight = v.isMember("easingRight") ? v["easingRight"].asInt() : 1;
			rotate.bezier = v.isMember("bezier") ? v["bezier"].asInt() : 0;
			rotate.bezierP1 = v.isMember("bezierP1") ? v["bezierP1"].asDouble() : 0;
			rotate.bezierP2 = v.isMember("bezierP2") ? v["bezierP2"].asDouble() : 0;
			rotate.bezierP3 = v.isMember("bezierP3") ? v["bezierP3"].asDouble() : 0;
			rotate.bezierP4 = v.isMember("bezierP4") ? v["bezierP4"].asDouble() : 0;
			rotate.next = j != item["judgeLineRotateEvents"].size() - 1
				? levelData.get<RotateEventEntity>(levelData.size() - 1)
				: RotateEventEntity();
			levelData.append(rotate); INFO;
		}
		judgeline.rotateEvent = item["judgeLineRotateEvents"].size()
			? levelData.back<RotateEventEntity>() 
			: RotateEventEntity();

		// 添加 Disappear Event
		for (int j = item["judgeLineDisappearEvents"].size() - 1; j >= 0; j--) {
			auto v = item["judgeLineDisappearEvents"][j];
			DisappearEventEntity disappear;
			disappear.startTime = v["startTime"].asDouble();
			disappear.endTime = v["endTime"].asDouble();
			disappear.start = v["start"].asDouble();
			disappear.end = v["end"].asDouble();
			disappear.easing = v["easing"].asInt();
			disappear.easingLeft = v.isMember("easingLeft") ? v["easingLeft"].asInt() : 0;
			disappear.easingRight = v.isMember("easingRight") ? v["easingRight"].asInt() : 1;
			disappear.bezier = v.isMember("bezier") ? v["bezier"].asInt() : 0;
			disappear.bezierP1 = v.isMember("bezierP1") ? v["bezierP1"].asDouble() : 0;
			disappear.bezierP2 = v.isMember("bezierP2") ? v["bezierP2"].asDouble() : 0;
			disappear.bezierP3 = v.isMember("bezierP3") ? v["bezierP3"].asDouble() : 0;
			disappear.bezierP4 = v.isMember("bezierP4") ? v["bezierP4"].asDouble() : 0;
			disappear.next = j != item["judgeLineDisappearEvents"].size() - 1
				? levelData.get<DisappearEventEntity>(levelData.size() - 1)
				: DisappearEventEntity();
			levelData.append(disappear); INFO;
		}
		judgeline.disappearEvent = item["judgeLineDisappearEvents"].size()
			? levelData.back<DisappearEventEntity>() 
			: DisappearEventEntity();

		judgeline.bpm = bpm;
		levelData.append(judgeline); INFO;

		// 添加按键
		for (int j = 0; j < item["notesAbove"].size(); j++) {
			auto v = item["notesAbove"][j];
			CommonNoteEntity note;
			// note.type = v["type"].asInt();
			note.time = v["time"].asDouble();
			note.positionX = v["positionX"].asDouble();
			note.holdTime = v["holdTime"].asDouble();
			note.speed = v["speed"].asDouble();
			note.floorPosition = v["floorPosition"].asDouble();
			note.isAbove = 1;
			note.isMulti = noteNumber[v["time"].asDouble() / bpm * 1.875] > 1;
			note.isFake = v.isMember("isFake") ? v["isFake"].asBool() : false;
			note.judgeline = judgeline;
			note.bpm = bpm;
			note.size = v.isMember("size") ? v["size"].asDouble() : 1;
			note.yOffset = v.isMember("yOffset") ? v["yOffset"].asDouble() : 0;
			note.visibleTime = v.isMember("visibleTime") ? v["visibleTime"].asDouble() : 999999;
			note.alpha = v.isMember("alpha") ? v["alpha"].asDouble() : 1;
			switch(v["type"].asInt()) {
				case 1: {
					if (note.isFake.value) levelData.append(transform<FakeNormalNoteEntity>(note)); 
					else levelData.append(transform<NormalNoteEntity>(note)); 
				} break;
				case 2: {
					if (note.isFake.value) levelData.append(transform<FakeDragNoteEntity>(note)); 
					else levelData.append(transform<DragNoteEntity>(note)); 
				} break;
				case 3: {
					HoldNoteEntity hold = transform<HoldNoteEntity>(note);
					// 计算 endFloorPosition
					int k = lower_bound(
						speedEvents.begin(), 
						speedEvents.end(), 
						hold.time.value + hold.holdTime.value, 
						[&](PGRSpeedEvent a, double b){ return a.startTime < b; }
					) - speedEvents.begin() - 1;
					hold.endFloorPosition = speedEvents[k].floorPosition + 
						(hold.time.value + hold.holdTime.value - speedEvents[k].startTime) * speedEvents[k].value / bpm * 1.875;
					if (note.isFake.value) levelData.append(transform<FakeHoldNoteEntity>(hold)); 
					else levelData.append(hold);
				} break;
				case 4: {
					if (note.isFake.value) levelData.append(transform<FakeFlickNoteEntity>(note)); 
					else levelData.append(transform<FlickNoteEntity>(note)); 
				} break;
			} INFO;
		}
		for (int j = 0; j < item["notesBelow"].size(); j++) {
			auto v = item["notesBelow"][j];
			CommonNoteEntity note;
			// note.type = v["type"].asInt();
			note.time = v["time"].asDouble();
			note.positionX = v["positionX"].asDouble();
			note.holdTime = v["holdTime"].asDouble();
			note.speed = v["speed"].asDouble();
			note.floorPosition = -1 * v["floorPosition"].asDouble();
			note.isAbove = 0;
			note.isMulti = noteNumber[v["time"].asDouble() / bpm * 1.875] > 1;
			note.isFake = v.isMember("isFake") ? v["isFake"].asBool() : false;
			note.judgeline = judgeline;
			note.bpm = bpm;
			note.size = v.isMember("size") ? v["size"].asDouble() : 1;
			note.yOffset = v.isMember("yOffset") ? v["yOffset"].asDouble() : 0;
			note.visibleTime = v.isMember("visibleTime") ? v["visibleTime"].asDouble() : 999999;
			note.alpha = v.isMember("alpha") ? v["alpha"].asDouble() : 1;
			switch(v["type"].asInt()) {
				case 1: {
					if (note.isFake.value) levelData.append(transform<FakeNormalNoteEntity>(note)); 
					else levelData.append(transform<NormalNoteEntity>(note)); 
				} break;
				case 2: {
					if (note.isFake.value) levelData.append(transform<FakeDragNoteEntity>(note)); 
					else levelData.append(transform<DragNoteEntity>(note)); 
				} break;
				case 3: {
					HoldNoteEntity hold = transform<HoldNoteEntity>(note);
					// 计算 endFloorPosition
					int k = lower_bound(
						speedEvents.begin(), 
						speedEvents.end(), 
						hold.time.value + hold.holdTime.value, 
						[&](PGRSpeedEvent a, double b){ return a.startTime < b; }
					) - speedEvents.begin() - 1;
					hold.endFloorPosition = -1 * (speedEvents[k].floorPosition + 
						(hold.time.value + hold.holdTime.value - speedEvents[k].startTime) * speedEvents[k].value / bpm * 1.875);
					if (note.isFake.value) levelData.append(transform<FakeHoldNoteEntity>(hold)); 
					else levelData.append(hold);
				} break;
				case 4: {
					if (note.isFake.value) levelData.append(transform<FakeFlickNoteEntity>(note)); 
					else levelData.append(transform<FlickNoteEntity>(note)); 
				} break;
			} INFO;
		}
	}
	
	Json::Value data = levelData.toJsonObject();
	data["formatVersion"] = 6;
	return json_encode(data);
}



// ========================================================================================
//
//                 PEC Format Chart --> Official Phigros Format Chart
//
// ========================================================================================



// 结构体定义
struct PECEvent {
	int type, lineId;
	double startTime, endTime, speed;
	double offsetX, offsetY, rotate, alpha;
	int easing;
	string cmd;
};

int PECEasingMap[] = {
	0, 0, 0,
	17, 18, 19,
	1, 2, 3,
	5, 6, 7,
	9, 10, 11,
	13, 14, 15,
	21, 22, 23, 
	25, 26, 27,
	33, 34, 35,
	29, 30, 31,
	0, 0, 0 // Bounce is not supported
};
int PRENoteTypeMap[] = { 0, 1, 3, 4, 2 };
string trim(string s) {
	while (s.size() && (s.front() <= 32 || s.front() >= 127)) s = s.substr(1);
	while (s.size() && (s.back() <= 32 || s.back() >= 127)) s.pop_back();
	return s;
}

void PECSolveEvent(vector<PGREvent> &events) {
	for (int i = 1; i < events.size(); i++) events[i].start = events[i - 1].end;
	for (int i = events.size() - 2; i >= 0; i--) {
		double et = events[i].endTime, st = events[i + 1].startTime;
		if (et < st) events.push_back({ et, st, events[i].end, events[i].end, 0 });
		if (et > st) events[i + 1].startTime = events[i].endTime;
	} sort(events.begin(), events.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
}
string fromPEC(string txt, double bgmOffset = 0) {
	int easingSize = sizeof(PECEasingMap) / sizeof(int);

	// 读取数据
	stringstream ss; ss << txt;
	double offset; ss >> offset; offset = offset / 1000.0 - 0.175 + bgmOffset;
	vector<BPM> bpms;
	vector<PGRNote> notes;
	vector<PECEvent> events;
	int maxLineId = 0;
	while (!ss.eof()) {
		string cmd; ss >> cmd;
		if (cmd == "") break;
		if (cmd == "bp") {
			BPM b; ss >> b.startBeat >> b.bpm;
			bpms.push_back(b);
		} else if (cmd[0] == 'n') {
			PGRNote n; n.type = PRENoteTypeMap[cmd[1] - '0'];
			ss >> n.lineId >> n.time;
			maxLineId = max(maxLineId, n.lineId);
			if (n.type == 3) ss >> n.endTime;
			else n.endTime = n.time;
			int isAbove;
			ss >> n.offsetX >> isAbove >> n.isFake;
			n.offsetX = n.offsetX / 1024.0 * 160.0 / 18.0;
			n.isAbove = isAbove == 1;
			notes.push_back(n);
		} else if (cmd[0] == 'c') {
			PECEvent e; e.type = cmd[1];
			ss >> e.lineId >> e.startTime;
			maxLineId = max(maxLineId, e.lineId);
			if (e.type == 'v') ss >> e.speed;
			if (string("mrf").find(e.type) != string::npos) ss >> e.endTime;
			else e.endTime = e.startTime;
			if (string("pm").find(e.type) != string::npos) ss >> e.offsetX >> e.offsetY;
			if (string("dr").find(e.type) != string::npos) ss >> e.rotate;
			if (string("af").find(e.type) != string::npos) ss >> e.alpha;
			if (string("mr").find(e.type) != string::npos) ss >> e.easing;
			else e.easing = 2;
			events.push_back(e);
		} else if (cmd[0] == '#') ss >> notes.back().speed;
		else if (cmd[0] == '&') ss >> notes.back().size;
	}

	// 计算 BPM basicTime;
	sort(bpms.begin(), bpms.end(), [&](BPM a, BPM b){ return a.startBeat < b.startBeat; });
	for (int i = 0; i < bpms.size(); i++) bpms[i].basicTime = i ?
		bpms[i - 1].basicTime + (bpms[i].startBeat - bpms[i - 1].startBeat) / bpms[i - 1].bpm :
		0;
	
	auto pec2pgr = [&](double beat){
		int i = lower_bound(bpms.begin(), bpms.end(), beat, [&](BPM a, double b){ return a.startBeat < b; }) - bpms.begin() - 1;
		if (i < 0) return 0.0;
		return round(((beat - bpms[i].startBeat) / bpms[i].bpm + bpms[i].basicTime) * basicBpm * 32);
	};
	// 先将所有的事件添加进判定线去
	vector<PGRJudgeline> judgelines(maxLineId + 1);
	for (auto e : events) {
		PGRJudgeline &j = judgelines[e.lineId];
		double st = pec2pgr(e.startTime), et = pec2pgr(e.endTime);
		if (e.type == 'v') j.speedEvents.push_back({ st, et, e.speed / 5.85, 0 });
		if (string("af").find(e.type) != string::npos) 
			j.disappearEvents.push_back({ st, et, 0, e.alpha / 255.0, PECEasingMap[e.easing] });
		if (string("pm").find(e.type) != string::npos)
			j.moveXEvents.push_back({ st, et, 0, e.offsetX / 2048.0, PECEasingMap[e.easing] }),
			j.moveYEvents.push_back({ st, et, 0, e.offsetY / 1400.0, PECEasingMap[e.easing] });
		if (string("dr").find(e.type) != string::npos) 
			j.rotateEvents.push_back({ st, et, 0, -e.rotate, PECEasingMap[e.easing] });
	}

	// 添加按键
	for (auto n : notes)
		n.time = pec2pgr(n.time), n.endTime = pec2pgr(n.endTime),
		(n.isAbove ? judgelines[n.lineId].notesAbove : judgelines[n.lineId].notesBelow).push_back(n);

	// 事件排序并补全
	for (auto &j : judgelines) {
		j.bpm = basicBpm;
		sort(j.speedEvents.begin(), j.speedEvents.end(), [&](PGRSpeedEvent a, PGRSpeedEvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.moveXEvents.begin(), j.moveXEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.moveYEvents.begin(), j.moveYEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.rotateEvents.begin(), j.rotateEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.disappearEvents.begin(), j.disappearEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		for (int i = j.speedEvents.size() - 2; i >= 0; i--) j.speedEvents[i].endTime = j.speedEvents[i + 1].startTime;
		PECSolveEvent(j.moveXEvents); PECSolveEvent(j.moveYEvents);
		PECSolveEvent(j.rotateEvents); PECSolveEvent(j.disappearEvents);
	}

	// 给所有的事件加上个尾巴，不然 tmd 会出事
	for (auto &j : judgelines) {
		j.speedEvents.push_back({ j.speedEvents.back().endTime, 999999, j.speedEvents.back().value, 0 });
		j.moveXEvents.push_back({ j.moveXEvents.back().endTime, 999999, j.moveXEvents.back().end, j.moveXEvents.back().end, 0 });
		j.moveYEvents.push_back({ j.moveYEvents.back().endTime, 999999, j.moveYEvents.back().end, j.moveYEvents.back().end, 0 });
		j.rotateEvents.push_back({ j.rotateEvents.back().endTime, 999999, j.rotateEvents.back().end, j.rotateEvents.back().end, 0 });
		j.disappearEvents.push_back({ j.disappearEvents.back().endTime, 999999, j.disappearEvents.back().end, j.disappearEvents.back().end, 0 });
	}

	// floorPosition 计算
	for (auto &j : judgelines) {
		double bpm = j.bpm; double fp = 0;
		for (auto &s : j.speedEvents) {
			s.floorPosition = fp;
			fp += (s.endTime - s.startTime) * s.value / bpm * 1.875;
		}

		for (auto &n : j.notesAbove) {
			int i = lower_bound(j.speedEvents.begin(), j.speedEvents.end(), n.time, [&](PGRSpeedEvent a, double b){ return a.startTime < b; }) - j.speedEvents.begin() - 1;
			if (n.type == 3) n.speed *= j.speedEvents[i].value;
			n.floorPosition = j.speedEvents[i].floorPosition + (n.time - j.speedEvents[i].startTime) * j.speedEvents[i].value / bpm * 1.875;
		}
		for (auto &n : j.notesBelow) {
			int i = lower_bound(j.speedEvents.begin(), j.speedEvents.end(), n.time, [&](PGRSpeedEvent a, double b){ return a.startTime < b; }) - j.speedEvents.begin() - 1;
			if (n.type == 3) n.speed *= j.speedEvents[i].value;
			n.floorPosition = j.speedEvents[i].floorPosition + (n.time - j.speedEvents[i].startTime) * j.speedEvents[i].value / bpm * 1.875;
		}
	}

	// 输出
	Json::Value obj;
	obj["formatVersion"] = optimizer_version;
	obj["judgeLineList"].resize(0);
	obj["offset"] = offset;
	for (auto &j : judgelines) obj["judgeLineList"].append(j.toJsonObject());
	return json_encode(obj);
}



// ========================================================================================
//
//                 RPE Format Chart --> Official Phigros Format Chart
//
// ========================================================================================



int RPENoteTypeMap[] = { 0, 1, 3, 2, 4 };
double BeatToDouble(Json::Value beat) {
	return beat[0].asDouble() + beat[1].asDouble() / beat[2].asDouble();
}
string to_string(double v, int precision) {
	stringstream ss; ss << fixed << setprecision(precision) << v; return ss.str();
}
vector<PGRSpeedEvent> RPESolveSpeedEvent(vector<PGREvent> events, int id, double factor) {
	// 排序
	vector<PGRSpeedEvent> res;
	sort(events.begin(), events.end(), [&](PGREvent a, PGREvent b){ return a.startTime < b.startTime; });
	// 处理异常 1
	if (events.size() == 0) {
		res.push_back({ 0, 999999, 0.0, 0 });
		cerr << "Warning: Invalid Speed Event List [" << id << "]: Empty Speed Event List." << endl;
	}
	if (events[0].startTime > 0) res.push_back({ 0, events[0].startTime, 0.0, 0 });
	// 处理异常 2，同时拆分 Speed Event
	double lastTime = events[0].startTime, lastValue = 0;
	for (int i = 0; i < events.size(); i++) {
		// 判断异常 3
		if (events[i].startTime > events[i].endTime) {
			cerr << "Warning: Invalid Speed Event [" <<
				id << ", " <<
				to_string(events[i].startTime, 2) << ", " <<
				to_string(events[i].endTime, 2) << ", " <<
				to_string(events[i].start, 2) << ", " <<
				to_string(events[i].end, 2) <<
			"]: Start Time is greater than End Time." << endl;
			continue;
		}
		if (lastTime < events[i].startTime) res.push_back({ lastTime, events[i].startTime, lastValue, 0 }); // 相离
		if (lastTime == events[i].startTime) ; // 相切
		if (lastTime <= events[i].startTime) {
			double startTime = events[i].startTime, endTime = events[i].endTime;
			double start = events[i].start, end = events[i].end;
			if (start == end) res.push_back({ startTime, endTime, start, 0 });
			else {
				double delta = (end - start) / (endTime - startTime);
				for (double j = startTime; j < endTime; j++) 
					res.push_back({ j, j + 1, start + (j - startTime) * delta, 0 });
			}
		}

		// 相交情况报个错(异常 2)
		if (lastTime > events[i].startTime) cerr << "Warning: Overlapped Speed Event [" <<
				to_string(id) << ", " <<
				to_string(events[i - 1].startTime, 2) << ", " << 
				to_string(events[i - 1].endTime, 2) << ", " << 
				to_string(events[i - 1].start, 2) << ", " << 
				to_string(events[i - 1].end, 2) << 
			"] and [" <<
				to_string(id) << ", " <<
				to_string(events[i].startTime, 2) << ", " << 
				to_string(events[i].endTime, 2) << ", " <<
				to_string(events[i].start, 2) << ", " <<
				to_string(events[i].end, 2) << 
			"]." << endl;

		if (lastTime >= events[i].endTime) continue; // 包含
		if (lastTime > events[i].startTime) { // 相交
			double startTime = lastTime, endTime = events[i].endTime;
			if (events[i].start == events[i].end) res.push_back({ lastTime, events[i].startTime, lastValue, 0 });
			else {
				double delta = (events[i].end - events[i].start) / (events[i].endTime - events[i].startTime);
				for (double j = startTime; j < endTime; j++) 
					res.push_back({ j, j + 1, events[i].start + (j - events[i].startTime) * delta, 0 });
			}
		}
		lastTime = events[i].endTime, lastValue = events[i].end;
	}
	// 事件排序
	sort(res.begin(), res.end(), [&](PGRSpeedEvent a, PGRSpeedEvent b){ return a.startTime < b.startTime; });
	// 添加尾巴
	if (res.size()) res.push_back({ res.back().endTime, 999999, 0, 0 });
	else res.push_back({ 0, 999999, 0, 0 });
	// floorPosition 计算
	for (int i = 1; i < res.size(); i++) 
		res[i].floorPosition = res[i - 1].floorPosition + (res[i - 1].endTime - res[i - 1].startTime) * res[i - 1].value / basicBpm / factor * 1.875;
	// cout << "Speed List [" << id << "]: [" << endl;
	// for (int i = 0; i < res.size(); i++) cout << "    [" << 
	// 	res[i].startTime << ", " << 
	// 	res[i].endTime << ", " << 
	// 	res[i].value << ", " << 
	// 	res[i].floorPosition <<
	// "]" << endl;
	// cout << "]" << endl;
	return res;
}
void RPESolveEvent(vector<PGREvent> &events, int id, string name) {
	// 排序
	vector<PGREvent> extra;
	sort(events.begin(), events.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
	// 处理异常 1 (事件延拓)
	if (events.size() == 0) {
		cerr << "Warning: Invalid " << name << " List [" << id << "]: Empty " << name << " List." << endl;
		return;
	}
	if (events[0].startTime > 0) {
		events[0].easingLeft = 
			events[0].easingRight - 
			(events[0].easingRight - events[0].easingLeft) / 
			(events[0].endTime - events[0].startTime) * events[0].endTime;
		events[0].startTime = 0;
	}
	if (events.back().endTime < 999999) events.push_back({
		events.back().endTime,
		999999,
		0, 0,
		0, 0, 1,
		0, 0, 0, 0, 0
	});
	// 同时处理异常 2 和 异常 3
	for (int i = 0; i < events.size(); i++) {
		// 先处理异常 3
		if (events[i].startTime > events[i].endTime) {
			cerr << "Warning: Invalid " << name << " [" <<
				id << ", " <<
				to_string(events[i].startTime, 2) << ", " <<
				to_string(events[i].endTime, 2) << ", " <<
				to_string(events[i].start, 2) << ", " <<
				to_string(events[i].end, 2) <<
			"]: Start Time is greater than End Time." << endl;
			events[i].endTime = events[i].startTime;
			events[i].start = events[i].end;
		}
		// 再处理异常 2
		if (!i) continue;
		auto &last = events[i - 1], curr = events[i];
		// 相离
		if (last.endTime < curr.startTime) extra.push_back({
			last.endTime,
			curr.startTime,
			last.end,
			last.end,
			0, 0, 1,
			0, 0, 0, 0, 0
		});
		// 相切
		if (last.endTime == curr.startTime) ;
		// 重叠/包含
		if (last.endTime > curr.startTime) {
			cerr << "Warning: Overlapped " << name << " [" <<
				to_string(id) << ", " <<
				to_string(events[i - 1].startTime, 2) << ", " << 
				to_string(events[i - 1].endTime, 2) << ", " << 
				to_string(events[i - 1].start, 2) << ", " << 
				to_string(events[i - 1].end, 2) << 
			"] and [" <<
				to_string(id) << ", " <<
				to_string(events[i].startTime, 2) << ", " << 
				to_string(events[i].endTime, 2) << ", " <<
				to_string(events[i].start, 2) << ", " <<
				to_string(events[i].end, 2) << 
			"]." << endl;
			// 截断上一事件
			last.easingRight = 
				last.easingLeft + 
				(last.easingRight - last.easingLeft) / 
				(last.endTime - last.startTime) * 
				(curr.startTime - last.startTime);
			last.endTime = curr.startTime;
		}
	}
	// 插入额外事件并排序
	events.insert(events.end(), extra.begin(), extra.end());
	sort(events.begin(), events.end(), [&](PGREvent a, PGREvent b){ return a.startTime < b.startTime; });
	// cout << name << " [" << id << "]: [" << endl;
	// for (int i = 0; i < events.size(); i++) 
	// 	cout << "    [" << 
	// 		events[i].startTime << ", " << 
	// 		events[i].endTime << ", " << 
	// 		events[i].start << ", " << 
	// 		events[i].end << ", " <<
	// 		events[i].easing << ", " <<
	// 		events[i].easingLeft << ", " <<
	// 		events[i].easingRight << 
	// 	"]" << endl;
	// cout << "]" << endl;
}
string fromRPE(string json, double bgmOffset = 0) {
	Json::Value rpe; json_decode(json, rpe);
	double offset = rpe["META"]["offset"].asDouble() + bgmOffset;

	// 添加 BPM
	vector<BPM> bpms;
	for (int i = 0; i < rpe["BPMList"].size(); i++) 
		bpms.push_back({ 
			BeatToDouble(rpe["BPMList"][i]["startTime"]), 
			rpe["BPMList"][i]["bpm"].asDouble() 
		});
	
	// 计算 BPM basicTime;
	sort(bpms.begin(), bpms.end(), [&](BPM a, BPM b){ return a.startBeat < b.startBeat; });
	for (int i = 0; i < bpms.size(); i++) bpms[i].basicTime = i ?
		bpms[i - 1].basicTime + (bpms[i].startBeat - bpms[i - 1].startBeat) / bpms[i - 1].bpm :
		0;
	
	auto rpe2pgr = [&](double beat, double factor){
		int i = lower_bound(bpms.begin(), bpms.end(), beat, [&](BPM a, double b){ return a.startBeat < b; }) - bpms.begin() - 1;
		if (i < 0) return 0.0;
		return round(((beat - bpms[i].startBeat) / bpms[i].bpm + bpms[i].basicTime) * basicBpm * factor * 32);
	};
	vector<PGRJudgeline> judgelines(rpe["judgeLineList"].size());
	for (int i = 0; i < rpe["judgeLineList"].size(); i++) {
		auto item = rpe["judgeLineList"][i];
		// 先将所有的事件添加进判定线去
		judgelines[i].bpm = basicBpm * item["bpmfactor"].asDouble();
		for (int j = 0; j < item["eventLayers"].size(); j++) {
			auto layer = item["eventLayers"][j];
			for (int k = 0; k < layer["speedEvents"].size(); k++)
				judgelines[i].rpeSpeedEvents.push_back({
					rpe2pgr(BeatToDouble(layer["speedEvents"][k]["startTime"]), item["bpmfactor"].asDouble()),
					rpe2pgr(BeatToDouble(layer["speedEvents"][k]["endTime"]), item["bpmfactor"].asDouble()),
					layer["speedEvents"][k]["start"].asDouble() * 11 / 45,
					layer["speedEvents"][k]["end"].asDouble() * 11 / 45,
					0,
				});
			for (int k = 0; k < layer["moveXEvents"].size(); k++)
				judgelines[i].moveXEvents.push_back({
					rpe2pgr(BeatToDouble(layer["moveXEvents"][k]["startTime"]), item["bpmfactor"].asDouble()),
					rpe2pgr(BeatToDouble(layer["moveXEvents"][k]["endTime"]), item["bpmfactor"].asDouble()),
					(layer["moveXEvents"][k]["start"].asDouble() + 675) / 1350,
					(layer["moveXEvents"][k]["end"].asDouble() + 675) / 1350,
					layer["moveXEvents"][k]["easingType"].asInt(),
					layer["moveXEvents"][k]["easingLeft"].asDouble(),
					layer["moveXEvents"][k]["easingRight"].asDouble(),
					layer["moveXEvents"][k]["bezier"].asBool(),
					layer["moveXEvents"][k]["bezierP1"].asDouble(),
					layer["moveXEvents"][k]["bezierP2"].asDouble(),
					layer["moveXEvents"][k]["bezierP3"].asDouble(),
					layer["moveXEvents"][k]["bezierP4"].asDouble()
				});
			for (int k = 0; k < layer["moveYEvents"].size(); k++)
				judgelines[i].moveYEvents.push_back({
					rpe2pgr(BeatToDouble(layer["moveYEvents"][k]["startTime"]), item["bpmfactor"].asDouble()),
					rpe2pgr(BeatToDouble(layer["moveYEvents"][k]["endTime"]), item["bpmfactor"].asDouble()),
					(layer["moveYEvents"][k]["start"].asDouble() + 450) / 900,
					(layer["moveYEvents"][k]["end"].asDouble() + 450) / 900,
					layer["moveYEvents"][k]["easingType"].asInt(),
					layer["moveYEvents"][k]["easingLeft"].asDouble(),
					layer["moveYEvents"][k]["easingRight"].asDouble(),
					layer["moveYEvents"][k]["bezier"].asBool(),
					layer["moveYEvents"][k]["bezierP1"].asDouble(),
					layer["moveYEvents"][k]["bezierP2"].asDouble(),
					layer["moveYEvents"][k]["bezierP3"].asDouble(),
					layer["moveYEvents"][k]["bezierP4"].asDouble()
				});
			for (int k = 0; k < layer["rotateEvents"].size(); k++)
				judgelines[i].rotateEvents.push_back({
					rpe2pgr(BeatToDouble(layer["rotateEvents"][k]["startTime"]), item["bpmfactor"].asDouble()),
					rpe2pgr(BeatToDouble(layer["rotateEvents"][k]["endTime"]), item["bpmfactor"].asDouble()),
					-layer["rotateEvents"][k]["start"].asDouble(),
					-layer["rotateEvents"][k]["end"].asDouble(),
					layer["rotateEvents"][k]["easingType"].asInt(),
					layer["rotateEvents"][k]["easingLeft"].asDouble(),
					layer["rotateEvents"][k]["easingRight"].asDouble(),
					layer["rotateEvents"][k]["bezier"].asBool(),
					layer["rotateEvents"][k]["bezierP1"].asDouble(),
					layer["rotateEvents"][k]["bezierP2"].asDouble(),
					layer["rotateEvents"][k]["bezierP3"].asDouble(),
					layer["rotateEvents"][k]["bezierP4"].asDouble()
				});
			for (int k = 0; k < layer["alphaEvents"].size(); k++) 
				judgelines[i].disappearEvents.push_back({
					rpe2pgr(BeatToDouble(layer["alphaEvents"][k]["startTime"]), item["bpmfactor"].asDouble()),
					rpe2pgr(BeatToDouble(layer["alphaEvents"][k]["endTime"]), item["bpmfactor"].asDouble()),
					max(0.0, layer["alphaEvents"][k]["start"].asDouble()) / 255.0,
					max(0.0, layer["alphaEvents"][k]["end"].asDouble()) / 255.0,
					layer["alphaEvents"][k]["easingType"].asInt(),
					layer["alphaEvents"][k]["easingLeft"].asDouble(),
					layer["alphaEvents"][k]["easingRight"].asDouble(),
					layer["alphaEvents"][k]["bezier"].asBool(),
					layer["alphaEvents"][k]["bezierP1"].asDouble(),
					layer["alphaEvents"][k]["bezierP2"].asDouble(),
					layer["alphaEvents"][k]["bezierP3"].asDouble(),
					layer["alphaEvents"][k]["bezierP4"].asDouble()
				});
		}
		// 添加按键
		for (int j = 0; j < item["notes"].size(); j++) {
			auto note = item["notes"][j];
			(note["above"].asBool() ? judgelines[i].notesAbove : judgelines[i].notesBelow).push_back({
				RPENoteTypeMap[note["type"].asInt()],
				i,
				rpe2pgr(BeatToDouble(note["startTime"]), item["bpmfactor"].asDouble()),
				rpe2pgr(BeatToDouble(note["endTime"]), item["bpmfactor"].asDouble()),
				note["positionX"].asDouble(),
				note["above"].asBool(),
				note["isFake"].asBool(),
				note["speed"].asDouble(),
				note["size"].asDouble(),
				note["yOffset"].asDouble(),
				note["visibleTime"].asDouble(),
				note["alpha"].asDouble(),
				0
			});
		}
	}

	// 事件处理
	int id = 0;
	for (auto &j: judgelines) {
		sort(j.moveXEvents.begin(), j.moveXEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.moveYEvents.begin(), j.moveYEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.rotateEvents.begin(), j.rotateEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		sort(j.disappearEvents.begin(), j.disappearEvents.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		j.speedEvents = RPESolveSpeedEvent(j.rpeSpeedEvents, id, j.bpm / basicBpm);
		RPESolveEvent(j.moveXEvents, id, "MoveX Event"); RPESolveEvent(j.moveYEvents, id, "MoveY Event");
		RPESolveEvent(j.rotateEvents, id, "Rotate Event"); RPESolveEvent(j.disappearEvents, id, "Alpha Event");
		id++;
	}

	// floorPosition 计算
	for (auto &j : judgelines) {
		double bpm = j.bpm; double fp = 0;
		for (auto &n : j.notesAbove) {
			int i = lower_bound(j.speedEvents.begin(), j.speedEvents.end(), n.time, [&](PGRSpeedEvent a, double b){ return a.startTime < b; }) - j.speedEvents.begin() - 1;
			if (n.type == 3) n.speed *= j.speedEvents[i].value;
			n.floorPosition = j.speedEvents[i].floorPosition + (n.time - j.speedEvents[i].startTime) * j.speedEvents[i].value / bpm * 1.875;
		}
		for (auto &n : j.notesBelow) {
			int i = lower_bound(j.speedEvents.begin(), j.speedEvents.end(), n.time, [&](PGRSpeedEvent a, double b){ return a.startTime < b; }) - j.speedEvents.begin() - 1;
			if (n.type == 3) n.speed *= j.speedEvents[i].value;
			n.floorPosition = j.speedEvents[i].floorPosition + (n.time - j.speedEvents[i].startTime) * j.speedEvents[i].value / bpm * 1.875;
		}
	}

	Json::Value obj;
	obj["formatVersion"] = optimizer_version;
	obj["judgeLineList"].resize(0);
	obj["offset"] = offset;
	for (auto &j : judgelines) obj["judgeLineList"].append(j.toJsonObject());
	return json_encode(obj);
}
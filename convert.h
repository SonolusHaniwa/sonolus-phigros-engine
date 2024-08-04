#pragma once
#define INFO if (levelData.size() * 100 / total != last) \
	last = levelData.size() * 100 / total, cout << "[INFO] Processed: " << levelData.size() << "/" << total << "(" << last << "%)" << endl;

const int official_version = 3;
const int optimizer_version = 13;
const int pec_version = -1;
const int pec_conventor_version = 1000;
const int rpe_version = 123;
const int rpe_conventor_version = 2123;

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
	defineLevelDataValue(value);
	defineLevelDataRef(next);
};

class CommonEventEntity: public LevelEntity {
	public:

	defineLevelDataValue(startTime);
	defineLevelDataValue(endTime);
	defineLevelDataValue(start);
	defineLevelDataValue(end);
	defineLevelDataValue(easing);
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
	double speed, size;
	double floorPosition = 0;
	string cmd;

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

	Json::Value toJsonObject() {
		Json::Value obj;
		obj["startTime"] = startTime;
		obj["endTime"] = endTime;
		obj["start"] = start;
		obj["end"] = end;
		obj["easing"] = easing;
		return obj;
	}
};
struct PGRJudgeline {
	double bpm;
	vector<PGRSpeedEvent> speedEvents;
	vector<PGREvent> moveXEvents, moveYEvents, rotateEvents, disappearEvents;
	vector<PGRNote> notesAbove, notesBelow;

	Json::Value toJsonObject() {
		Json::Value obj;
		obj["bpm"] = bpm;
		obj["numOfNotes"] = notesAbove.size() + notesBelow.size();
		obj["numOfNotesAbove"] = notesAbove.size();
		obj["numOfNotesBelow"] = notesBelow.size();
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
		else if (fmt == optimizer_version || fmt == pec_conventor_version) 
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
			speed.value = v["value"].asDouble();
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
		// for (auto v : speedEvents) cout << "V: " << v.startTime << " " << v.endTime << " " << v.value << " " << v.floorPosition << endl;

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
		} else if (fmt == optimizer_version || fmt == pec_conventor_version) {
			for (int j = item["judgeLineMoveXEvents"].size() - 1; j >= 0; j--) {
				auto v = item["judgeLineMoveXEvents"][j];
				MoveXEventEntity moveX;
				moveX.startTime = v["startTime"].asDouble();
				moveX.endTime = v["endTime"].asDouble();
				moveX.start = v["start"].asDouble();
				moveX.end = v["end"].asDouble();
				moveX.easing = v["easing"].asInt();
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
					levelData.append(hold); 
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
					levelData.append(hold); 
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
int noteTypeMap[] = { 0, 1, 3, 4, 2 };
string trim(string s) {
	while (s.size() && (s.front() <= 32 || s.front() >= 127)) s = s.substr(1);
	while (s.size() && (s.back() <= 32 || s.back() >= 127)) s.pop_back();
	return s;
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
			PGRNote n; n.type = noteTypeMap[cmd[1] - '0'];
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
		auto solveEvent = [&](vector<PGREvent> &events, bool output = false){
			for (int i = 1; i < events.size(); i++) events[i].start = events[i - 1].end;
			for (int i = events.size() - 2; i >= 0; i--) {
				double et = events[i].endTime, st = events[i + 1].startTime;
				if (et < st) events.push_back({ et, st, events[i].end, events[i].end, 0 });
				if (et > st) events[i + 1].startTime = events[i].endTime;
			} sort(events.begin(), events.end(), [&](PGREvent a, PGREvent b){ return a.startTime + a.endTime < b.startTime + b.endTime; });
		};
		solveEvent(j.moveXEvents); solveEvent(j.moveYEvents);
		solveEvent(j.rotateEvents); solveEvent(j.disappearEvents);
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
	for (auto &j : judgelines) obj["judgeLineList"].append(j.toJsonObject());
	return json_encode(obj);
}
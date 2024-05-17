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

class JudgelineEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Judgeline");
	defineLevelDataRef(speedEvent);
	defineLevelDataRef(moveXEvent);
	defineLevelDataRef(moveYEvent);
	defineLevelDataRef(rotateEvent);
	defineLevelDataRef(disappearEvent);
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

class NoteEntity: public LevelEntity {
	public:

	defineArchetypeName("Phigros Note");
	defineLevelDataValue(type);
	defineLevelDataValue(time);
	defineLevelDataValue(positionX);
	defineLevelDataValue(holdTime);
	defineLevelDataValue(speed);
	defineLevelDataValue(floorPosition);
	defineLevelDataValue(isAbove);
	defineLevelDataValue(isMulti);
	defineLevelDataRef(judgeline);
};



// ========================================================================================
//
//               Official Phigros Format Chart --> Sonolus Level Data
//
// ========================================================================================



string fromPGS(string json, double bgmOffset = 0) {
	srand(time(0));
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
	levelData.append(InitializationEntity());
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		double bpm = item["bpm"].asDouble();
		JudgelineEntity judgeline;

		// 添加 Speed Event
		for (int j = item["speedEvents"].size() - 1; j >= 0; j--) {
			auto v = item["speedEvents"][j];
			SpeedEventEntity speed;
			speed.startTime = v["startTime"].asDouble() / bpm * 1.875;
			speed.endTime = v["endTime"].asDouble() / bpm * 1.875;
			speed.value = v["value"].asDouble();
			speed.next = j != item["speedEvents"].size() - 1 
				? levelData.back<SpeedEventEntity>() 
				: SpeedEventEntity();
			levelData.append(speed); INFO;
		}
		judgeline.speedEvent = item["speedEvents"].size()
				? levelData.back<SpeedEventEntity>() 
				: SpeedEventEntity();

		// 添加 Move Event
		if (fmt == official_version) {
			for (int j = item["judgeLineMoveEvents"].size() - 1; j >= 0; j--) {
				auto v = item["judgeLineMoveEvents"][j];
				
				MoveXEventEntity moveX;
				moveX.startTime = v["startTime"].asDouble() / bpm * 1.875;
				moveX.endTime = v["endTime"].asDouble() / bpm * 1.875;
				moveX.start = v["start"].asDouble();
				moveX.end = v["end"].asDouble();
				moveX.easing = v["easing"].asInt();
				moveX.next = j != item["judgeLineMoveEvents"].size() - 1
					? levelData.get<MoveXEventEntity>(levelData.size() - 2)
					: MoveXEventEntity();
				levelData.append(moveX); INFO;
				
				MoveYEventEntity moveY;
				moveY.startTime = v["startTime"].asDouble() / bpm * 1.875;
				moveY.endTime = v["endTime"].asDouble() / bpm * 1.875;
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
				moveX.startTime = v["startTime"].asDouble() / bpm * 1.875;
				moveX.endTime = v["endTime"].asDouble() / bpm * 1.875;
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
				moveY.startTime = v["startTime"].asDouble() / bpm * 1.875;
				moveY.endTime = v["endTime"].asDouble() / bpm * 1.875;
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
			rotate.startTime = v["startTime"].asDouble() / bpm * 1.875;
			rotate.endTime = v["endTime"].asDouble() / bpm * 1.875;
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
			disappear.startTime = v["startTime"].asDouble() / bpm * 1.875;
			disappear.endTime = v["endTime"].asDouble() / bpm * 1.875;
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
		
		levelData.append(judgeline); INFO;

		// 添加按键
		for (int j = 0; j < item["notesAbove"].size(); j++) {
			auto v = item["notesAbove"][j];
			NoteEntity note;
			note.type = v["type"].asInt();
			note.time = v["time"].asDouble() / bpm * 1.875;
			note.positionX = v["positionX"].asDouble();
			note.holdTime = v["holdTime"].asDouble() / bpm * 1.875;
			note.speed = v["speed"].asDouble();
			note.floorPosition = v["floorPosition"].asDouble();
			note.isAbove = 1;
			note.isMulti = 0;
			// note.isFake = fmt == pec_conventor_version ? v["isFake"].asInt() : 0;
			note.judgeline = judgeline;
			levelData.append(note); INFO;
		}
		for (int j = 0; j < item["notesBelow"].size(); j++) {
			auto v = item["notesBelow"][j];
			NoteEntity note;
			note.type = v["type"].asInt();
			note.time = v["time"].asDouble() / bpm * 1.875;
			note.positionX = v["positionX"].asDouble();
			note.holdTime = v["holdTime"].asDouble() / bpm * 1.875;
			note.speed = v["speed"].asDouble();
			note.floorPosition = v["floorPosition"].asDouble();
			note.isAbove = 0;
			note.isMulti = 0;
			// note.isFake = fmt == pec_conventor_version ? v["isFake"].asInt() : 0;
			note.judgeline = judgeline;
			levelData.append(note); INFO;
		}
	}

	// cout << levelData.toJsonObject() << endl;
	
	Json::Value data = levelData.toJsonObject();
	data["formatVersion"] = 2;
	return json_encode(data);
}



// ========================================================================================
//
//                 PEC Format Chart --> Official Phigros Format Chart
//
// ========================================================================================



int PECEasingMap[] = {
	37, 37, 0,
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
string trim(string s) {
	while (s.size() && (s.front() <= 32 || s.front() >= 127)) s = s.substr(1);
	while (s.size() && (s.back() <= 32 || s.back() >= 127)) s.pop_back();
	return s;
}
string fromPEC(string txt, double bgmOffset = 0) {
	srand(time(0));
	int EasingNum = sizeof(PECEasingMap) / sizeof(int);
	auto lines = explode("\n", txt.c_str());
	for (int i = 0; i < lines.size(); i++) lines[i] = trim(lines[i]);
	int total = 0, current = 0; int last = 0, pt = 0;
	for (int i = 0; i < lines.size(); i++) if (lines[i] != "") lines[pt++] = lines[i];
	lines.resize(pt);
	for (int i = 0; i < lines.size(); i++) if (lines[i][0] == 'c' || lines[i][0] == 'n') total++;
	cout << "[INFO] Total Entities: " << total << endl;
	Json::Value chart;
	chart["formatVersion"] = pec_conventor_version;
	chart["offset"] = stod(lines[0]) / 1e3 - 0.175;
	chart["judgeLineList"].resize(0);

	struct BPM {
		double basicBpm = 120;
		double startBeat, endBeat, bpm;
		double basicTime;
	};
	struct Note {
		int type;
		int lineId;
		double time;
		double endTime;
		double offsetX;
		bool isAbove;
		bool isFake;
		double speed;
		double size;
		string cmd;
	};
	struct Judgeline {
		int type;
		int lineId;
		double startTime;
		double endTime;
		double speed;
		double offsetX;
		double offsetY;
		double rotate;
		double alpha;
		int easingType;
		string cmd;
	};
	vector<BPM> bpmList;
	vector<Note> notes;
	vector<Judgeline> judgelines;
	vector<vector<string> > cmds;
	auto pec2pgr = [&](double beat){
		double time = 0;
		int i = lower_bound(bpmList.begin(), bpmList.end(), beat, [&](BPM a, double b){ return a.startBeat < b; }) - bpmList.begin();
		return round(((beat - bpmList[i].startBeat) / bpmList[i].bpm + bpmList[i].basicTime) * bpmList[i].basicBpm * 32);
	};

	for (int i = 1; i < lines.size(); i++) {
		auto args = explode(" ", lines[i].c_str()); int pt = 0;
		for (int j = 0; j < args.size(); j++) args[j] = trim(args[j]);
		for (int j = 0; j < args.size(); j++) if (args[j] != "") args[pt++] = args[j];
		args.resize(pt);
		if (args.size() == 0) continue;
		cmds.push_back(args);
	}
	for (int i = 0; i < cmds.size(); i++) {
		auto args = cmds[i];
		if (args[0] == "bp") bpmList.push_back({ 120, stod(args[1]), 0, stod(args[2]) });
		else if (args[0] == "n1" || args[0] == "n2" || args[0] == "n3" || args[0] == "n4") {
			Note note; int pt = 1;
			note.type = args[0][1] - '0';
			note.lineId = stod(args[pt++]);
			note.time = stod(args[pt++]);
			note.endTime = args[0] == "n2" ? stod(args[pt++]) : note.time;
			note.offsetX = stod(args[pt++]);
			note.isAbove = stoi(args[pt++]);
			note.isFake = stoi(args[pt++]);
			note.speed = i + 1 < cmd.size() && cmd[i + 1][0] == '#' && cmd[i + 1].size() >= 2 ? stod(cmd[i++][1]) : 1;
			note.size = i + 1 < cmd.size() && cmd[i + 1][0] == '&' && cmd[i + 1].size() >= 2 ? stod(cmd[i++][1]) : 1;
			// note.cmd = "";
			notes.push_back(note);
		} else if (args[0] == "cv" || args[0] == "cp" || args[0] == "cd" || args[0] == "ca" || args[0] == "cm" || args[0] == "cr" || args[0] == "cf") {
			Judgeline judgeline; int pt = 1;
			judgeline.type = args[0][1];
			judgeline.lineId = stod(args[pt++]);
			judgeline.startTime = stod(args[pt++]);
			judgeline.speed = args[0] == "cv" ? stod(args[pt++]) : 1;
			judgeline.endTime = args[0] == "cm" || args[0] == "cr" || args[0] == "cf" ? stod(args[pt++]) : startTime;
			judgeline.offsetX = args[0] == "cp" || args[0] == "cm" ? stod(args[pt++]) : 0;
			judgeline.offsetY = args[0] == "cp" || args[0] == "cm" ? stod(args[pt++]) : 0;
			judgeline.rotate = args[0] == "cd" || args[0] == "cr" ? stod(args[pt++]) : 0;
			judgeline.alpha = args[0] == "ca" || args[0] == "cf" ? stod(args[pt++]) : 0;
			judgeline.easingType = args[0] == "cm" || args[0] == "cr" ? stoi(args[pt++]) : 0;
			// judgeline.cmd = "";
			judgelines.push_back(judgeline);
		}
	}
	return fromPGS(json_encode(chart), bgmOffset);
}
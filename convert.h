#pragma once
#define INFO if (levelData.size() * 100 / total != last) \
	last = levelData.size() * 100 / total, cout << "[INFO] Processed: " << levelData.size() << "/" << total << "(" << last << "%)" << endl;

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

string fromPGS(string json, double bgmOffset = 0) {
	srand(time(0));
	Json::Value obj; json_decode(json, obj);
	int fmt = obj["formatVersion"].asInt();
	// cout << obj << endl;
	bgmOffset += obj["offset"].asDouble();
	Json::Value entities; int total = 0, current = 0; int last = 0;
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		total++;
		total += item["speedEvents"].size();
		if (fmt == 3) total += item["judgeLineMoveEvents"].size() * 2;
		else if (fmt == 13) total += item["judgeLineMoveXEvents"].size() + item["judgeLineMoveYEvents"].size();
		else cout << "Unknown format version: " << fmt << endl, exit(1);
		total += item["judgeLineRotateEvents"].size();
		total += item["judgeLineDisappearEvents"].size();
		total += item["notesAbove"].size();
		total += item["notesBelow"].size();
	}
	cout << "[INFO] Total Entities: " << total << endl;
	LevelRawData levelData;
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		double bpm = item["bpm"].asDouble();
		JudgelineEntity judgeline;

		// 添加 Speed Event
		for (int j = item["speedEvents"].size() - 1; j >= 0; j--) {
			auto v = item["speedEvents"][j];
			SpeedEventEntity speed;
			speed.startTime = v["startTime"].asDouble();
			speed.endTime = v["endTime"].asDouble();
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
		if (fmt == 3) {
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
		} else if (fmt == 13) {
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
		for (int j = 0; j < item["judgeLineDisappearEvents"].size(); j++) {
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
			// note.isFake = 0;
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
			// note.isFake = 0;
			note.judgeline = judgeline;
			levelData.append(note); INFO;
		}
	}

	// cout << levelData.toJsonObject() << endl;
	
	Json::Value data = levelData.toJsonObject();
	data["formatVersion"] = 2;
	return json_encode(data);
}

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
	while (s.front() == ' ' && s.size()) s = s.substr(1);
	while (s.front() == '\t' && s.size()) s = s.substr(1);
	while (s.back() == ' ' && s.size()) s.pop_back();
	while (s.back() == '\t' && s.size()) s.pop_back();
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
	chart["formatVersion"] = 1000;
	chart["offset"] = stod(lines[0]);
	chart["judgeLineList"].resize(0);
	for (int i = 1; i < lines.size(); i++) {
		auto args = explode(" ", lines[i].c_str()); int pt = 0;
		for (int j = 0; j < args.size(); j++) args[j] = trim(args[j]);
		for (int j = 0; j < args.size(); j++) if (args[j] != "") args[pt++] = args[j];
		args.resize(pt);
		
	}
	return fromPGS(json_encode(chart), bgmOffset);
}
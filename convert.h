#pragma once
#define INFO if (entities.size() * 100 / total != last) \
	last = entities.size() * 100 / total, cout << "[INFO] Processed: " << entities.size() << "/" << total << "(" << last << "%)" << endl;

string getRef(int len) {
	string res = "", key = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	for (int i = 0; i < len; i++) res += key[rand() % key.size()];
	return res;
}
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
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		double bpm = item["bpm"].asDouble();
		string judgelineRef = getRef(32);
		string speedRef = getRef(32), moveXRef = getRef(32), moveYRef = getRef(32), rotateRef = getRef(32), disappearRef = getRef(32);
		Json::Value single;
		single["archetype"] = "Phigros Judgeline";
		single["name"] = judgelineRef;
		single["data"][0]["name"] = "speedEvent"; single["data"][0]["ref"] = speedRef;
		single["data"][1]["name"] = "moveXEvent"; single["data"][1]["ref"] = moveXRef;
		single["data"][2]["name"] = "moveYEvent"; single["data"][2]["ref"] = moveYRef;
		single["data"][3]["name"] = "rotateEvent"; single["data"][3]["ref"] = rotateRef;
		single["data"][4]["name"] = "disappearEvent"; single["data"][4]["ref"] = disappearRef;
		entities.append(single); INFO;
		for (int j = 0; j < item["speedEvents"].size(); j++) {
			auto v = item["speedEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Speed Event";
			single["name"] = speedRef; speedRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "value"; single["data"][2]["value"] = v["value"].asDouble();
			single["data"][3]["name"] = "next"; single["data"][3]["ref"] = speedRef;
			entities.append(single); INFO;
		}
		if (fmt == 3) {
			for (int j = 0; j < item["judgeLineMoveEvents"].size(); j++) {
				auto v = item["judgeLineMoveEvents"][j];
				Json::Value single;
				single["archetype"] = "Phigros Judgeline Move X Event";
				single["name"] = moveXRef; moveXRef = getRef(32);
				single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
				single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
				single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
				single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
				single["data"][4]["name"] = "easing"; single["data"][4]["value"] = 0;
				single["data"][5]["name"] = "next"; single["data"][5]["ref"] = moveXRef;
				entities.append(single); INFO;
				single["archetype"] = "Phigros Judgeline Move Y Event";
				single["name"] = moveYRef; moveYRef = getRef(32);
				single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start2"].asDouble();
				single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end2"].asDouble();
				single["data"][5]["name"] = "next"; single["data"][5]["ref"] = moveYRef;
				entities.append(single); INFO;
			} 
		} else if (fmt == 13) {
			for (int j = 0; j < item["judgeLineMoveXEvents"].size(); j++) {
				auto v = item["judgeLineMoveXEvents"][j];
				Json::Value single;
				single["archetype"] = "Phigros Judgeline Move X Event";
				single["name"] = moveXRef; moveXRef = getRef(32);
				single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
				single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
				single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
				single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
				single["data"][4]["name"] = "easing"; single["data"][4]["value"] = v["easing"].asInt();
				single["data"][5]["name"] = "next"; single["data"][5]["ref"] = moveXRef;
				entities.append(single); INFO;
			} 
			for (int j = 0; j < item["judgeLineMoveYEvents"].size(); j++) {
				auto v = item["judgeLineMoveYEvents"][j];
				Json::Value single;
				single["archetype"] = "Phigros Judgeline Move Y Event";
				single["name"] = moveYRef; moveYRef = getRef(32);
				single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
				single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
				single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
				single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
				single["data"][4]["name"] = "easing"; single["data"][4]["value"] = v["easing"].asInt();
				single["data"][5]["name"] = "next"; single["data"][5]["ref"] = moveYRef;
				entities.append(single); INFO;
			} 
		}
		for (int j = 0; j < item["judgeLineRotateEvents"].size(); j++) {
			auto v = item["judgeLineRotateEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Rotate Event";
			single["name"] = rotateRef; rotateRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
			single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
			single["data"][4]["name"] = "easing"; single["data"][4]["value"] = v["easing"].asInt();
			single["data"][5]["name"] = "next"; single["data"][5]["ref"] = rotateRef;
			entities.append(single); INFO;
		}
		for (int j = 0; j < item["judgeLineDisappearEvents"].size(); j++) {
			auto v = item["judgeLineDisappearEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Disappear Event";
			single["name"] = disappearRef; disappearRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
			single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
			single["data"][4]["name"] = "easing"; single["data"][4]["value"] = v["easing"].asInt();
			single["data"][5]["name"] = "next"; single["data"][5]["ref"] = disappearRef;
			entities.append(single); INFO;
		}
		for (int j = 0; j < item["notesAbove"].size(); j++) {
			auto v = item["notesAbove"][j];
			Json::Value single;
			single["archetype"] = "Phigros Note";
			single["data"][0]["name"] = "type"; single["data"][0]["value"] = v["type"].asInt();
			single["data"][1]["name"] = "time"; single["data"][1]["value"] = v["time"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "positionX"; single["data"][2]["value"] = v["positionX"].asDouble();
			single["data"][3]["name"] = "holdTime"; single["data"][3]["value"] = v["holdTime"].asDouble() / bpm * 1.875;
			single["data"][4]["name"] = "speed"; single["data"][4]["value"] = v["speed"].asDouble();
			single["data"][5]["name"] = "floorPosition"; single["data"][5]["value"] = v["floorPosition"].asDouble();
			single["data"][6]["name"] = "judgeline"; single["data"][6]["ref"] = judgelineRef;
			entities.append(single); INFO;
		}
		for (int j = 0; j < item["notesBelow"].size(); j++) {
			auto v = item["notesBelow"][j];
			Json::Value single;
			single["archetype"] = "Phigros Note";
			single["data"][0]["name"] = "type"; single["data"][0]["value"] = v["type"].asInt();
			single["data"][1]["name"] = "time"; single["data"][1]["value"] = v["time"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "positionX"; single["data"][2]["value"] = v["positionX"].asDouble();
			single["data"][3]["name"] = "holdTime"; single["data"][3]["value"] = v["holdTime"].asDouble() / bpm * 1.875;
			single["data"][4]["name"] = "speed"; single["data"][4]["value"] = v["speed"].asDouble();
			single["data"][5]["name"] = "floorPosition"; single["data"][5]["value"] = -1 * v["floorPosition"].asDouble();
			single["data"][6]["name"] = "judgeline"; single["data"][6]["ref"] = judgelineRef;
			entities.append(single); INFO;
		}
	}
	
	Json::Value data;
	data["formatVersion"] = 1;
	data["bgmOffset"] = bgmOffset;
	data["entities"] = entities;
	return json_encode(data);
}

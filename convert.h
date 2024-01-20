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
	auto obj = json_decode(json);
	// cout << obj << endl;
	bgmOffset += obj["offset"].asDouble();
	Json::Value entities; int total = 0, current = 0; int last = 0;
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		total++;
		total += item["speedEvents"].size();
		total += item["judgeLineMoveEvents"].size();
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
		string speedRef = getRef(32), moveRef = getRef(32), rotateRef = getRef(32), disappearRef = getRef(32);
		Json::Value single;
		single["archetype"] = "Phigros Judgeline";
		single["ref"] = judgelineRef;
		single["data"][0]["name"] = "speedEvent"; single["data"][0]["ref"] = speedRef;
		single["data"][1]["name"] = "moveEvent"; single["data"][1]["ref"] = moveRef;
		single["data"][2]["name"] = "rotateEvent"; single["data"][2]["ref"] = rotateRef;
		single["data"][3]["name"] = "disappearEvent"; single["data"][3]["ref"] = disappearRef;
		entities.append(single); INFO;
		for (int j = 0; j < item["speedEvents"].size(); j++) {
			auto v = item["speedEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Speed Event";
			single["ref"] = speedRef; speedRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "value"; single["data"][2]["value"] = v["value"].asDouble();
			single["data"][3]["name"] = "next"; single["data"][3]["ref"] = speedRef;
			entities.append(single); INFO;
		}
		for (int j = 0; j < item["judgeLineMoveEvents"].size(); j++) {
			auto v = item["judgeLineMoveEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Move Event";
			single["ref"] = moveRef; moveRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
			single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
			single["data"][4]["name"] = "start2"; single["data"][4]["value"] = v["start2"].asDouble();
			single["data"][5]["name"] = "end2"; single["data"][5]["value"] = v["end2"].asDouble();
			single["data"][6]["name"] = "next"; single["data"][6]["ref"] = moveRef;
			entities.append(single); INFO;
		}
		for (int j = 0; j < item["judgeLineRotateEvents"].size(); j++) {
			auto v = item["judgeLineRotateEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Rotate Event";
			single["ref"] = rotateRef; rotateRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
			single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
			single["data"][4]["name"] = "next"; single["data"][4]["ref"] = rotateRef;
			entities.append(single); INFO;
		}
		for (int j = 0; j < item["judgeLineDisappearEvents"].size(); j++) {
			auto v = item["judgeLineDisappearEvents"][j];
			Json::Value single;
			single["archetype"] = "Phigros Judgeline Disappear Event";
			single["ref"] = disappearRef; disappearRef = getRef(32);
			single["data"][0]["name"] = "startTime"; single["data"][0]["value"] = v["startTime"].asDouble() / bpm * 1.875;
			single["data"][1]["name"] = "endTime"; single["data"][1]["value"] = v["endTime"].asDouble() / bpm * 1.875;
			single["data"][2]["name"] = "start"; single["data"][2]["value"] = v["start"].asDouble();
			single["data"][3]["name"] = "end"; single["data"][3]["value"] = v["end"].asDouble();
			single["data"][4]["name"] = "next"; single["data"][4]["ref"] = disappearRef;
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

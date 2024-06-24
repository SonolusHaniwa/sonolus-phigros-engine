#include<bits/stdc++.h>
#include"modules/json.h"
using namespace std;

string readFile(string path) {
    ifstream fin(path);
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) {
        cout << "No such file or directory: \"" << path << "\"" << endl;
        exit(1);
    } fin.seekg(0, ios::beg);
    char* buffer = new char[len];
    fin.read(buffer, len);
    fin.close(); return string(buffer, len);
}

struct Event {
    double start;
    double end;
   	double from;
    double to;
    int easing = 0;
};
double EPS = 1e-8;
double EPS2 = 1e-1;
const double PI = acos(-1);
const double c1 = 1.70158;
const double c3 = c1 + 1;
const double c4 = (2 * PI) / 3;
double minimalSimilarity = 0.99;
int checkSize = 2;
int randomSize = 100;
int maxTime = 16;
int maxAcceptNumber = 40;
function<double(double)> EasingFunction[] = {
    [](double x){ return x; },
    [](double x){ return x * x; },
    [](double x){ return 1 - (1 - x) * (1 - x); },
    [](double x){ return (x < 0.5 ? EasingFunction[1](x * 2) / 2 : EasingFunction[2](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[2](x * 2) / 2 : EasingFunction[1](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return x * x * x; },
    [](double x){ return 1 - pow(1 - x, 3); },
    [](double x){ return (x < 0.5 ? EasingFunction[5](x * 2) / 2 : EasingFunction[6](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[6](x * 2) / 2 : EasingFunction[5](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return x * x * x * x; },
    [](double x){ return 1 - pow(1 - x, 4); },
    [](double x){ return (x < 0.5 ? EasingFunction[9](x * 2) / 2 : EasingFunction[10](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[10](x * 2) / 2 : EasingFunction[9](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return x * x * x * x * x; },
    [](double x){ return 1 - pow(1 - x, 5); },
    [](double x){ return (x < 0.5 ? EasingFunction[13](x * 2) / 2 : EasingFunction[14](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[14](x * 2) / 2 : EasingFunction[13](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return 1 - cos((x * PI) / 2); },
    [](double x){ return sin((x * PI) / 2); },
    [](double x){ return (x < 0.5 ? EasingFunction[17](x * 2) / 2 : EasingFunction[18](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[18](x * 2) / 2 : EasingFunction[17](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x == 0 ? 0 : pow(2, 10 * x - 10)); },
    [](double x){ return (x == 1 ? 1 : 1 - pow(2, -10 * x)); },
    [](double x){ return (x < 0.5 ? EasingFunction[21](x * 2) / 2 : EasingFunction[22](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[22](x * 2) / 2 : EasingFunction[21](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return 1 - sqrt(1 - pow(x, 2)); },
    [](double x){ return sqrt(1 - pow(x - 1, 2)); },
    [](double x){ return (x < 0.5 ? EasingFunction[25](x * 2) / 2 : EasingFunction[26](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[26](x * 2) / 2 : EasingFunction[25](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return c3 * x * x * x - c1 * x * x; },
    [](double x){ return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2); },
    [](double x){ return (x < 0.5 ? EasingFunction[29](x * 2) / 2 : EasingFunction[30](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[30](x * 2) / 2 : EasingFunction[29](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x == 0 ? 0 : x == 1 ? 1 : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4)); },
    [](double x){ return (x == 0 ? 0 : x == 1 ? 1 : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1); },
    [](double x){ return (x < 0.5 ? EasingFunction[33](x * 2) / 2 : EasingFunction[34](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? EasingFunction[34](x * 2) / 2 : EasingFunction[33](x * 2 - 1) / 2 + 0.5); },
    [](double x){ return (x < 0.5 ? 0 : 1); }
};

double getEaseValue(int id, double start, double end, double from, double to, double t) {
	return EasingFunction[id]((t - start) / (end - start)) * (to - from) + from;
}
double calcR2(vector<Event> &eventList, int l, int r, int easing, bool debug = false) {
    double startTime = eventList[l].start, endTime = eventList[r].end;
    double start = eventList[l].from, end = eventList[r].to;
    double sumE = 0, sumD = 0, ave = 0;
    for (int i = l; i <= r; i++) sumD += eventList[i].to + eventList[i].from;
    ave = sumD / (r - l + 1) / 2; sumD = 0;
    for (int i = l; i <= r; i++) {
        double y = getEaseValue(easing, startTime, endTime, start, end, eventList[i].start);
        sumE += (y - eventList[i].from) * (y - eventList[i].from);
        sumD += (eventList[i].from - ave) * (eventList[i].from - ave);

		y = getEaseValue(easing, startTime, endTime, start, end, eventList[i].end);
        sumE += (y - eventList[i].to) * (y - eventList[i].to);
        sumD += (eventList[i].to - ave) * (eventList[i].to - ave);
    }
    if (abs(sumD) < EPS) return 1;
    return 1 - sumE / sumD;
}

int eventOptimizer(Json::Value events, Json::Value &resEvents, string eventsName = "") {
    const int easingSize = sizeof(EasingFunction) / sizeof(EasingFunction[0]);
    vector<Event> eventList;
    for (int i = 0; i < events.size(); i++) {
        auto item = events[i];
        Event event;
        event.start = max(0.0, item["startTime"].asDouble());
        event.end = item["endTime"].asDouble();
        event.from = item["start"].asDouble();
        event.to = item["end"].asDouble();
        if (event.start == event.end) continue;
        eventList.push_back(event);
    }
    sort(eventList.begin(), eventList.end(), [](Event a, Event b) {
        return a.start < b.start;
    });
    vector<Event> resEventList;


    
    /* srand(time(NULL) + clock());
    int* Rs = new int[eventList.size()];
    for (int L = 0; L < eventList.size(); L++) {
        for (int R = L; R < eventList.size(); R++) Rs[R] = R;
        random_shuffle(Rs + L + 1, Rs + min(L + randomSize + 1, (int)eventList.size()));
        int maxR = L, maxREasing = 0, acceptNumber = 0;
        for (int k = L; k < min(L + randomSize + 1, (int)eventList.size()); k++) {
            int R = Rs[k];
    		double startTime = eventList[L].start, endTime = eventList[R].end;
    		double from = eventList[L].from, to = eventList[R].to;
    		for (int i = 0; i < easingSize; i++) {
                bool ok = true;
    			for (int j = L; j < L + checkSize; j++) {
    				ok &= abs(eventList[j].from - getEaseValue(i, startTime, endTime, from, to, eventList[j].start)) <= EPS2
    				   && abs(eventList[j].to - getEaseValue(i, startTime, endTime, from, to, eventList[j].end)) <= EPS2;
    			} if (!ok) continue;
    			if (calcR2(eventList, L, R, i) < minimalSimilarity) continue;
                if (R > maxR) maxR = R, maxREasing = i;
                acceptNumber++;
                if (acceptNumber >= maxAcceptNumber) goto end;
                else break;
    		}
        }
        end:
        resEventList.push_back({
            start: eventList[L].start,
            end: eventList[maxR].end,
            from: eventList[L].from,
            to: eventList[maxR].to,
            easing: maxREasing
        }); L = maxR;
    }
    delete[] Rs; */



    
    int* Rs = new int[eventList.size()];
    for (int L = 0; L < eventList.size(); L++) {
    	int R = L;
    	if (eventList[L].end - eventList[L].start <= maxTime)
   			while (eventList[R + 1].end - eventList[R + 1].start <= maxTime) R++;
   		double startTime = eventList[L].start, endTime = eventList[R].end;
   		double from = eventList[L].from, to = eventList[R].to; bool ok = false;
   		int maxRange = min(R + 1, (int)eventList.size());
   		for (int k = L; k < maxRange; k++) Rs[k] = maxRange - 1 - (k - L);
        random_shuffle(Rs + L + 1, Rs + maxRange);
        // cout << L << " " << maxRange << endl;
        int maxR = L, maxREasing = 0, acceptNumber = 0;
        for (int k = L; k < maxRange; k++) {
        	if (k != L && Rs[k] - L >= randomSize) continue;
            int R = Rs[k];
    		double startTime = eventList[L].start, endTime = eventList[R].end;
    		double from = eventList[L].from, to = eventList[R].to;
    		for (int i = 0; i < easingSize; i++) {
                bool ok = true;
    			for (int j = L; j < L + checkSize; j++) {
    				ok &= abs(eventList[j].from - getEaseValue(i, startTime, endTime, from, to, eventList[j].start)) <= EPS2
    				   && abs(eventList[j].to - getEaseValue(i, startTime, endTime, from, to, eventList[j].end)) <= EPS2;
    			} if (!ok) continue;
    			if (calcR2(eventList, L, R, i) < minimalSimilarity) continue;
                if (R > maxR) maxR = R, maxREasing = i;
                acceptNumber++;
                if (acceptNumber >= maxAcceptNumber) goto end;
                else break;
    		}
        }
        end:
        resEventList.push_back({
            start: eventList[L].start,
            end: eventList[maxR].end,
            from: eventList[L].from,
            to: eventList[maxR].to,
            easing: maxREasing
        }); L = maxR;
   		// if (!ok) cout << "nmsl" << endl;
    }
    delete[] Rs;



    cout << "Optimized " << eventsName << ": " << events.size() << " --> " << resEventList.size() << "(" << 100.0 - 100.0 * resEventList.size() / eventList.size() << "%)" << endl;
    resEvents.resize(0);
    for (int i = 0; i < resEventList.size(); i++) {
        auto item = resEventList[i];
        Json::Value event;
        event["startTime"] = item.start;
        event["endTime"] = item.end;
        event["start"] = item.from;
        event["end"] = item.to;
        event["easing"] = item.easing;
        resEvents.append(event);
    }
    return resEventList.size();
}

int main(int argc, char** argv) {
    if (argc < 3) {
    	cout << "Optimize Phigros's Chart using Easing Function." << endl << endl;
        cout << "Usage: " << argv[0] << " <input> <ouput> [options]" << endl;
        cout << "Compiled at " << __TIME__ << " " << __DATE__ << endl;
        cout << "Options:" << endl;
        cout << "  -h, --help: Show this help message" << endl;
        cout << "  -q: Quiet mode" << endl;
        cout << "  -Q: Super quiet mode" << endl;
        cout << "  -l=<limit>: Set minimal similarity limit, default: " << minimalSimilarity << endl;
        cout << "  -e=<num>: Set R2 EPS value(1e-num), default: " << (int)-(log(EPS) / log(10)) << endl;
        cout << "  -E=<num>: Set pre-check EPS value(1e-num), default: " << (int)-(log(EPS) / log(10)) << endl;
        cout << "  -s=<num>: Set pre-check size, default: " << checkSize << endl;
        cout << "  -r=<num>: Set max random size, default: " << randomSize << endl;
        cout << "  -m=<num>: Set max accept number, default: " << maxAcceptNumber << endl;
        return 1;
    }

    for (int i = 3; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            cout << "Usage: " << argv[0] << " <*.json> <*.json> [options]" << endl;
            cout << "Compiled at " << __TIME__ << " " << __DATE__ << endl;
            cout << "Options:" << endl;
            cout << "  -h, --help: Show this help message" << endl;
            cout << "  -q: Quiet mode" << endl;
            cout << "  -Q: Super quiet mode" << endl;
            cout << "  -l=<limit>: Set minimal similarity limit, default: " << minimalSimilarity << endl;
            cout << "  -e=<num>: Set EPS value(1e-num), default: " << (int)-(log(EPS) / log(10)) << endl;
            cout << "  -E=<num>: Set pre-check EPS value(1e-num), default: " << (int)-(log(EPS) / log(10)) << endl;
            cout << "  -s=<num>: Set pre-check size, default: " << checkSize << endl;
            cout << "  -r=<num>: Set max random size, default: " << randomSize << endl;
            cout << "  -m=<num>: Set max accept number, default: " << maxAcceptNumber << endl;
            return 0;
        } else if (arg == "-q") {
            freopen("/dev/null", "w", stdout);
        } else if (arg == "-Q") {
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
        } else if (arg.substr(0, 3) == "-l=") {
            minimalSimilarity = stod(arg.substr(3));
        } else if (arg.substr(0, 3) == "-e=") {
            EPS = pow(10, -stod(arg.substr(3)));
        } else if (arg.substr(0, 3) == "-E=") {
            EPS2 = pow(10, -stod(arg.substr(3)));
        } else if (arg.substr(0, 3) == "-s=") {
            checkSize = stoi(arg.substr(3));
        } else if (arg.substr(0, 3) == "-r=") {
            randomSize = stoi(arg.substr(3));
        } else if (arg.substr(0, 3) == "-m=") {
            maxAcceptNumber = stoi(arg.substr(3));
        }
    }

    string json = readFile(argv[1]);
    Json::Value obj, resobj; json_decode(json, obj);
    resobj = obj;
    cout << "Loading finished." << endl;
    int total = 0, after = 0; 
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
		auto item = obj["judgeLineList"][i];
		total++;
		total += item["speedEvents"].size();
		total += item["judgeLineMoveEvents"].size() * 2;
		total += item["judgeLineRotateEvents"].size();
		total += item["judgeLineDisappearEvents"].size();
	}
	for (int i = 0; i < obj["judgeLineList"].size(); i++) { 
        auto item = obj["judgeLineList"][i];
        int total2 = 0, after2 = 0;
		total2 += item["speedEvents"].size();
		total2 += item["judgeLineMoveEvents"].size() * 2;
		total2 += item["judgeLineRotateEvents"].size();
		total2 += item["judgeLineDisappearEvents"].size();
        item["judgeLineMoveXEvents"].resize(0); item["judgeLineMoveYEvents"].resize(0);
        for (int j = 0; j < item["judgeLineMoveEvents"].size(); j++) {
            auto event = item["judgeLineMoveEvents"][j];
            item["judgeLineMoveXEvents"].append(event);
            event["start"] = event["start2"]; event["end"] = event["end2"];
            item["judgeLineMoveYEvents"].append(event);
        }
        resobj["judgeLineList"][i]["judgeLineMoveXEvents"] = item["judgeLineMoveXEvents"];
        resobj["judgeLineList"][i]["judgeLineMoveYEvents"] = item["judgeLineMoveYEvents"];
        after2 += item["speedEvents"].size();
        after2 += eventOptimizer(
            item["judgeLineMoveXEvents"], 
            resobj["judgeLineList"][i]["judgeLineMoveXEvents"], 
            "#" + to_string(i) + " judgeLineMoveXEvents");
        after2 += eventOptimizer(
            item["judgeLineMoveYEvents"], 
            resobj["judgeLineList"][i]["judgeLineMoveYEvents"], 
            "#" + to_string(i) + " judgeLineMoveYEvents");
        after2 += eventOptimizer(
            item["judgeLineRotateEvents"], 
            resobj["judgeLineList"][i]["judgeLineRotateEvents"], 
            "#" + to_string(i) + " judgeLineRotateEvents");
        after2 += eventOptimizer(
            item["judgeLineDisappearEvents"], 
            resobj["judgeLineList"][i]["judgeLineDisappearEvents"], 
            "#" + to_string(i) + " judgeLineDisappearEvents");
        resobj["judgeLineList"][i].removeMember("judgeLineMoveEvents");
        cerr << "Optimized #" << i << " judgeLine: " << total2 << " --> " << after2 << "(" << 100.0 - 100.0 * after2 / total2 << "%)" << endl;
        after += after2;
    }
    cerr << "Optimization Summary: " << total << " --> " << after << "(" << 100.0 - 100.0 * after / total << "%)" << endl;
    resobj["formatVersion"] = 13;
    json = json_encode(resobj);
    ofstream fout(argv[2]);
    fout << json;
    fout.close();
}
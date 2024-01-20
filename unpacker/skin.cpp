#include<bits/stdc++.h>
using namespace std;
#include"../modules/buffer.h"
#include"../modules/json.h"
#include"../modules/gzip.h"
#include"../modules/png.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " [SkinTexture] [SkinData]" << endl;
		return 1;
	}

	cout << "Reading Skin Texture..." << endl;
	image texture = readImage(argv[1]);

	cout << "Reading Skin Data..." << endl;
	ifstream fin(argv[2]);
	fin.seekg(0, ios::end); int len = fin.tellg();
	char* ch = new char[len]; fin.seekg(0, ios::beg);
	fin.read(ch, len); string data = "";
	for (int i = 0; i < len; i++) data.push_back(ch[i]);
	string json = decompress_gzip(data).asString();
	Json::Value obj; json_decode(json, obj);

	cout << "Unpacking Skin Sprites..." << endl;
	for (int i = 0; i < obj["sprites"].size(); i++) {
		auto item = obj["sprites"][i];
		int w = item["w"].asInt(), h = item["h"].asInt();
		int x = item["x"].asInt(), y = item["y"].asInt();
		image img(w, h);
		for (int j = 0; j < h; j++) {
			for (int k = 0; k < w; k++) {
				img.data[j][k * 4] = texture.data[j + y][(k + x) * 4];
				img.data[j][k * 4 + 1] = texture.data[j + y][(k + x) * 4 + 1];
				img.data[j][k * 4 + 2] = texture.data[j + y][(k + x) * 4 + 2];
				img.data[j][k * 4 + 3] = texture.data[j + y][(k + x) * 4 + 3];
			}
		} writeImage("../skin/" + item["name"].asString() + ".png", img);
	}
}

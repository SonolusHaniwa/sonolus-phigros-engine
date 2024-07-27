#pragma once

class SkinDataSprite {
	public:
	string name;
	int x, y, w, h;

	Json::Value toJsonObject() {
		Json::Value res;
		res["name"] = name;
		res["x"] = x; res["y"] = y; res["w"] = w; res["h"] = h;
		string x[8] = { "x1", "x2", "x3", "x4", "y1", "y2", "y3", "y4" };
		for (int i = 0; i < 8; i++) res["transform"][x[i]][x[i]] = 1;
		return res;
	}
};

class SkinData {
	public:
	int width, height;	
	bool interpolation = false;
	vector<SkinDataSprite> sprites;

	Json::Value toJsonObject() {
		Json::Value res;
		res["width"] = width;
		res["height"] = height;
		res["interpolation"] = interpolation;
		res["sprites"].resize(0);
		for (int i = 0; i < sprites.size(); i++) res["sprites"].append(sprites[i].toJsonObject());
		return res;
	}
};

void packSkin(string textureDir, string dataDir) {
	cout << "Reading Skin Sprites..." << endl;
	vector<image> imgs; SkinData data;
	for (int i = 0; i < engineData.skin_sprites.size(); i++) {
		string imgPath = "./skin/" + engineData.skin_sprites[i].first + ".png";
		if (!fileExists(imgPath)) throw runtime_error(("Unknown Skin Sprite \"" + imgPath + "\"").c_str());
		imgs.push_back(readImage(imgPath));
		data.sprites.push_back({ engineData.skin_sprites[i].first, 0, 0, 0, 0 });
	}

	cout << "Packing Skin Texture..." << endl;
	using namespace rbp;
	vector<RectSize> rects, rects_back; vector<Rect> result_rects; 
	vector<size_t> rects_id, rects_id_back, result_ids;
	for (int i = 0; i < imgs.size(); i++) rects.push_back({imgs[i].width, imgs[i].height});
	for (int i = 0; i < imgs.size(); i++) rects_id.push_back(i);
	rects_id_back = rects_id; rects_back = rects;
	int width = 512, height = 512;
	while (true) {
		rects = rects_back, rects_id = rects_id_back;
		result_rects.clear(); result_ids.clear();
		auto RectsFactory = MaxRects(width, height, false);
		int res = RectsFactory.insert(1, rects, rects_id, result_rects, result_ids);
		if (res == rects.size()) break;
		width *= 2, height *= 2;
	}

	cout << "Writing Skin Texture..." << endl;
	image outimg = image(width, height);
	outimg.width = width, outimg.height = height;
	data.width = width; data.height = height;
	for (int i = 0; i < result_rects.size(); i++) {
		int id = result_ids[i], x = result_rects[i].x, y = result_rects[i].y;
		data.sprites[id].x = x; data.sprites[id].y = y;
		data.sprites[id].w = result_rects[i].width; data.sprites[id].h = result_rects[i].height;
		for (int j = y; j < y + result_rects[i].height; j++) {
			for (int k = x; k < x + result_rects[i].width; k++) {
				outimg.data[j][k * 4] = imgs[id].data[j - y][(k - x) * imgs[id].channel];
				outimg.data[j][k * 4 + 1] = imgs[id].data[j - y][(k - x) * imgs[id].channel + 1];
				outimg.data[j][k * 4 + 2] = imgs[id].data[j - y][(k - x) * imgs[id].channel + 2];
				outimg.data[j][k * 4 + 3] = imgs[id].channel < 4 ? 255 : imgs[id].data[j - y][(k - x) * imgs[id].channel + 3];
			}
		}
	} writeImage(textureDir, outimg);

	cout << "Writing Skin Data..." << endl;
	string json = json_encode(data.toJsonObject());
	buffer dataBuffer = compress_gzip(json);
	ofstream fout(dataDir);
	char* ch = new char[dataBuffer.v.size()];
	for (int i = 0; i < dataBuffer.v.size(); i++) ch[i] = dataBuffer.v[i];
	fout.write(ch, dataBuffer.v.size()); delete[] ch;
	fout.close();
}

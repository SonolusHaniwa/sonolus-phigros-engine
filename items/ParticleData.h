#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

class TransformExpression {
	public:
	vector<pair<string, double> > expression;

	Json::Value toJsonObject() {
		Json::Value json; json = Json::objectValue;
		for (int i = 0 ; i < expression.size(); i++)
			json[expression[i].first] = expression[i].second;
		return json;
	}
};

class TransformExpressionInput {
	public:
	string name = "";
	double multiplier = 1;

	TransformExpressionInput(string name): name(name){}
	TransformExpressionInput operator * (double multiplier) {
		TransformExpressionInput res = (*this);
		res.multiplier *= multiplier;
		return res;
	}
	operator TransformExpression() {
		TransformExpression exp;
		exp.expression.push_back({name, multiplier});
		return exp;
	}
};

TransformExpression operator + (TransformExpression exp, TransformExpressionInput input) {
	exp.expression.push_back({input.name, input.multiplier});
	return exp;
}
TransformExpression operator - (TransformExpression exp, TransformExpressionInput input) {
	exp.expression.push_back({input.name, -1 * input.multiplier});
	return exp;
}
TransformExpression operator + (TransformExpressionInput input1, TransformExpressionInput input2) {
	TransformExpression exp;
	exp.expression.push_back({input1.name, input1.multiplier});
	exp.expression.push_back({input2.name, input2.multiplier});
	return exp;
}
TransformExpression operator - (TransformExpressionInput input1, TransformExpressionInput input2) {
	TransformExpression exp;
	exp.expression.push_back({input1.name, input1.multiplier});
	exp.expression.push_back({input2.name, -1 * input2.multiplier});
	return exp;
}

class ParticleSpriteProperty {
	public:
	TransformExpression from;
	TransformExpression to;
	string ease;

	Json::Value toJsonObject() {
		Json::Value res; res = Json::objectValue;
		if (from.expression.size()) res["from"] = from.toJsonObject();
		if (to.expression.size()) res["to"] = to.toJsonObject();
		if (ease != "") res["ease"] = ease;
		return res;
	}
};
class ParticleDataEffectSprite {
	public:
	int sprite = 0;
	string color = "#fff";
	double start = 0;
	double duration = 1;
	ParticleSpriteProperty x;
	ParticleSpriteProperty y;
	ParticleSpriteProperty w;
	ParticleSpriteProperty h;
	ParticleSpriteProperty r;
	ParticleSpriteProperty a;
	
	Json::Value toJsonObject() {
		Json::Value res;
		res["sprite"] = sprite;
		res["color"] = color;
		res["start"] = start;
		res["duration"] = duration;
		res["x"] = x.toJsonObject();
		res["y"] = y.toJsonObject();
		res["w"] = w.toJsonObject();
		res["h"] = h.toJsonObject();
		res["r"] = r.toJsonObject();
		res["a"] = a.toJsonObject();
		return res;
	}
};
class ParticleDataEffect {
	public:
	string name;
	TransformExpression x1, y1, x2, y2, x3, y3, x4, y4;
	vector<pair<int, vector<ParticleDataEffectSprite> > > groups;

	Json::Value toJsonObject() {
		Json::Value res;
		res["name"] = name;
		res["transform"]["x1"] = x1.toJsonObject();
		res["transform"]["y1"] = y1.toJsonObject();
		res["transform"]["x2"] = x2.toJsonObject();
		res["transform"]["y2"] = y2.toJsonObject();
		res["transform"]["x3"] = x3.toJsonObject();
		res["transform"]["y3"] = y3.toJsonObject();
		res["transform"]["x4"] = x4.toJsonObject();
		res["transform"]["y4"] = y4.toJsonObject();
		res["groups"].resize(0);
		for (int i = 0; i < groups.size(); i++) {
			res["groups"][i]["count"] = groups[i].first;
			res["groups"][i]["particles"].resize(0);
			for (int j = 0; j < groups[i].second.size(); j++) 
				res["groups"][i]["particles"].append(groups[i].second[j].toJsonObject());
		} return res;
	}
};

vector<string> sprites;
vector<ParticleDataEffect> particle_effects;
#define defineSprite(name) int name = [](){sprites.push_back(#name);return sprites.size() - 1;}()
void defineEffect(ParticleDataEffect effect) { particle_effects.push_back(effect); }
Json::Value packSprites(string texturePath) {
	cout << "Reading sprites..." << endl;
	vector<image> imgs;
	for (int i = 0; i < sprites.size(); i++) {
		string imgPath = "./particle/sprites/" + sprites[i] + ".png";
		if (!fileExists(imgPath)) 
			throw runtime_error(("Unknown Sprites \"particle/sprites/" + sprites[i] + ".png").c_str());
		image img = readImage(imgPath);
		imgs.push_back(img);
	}

	cout << "Calculating texture..." << endl;
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

	cout << "Writing texture..." << endl;
	image outimg; Json::Value res;
	outimg.width = width, outimg.height = height;
	res["width"] = width; res["height"] = height;
	res["sprites"].resize(0);
	outimg.data = new png_bytep[height];
	for (int i = 0; i < height; i++) outimg.data[i] = reinterpret_cast<png_bytep>(new Byte[width * 4]);
	for (int i = 0; i < result_rects.size(); i++) {
		int id = result_ids[i], x = result_rects[i].x, y = result_rects[i].y;
		res["sprites"][id]["x"] = x + 1; res["sprites"][id]["y"] = y + 1;
		res["sprites"][id]["w"] = result_rects[i].width, res["sprites"][id]["h"] = result_rects[i].height;
		for (int j = y; j < y + result_rects[i].height; j++) {
			for (int k = x; k < x + result_rects[i].width; k++) {
				outimg.data[j][k * 4] = imgs[id].data[j - y][(k - x) * imgs[id].channel];
				outimg.data[j][k * 4 + 1] = imgs[id].data[j - y][(k - x) * imgs[id].channel + 1];
				outimg.data[j][k * 4 + 2] = imgs[id].data[j - y][(k - x) * imgs[id].channel + 2];
				outimg.data[j][k * 4 + 3] = imgs[id].channel < 4 ? 255 : imgs[id].data[j - y][(k - x) * imgs[id].channel + 3];
			}
		}
	}
	writeImage(texturePath, outimg);
	cout << "Success to pack sprites into texture!" << endl;
	return res;
}

namespace particleData {
	TransformExpressionInput c("c");
	TransformExpressionInput x1("x1"), y1("y1"), x2("x2"), y2("y2"), x3("x3"), y3("y3"), x4("x4"), y4("y4");
	TransformExpressionInput r1("r1"), r2("r2"), r3("r3"), r4("r4");
	TransformExpressionInput sinr1("sinr1"), sinr2("sinr2"), sinr3("sinr3"), sinr4("sinr4");
	TransformExpressionInput cosr1("cosr1"), cosr2("cosr2"), cosr3("cosr3"), cosr4("cosr4");
};

#endif

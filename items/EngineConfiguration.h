#ifndef ENGINECONFIGURATION_H
#define ENGINECONFIGURATION_H

using namespace std;

class EngineConfigurationOption {
    public:

    string name = "";
    string scope = "";
    int advanced = 0;
    int standard = 0;
    string type = "slider";
    double def = 0;
    double min = 0;
    double max = 0;
    double step = 1;
    vector<string> values = {};
    string unit = "";

    Json::Value toJsonObject() {
        if (type == "select") {
            Json::Value res;
            res["name"] = name;
            res["standard"] = (int)standard;
            res["scope"] = scope;
            res["type"] = type;
            res["def"] = def;
            res["values"].resize(0);
            for (int i = 0; i < values.size(); i++) res["values"].append(values[i]);
            return res;
        } else if (type == "slider") {
            Json::Value res;
            res["name"] = name;
            res["standard"] = standard;
            res["scope"] = scope;
            res["type"] = type;
            res["def"] = def;
            res["min"] = min;
            res["max"] = max;
            res["step"] = step;
            res["unit"] = unit;
            return res;
        } else if (type == "toggle") {
            Json::Value res;
            res["name"] = name;
            res["standard"] = (int)standard;
            res["scope"] = scope;
            res["type"] = type;
            res["def"] = (int)def;
            return res;
        } else throw invalid_argument("Invalid option type.");
    }
};

class EngineConfigurationVisibility {
    public:

    double scale = 0;
    double alpha = 0;

    Json::Value toJsonObject() {
        Json::Value res;
        res["scale"] = scale;
        res["alpha"] = alpha;
        return res;
    }
};

class EngineConfigurationAnimationTween {
    public:
    
    double from = 0;
    double to = 0;
    double duration = 0;
    string ease = "";

    Json::Value toJsonObject() {
        Json::Value res;
        res["from"] = from;
        res["to"] = to;
        res["duration"] = duration;
        res["ease"] = ease;
        return res;
    }
};

class EngineConfigurationAnimation {
    public:

    EngineConfigurationAnimationTween scale = {};
    EngineConfigurationAnimationTween alpha = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["scale"] = scale.toJsonObject();
        res["alpha"] = alpha.toJsonObject();
        return res;
    }
};

class EngineConfigurationUI {
    public:

    string primaryMetric = "";
    string secondaryMetric = "";
    EngineConfigurationVisibility menuVisibility = {};
    EngineConfigurationVisibility judgmentVisibility = {};
    EngineConfigurationVisibility comboVisibility = {};
    EngineConfigurationVisibility primaryMetricVisibility = {};
    EngineConfigurationVisibility secondaryMetricVisbility = {};
    EngineConfigurationVisibility progressVisibility = {};
    EngineConfigurationVisibility tutorialNavigationVisibility = {};
    EngineConfigurationVisibility tutorialInstructionVisibility = {};
    EngineConfigurationAnimation judgmentAnimation = {};
    EngineConfigurationAnimation comboAnimation = {};
    string judgmentErrorStyle = "";
    string judgmentErrorPlacement = "";
    int judgmentErrorMin = 0;
    string scope = "";

    Json::Value toJsonObject() {
        Json::Value res;
        res["scope"] = scope;
        res["primaryMetric"] = primaryMetric;
        res["secondaryMetric"] = secondaryMetric;
        res["menuVisibility"] = menuVisibility.toJsonObject();
        res["judgmentVisibility"] = judgmentVisibility.toJsonObject();
        res["comboVisibility"] = comboVisibility.toJsonObject();
        res["primaryMetricVisibility"] = primaryMetricVisibility.toJsonObject();
        res["secondaryMetricVisibility"] = secondaryMetricVisbility.toJsonObject();
        res["progressVisibility"] = progressVisibility.toJsonObject();
        res["tutorialNavigationVisibility"] = tutorialNavigationVisibility.toJsonObject();
        res["tutorialInstructionVisibility"] = tutorialInstructionVisibility.toJsonObject();
        res["judgmentAnimation"] = judgmentAnimation.toJsonObject();
        res["comboAnimation"] = comboAnimation.toJsonObject();
        res["judgmentErrorStyle"] = judgmentErrorStyle;
        res["judgmentErrorPlacement"] = judgmentErrorPlacement;
        res["judgmentErrorMin"] = judgmentErrorMin;
        return res;
    }
};

class EngineConfiguration {
    public:

    vector<EngineConfigurationOption> options = {};
    EngineConfigurationUI ui = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["options"].resize(0);
        for (int i = 0; i < options.size(); i++) res["options"].append(options[i].toJsonObject());
        res["ui"] = ui.toJsonObject();
        return res;
    }
};

#endif

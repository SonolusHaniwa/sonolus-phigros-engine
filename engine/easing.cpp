const double c1 = 1.70158;
const double c3 = c1 + 1;
const double c4 = (2 * PI) / 3;
function<let(let)> EasingFunction[] = {
    [](let x){ return x; },
    [](let x){ return x * x; },
    [](let x){ return 1 - (1 - x) * (1 - x); },
    [](let x){ return If(x < 0.5, EasingFunction[1](x * 2) / 2, EasingFunction[2](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[2](x * 2) / 2, EasingFunction[1](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return x * x * x; },
    [](let x){ return 1 - Power({1 - x, 3}); },
    [](let x){ return If(x < 0.5, EasingFunction[5](x * 2) / 2, EasingFunction[6](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[6](x * 2) / 2, EasingFunction[5](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return x * x * x * x; },
    [](let x){ return 1 - Power({1 - x, 4}); },
    [](let x){ return If(x < 0.5, EasingFunction[9](x * 2) / 2, EasingFunction[10](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[10](x * 2) / 2, EasingFunction[9](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return x * x * x * x * x; },
    [](let x){ return 1 - Power({1 - x, 5}); },
    [](let x){ return If(x < 0.5, EasingFunction[13](x * 2) / 2, EasingFunction[14](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[14](x * 2) / 2, EasingFunction[13](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return 1 - Cos((x * PI) / 2); },
    [](let x){ return Sin((x * PI) / 2); },
    [](let x){ return If(x < 0.5, EasingFunction[17](x * 2) / 2, EasingFunction[18](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[18](x * 2) / 2, EasingFunction[17](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x == 0, 0, Power({2, 10 * x - 10})); },
    [](let x){ return If(x == 1, 1, 1 - Power({2, -10 * x})); },
    [](let x){ return If(x < 0.5, EasingFunction[21](x * 2) / 2, EasingFunction[22](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[22](x * 2) / 2, EasingFunction[21](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return 1 - Power({1 - Power({x, 2}), 0.5}); },
    [](let x){ return Power({1 - Power({x - 1, 2}), 0.5}); },
    [](let x){ return If(x < 0.5, EasingFunction[25](x * 2) / 2, EasingFunction[26](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[26](x * 2) / 2, EasingFunction[25](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return c3 * x * x * x - c1 * x * x; },
    [](let x){ return 1 + c3 * Power({x - 1, 3}) + c1 * Power({x - 1, 2}); },
    [](let x){ return If(x < 0.5, EasingFunction[29](x * 2) / 2, EasingFunction[30](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[30](x * 2) / 2, EasingFunction[29](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x == 0, 0, If(x == 1, 1, -1 * Power({2, 10 * x - 10}) * Sin((x * 10 - 10.75) * c4))); },
    [](let x){ return If(x == 0, 0, If(x == 1, 1, Power({2, -10 * x}) * Sin((x * 10 - 0.75) * c4) + 1)); },
    [](let x){ return If(x < 0.5, EasingFunction[33](x * 2) / 2, EasingFunction[34](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, EasingFunction[34](x * 2) / 2, EasingFunction[33](x * 2 - 1) / 2 + 0.5); },
    [](let x){ return If(x < 0.5, 0, 1); }
};

let getEaseValue(let id, let start, let end, let from, let to, let t) {
	vector<pair<let, let> > branches;
	for (int i = 0; i < sizeof(EasingFunction) / sizeof(EasingFunction[0]); i++) 
		branches.push_back({i, EasingFunction[i]((t - start) / (end - start)) * (to - from) + from});
	return SwitchWithDefault(id, branches, branches[0].second);
}
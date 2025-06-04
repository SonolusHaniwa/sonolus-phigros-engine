FuncNode c1 = 1.70158;
FuncNode c3 = c1 + 1;
FuncNode c4 = (2 * PI) / 3;
function<FuncNode(FuncNode)> EasingFunction[] = {
    [](FuncNode x){ return x; },
    [](FuncNode x){ return x * x; },
    [](FuncNode x){ return 1 - (1 - x) * (1 - x); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[1](x * 2) / 2, EasingFunction[2](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[2](x * 2) / 2, EasingFunction[1](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return x * x * x; },
    [](FuncNode x){ return 1 - Power({1 - x, 3}); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[5](x * 2) / 2, EasingFunction[6](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[6](x * 2) / 2, EasingFunction[5](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return x * x * x * x; },
    [](FuncNode x){ return 1 - Power({1 - x, 4}); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[9](x * 2) / 2, EasingFunction[10](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[10](x * 2) / 2, EasingFunction[9](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return x * x * x * x * x; },
    [](FuncNode x){ return 1 - Power({1 - x, 5}); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[13](x * 2) / 2, EasingFunction[14](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[14](x * 2) / 2, EasingFunction[13](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return 1 - Cos((x * PI) / 2); },
    [](FuncNode x){ return Sin((x * PI) / 2); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[17](x * 2) / 2, EasingFunction[18](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[18](x * 2) / 2, EasingFunction[17](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x == 0, 0, Power({2, 10 * x - 10})); },
    [](FuncNode x){ return If(x == 1, 1, 1 - Power({2, -10 * x})); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[21](x * 2) / 2, EasingFunction[22](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[22](x * 2) / 2, EasingFunction[21](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return 1 - Power({1 - Power({x, 2}), 0.5}); },
    [](FuncNode x){ return Power({1 - Power({x - 1, 2}), 0.5}); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[25](x * 2) / 2, EasingFunction[26](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[26](x * 2) / 2, EasingFunction[25](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return c3 * x * x * x - c1 * x * x; },
    [](FuncNode x){ return 1 + c3 * Power({x - 1, 3}) + c1 * Power({x - 1, 2}); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[29](x * 2) / 2, EasingFunction[30](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[30](x * 2) / 2, EasingFunction[29](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x == 0, 0, If(x == 1, 1, -1 * Power({2, 10 * x - 10}) * Sin((x * 10 - 10.75) * c4))); },
    [](FuncNode x){ return If(x == 0, 0, If(x == 1, 1, Power({2, -10 * x}) * Sin((x * 10 - 0.75) * c4) + 1)); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[33](x * 2) / 2, EasingFunction[34](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, EasingFunction[34](x * 2) / 2, EasingFunction[33](x * 2 - 1) / 2 + 0.5); },
    [](FuncNode x){ return If(x < 0.5, 0, 1); },
};

FuncNode getEaseValue(FuncNode id, FuncNode start, FuncNode end, FuncNode from, FuncNode to, FuncNode t, FuncNode l, FuncNode r) {
    id = Round(id);
	vector<SwitchWithDefault_Group_test_consequent> branches;
	for (int i = 0; i < sizeof(EasingFunction) / sizeof(EasingFunction[0]); i++) 
		branches.push_back({i, EasingFunction[i]((t - start) / (end - start) * (r - l) + l) * (to - from) + from});
	return If(t < start || t > end,
		If(t < start, from, to),
		SwitchWithDefault(id, branches, branches[0].consequent)
	);
}

FuncNode getBezierValue(FuncNode p1, FuncNode p2, FuncNode p3, FuncNode p4, FuncNode start, FuncNode end, FuncNode from, FuncNode to, FuncNode t, FuncNode l, FuncNode r) {
    FuncNode t1 = (t - start) / (end - start) * (r - l) + l;
    return 
        Power({(1 - t1), 3}) * p1 + 
        3 * t1 * Power({(1 - t1), 2}) * p2 + 
        3 * Power({t1, 2}) * (1 - t1) * p3 +
        Power({t1, 3}) * p4
    ;
}
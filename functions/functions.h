FuncNode Abs(FuncNode value) {
    return FuncNode(RuntimeFunction.Abs, {value});
}

// vector<FuncNode> Unpack(vector<FuncNode> value, string search) {
// 	vector<FuncNode> res;
// 	for (int i = 0; i < value.size(); i++) {
// 		auto tmp = dataContainer[value[i].getNodeId()];
// 		if (tmp.type != "value" && tmp.func.func == search) {
// 			vector<FuncNode> args; FuncNode x;
// 			for (int j = 0; j < tmp.func.args.size(); j++)
// 				x.nodeId = tmp.func.args[j], args.push_back(x);
// 			args = Unpack(args, search);
// 			for (int j = 0; j < args.size(); j++) res.push_back(args[j]);
// 		} else res.push_back(value[i]);
// 	} return res;
// }

FuncNode Add(vector<FuncNode> value) {
	// value = Unpack(value, RuntimeFunction.Add);
	// double addValue = 0; vector<FuncNode> args;
	// for (int i = 0; i < value.size(); i++) {
	// 	auto tmp = dataContainer[value[i].getNodeId()];
	// 	if (tmp.type == "value") addValue += tmp.value.value;
	// 	else args.push_back(value[i]);
	// } if (abs(addValue) > 1e-10) args.push_back(addValue);
    return FuncNode(RuntimeFunction.Add, value);
}

FuncNode And(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.And, value);
}

FuncNode Arccos(FuncNode value) {
    return FuncNode(RuntimeFunction.Arccos, {value});
}

FuncNode Arcsin(FuncNode value) {
    return FuncNode(RuntimeFunction.Arcsin, {value});
}

FuncNode Arctan(FuncNode value) {
    return FuncNode(RuntimeFunction.Arctan, {value});
}

//
FuncNode Arctan2(FuncNode y, FuncNode x) {
    return FuncNode(RuntimeFunction.Arctan2, {y, x});
}

FuncNode BeatToBPM(FuncNode beat) {
    return FuncNode(RuntimeFunction.BeatToBPM, {beat});
}

FuncNode BeatToStartingBeat(FuncNode beat) {
    return FuncNode(RuntimeFunction.BeatToStartingBeat, {beat});
}

FuncNode BeatToStartingTime(FuncNode beat) {
    return FuncNode(RuntimeFunction.BeatToStartingTime, {beat});
}

FuncNode BeatToTime(FuncNode beat) {
    return FuncNode(RuntimeFunction.BeatToTime, {beat});
}

FuncNode __builtin_Block(FuncNode body) {
    return FuncNode(RuntimeFunction.Block, {body});
}
#define Block(body) [&](){ \
	blockCounter.top()++; \
	auto res = __builtin_Block(body); \
	blockCounter.top()--; \
	return res; \
}()

void Break(FuncNode count, FuncNode value) {
    R(FuncNode(RuntimeFunction.Break, {count, value}));
}

FuncNode Ceil(FuncNode value) {
    return FuncNode(RuntimeFunction.Ceil, {value});
}

FuncNode Clamp(FuncNode x, FuncNode a, FuncNode b) {
    return FuncNode(RuntimeFunction.Clamp, {x, a, b});
}

FuncNode Copy(FuncNode srcId, FuncNode srcIndex, FuncNode dstId, FuncNode dstIndex, FuncNode count) {
    return FuncNode(RuntimeFunction.Copy, {srcId, srcIndex, dstId, dstIndex, count});
} 

FuncNode Cos(FuncNode value) {
    return FuncNode(RuntimeFunction.Cos, {value});
}

FuncNode Cosh(FuncNode value) {
    return FuncNode(RuntimeFunction.Cosh, {value});
}

void Debuglog(FuncNode value) {
    R(FuncNode(RuntimeFunction.DebugLog, {value}));
}

FuncNode DecrementPostPointed(FuncNode id, FuncNode index, FuncNode offset) {
    return FuncNode(RuntimeFunction.DecrementPostPointed, {id, index, offset});
}

FuncNode DecrementPostShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.DecrementPostShifted, {id, x, y, s});
}

FuncNode DecrementPost(FuncNode id, FuncNode index) {
    return FuncNode(RuntimeFunction.DecrementPost, {id, index});
}

FuncNode DecrementPrePointed(FuncNode id, FuncNode index, FuncNode offset) {
    return FuncNode(RuntimeFunction.DecrementPrePointed, {id, index, offset});
}

FuncNode DecrementPreShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.DecrementPreShifted, {id, x, y, s});
}

FuncNode DecrementPre(FuncNode id, FuncNode index) {
    return FuncNode(RuntimeFunction.DecrementPre, {id, index});
}

void DebugPause() {
    R(FuncNode(RuntimeFunction.DebugPause, {}));
}

FuncNode Degree(FuncNode value) {
    return FuncNode(RuntimeFunction.Degree, {value});
}

void DestroyParticleEffect(FuncNode particleId) {
    R(FuncNode(RuntimeFunction.DestroyParticleEffect, {particleId}));
}

FuncNode Divide(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Divide, value);
}

FuncNode DoWhile(FuncNode body, FuncNode test) {
    return FuncNode(RuntimeFunction.DoWhile, {body, test});
}

void Draw(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a) {
    R(FuncNode(RuntimeFunction.Draw, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a}));
}

FuncNode DrawCurvedB(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
    return FuncNode(RuntimeFunction.DrawCurvedB, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a, n, p, q});
}

FuncNode DrawCurvedBT(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a, FuncNode n, FuncNode p1, FuncNode q1, FuncNode p2, FuncNode q2) {
    return FuncNode(RuntimeFunction.DrawCurvedBT, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a, n, p1, q1, p2, q2});
}

FuncNode DrawCurvedL(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
    return FuncNode(RuntimeFunction.DrawCurvedL, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a, n, p, q});
}

FuncNode DrawCurvedLR(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a, FuncNode n, FuncNode p1, FuncNode q1, FuncNode p2, FuncNode q2) {
    return FuncNode(RuntimeFunction.DrawCurvedLR, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a, n, p1, q1, p2, q2});
}

FuncNode DrawCurvedR(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
    return FuncNode(RuntimeFunction.DrawCurvedR, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a, n, p, q});
}

FuncNode DrawCurvedT(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode z, FuncNode a, FuncNode n, FuncNode p, FuncNode q) {
    return FuncNode(RuntimeFunction.DrawCurvedT, {id, x1, y1, x2, y2, x3, y3, x4, y4, z, a, n, p, q});
}

FuncNode Ease(FuncNode value, string easeType) {
    return FuncNode(easeType, {value});
}

FuncNode Equal(FuncNode lhs, FuncNode rhs) {
    return FuncNode(RuntimeFunction.Equal, {lhs, rhs});
}

FuncNode Execute(vector<FuncNode> value) {
	if (value.size() == 1) return value[0];
    return FuncNode(RuntimeFunction.Execute, value);
}

FuncNode Execute0(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Execute0, value);
}

void ExportValue(FuncNode index, FuncNode value) {
    R(FuncNode(RuntimeFunction.ExportValue, {index, value}));
}

FuncNode Floor(FuncNode value) {
    return FuncNode(RuntimeFunction.Floor, {value});
}

FuncNode Frac(FuncNode value) {
    return FuncNode(RuntimeFunction.Frac, {value});
}

FuncNode Get(FuncNode id, FuncNode index) {
    return FuncNode(RuntimeFunction.Get, {id, index});
}

FuncNode GetPointed(FuncNode id, FuncNode index, FuncNode offset) {
    return FuncNode(RuntimeFunction.GetPointed, {id, index, offset});
}

FuncNode GetShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.GetShifted, {id, x, y, s});
}

FuncNode Greater(FuncNode lhs, FuncNode rhs) {
    return FuncNode(RuntimeFunction.Greater, {lhs, rhs});
}

FuncNode GreaterOr(FuncNode lhs, FuncNode rhs) {
    return FuncNode(RuntimeFunction.GreaterOr, {lhs, rhs});
}

FuncNode HasEffectClip(FuncNode id) {
    return FuncNode(RuntimeFunction.HasEffectClip, {id});
}

FuncNode HasParticleEffect(FuncNode id) {
    return FuncNode(RuntimeFunction.HasParticleEffect, {id});
}

FuncNode HasSkinSprite(FuncNode id) {
    return FuncNode(RuntimeFunction.HasSkinSprite, {id});
}

FuncNode If(FuncNode test, FuncNode consequent, FuncNode alternate) {
    return FuncNode(RuntimeFunction.If, {test, consequent, alternate});
}

FuncNode IncrementPostPointed(FuncNode id, FuncNode index, FuncNode offset) {
    return FuncNode(RuntimeFunction.IncrementPostPointed, {id, index, offset});
}

FuncNode IncrementPostShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.IncrementPostShifted, {id, x, y, s});
}

FuncNode IncrementPost(FuncNode id, FuncNode index) {
    return FuncNode(RuntimeFunction.IncrementPost, {id, index});
}

FuncNode IncrementPrePointed(FuncNode id, FuncNode index, FuncNode offset) {
    return FuncNode(RuntimeFunction.IncrementPrePointed, {id, index, offset});
}

FuncNode IncrementPreShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.IncrementPreShifted, {id, x, y, s});
}

FuncNode IncrementPre(FuncNode id, FuncNode index) {
    return FuncNode(RuntimeFunction.IncrementPre, {id, index});
}

/*
FuncNode IsDebug() {
    return FuncNode(RuntimeFunction.IsDebug, {});
}
*/

FuncNode Judge(FuncNode target, FuncNode source, FuncNode minPerfect, FuncNode maxPerfect, FuncNode minGreat, FuncNode maxGreat, FuncNode minGood, FuncNode maxGood) {
    return FuncNode(RuntimeFunction.Judge, {target, source, minPerfect, maxPerfect, minGreat, maxGreat, minGood, maxGood});
}

FuncNode JudgeSimple(FuncNode target, FuncNode source, FuncNode maxPerfect, FuncNode maxGreat, FuncNode maxGood) {
    return FuncNode(RuntimeFunction.JudgeSimple, {target, source, maxPerfect, maxGreat, maxGood});
}

FuncNode JumpLoop(vector<FuncNode> branch) {
    return FuncNode(RuntimeFunction.JumpLoop, branch);
}

FuncNode Lerp(FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.Lerp, {x, y, s});
}

FuncNode LerpClamped(FuncNode x, FuncNode y, FuncNode s) {
    return FuncNode(RuntimeFunction.LerpClamped, {x, y, s});
}

FuncNode Less(FuncNode lhs, FuncNode rhs) {
    return FuncNode(RuntimeFunction.Less, {lhs, rhs});
}

FuncNode LessOr(FuncNode lhs, FuncNode rhs) {
    return FuncNode(RuntimeFunction.LessOr, {lhs, rhs});
}

FuncNode Log(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Log, value);
}

FuncNode Max(FuncNode x, FuncNode y) {
    return FuncNode(RuntimeFunction.Max, {x, y});
}

FuncNode Min(FuncNode x, FuncNode y) {
    return FuncNode(RuntimeFunction.Min, {x, y});
}

FuncNode Mod(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Mod, value);
}

void MoveParticleEffect(FuncNode particleId, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4) {
    R(FuncNode(RuntimeFunction.MoveParticleEffect, {particleId, x1, y1, x2, y2, x3, y3, x4, y4}));
}

FuncNode Multiply(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Multiply, value);
}

FuncNode Negate(FuncNode value) {
    return FuncNode(RuntimeFunction.Negate, {value});
}

FuncNode Not(FuncNode value) {
    return FuncNode(RuntimeFunction.Not, {value});
}

FuncNode NotEqual(FuncNode lhs, FuncNode rhs) {
    return FuncNode(RuntimeFunction.NotEqual, {lhs, rhs});
}

FuncNode Or(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Or, value);
}

FuncNode Paint(FuncNode id, FuncNode x, FuncNode y, FuncNode size, FuncNode rotation, FuncNode z, FuncNode a) {
    return FuncNode(RuntimeFunction.Paint, {id, x, y, size, rotation, z, a});
}
void Play(FuncNode id, FuncNode distance) {
    R(FuncNode(RuntimeFunction.Play, {id, distance}));
}

FuncNode PlayLooped(FuncNode id) {
    return FuncNode(RuntimeFunction.PlayLooped, {id});
}

FuncNode PlayLoopedScheduled(FuncNode id, FuncNode startTime) {
    return FuncNode(RuntimeFunction.PlayLoopedScheduled, {id, startTime});
}

//
void PlayScheduled(FuncNode id, FuncNode time, FuncNode distance) {
    R(FuncNode(RuntimeFunction.PlayScheduled, {id, time, distance}));
}

FuncNode Power(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Power, value);
}

void Print(FuncNode value, FuncNode format, FuncNode decimalPlaces, FuncNode anchorX, FuncNode anchorY, FuncNode pivotX, FuncNode pivotY, FuncNode width, FuncNode height, FuncNode rotation, FuncNode color, FuncNode alpha, FuncNode horizontalAlign, FuncNode background) {
    R(FuncNode(RuntimeFunction.Print, {value, format, decimalPlaces, anchorX, anchorY, pivotX, pivotY, width, height, rotation, color, alpha, horizontalAlign, background}));
}

FuncNode Radian(FuncNode value) {
    return FuncNode(RuntimeFunction.Radian, {value});
}

FuncNode Random(FuncNode min, FuncNode max) {
    return FuncNode(RuntimeFunction.Random, {min, max});
}

FuncNode RandomInteger(FuncNode min, FuncNode max) {
    return FuncNode(RuntimeFunction.RandomInteger, {min, max});
}

FuncNode Rem(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Rem, value);
}

FuncNode Remap(FuncNode a, FuncNode b, FuncNode c, FuncNode d, FuncNode value) {
    return FuncNode(RuntimeFunction.Remap, {a, b, c, d, value});
}

FuncNode Round(FuncNode value) {
    return FuncNode(RuntimeFunction.Round, {value});
}

void Set(FuncNode id, FuncNode index, FuncNode value) {
    R(FuncNode(RuntimeFunction.Set, {id, index, value}));
}

FuncNode SetShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetShifted, {id, x, y, s, value});
}

FuncNode SetAddPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetAddPointed, {id, index, offset, value});
}

FuncNode SetAddShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetAddShifted, {id, x, y, s, value});
}

FuncNode SetAdd(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetAdd, {id, index, value});
}

FuncNode SetDividePointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetDividePointed, {id, index, offset, value});
}

FuncNode SetDivideShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetDivideShifted, {id, x, y, s, value});
}

FuncNode SetDivide(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetDivide, {id, index, value});
}

FuncNode SetModPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetModPointed, {id, index, offset, value});
}

FuncNode SetModShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetModShifted, {id, x, y, s, value});
}

FuncNode SetMod(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetMod, {id, index, value});
}

FuncNode SetMultiplyPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetMultiplyPointed, {id, index, offset, value});
}

FuncNode SetMultiplyShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetMultiplyShifted, {id, x, y, s, value});
}

FuncNode SetMultiply(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetMultiply, {id, index, value});
}

FuncNode SetPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetPointed, {id, index, offset, value});
}

FuncNode SetPowerPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetPowerPointed, {id, index, offset, value});
}

FuncNode SetPowerShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetPowerShifted, {id, x, y, s, value});
}

FuncNode SetPower(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetPower, {id, index, value});
}

FuncNode SetRemPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetRemPointed, {id, index, offset, value});
}

FuncNode SetRemShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetRemShifted, {id, x, y, s, value});
}

FuncNode SetRem(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetRem, {id, index, value});
}

FuncNode SetSubtractPointed(FuncNode id, FuncNode index, FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.SetSubtractPointed, {id, index, offset, value});
}

FuncNode SetSubtractShifted(FuncNode id, FuncNode x, FuncNode y, FuncNode s, FuncNode value) {
    return FuncNode(RuntimeFunction.SetSubtractShifted, {id, x, y, s, value});
}

FuncNode SetSubtract(FuncNode id, FuncNode index, FuncNode value) {
    return FuncNode(RuntimeFunction.SetSubtract, {id, index, value});
}

FuncNode Sign(FuncNode value) {
    return FuncNode(RuntimeFunction.Sign, {value});
}

FuncNode Sin(FuncNode value) {
    return FuncNode(RuntimeFunction.Sin, {value});
}

FuncNode Sinh(FuncNode value) {
    return FuncNode(RuntimeFunction.Sinh, {value});
}

/*
FuncNode Smoothstep(FuncNode a, FuncNode b, FuncNode x) {
    return FuncNode(RuntimeFunction.Smoothstep, {a, b, x});
}
*/

void Spawn(FuncNode id, vector<FuncNode> data) {
	data.insert(data.begin(), id);
    R(FuncNode(RuntimeFunction.Spawn, data));
}

void SpawnParticleEffect(FuncNode id, FuncNode x1, FuncNode y1, FuncNode x2, FuncNode y2, FuncNode x3, FuncNode y3, FuncNode x4, FuncNode y4, FuncNode duration, FuncNode isLooped) {
    R(FuncNode(RuntimeFunction.SpawnParticleEffect, {id, x1, y1, x2, y2, x3, y3, x4, y4, duration, isLooped}));
}

FuncNode StackEnter(FuncNode size) {
    return FuncNode(RuntimeFunction.StackEnter, {size});
}

FuncNode StackGetFramePointer() {
    return FuncNode(RuntimeFunction.StackGetFramePointer, {});
}

FuncNode StackGetFrame(FuncNode offset) {
    return FuncNode(RuntimeFunction.StackGetFrame, {offset});
}

FuncNode StackGetPointer() {
    return FuncNode(RuntimeFunction.StackGetPointer, {});
}

FuncNode StackGet(FuncNode offset) {
    return FuncNode(RuntimeFunction.StackGet, {offset});
}

FuncNode StackGrow(FuncNode size) {
    return FuncNode(RuntimeFunction.StackGrow, {size});
}

FuncNode StackInit() {
    return FuncNode(RuntimeFunction.StackInit, {});
}

FuncNode StackLeave() {
    return FuncNode(RuntimeFunction.StackLeave, {});
}

FuncNode StackPop() {
    return FuncNode(RuntimeFunction.StackPop, {});
}

FuncNode StackPush(FuncNode value) {
    return FuncNode(RuntimeFunction.StackPush, {value});
}

FuncNode StackSetFramePointer(FuncNode value) {
    return FuncNode(RuntimeFunction.StackSetFramePointer, {value});
}

FuncNode StackSetFrame(FuncNode offset, FuncNode index) {
    return FuncNode(RuntimeFunction.StackSetFrame, {offset, index});
}

FuncNode StackSetPointer(FuncNode value) {
    return FuncNode(RuntimeFunction.StackSetPointer, {value});
}

FuncNode StackSet(FuncNode offset, FuncNode value) {
    return FuncNode(RuntimeFunction.StackSet, {offset, value});
}

void StopLooped(FuncNode loopId) {
    R(FuncNode(RuntimeFunction.StopLooped, {loopId}));
}

void StopLoopedScheduled(FuncNode loopId, FuncNode endTime) {
    R(FuncNode(RuntimeFunction.StopLoopedScheduled, {loopId, endTime}));
}

FuncNode Subtract(vector<FuncNode> value) {
    return FuncNode(RuntimeFunction.Subtract, value);
}

FuncNode Switch(FuncNode discriminant, vector<pair<FuncNode, FuncNode> > consequent) {
    vector<FuncNode> args; args.push_back(discriminant);
    for (int i = 0 ; i < consequent.size(); i++) args.push_back(consequent[i].first), args.push_back(consequent[i].second);
    return FuncNode(RuntimeFunction.Switch, args);
}

FuncNode SwitchWithDefault(FuncNode discriminant, vector<pair<FuncNode, FuncNode> > consequent, FuncNode consequent_def) {
    vector<FuncNode> args; args.push_back(discriminant);
    for (int i = 0 ; i < consequent.size(); i++) args.push_back(consequent[i].first), args.push_back(consequent[i].second);
    args.push_back(consequent_def);
    return FuncNode(RuntimeFunction.SwitchWithDefault, args);
}

FuncNode SwitchInteger(FuncNode discriminant, vector<FuncNode> consequent) {
    consequent.insert(consequent.begin(), discriminant);
    return FuncNode(RuntimeFunction.SwitchInteger, consequent);
}

FuncNode SwitchIntegerWithDefault(FuncNode discriminant, vector<FuncNode> consequent, FuncNode consequent_def) {
    consequent.insert(consequent.begin(), discriminant); consequent.push_back(consequent_def);
    return FuncNode(RuntimeFunction.SwitchIntegerWithDefault, consequent);
}

FuncNode Tan(FuncNode value) {
    return FuncNode(RuntimeFunction.Tan, {value});
}

FuncNode Tanh(FuncNode value) {
    return FuncNode(RuntimeFunction.Tanh, {value});
}

FuncNode TimeToScaledTime(FuncNode time) {
    return FuncNode(RuntimeFunction.TimeToScaledTime, {time});
}

FuncNode TimeToStartingScaledTime(FuncNode time) {
    return FuncNode(RuntimeFunction.TimeToStartingScaledTime, {time});
}

FuncNode TimeToStartingTime(FuncNode time) {
    return FuncNode(RuntimeFunction.TimeToStartingTime, {time});
}

FuncNode TimeToTimeScale(FuncNode time) {
    return FuncNode(RuntimeFunction.TimeToTimeScale, {time});
}

FuncNode Trunc(FuncNode value) {
    return FuncNode(RuntimeFunction.Trunc, {value});
}

FuncNode Unlerp(FuncNode a, FuncNode b, FuncNode x) {
    return FuncNode(RuntimeFunction.Unlerp, {a, b, x});
}

FuncNode UnlerpClamped(FuncNode a, FuncNode b, FuncNode x) {
    return FuncNode(RuntimeFunction.UnlerpClamped, {a, b, x});
}

void While(FuncNode test, FuncNode body) {
    R(FuncNode(RuntimeFunction.While, {test, body}));
}

// 自定义函数

void Return(FuncNode code) {
    Break(blockCounter.top(), code);
}

void Exit(FuncNode code) {
	stack<int> tmp = blockCounter; int blockCount = 0;
	while (tmp.size()) blockCount += tmp.top(), tmp.pop();
	Break(blockCount - 1, code);
}

FuncNode If(FuncNode cond, FuncNode block) {
    return If(cond, block, 0);
}

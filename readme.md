# Sonolus Phigros Engine

A recreation of Phigros engine in [Sonolus](https://sonolus.com). For Sonolus 0.8.0+.

Use [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h) and [sonolus-server-cpp](https://github.com/SonolusHaniwa/sonolus-server-cpp) to develop.

Todo List:

- [ ] Finish writing buckets system
- [ ] Finish writing tutorial mode
- [ ] Finish writing preview mode
- [ ] Finish writing watch mode
- [x] Designed particle effect for phigros

## Links

- [Sonolus Website](https://sonolus.com) 
- [Sonolus Wiki](https://github.com/NonSpicyBurrito/sonolus-wiki)
- [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h)

## Installation

You need to install [sonolus-server-cpp](https://github.com/SonolusHaniwa/sonolus-server-cpp) first!

```bash
./sonolus buildcpp play phigros
```

## Documentation

### Chart Optimizier

Because Pigeon Games uses straight lines to fit of all event curves, there are too many events in the official chart.

In order to reduce the number of events, I have implemented such a optimizer to fit all straight lines back to curves.

Tips: I use random algorithms for fitting, therefore this program may not have much technical content.

Use commands below to compile the `optimizer` program:

```bash
g++ optimizer.cpp -o optimizer -ljsoncpp -O3
```

Basic usage is as follows:

```bash
./optimizer Chart_AT.json Chart_AT_Optimized.json
```

According to the original official chart format, this program has made the following changes to the generated chart file:

- Change `formatVersion` from `3` to `13`
- Add `easing` property to each event
- Split `judgeLineMoveEvents` into `judgeLineMoveXEvents` and `judgeLineMoveYEvents`

The easing functions are as follows:

| Easing | Name | Expression |
| :-: | :-: | - |
| 0 | EaseLinear | `[](double x){ return x; }` |
| 1 | EaseInQuad | `[](double x){ return x * x; }` |
| 2 | EaseOutQuad | `[](double x){ return 1 - (1 - x) * (1 - x); }` |
| 3 | EaseInOutQuad | `[](double x){ return (x < 0.5 ? EasingFunction[1](x * 2) / 2 : EasingFunction[2](x * 2 - 1) / 2 + 0.5); }` |
| 4 | EaseOutInQuad | `[](double x){ return (x < 0.5 ? EasingFunction[2](x * 2) / 2 : EasingFunction[1](x * 2 - 1) / 2 + 0.5); }` |
| 5 | EaseInCubic | `[](double x){ return x * x * x; }` |
| 6 | EaseOutCubic | `[](double x){ return 1 - pow(1 - x, 3); }` |
| 7 | EaseInOutCubic | `[](double x){ return (x < 0.5 ? EasingFunction[5](x * 2) / 2 : EasingFunction[6](x * 2 - 1) / 2 + 0.5); }` |
| 8 | EaseOutInCubic | `[](double x){ return (x < 0.5 ? EasingFunction[6](x * 2) / 2 : EasingFunction[5](x * 2 - 1) / 2 + 0.5); }` |
| 9 | EaseInQuart | `[](double x){ return x * x * x * x; }` |
| 10 | EaseOutQuart | `[](double x){ return 1 - pow(1 - x, 4); }` |
| 11 | EaseInOutQuart | `[](double x){ return (x < 0.5 ? EasingFunction[9](x * 2) / 2 : EasingFunction[10](x * 2 - 1) / 2 + 0.5); }` |
| 12 | EaseOutInQuart | `[](double x){ return (x < 0.5 ? EasingFunction[10](x * 2) / 2 : EasingFunction[9](x * 2 - 1) / 2 + 0.5); }` |
| 13 | EaseInQuint | `[](double x){ return x * x * x * x * x; }` |
| 14 | EaseOutQuint | `[](double x){ return 1 - pow(1 - x, 5); }` |
| 15 | EaseInOutQuint | `[](double x){ return (x < 0.5 ? EasingFunction[13](x * 2) / 2 : EasingFunction[14](x * 2 - 1) / 2 + 0.5); }` |
| 16 | EaseOutInQuint | `[](double x){ return (x < 0.5 ? EasingFunction[14](x * 2) / 2 : EasingFunction[13](x * 2 - 1) / 2 + 0.5); }` |
| 17 | EaseInSine | `[](double x){ return 1 - cos((x * acos(-1)) / 2); }` |
| 18 | EaseOutSine | `[](double x){ return sin((x * acos(-1)) / 2); }` |
| 19 | EaseInOutSine | `[](double x){ return (x < 0.5 ? EasingFunction[17](x * 2) / 2 : EasingFunction[18](x * 2 - 1) / 2 + 0.5); }` |
| 20 | EaseOutInSine | `[](double x){ return (x < 0.5 ? EasingFunction[18](x * 2) / 2 : EasingFunction[17](x * 2 - 1) / 2 + 0.5); }` |
| 21 | EaseInExpo | `[](double x){ return (x == 0 ? 0 : pow(2, 10 * x - 10)); }` |
| 22 | EaseOutExpo | `[](double x){ return (x == 1 ? 1 : 1 - pow(2, -10 * x)); }` |
| 23 | EaseInOutExpo | `[](double x){ return (x < 0.5 ? EasingFunction[21](x * 2) / 2 : EasingFunction[22](x * 2 - 1) / 2 + 0.5); }` |
| 24 | EaseOutInExpo | `[](double x){ return (x < 0.5 ? EasingFunction[22](x * 2) / 2 : EasingFunction[21](x * 2 - 1) / 2 + 0.5); }` |
| 25 | EaseInCirc | `[](double x){ return 1 - sqrt(1 - pow(x, 2)); }` |
| 26 | EaseOutCirc | `[](double x){ return sqrt(1 - pow(x - 1, 2)); }` |
| 27 | EaseInOutCirc | `[](double x){ return (x < 0.5 ? EasingFunction[25](x * 2) / 2 : EasingFunction[26](x * 2 - 1) / 2 + 0.5); }` |
| 28 | EaseOutInCirc | `[](double x){ return (x < 0.5 ? EasingFunction[26](x * 2) / 2 : EasingFunction[25](x * 2 - 1) / 2 + 0.5); }` |
| 29 | EaseInElastic | `[](double x){ return 2.70158 * x * x * x - 1.70158 * x * x; }` |
| 30 | EaseOutElastic | `[](double x){ return 1 + 2.70158 * pow(x - 1, 3) + 1.70158 * pow(x - 1, 2); }` |
| 31 | EaseInOutElastic | `[](double x){ return (x < 0.5 ? EasingFunction[29](x * 2) / 2 : EasingFunction[30](x * 2 - 1) / 2 + 0.5); }` |
| 32 | EaseOutInElastic | `[](double x){ return (x < 0.5 ? EasingFunction[30](x * 2) / 2 : EasingFunction[29](x * 2 - 1) / 2 + 0.5); }` |
| 33 | EaseInBack | `[](double x){ return (x == 0 ? 0 : x == 1 ? 1 : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * (2 * acos(-1)) / 3)); }` |
| 34 | EaseOutBack | `[](double x){ return (x == 0 ? 0 : x == 1 ? 1 : pow(2, -10 * x) * sin((x * 10 - 0.75) * (2 * acos(-1)) / 3) + 1); }` |
| 35 | EaseInOutBack | `[](double x){ return (x < 0.5 ? EasingFunction[33](x * 2) / 2 : EasingFunction[34](x * 2 - 1) / 2 + 0.5); }` |
| 36 | EaseOutInBack | `[](double x){ return (x < 0.5 ? EasingFunction[34](x * 2) / 2 : EasingFunction[33](x * 2 - 1) / 2 + 0.5); }` |
| 37 | EaseNone | `[](double x){ return (x < 0.5 ? 0 : 1); }` |

Performances:

| Chart Name | PRAGMATISM RESURRECTION AT |  祈-我ら神祖と共に歩む者なり- AT | ERABYE CONNEC10N AT | Indelible Scar AT |
| - | :-: | :-: | :-: | :-: |
| **Execution Time** | 8.752s | 12.835s | 10.228s | 6.163s |
| **Events(Before)** | 154436 | 218502 | 146286 | 120007 |
| **Events(After)** | 7779 | 8669 | 8080 | 5043 |
| **Chart.json(Before)** | 9256343 | 13226001 | 8741497 | 7308163 |
| **Chart.json(After)** | 754428 | 959750 | 849974 | 567775 |
| **LevelData(Before)** | 7406212 | 10463793 | 6893964 | 6000468 |
| **LevelData(After)** | 387917 | 449083 | 405189 | 272209 |
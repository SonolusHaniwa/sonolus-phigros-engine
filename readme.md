![b4b42df0b336e852d14b4d334c3d99f8992626d8](https://github.com/user-attachments/assets/03fe10a1-d4f4-40e0-9d87-a76897df33d4)

<h1 align="center">Sonolus Phigros Engine</h1>

<p align="center">A recreation of Phigros engine in <a href="https://sonolus.com">Sonolus</a>. For Sonolus 0.8.0+.</p>

<p align="center">Use <a href="https://github.com/SonolusHaniwa/sonolus.h">sonolus.h</a> and <a href="https://github.com/SonolusHaniwa/sonolus-server-cpp">sonolus-server-cpp</a> to develop.</p>

Todo List:

- [x] Finish writing play mode
- [x] Finish writing buckets system
- [x] Finish writing tutorial mode
- [x] Finish writing watch mode
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

### Merge Sorting

Because in watch mode, it is necessary to sort each entity by time in order to calculate the correct combo number and other information

But in a level, we not only have note entities, but also judgment line event entities, which means we cannot directly sort them. We must filter out note entities and sort them

And the entity index in Sonolus cannot be modified

In summary, we consider creating a linked list for all note entities and storing the index of the next entity in the Entity Shared Memory block of that note entity

If you simply use bubble sort, when the number of notes is too large, it will get stuck on the loading page or even cause the application to crash

Consider using merge sort

However, Sonolus does not support the use of recursive functions, so we are considering using a non-recursive version of merge sort

Algorithm keywords: merge sort, linked list, non-recursive

The detailed alogrithm implementation: [./engine/watch/Preprocess.cpp](./engine/watch/Preprocess.cpp)

The alogrithm implementation in C++: [./mergeSort.cpp](./mergeSort.cpp)

Accepted Submission in Online Judge Platform: <https://www.luogu.com.cn/record/167007458> (arraySize <= 1e5, 1s, 256MB)

![image](https://github.com/user-attachments/assets/a9ed1c68-bcdd-4894-a39d-987c422cc5a0)

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

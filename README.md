<p align="center"><img src="https://github.com/SonolusHaniwa/sonolus-server-cpp/assets/63852815/e7f00aea-cc1b-428b-8611-0c1844dcd15a"/></p>

<h1 align="center">Sonolus Engine Library in C++</h1>

使用 C++ 开发的一个 Sonolus 引擎开发库。

样例引擎地址: https://github.com/SonolusHaniwa/sonolus-sirius-engine

## 链接

- [Sonolus Website](https://sonolus.com/)
- [Sonolus Wiki](https://wiki.sonolus.com/)

## 构建

### 下载依赖

```bash
sudo apt install libjsoncpp-dev libssl-dev zlib1g-dev libpng-dev libzip-dev -y
```

在主程序中直接引入 `sonolus.h` 即可。`main.cpp` 为示例程序。

### 编译

基础编译指令:

```bash
g++ main.cpp -omain -ljsoncpp -lssl -lcrypto -lz -lpng -lzip
```

**附加编译选项列表**

- `-Dplay`: 编译 EnginePlayData 和 EngineConfiguration
- `-Dtutorial`: 编译 EngineTutorialData 和 EngineConfiguration
- `-Dpreview`: 编译 EnginePreviewData 和 EngineConfiguration
- `-Dwatch`: 编译 EngineWatchData 和 EngineConfiguration
- `-DENABLE_TRACE`: 提供编译报错时的函数调用结构，需要 `cpptrace` 库支持

## Todo List

- [x] 实现 `EngineData` 和 `EngineConfiguration` 基础结构。
- [x] 实现 `FuncNode` 的运算符重载功能。
- [x] 实现 `build` 功能。
- [x] 预定义部分 `block` 指针。
- [x] 以插件形式接入 [sonolus-server-cpp](https://github.com/SonolusHaniwa/sonolus-server-cpp)
- [x] 实现简单的编译报错功能。
- [ ] 完成英文文档。
- [ ] 完成 wiki 界面。

## 更新日志

### v2.0.0 for Sonolus 0.8.0 2024.4.10

1. 提供对 WatchData 和 PreviewData 的支持
2. 新增 WHILE 宏定义
3. 对代码进行重构，现在用 sonolus.h 编写出来的代码更像自然语言，而非机械语言
4. 提供对 Skin/SFX 的打包接口
5. 提供一点点编译报错功能

### v1.1.0 for Sonolus 0.7.2 2023.8.9

1. 新增对 TutorialData 的支持
2. 新增 IF/ELSE/FOR 宏定义
3. 修改整体架构

### v1.0.0-alpha for Sonolus 0.7.0 2023.6.17

所有基本结构体已全部完工，正在进行初步测试中。

具体修改参照 https://github.com/Sonolus/wiki-engine-specs/pull/7/files

### v1.0.0-alpha for Sonolus 0.6.5 2023.5.20

🎁 520 特供 💝

所有基本结构体已全部完工，正在进行初步测试中。

1. 实现 `EngineData` 和 `EngineConfiguration` 基础结构。
2. 实现 `FuncNode` 的运算符重载功能。
3. 实现 `build` 功能。
4. 预定义部分 `block` 指针，共 18 个。

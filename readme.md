# Sonolus Sirius Engine

## ⚠️ Please close `Settings > Graphics > GPU Particle System` in Sonolus before you play this engine!

A recreation of World Dai Star: Dream's Stellarium engine in [Sonolus](https://sonolus.com). For Sonolus 0.7.4.

适用于 Sonolus 0.7.4 的 World Dai Star: Dream's Stellarium 引擎。

Use [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h) and [sonolus-server-cpp](https://github.com/SonolusHaniwa/sonolus-server-cpp) to develop.

Todo List:

- [x] Finish writing buckets system
- [x] Finish writing tutorial mode
- [x] Finish writing preview mode
- [x] Finish writing watch mode
- [ ] Designed particle effect for sirius
- [x] Update to Sonolus v0.7.4

使用 Sonolus Server C++ v1.4.7 进行开发，并在 Ubuntu amd64 环境下编译通过。

## Links

- [Sonolus Website](https://sonolus.com) 
- [Sonolus Wiki](https://github.com/NonSpicyBurrito/sonolus-wiki)
- [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h)

## Installation

```bash
sudo apt install libjsoncpp-dev openssl zlib1g-dev libssl-dev -y # 安装环境依赖
git clone https://github.com/SonolusHaniwa/sonolus-sirius-engine # 拉取源码
cd sonolus-sirius-engine # 修改目录
g++ main.cpp -o main -ljsoncpp -lssl -lcrypto -lz -Dplay # 编译程序
./main # 在 dist 文件夹下生成 EnginePlayData 和 EngineConfiguration
g++ main.cpp -o main -ljsoncpp -lssl -lcrypto -lz -Dtutorial # 编译程序
./main # 在 dist 文件夹下生成 EngineTutorialData 和 EngineConfiguration
g++ main.cpp -o main -ljsoncpp -lssl -lcrypto -lz -Dpreview # 编译程序
./main # 在 dist 文件夹下生成 EnginePreviewData 和 EngineConfiguration
g++ main.cpp -o main -ljsoncpp -lssl -lcrypto -lz -Dwatch # 编译程序
./main # 在 dist 文件夹下生成 EngineWatchData 和 EngineConfiguration
./main txt2data LevelData.txt music_config.txt dist/LevelData # 在 dist 文件夹下生成 LevelData

g++ skin_maker.cpp -os -ljsoncpp -lpng # 编译皮肤生成器
./s # 生成皮肤及相关文件
```

## JavaScript Convertor Module

**You need install [emscripten](https://github.com/emscripten-core/emscripten) first.**

Compile Command:

```bash
sudo chmod 0777 ./emake && ./emake
```

It will create `libsirius.js` and `libsirius.wasm`. Here's a simple code for your reference:

```javascript
import factory from './libsirius.js';

async function fromSUS(sus) {
	var inst = await factory();
	var ptr = inst.stringToNewUTF8(sus); // 将字符串转为 char* 指针
	// char* _sus2txt(char*): 将 sus 转为 txt 文件
	// char* _txt2data(char*): 将 txt 转为未加密的 LevelData
	var res = inst._txt2data(inst._sus2txt(ptr));
	var dat = inst.UTF8ToString(res); // 从 char* 转回为字符串
	return dat;
}

var txt = fromSUS(sus); // 将 sus 转为 LevelData(未加密)
```

## Documentation

### `char* sus2txt(char* sus)` / `string fromSUS(string text)`

Convert sus chart to wds txt chart.

- `sus` / `text`: Chart content.

### `char* txt2data(char* txt)` / `string fromSirius(string text, double chartOffset, double bgmOffset = 0)`

Convert wds txt chart to Sonolus LevelData (without gzip encoded).

- `txt` / `text`: Chart content.
- `chartOffset`: Chart offset.
- `bgmOffset`: BGM offset (or delay seconds in music_config.txt).

## Custom Resources

### 自制谱方法

请使用 Ched 编写世界大明星谱面，下列是音符对照表:

|Sirius|Ched|
|:-:|:-:|
|Tap 音符|Tap 音符|
|高亮 Tap 音符|高亮 Tap 音符|
|Flick 音符|Flick 音符 + 向上 Air 音符|
|Hold 音符(青色)|Slide 音符|
|Scratch Hold 音符(紫色)|Slide 终点 + 向上 Air 音符|
|高亮 (Scratch) Hold 音符|Slide 起点 + 高亮 Tap 音符|
|Scratch 音符(在 Hold 音符中)|Slide 中途 + 向上 Air 音符|
|(Scratch) Hold 中继点|Hold 中继点|
|Jump Scratch 音符|用 Flick 音符覆盖一个 Slide 音符的起点和另一个 Slide 音符的重点 + 指定方向的 Air 音符|
|分裂线|使用 HiSpeed 事件指定，整数部分指定轨道数，小数部分指定分裂线样式。整数部分为整十数表示分裂线结束|

样例谱面: [chs 格式](https://oneindex.littleyang.me/api/raw?path=/data/satellite.chs) [sus 格式](https://oneindex.littleyang.me/api/raw?path=/data/satellite.sus) [谱面预览视频](https://www.bilibili.com/video/BV1AH4y1R7xX)

### Skin Sprites

|ID|Sprite|
|:-:|:-:|
|0|#STAGE_MIDDLE|
|1|#STAGE_BOTTOM_BORDER|
|2|#STAGE_COVER|
|3|#JUDGMENT_LINE|
|4|#NOTE_HEAD_RED|
|5|#NOTE_HEAD_YELLOW|
|6|#NOTE_HEAD_BLUE|
|7|#NOTE_HEAD_PURPLE|
|8|#NOTE_TICK_BLUE|
|9|#NOTE_TICK_PURPLE|
|10|#DIRECTIONAL_MARKER_PURPLE|
|11|#NOTE_CONNECTION_BLUE|
|12|#NOTE_CONNECTION_PURPLE|
|13|#LANE|
|14|Sirius Split Line #1|
|15|Sirius Split Line Transform 1 #1|
|16|Sirius Split Line Transform 2 #1|
|17|Sirius Split Line #10010|
|18|Sirius Split Line Transform 1 #10010|
|19|Sirius Split Line Transform 2 #10010|
|20|Sirius Split Line #10020|
|21|Sirius Split Line Transform 1 #10020|
|22|Sirius Split Line Transform 2 #10020|
|23|Sirius Split Line #10030|
|24|Sirius Split Line Transform 1 #10030|
|25|Sirius Split Line Transform 2 #10030|
|26|Sirius Split Line #10040|
|27|Sirius Split Line Transform 1 #10040|
|28|Sirius Split Line Transform 2 #10040|
|29|Sirius Split Line #10050|
|30|Sirius Split Line Transform 1 #10050|
|31|Sirius Split Line Transform 2 #10050|
|32|Sirius Split Line #10060|
|33|Sirius Split Line Transform 1 #10060|
|34|Sirius Split Line Transform 2 #10060|
|35|Sirius Split Line #10070|
|36|Sirius Split Line Transform 1 #10070|
|37|Sirius Split Line Transform 2 #10070|
|38|Sirius Split Line #1010|
|39|Sirius Split Line Transform 1 #1010|
|40|Sirius Split Line Transform 2 #1010|
|41|Sirius Split Line #10100|
|42|Sirius Split Line Transform 1 #10100|
|43|Sirius Split Line Transform 2 #10100|
|44|Sirius Split Line #10110|
|45|Sirius Split Line Transform 1 #10110|
|46|Sirius Split Line Transform 2 #10110|
|47|Sirius Split Line #10120|
|48|Sirius Split Line Transform 1 #10120|
|49|Sirius Split Line Transform 2 #10120|
|50|Sirius Split Line #10130|
|51|Sirius Split Line Transform 1 #10130|
|52|Sirius Split Line Transform 2 #10130|
|53|Sirius Split Line #10140|
|54|Sirius Split Line Transform 1 #10140|
|55|Sirius Split Line Transform 2 #10140|
|56|Sirius Split Line #10150|
|57|Sirius Split Line Transform 1 #10150|
|58|Sirius Split Line Transform 2 #10150|
|59|Sirius Split Line #10160|
|60|Sirius Split Line Transform 1 #10160|
|61|Sirius Split Line Transform 2 #10160|
|62|Sirius Split Line #10161|
|63|Sirius Split Line Transform 1 #10161|
|64|Sirius Split Line Transform 2 #10161|
|65|Sirius Split Line #10162|
|66|Sirius Split Line Transform 1 #10162|
|67|Sirius Split Line Transform 2 #10162|
|68|Sirius Split Line #10170|
|69|Sirius Split Line Transform 1 #10170|
|70|Sirius Split Line Transform 2 #10170|
|71|Sirius Split Line #10180|
|72|Sirius Split Line Transform 1 #10180|
|73|Sirius Split Line Transform 2 #10180|
|74|Sirius Split Line #10190|
|75|Sirius Split Line Transform 1 #10190|
|76|Sirius Split Line Transform 2 #10190|
|77|Sirius Split Line #10191|
|78|Sirius Split Line Transform 1 #10191|
|79|Sirius Split Line Transform 2 #10191|
|80|Sirius Split Line #1020|
|81|Sirius Split Line Transform 1 #1020|
|82|Sirius Split Line Transform 2 #1020|
|83|Sirius Split Line #10200|
|84|Sirius Split Line Transform 1 #10200|
|85|Sirius Split Line Transform 2 #10200|
|86|Sirius Split Line #10220|
|87|Sirius Split Line Transform 1 #10220|
|88|Sirius Split Line Transform 2 #10220|
|89|Sirius Split Line #10230|
|90|Sirius Split Line Transform 1 #10230|
|91|Sirius Split Line Transform 2 #10230|
|92|Sirius Split Line #10240|
|93|Sirius Split Line Transform 1 #10240|
|94|Sirius Split Line Transform 2 #10240|
|95|Sirius Split Line #10250|
|96|Sirius Split Line Transform 1 #10250|
|97|Sirius Split Line Transform 2 #10250|
|98|Sirius Split Line #10260|
|99|Sirius Split Line Transform 1 #10260|
|100|Sirius Split Line Transform 2 #10260|
|101|Sirius Split Line #1030|
|102|Sirius Split Line Transform 1 #1030|
|103|Sirius Split Line Transform 2 #1030|
|104|Sirius Split Line #10320|
|105|Sirius Split Line Transform 1 #10320|
|106|Sirius Split Line Transform 2 #10320|
|107|Sirius Split Line #10321_1|
|108|Sirius Split Line Transform 1 #10321_1|
|109|Sirius Split Line Transform 2 #10321_1|
|110|Sirius Split Line #10321_2|
|111|Sirius Split Line Transform 1 #10321_2|
|112|Sirius Split Line Transform 2 #10321_2|
|113|Sirius Split Line #10321_3|
|114|Sirius Split Line Transform 1 #10321_3|
|115|Sirius Split Line Transform 2 #10321_3|
|116|Sirius Split Line #10321_4|
|117|Sirius Split Line Transform 1 #10321_4|
|118|Sirius Split Line Transform 2 #10321_4|
|119|Sirius Split Line #10321_5|
|120|Sirius Split Line Transform 1 #10321_5|
|121|Sirius Split Line Transform 2 #10321_5|
|122|Sirius Split Line #10322_1|
|123|Sirius Split Line Transform 1 #10322_1|
|124|Sirius Split Line Transform 2 #10322_1|
|125|Sirius Split Line #10322_2|
|126|Sirius Split Line Transform 1 #10322_2|
|127|Sirius Split Line Transform 2 #10322_2|
|128|Sirius Split Line #10322_3|
|129|Sirius Split Line Transform 1 #10322_3|
|130|Sirius Split Line Transform 2 #10322_3|
|131|Sirius Split Line #10322_4|
|132|Sirius Split Line Transform 1 #10322_4|
|133|Sirius Split Line Transform 2 #10322_4|
|134|Sirius Split Line #10322_5|
|135|Sirius Split Line Transform 1 #10322_5|
|136|Sirius Split Line Transform 2 #10322_5|
|137|Sirius Split Line #10323_1|
|138|Sirius Split Line Transform 1 #10323_1|
|139|Sirius Split Line Transform 2 #10323_1|
|140|Sirius Split Line #10323_2|
|141|Sirius Split Line Transform 1 #10323_2|
|142|Sirius Split Line Transform 2 #10323_2|
|143|Sirius Split Line #10323_3|
|144|Sirius Split Line Transform 1 #10323_3|
|145|Sirius Split Line Transform 2 #10323_3|
|146|Sirius Split Line #10323_4|
|147|Sirius Split Line Transform 1 #10323_4|
|148|Sirius Split Line Transform 2 #10323_4|
|149|Sirius Split Line #10323_5|
|150|Sirius Split Line Transform 1 #10323_5|
|151|Sirius Split Line Transform 2 #10323_5|
|152|Sirius Split Line #10324_1|
|153|Sirius Split Line Transform 1 #10324_1|
|154|Sirius Split Line Transform 2 #10324_1|
|155|Sirius Split Line #10324_2|
|156|Sirius Split Line Transform 1 #10324_2|
|157|Sirius Split Line Transform 2 #10324_2|
|158|Sirius Split Line #10324_3|
|159|Sirius Split Line Transform 1 #10324_3|
|160|Sirius Split Line Transform 2 #10324_3|
|161|Sirius Split Line #10325_1|
|162|Sirius Split Line Transform 1 #10325_1|
|163|Sirius Split Line Transform 2 #10325_1|
|164|Sirius Split Line #10325_2|
|165|Sirius Split Line Transform 1 #10325_2|
|166|Sirius Split Line Transform 2 #10325_2|
|167|Sirius Split Line #10325_3|
|168|Sirius Split Line Transform 1 #10325_3|
|169|Sirius Split Line Transform 2 #10325_3|
|170|Sirius Split Line #10325_4|
|171|Sirius Split Line Transform 1 #10325_4|
|172|Sirius Split Line Transform 2 #10325_4|
|173|Sirius Split Line #10326_1|
|174|Sirius Split Line Transform 1 #10326_1|
|175|Sirius Split Line Transform 2 #10326_1|
|176|Sirius Split Line #10326_2|
|177|Sirius Split Line Transform 1 #10326_2|
|178|Sirius Split Line Transform 2 #10326_2|
|179|Sirius Split Line #10326_3|
|180|Sirius Split Line Transform 1 #10326_3|
|181|Sirius Split Line Transform 2 #10326_3|
|182|Sirius Split Line #10326_4|
|183|Sirius Split Line Transform 1 #10326_4|
|184|Sirius Split Line Transform 2 #10326_4|
|185|Sirius Split Line #10326_5|
|186|Sirius Split Line Transform 1 #10326_5|
|187|Sirius Split Line Transform 2 #10326_5|
|188|Sirius Split Line #10327_1|
|189|Sirius Split Line Transform 1 #10327_1|
|190|Sirius Split Line Transform 2 #10327_1|
|191|Sirius Split Line #10327_2|
|192|Sirius Split Line Transform 1 #10327_2|
|193|Sirius Split Line Transform 2 #10327_2|
|194|Sirius Split Line #10327_3|
|195|Sirius Split Line Transform 1 #10327_3|
|196|Sirius Split Line Transform 2 #10327_3|
|197|Sirius Split Line #10327_4|
|198|Sirius Split Line Transform 1 #10327_4|
|199|Sirius Split Line Transform 2 #10327_4|
|200|Sirius Split Line #10328_1|
|201|Sirius Split Line Transform 1 #10328_1|
|202|Sirius Split Line Transform 2 #10328_1|
|203|Sirius Split Line #10328_2|
|204|Sirius Split Line Transform 1 #10328_2|
|205|Sirius Split Line Transform 2 #10328_2|
|206|Sirius Split Line #10328_3|
|207|Sirius Split Line Transform 1 #10328_3|
|208|Sirius Split Line Transform 2 #10328_3|
|209|Sirius Split Line #10328_4|
|210|Sirius Split Line Transform 1 #10328_4|
|211|Sirius Split Line Transform 2 #10328_4|
|212|Sirius Split Line #10328_5|
|213|Sirius Split Line Transform 1 #10328_5|
|214|Sirius Split Line Transform 2 #10328_5|
|215|Sirius Split Line #10340|
|216|Sirius Split Line Transform 1 #10340|
|217|Sirius Split Line Transform 2 #10340|
|218|Sirius Split Line #10350|
|219|Sirius Split Line Transform 1 #10350|
|220|Sirius Split Line Transform 2 #10350|
|221|Sirius Split Line #10360|
|222|Sirius Split Line Transform 1 #10360|
|223|Sirius Split Line Transform 2 #10360|
|224|Sirius Split Line #10361|
|225|Sirius Split Line Transform 1 #10361|
|226|Sirius Split Line Transform 2 #10361|
|227|Sirius Split Line #10390|
|228|Sirius Split Line Transform 1 #10390|
|229|Sirius Split Line Transform 2 #10390|
|230|Sirius Split Line #10391|
|231|Sirius Split Line Transform 1 #10391|
|232|Sirius Split Line Transform 2 #10391|
|233|Sirius Split Line #10392|
|234|Sirius Split Line Transform 1 #10392|
|235|Sirius Split Line Transform 2 #10392|
|236|Sirius Split Line #10393|
|237|Sirius Split Line Transform 1 #10393|
|238|Sirius Split Line Transform 2 #10393|
|239|Sirius Split Line #1040|
|240|Sirius Split Line Transform 1 #1040|
|241|Sirius Split Line Transform 2 #1040|
|242|Sirius Split Line #1050|
|243|Sirius Split Line Transform 1 #1050|
|244|Sirius Split Line Transform 2 #1050|
|245|Sirius Split Line #1060a|
|246|Sirius Split Line Transform 1 #1060a|
|247|Sirius Split Line Transform 2 #1060a|
|248|Sirius Split Line #1060b|
|249|Sirius Split Line Transform 1 #1060b|
|250|Sirius Split Line Transform 2 #1060b|
|251|Sirius Split Line #2|
|252|Sirius Split Line Transform 1 #2|
|253|Sirius Split Line Transform 2 #2|
|254|Sirius Split Line #2010|
|255|Sirius Split Line Transform 1 #2010|
|256|Sirius Split Line Transform 2 #2010|
|257|Sirius Split Line #2020|
|258|Sirius Split Line Transform 1 #2020|
|259|Sirius Split Line Transform 2 #2020|
|260|Sirius Split Line #2030|
|261|Sirius Split Line Transform 1 #2030|
|262|Sirius Split Line Transform 2 #2030|
|263|Sirius Split Line #2040a|
|264|Sirius Split Line Transform 1 #2040a|
|265|Sirius Split Line Transform 2 #2040a|
|266|Sirius Split Line #2040b|
|267|Sirius Split Line Transform 1 #2040b|
|268|Sirius Split Line Transform 2 #2040b|
|269|Sirius Split Line #2050|
|270|Sirius Split Line Transform 1 #2050|
|271|Sirius Split Line Transform 2 #2050|
|272|Sirius Split Line #3|
|273|Sirius Split Line Transform 1 #3|
|274|Sirius Split Line Transform 2 #3|
|275|Sirius Split Line #3010|
|276|Sirius Split Line Transform 1 #3010|
|277|Sirius Split Line Transform 2 #3010|
|278|Sirius Split Line #3020|
|279|Sirius Split Line Transform 1 #3020|
|280|Sirius Split Line Transform 2 #3020|
|281|Sirius Split Line #3030|
|282|Sirius Split Line Transform 1 #3030|
|283|Sirius Split Line Transform 2 #3030|
|284|Sirius Split Line #3040|
|285|Sirius Split Line Transform 1 #3040|
|286|Sirius Split Line Transform 2 #3040|
|287|Sirius Split Line #3050|
|288|Sirius Split Line Transform 1 #3050|
|289|Sirius Split Line Transform 2 #3050|
|290|Sirius Split Line #4|
|291|Sirius Split Line Transform 1 #4|
|292|Sirius Split Line Transform 2 #4|
|293|Sirius Split Line #4010|
|294|Sirius Split Line Transform 1 #4010|
|295|Sirius Split Line Transform 2 #4010|
|296|Sirius Split Line #4020|
|297|Sirius Split Line Transform 1 #4020|
|298|Sirius Split Line Transform 2 #4020|
|299|Sirius Split Line #4030|
|300|Sirius Split Line Transform 1 #4030|
|301|Sirius Split Line Transform 2 #4030|
|302|Sirius Split Line #4040|
|303|Sirius Split Line Transform 1 #4040|
|304|Sirius Split Line Transform 2 #4040|
|305|Sirius Split Line #4050|
|306|Sirius Split Line Transform 1 #4050|
|307|Sirius Split Line Transform 2 #4050|
|308|Sirius Split Line #5|
|309|Sirius Split Line Transform 1 #5|
|310|Sirius Split Line Transform 2 #5|
|311|Sirius Split Line #6|
|312|Sirius Split Line Transform 1 #6|
|313|Sirius Split Line Transform 2 #6|
|314|Sirius Split Line #7|
|315|Sirius Split Line Transform 1 #7|
|316|Sirius Split Line Transform 2 #7|
|317|Sirius Split Line #8|
|318|Sirius Split Line Transform 1 #8|
|319|Sirius Split Line Transform 2 #8|
|320|Sirius Split Line #9|
|321|Sirius Split Line Transform 1 #9|
|322|Sirius Split Line Transform 2 #9|

### Effect Clips

|ID|Clip|
|:-:|:-:|
|0|Sirius Miss|
|1|Sirius Great|
|2|Sirius Flick Great|
|3|Sirius Perfect|
|4|Sirius Flick Perfect|
|5|Sirius Hold Start|
|6|Sirius Scratch|
|7|Sirius Hold|

### Particle Effects

|ID|Effect|
|:-:|:-:|
|||

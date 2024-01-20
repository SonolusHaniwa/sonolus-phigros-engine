# 编译 Skin Unpacker
g++ skin.cpp -oskin -ljsoncpp -lz -lpng
# 编译 Effect Unpacker
g++ effect.cpp -oeffect -ljsoncpp -lz -lzip
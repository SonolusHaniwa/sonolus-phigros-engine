ARCH="x64"
YELLOW="\e[93m"
RED="\e[91m"
RESET="\e[0m"

# 获取最新版本
echo -e $YELLOW"Checking latest version..."$RESET
VERSION=$(curl -s https://api.github.com/repos/SonolusHaniwa/sonolus-server-cpp/releases/latest | jq -r '.tag_name')

# 下载 Sonolus Server C++
echo -e $YELLOW"Downloading Sonolus Server $VERSION for $ARCH..."$RESET
wget https://github.com/SonolusHaniwa/sonolus-server-cpp/releases/download/$VERSION/sonolus-server-linux-$ARCH-$VERSION.zip -O sonolus-server/sonolus.zip
cd sonolus-server && unzip -n sonolus.zip

echo -e $YELLOW"Syncing data into database..."$RESET
wget "https://raw.githubusercontent.com/SonolusHaniwa/sonolus-server-cpp/main/data.sql" -O data.sql
sqlite3 sonolus.db ".read ./data.sql"

# 资源导入
./sonolus import ../phigros/phistore.scp
./sonolus import ../phigros/phigros-official.scp
./sonolus import ../phigros/phigros-2024-april.scp

./sonolus synccpp
cp plugins ../../ -r
./sonolus serve >/dev/null 2>&1 &
./sonolus buildcpp all .. -DDISABLE_TRACE
# sqlite3 sonolus.db "SELECT * FROM Engine";

# 数据导出
# ./exportor "http://127.0.0.1:8080" data.json 1
# ./exportor "http://127.0.0.1:8080" engine.json 2
sudo pkill sonolus

echo -e $YELLOW"Sync finished"$RESET

# 检查 build 目录是否存在
if [ -d "build" ]; then
    # 如果存在，清空 build 目录中的内容
    rm -rf build/*
else
    # 如果不存在，创建 build 目录
    mkdir build
fi

cd build
cmake ..
make
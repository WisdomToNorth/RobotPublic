
```bash
sudo apt clean
sudo apt update
ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && echo $CONTAINER_TIMEZONE > /etc/timezone
sudo apt update && sudo apt install -y tzdata
sudo apt install -y git
sudo apt install -y x11-apps
LD_LIBRARY_PATH=/usr/local/lib

sudo apt install -y build-essential libgl1-mesa-dev
sudo apt install -y libboost-all-dev
sudo apt install -y wget
sudo apt install -y p7zip-full p7zip-rar
sudo apt install -y libassimp-dev
sudo apt install libgtest-dev
sudo apt install -y liborocos-kdl-dev
sudo apt -y install build-essential
sudo apt -y install g++-12
sudo apt -y install tcl tcl-dev tk tk-dev libfreeimage-dev
sudo apt -y install libxmu-dev libxi-dev
sudo apt -y install libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
sudo apt -y install xvfb
sudo apt install libgtest-dev
sudo apt install -y p7zip-full p7zip-rar
sudo add-apt-repository universe
sudo apt update
sudo apt install libgoogle-glog-dev

# gtest
wget https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
tar -xvzf v1.14.0.tar.gz
cd googletest-1.14.0
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install

# ompl
mkdir opml
cd opml
wget https://ompl.kavrakilab.org/install-ompl-ubuntu.sh
chmod u+x install-ompl-ubuntu.sh
./install-ompl-ubuntu.sh
cd ..
rm -rf opml

git clone https://github.com/google/glog.git
cd glog
cmake -S . -B build -G "Unix Makefiles"
cmake --build build
sudo cmake --build build --target install
cd ..

```

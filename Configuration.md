# Config

## A Robot Framework

```bash
#need by qt@5 linux brew
sudo apt install libgl1-mesa-dev
#need by linux opengl
sudo apt install mesa-utils
#enable clang build on linux
sudo apt install libc++-dev libc++abi-dev
# enable ccache
sudo apt install ccache
sudo apt install libfontconfig1-dev
```

## Doc generate:
```bash
brew install doxygen
sudo apt install graphviz
```

## On Linux, convert all end LF to CRLF
```bash
sudo apt-get install dos2unix
find . -name "*.cpp" -type f -exec unix2dos {} \;
find . -name "*.h" -type f -exec unix2dos {} \;
```

## Codespace settings
run in terminal:
```bash
sudo apt update && sudo apt install build-essential curl file git mesa-common-dev ccache vim
#install homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
(echo; echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"') >> /home/vscode/.bashrc
(echo; echo 'export PATH="/home/linuxbrew/.linuxbrew/bin:$PATH"') >> /home/vscode/.bashrc
(echo; echo 'export PATH="/home/linuxbrew/.linuxbrew/opt/qt@5/bin:$PATH"') >> /home/vscode/.bashrc
source /home/vscode/.bashrc
brew install qt@5
brew install opencascade
brew install orocos-kdl
brew install assimp
brew install googletest
brew install fmt
brew install eigen
brew install ompl
brew install ninja
brew install cmake
brew install spdlog
git config --global core.editor vim

#Gui
sudo apt-get install libxcb-xinerama0
sudo apt-get install tightvncserver
vncserver :1
export DISPLAY=:1
/workspaces/RobotFramework/build/gcc_linux_release/executable/RobotGui
```

Set up CMake Tools
```json
    "terminal.integrated.profiles.linux": {
        "bash": {
            "path": "/usr/bin/bash"
        }
    },
    "terminal.integrated.env.linux": {
        "PATH": "/home/linuxbrew/.linuxbrew/bin:${env:PATH}",
        "OTHER_ENV_VARIABLE": "value"
    },
    "cmake.cmakePath": "/home/linuxbrew/.linuxbrew/bin/cmake",
    "cmake.configureOnOpen": false,
    "cmake.environment": {
        "PATH": "/home/linuxbrew/.linuxbrew/opt/qt@5/bin:/home/linuxbrew/.linuxbrew/bin:${env:PATH}",
    },
    "cmake.options.statusBarVisibility": "compact",
    "cmake.debugConfig": {
        "args": [
            "--gtest_filter=*"
        ]
    }
```


# ROBOT ROS2 SYSTEM

## 安装和运行

### software
* ubuntu22.04
* vscode(cmake\git\c++\gdb\Remote Development等插件)
* git

### ubuntu
* 更新为阿里云软件源
* 安装基本工具
  ```
  # 更新
  sudo apt update
  sudo apt upgrade

  # 复制拷贝
  sudo apt-get autoremove open-vm-tools
  sudo apt-get install -y open-vm-tool open-vm-tools-desktop
  reboot

  # 语言配置
  sudo apt-get install -y locales
  sudo apt-get install -y language-pack-en
  sudo locale-gen en_US en_US.UTF-8
  sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
  export LANG=en_US.UTF-8
  locale

  # 基本工具
  sudo apt-get install -y apt-utils
  sudo apt-get install -y dialog
  sudo apt-get install -y vim
  sudo apt-get install -y systemctl
  sudo apt-get install -y net-tools
  sudo apt-get install -y iputils-ping
  sudo apt-get install -y build-essential
  sudo apt-get install -y gdb git cmake
  sudo apt-get install -y libssl-dev
  sudo apt-get install -y bsdmainutils
  sudo apt-get install -y ntp
  sudo apt-get install -y openssh-server
  sudo echo "Port 22">>/etc/ssh/sshd_config
  sudo echo "PermitRootLogin yes">>/etc/ssh/sshd_config
  sudo systemctl enable ssh
  ```

* 安装c++/python开发环境
  ```
  # 安装c++环境
  sudo apt-get install -y libspdlog-dev
  sudo apt-get install -y libboost-all-dev

  # 安装python环境
  sudo apt-get install -y python3-dev python3-pip
  sudo ln -s /usr/bin/python3 /usr/bin/python
  ```

* 安装ros2环境
  ```
  # First ensure that the Ubuntu Universe repository is enabled.
  sudo apt install software-properties-common
  sudo add-apt-repository universe

  # Now add the ROS 2 GPG key with apt.
  sudo apt update && sudo apt install curl -y
  sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
  
  # Then add the repository to your sources list.
  echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

  # Desktop Install 
  sudo apt update && sudo apt upgrade
  sudo apt install -y ros-humble-desktop-full
  sudo apt install -y ros-dev-tools

  # Sourcing the setup script
  source /opt/ros/humble/setup.bash
  echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
  ```

## 获取源码
* git克隆源码
  ```
  git clone
  ```

* 虚拟机ubuntu构建工程目录
  ```
  mkdir /home/lyb/Robot_ROS2_System
  ```

* 主机vscode下载sftp插件，编写.vscode/sftp.json配置，上传代码
  ```
  {
      "name": "My Server",
      "host": "192.168.5.129",
      "protocol": "sftp",
      "port": 22,
      "username": "lyb",
      "remotePath": "/home/lyb/Robot_ROS2_System",
      "uploadOnSave": false,
      "useTempFile": false,
      "openSsh": false,
      "ignore": [ 
          "**/.vscode/**"
      ]
  }
  ```

## 工程搭建
* 主机使用vscode的ssh服务访问虚拟机工程
* 安装插件：c++/cmake/IntelliCode/Msg Language Support
* 创建功能包
  ```
  ros2 pkg create <package-name> --build-type {cmake,ament_cmake,ament_python} --dependencies <依赖名字>
  ```
* 编写cpp/hpp/CMakeLists.txt文件
* 编译和配置环境变量
  ```
  colcon build
  source install/setup.bash
  ```
* 常用命令
  ```
  # 节点
  ros2 run demo_node node_01
  ros2 node list
  ros2 node info <node_name>
  # 话题
  ros2 topic list
  # 接口
  ros2 interface show <pkg_path>/<if_name>
  # 服务
  ros2 service list
  ros2 service type <service_name>
  ros2 service call <service_name> <service_type> <service_data>
  # 动作
  ros2 action list                                                # 查看服务列表
  ros2 action info <action_name>                                  # 查看服务数据类型
  ros2 action send_goal <action_name> <action_type> <action_data> # 发送服务请求
  ```
## 工程目录

```
Robot_ROS2_System
├─ 📁.vscode
│  ├─ 📄c_cpp_properties.json
│  └─ 📄settings.json
├─ 📁app
│  ├─ 📁demo_node
│  │  ├─ 📁src
│  │  │  └─ 📄node_01.cpp
│  │  ├─ 📄CMakeLists.txt
│  │  └─ 📄package.xml
│  └─ 📁demo_topic
│     ├─ 📁src
│     │  ├─ 📄publisher.cpp
│     │  └─ 📄subscriber.cpp
│     ├─ 📄CMakeLists.txt
│     └─ 📄package.xml
├─ 📁scripts
│  └─ 📄compile.sh
├─ 📁tutorial
│  └─ 📄c++ ros2.pdf
├─ 📄.gitignore
├─ 📄LICENSE
├─ 📄README.md
└─ 📄Robot_ROS2_System.code-workspace
```
# detectLaneAdvanced
## Installation

* Installation ROS
```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu xenial main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
```
`sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu xenial main" > /etc/apt/sources.list.d/ros-latest.list`
```
sudo apt-get install ros-kinetic-rosbridge-server
or sudo apt-get install ros-melodic-rosbridge-server
cd home
mkdir catkin_ws
cd catkin_ws
mkdir src
catkin_make
r copy 3 file của thằng fpt vô thư mục src

edit  #INCLUDE_DIRS include in /catkin_ws/src/mastering_ros_demo_pkg

chạy lại catkin_make
echo "source ~/catkin_ws/devel/stuo.bash" >> ~/.bashrc
echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc
source ~/.bashrc
roslaunch lane_detect lane_detect.launch
```

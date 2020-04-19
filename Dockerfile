FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y \
&& apt-get install -y build-essential git \
&& apt-get install -y sudo wget nano \
&& mkdir $HOME/myfreeflyer \
&& export SOURCE_PATH=$HOME/myfreeflyer \
&& git clone https://github.com/nasa/astrobee.git $SOURCE_PATH \
&& cd $SOURCE_PATH/scripts/setup \
&& ./add_ros_repository.sh \
&& sed -i 's/main/xenial main/g' /etc/apt/sources.list.d/gazebo-stable.list \
&& sed -i 's/main/xenial main/g' /etc/apt/sources.list.d/ros-latest.list \
&& apt-get update \
&& cd debians \
&& ./build_install_debians.sh \
&& cd ../ \
&& ./install_desktop_16_04_packages.sh \
&& sudo rosdep init \
&& rosdep update \
&& cd $SOURCE_PATH

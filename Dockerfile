FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y \
&& apt-get install -y build-essential git sudo wget nano

RUN mkdir $HOME/myfreeflyer

WORKDIR $HOME/myfreeflyer

ENV SOURCE_PATH $HOME/myfreeflyer \

# Get Astrobee
RUN git clone https://github.com/nasa/astrobee.git

# get ROS
&& sed -i 's/main/xenial main/g' /etc/apt/sources.list.d/gazebo-stable.list \
&& sed -i 's/main/xenial main/g' /etc/apt/sources.list.d/ros-latest.list \
&& apt-get update \
&& cd debians \
&& ./build_install_debians.sh \
&& cd ../ \
&& ./install_desktop_16_04_packages.sh \
&& sudo rosdep init \
&& rosdep update \
&& cd $SOURCE_PATH \
&& export BUILD_PATH=$HOME/freeflyer_build/native \
&& export INSTALL_PATH=$HOME/freeflyer_install/native \
&& ./scripts/configure.sh -l -F -D \
&& cd $BUILD_PATH \
&& make -j8 \
&& cd $SOURCE_PATH

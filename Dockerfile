FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y \
&& apt-get install -y build-essential git sudo wget nano

RUN mkdir $HOME/myfreeflyer

WORKDIR $HOME/myfreeflyer

ENV SOURCE_PATH $HOME/myfreeflyer \

# Get Astrobee
RUN git clone https://github.com/nasa/astrobee.git

# update apt lists and install ROS
RUN ./scripts/setup/add_ros_repository.sh \
&& sed -i 's/main/xenial main/g' /etc/apt/sources.list.d/gazebo-stable.list \
&& sed -i 's/main/xenial main/g' /etc/apt/sources.list.d/ros-latest.list \
# do we need an upgrade after update? is that in a script?
&& apt-get update 

# Not yet sure what these do (see just above)
RUN ./debians/build_install_debians.sh \
&& ./install_desktop_16_04_packages.sh

# Update ROS
RUN sudo rosdep init \
&& rosdep update \
&& ./scripts/configure.sh -l -F -D

RUN cd $HOME/freeflyer_build/native \
&& make -j8

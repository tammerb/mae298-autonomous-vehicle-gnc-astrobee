FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

# apt-get upgrade isn't great practice in a dockerfile because it's bloating, but we'll keep it for now.
RUN apt-get update && apt-get upgrade -y \

# Put this on a seperate line so we can install things we miss without having to re-do everything
RUN apt-get update \
&& apt-get install -y build-essential git sudo wget nano lsb-release

WORKDIR $HOME/myfreeflyer

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

# Update ROS, get ready for build
RUN sudo rosdep init \
&& rosdep update \
&& ./scripts/configure.sh -l -F -D

# Finally build!
RUN cd $HOME/freeflyer_build/native \
&& make -j$((`nproc`+1))

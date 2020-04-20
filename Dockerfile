FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

# apt-get upgrade isn't great practice in a dockerfile because it's bloating, but we'll keep it for now.
RUN apt-get update && apt-get upgrade -y

# Put this on a seperate line so we can install things we miss without having to re-do everything
RUN apt-get update \
&& apt-get install -y build-essential git sudo wget nano lsb-release

# Get Astrobee
RUN git clone https://github.com/nasa/astrobee.git

WORKDIR $HOME/astrobee

# update apt lists and install ROS
RUN ./scripts/setup/add_ros_repository.sh

# You have to be in the directory to run this script because a variable assigns `pwd`...disgusting
RUN apt-get update \
&& cd scripts/setup/debians \
&& ./build_install_debians.sh

RUN apt-get update \
&& ./scripts/setup/install_desktop_16_04_packages.sh

# Update ROS, get ready for build
RUN sudo rosdep init \
&& rosdep update \
&& ./scripts/configure.sh -l -F -D

# Finally build!
RUN cd $HOME/freeflyer_build/native \
&& make -j$((`nproc`+1))

RUN echo "source /root/freeflyer_build/native/devel/setup.bash" >> /root/.bashrc
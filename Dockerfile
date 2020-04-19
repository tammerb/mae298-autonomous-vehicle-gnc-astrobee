FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y \
&& apt-get install -y build-essential git \
&& apt-get install -y sudo wget nano \
&& mkdir $HOME/myfreeflyer

WORKDIR $HOME/myfreeflyer
RUN export SOURCE_PATH=$HOME/myfreeflyer \
&& git clone https://github.com/nasa/astrobee.git $SOURCE_PATH

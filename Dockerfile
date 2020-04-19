FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y \
&& apt-get install -y build-essential git \
&& apt-get install -y sudo wget nano \

WORKDIR $HOME/freeflyer
RUN export SOURCE_PATH=$HOME/freeflyer \
&& git clone https://github.com/nasa/astrobee.git $SOURCE_PATH

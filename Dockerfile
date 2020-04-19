FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y && apt-get install -y build-essential git && apt-get install -y sudo wget nano && mkdir freeflyer

WORKDIR /freeflyer
RUN touch astrtest

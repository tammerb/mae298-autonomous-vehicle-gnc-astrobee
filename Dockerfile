FROM ubuntu:xenial
MAINTAINER Tammer Barkouki (thbarkouki@ucdavis.edu)

RUN apt-get update && apt-get upgrade -y && touch "test.txt"

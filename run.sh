#!/bin/bash

docker-compose up -d
docker exec -d astrobee_sim_container $HOME/freeflyer_build/native $$ source devel/setup.bash && roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true

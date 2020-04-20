#!/bin/bash

docker-compose up -d
docker exec -d astrobee_sim_container source /root/freeflyer_build/native/devel/setup.bash
docker exec -d astrobee_sim_container roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true

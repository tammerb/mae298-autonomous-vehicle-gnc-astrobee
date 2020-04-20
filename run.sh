#!/bin/bash

docker-compose up -d
docker exec -d astrobee_sim_container sh -c "source /root/freeflyer_build/native/devel/setup.bash \
&& roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true"

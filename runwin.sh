#!/bin/bash

docker container stop $(docker container ls -aq)

docker-compose up -d
winpty docker exec -d astrobee_sim_container //bin//bash -c "source /root/freeflyer_build/native/devel/setup.bash \
&& roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true"
echo "Standby for browser interface..."
sleep 3
echo "Opening browser interface"
start http://localhost:8080/vnc_auto.html &

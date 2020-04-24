#!/bin/bash

docker container stop $(docker container ls -aq)

docker-compose up -d
docker exec -d astrobee_sim_container /bin/bash -c "source /root/freeflyer_build/native/devel/setup.bash \
&& sed -i 's/nav_cam_rate = 0.0/nav_cam_rate = 1.0/' /root/myfreeflyer/astrobee/config/simulation/simulation.config \
&& sed -i 's/dock_cam_rate = 0.0/doc_cam_rate = 1.0/' /root/myfreeflyer/astrobee/config/simulation/simulation.config \
&& roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true" || winpty docker exec -d astrobee_sim_container //bin//bash -c "source /root/freeflyer_build/native/devel/setup.bash \
&& roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true"
echo "Opening browser interface..."
sleep 3
xdg-open http://localhost:8080/vnc_auto.html &
open http://localhost:8080/vnc_auto.html &
start http://localhost:8080/vnc_auto.html &

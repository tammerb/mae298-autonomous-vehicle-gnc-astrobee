#!/bin/bash
CAM_CONFIG=$HOME/../astrobee/astrobee/config/simulation/simulation.config
SETUP=$HOME/freeflyer_build/native/devel/setup.bash

docker container stop $(docker container ls -aq)

docker-compose up -d

for arg in "$@"
do
  case $arg in
    -c)
    docker exec -d astrobee_sim_container /bin/bash -c "sed -i 's/nav_cam_rate = 0.0/nav_cam_rate = 1.0/' $CAM_CONFIG \
    && sed -i 's/dock_cam_rate = 0.0/dock_cam_rate = 1.0/' $CAM_CONFIG"
    echo "Nav and Dock cams will be on."
    ;;
    *)
    echo "Incorrect flag, use '-c' flag to turn cameras on."
    ;;
  esac
done

docker exec -d astrobee_sim_container /bin/bash -c "source $SETUP \
&& roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true" \
|| winpty docker exec -d astrobee_sim_container //bin//bash -c "source SETUP \
&& roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true"

echo "Opening browser interface..."

sleep 2

xdg-open http://localhost:8080/vnc_auto.html \
|| start http://localhost:8080/vnc_auto.html \
|| open http://localhost:8080/vnc_auto.html

#!/bin/bash
CAM_CONFIG=/astrobee/astrobee/config/simulation/simulation.config
SETUP=/root/freeflyer_build/native/devel/setup.bash
LAUNCH="roslaunch astrobee sim.launch dds:=false robot:=sim_pub rviz:=true"
RVIZ=TRUE

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
    -s)
    LAUNCH="roslaunch astrobee sim.launch dds:=false robot:=sim_pub sviz:=true"
    RVIZ=FALSE
    echo "Launching with the gazebo GUI."
    ;;
    -g)
    LAUNCH="roslaunch astrobee sim.launch dds:=false robot:=sim_pub gviz:=true"
    RVIZ=FALSE
    echo "Launching with the GNC GUI."
  esac
done

if [ "$RVIZ" = TRUE ] ; then
    echo "Launching with the Rviz GUI."
fi

docker exec -d astrobee_sim_container /bin/bash -c "source $SETUP \
&& $LAUNCH" \
|| winpty docker exec -d astrobee_sim_container //bin//bash -c "source $SETUP \
&& $LAUNCH"

echo "Opening browser interface..."

sleep 2

xdg-open http://localhost:8080/vnc_auto.html \
|| start http://localhost:8080/vnc_auto.html \
|| open http://localhost:8080/vnc_auto.html

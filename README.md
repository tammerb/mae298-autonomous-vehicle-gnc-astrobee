# ros-astrobee
#### A Docker Image and run scripts for NASA's Astrobee Robot Software
(Astrobee source code available at https://github.com/nasa/astrobee/tree/972b78b6651e87a634f7ae99a3b12aea860053f1)

## Instructions to get started with a prebuilt Astrobee sim:
 
Install docker for your OS and check with `docker version`
*(If not running in Linux, start your Docker desktop.)*

Clone this repo and `cd` into it:
```
git clone https://github.com/tammerb/ros-astrobee.git
cd ros-astrobee/
```

### Pull the Docker image
Pull the pre-built Docker image (*note: the compressed image is ~1.5GB and 6GB once extracted*):
```
docker pull thbarkouki/ros-astrobee:latest
```

Check that the pull was successful with the `docker images` command.

### Run the Simulator:
Run the simulator with by executing the `run.sh` script, e.g with:
```
./run.sh [OPTIONS]
```
(maybe require `sudo`)

The NavCam and DockCam views are off by default. Add the `-c` flag to turn them on for debugging (resource intensive).
The default viewer is Rviz. Use the `-s` flag for Gazebo, or `-g` flag for GNC GUI.
See details here:
https://github.com/nasa/astrobee/tree/972b78b6651e87a634f7ae99a3b12aea860053f1/simulation#running-the-simulator

A browser tab should automatically open at http://localhost:8080/vnc_auto.html. If not, manually enter this address into your browser.
See below for **Flying Astrobee** instructions.

Once you're done, close the browsers tab, exit the terminal you're controlling the Astrobee from with `exit`, and stop and remove both containers with `docker-compose down` in the original terminal you started in.

### Interacting with Astrobee
In a new terminal from the one you launched the sim from, enter (aka attach to) the docker container and source the ros commands with:
```
docker attach astrobee_sim_container
source /root/freeflyer_build/native/devel/setup.bash
```

Now you should be able to "tele-operate" the astrobee using the teleop instructions available from the original Astrobee repo here:
https://github.com/nasa/astrobee/blob/972b78b6651e87a634f7ae99a3b12aea860053f1/management/executive/teleop_tool.md

To launch additional bash sessions attached to the same container, from a different terminal execute:
```
docker exec -it astrobee_sim_container bash
```

#### *For testing only:* This will start the containers (and install the image if needed), but not launch the sim:
Run `run_no_launch.sh`. This will start both the astrobee and noVNC containers, but will not launch the sim. You can open a second terminal and attach to the sim container by running `docker attach astrobee_sim_container`. You can then edit and launch the sim from inside. Exit the container with `exit`, and stop and remove both containers with `docker-compose down`.

## Instructions to build an astrobee simulator:
*coming soon*

# ros-astrobee

### Instructions to get started with a prebuilt astrobee sim:
 
Install docker for your OS and check with `docker version`

Clone this repo and `cd` into it:
`git clone https://github.com/tammerb/ros-astrobee.git`
` `
`cd ros-astrobee/`

Pull the pre-built *astrobee* Docker image (*note: the image is 1.7GB*):
`docker pull thbarkouki/astrobee:built`

Check that the pull was successful with the `docker images` command.

Run the simulator with `source run.sh` and then access via http://localhost:8080/vnc_auto.html in a web browser.

### Instructions to build an astrobee simulator:
*coming soon*

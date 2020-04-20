# ros-astrobee

### Instructions just to get started with a prebuilt astrobee sim:
 
Install docker for your OS and check with `docker version`

Clone this repo and `cd` into it:
`git clone https://github.com/tammerb/ros-astrobee.git`
` `
`cd ros-astrobee/`

Pull the pre-built *astrobee* Docker image:
`docker pull thbarkouki/astrobee:built`

and check with `docker images`

Run the simulator with `source run.sh` and then access via http://localhost:8080/vnc_auto.html in a web browser.

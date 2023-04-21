#!/bin/bash

TMUX_NAME=tmux_behavior
DOCKER_CONTAINER_NAME=ergocub_behavior_container

echo "Start this script inside the ergoCub behavior tree rooot folder"
usage() { echo "Usage: $0 [-i ip_address] [-n nameserver]" 1>&2; exit 1; }

while getopts i:hbn: flag
do
    case "${flag}" in
        i) SERVER_IP=${OPTARG};;
        n) YARP_NAMESERVER=${OPTARG};;
        b) JUST_BASH='1';;
        h) usage;;
        *) usage;;
    esac
done

# Start the container with the right options
docker run -itd --rm --network=host --ipc=host \
  --env DISPLAY=:0 --env QT_X11_NO_MITSHM=1 --env XDG_RUNTIME_DIR=/root/1000 --env XAUTHORITY=/home/btuc/.Xauthority \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw -v "${XAUTHORITY}":/home/btuc/.Xauthority:rw \
  -v "$(pwd)":/home/btuc/ergocub-behavior \
  --name $DOCKER_CONTAINER_NAME \
  ar0s/bt-assignment:idyntree bash

# Create tmux session
tmux new-session -d -s $TMUX_NAME
tmux set -t $TMUX_NAME -g mouse on

# Just bash?
if [ -n "$JUST_BASH" ] # Variable is non-null
then
  tmux send-keys -t $TMUX_NAME "docker exec -it $DOCKER_CONTAINER_NAME bash" Enter
  tmux a -t $TMUX_NAME
  exit 0
fi

# Set server
tmux send-keys -t $TMUX_NAME "docker exec -it $DOCKER_CONTAINER_NAME bash" Enter

if [ -n "$YARP_NAMESERVER" ] # Variable is non-null
then
  tmux send-keys -t $TMUX_NAME "yarp namespace $YARP_NAMESERVER" Enter
fi

if [ -n "$SERVER_IP" ] # Variable is non-null
then
  tmux send-keys -t $TMUX_NAME "yarp conf $SERVER_IP 10000" Enter
else
  tmux send-keys -t $TMUX_NAME "yarp detect --write" Enter
fi

# Behavior Tree
  tmux send-keys -t $TMUX_NAME "docker exec -it $DOCKER_CONTAINER_NAME bash" Enter
  tmux send-keys -t $TMUX_NAME "/home/btuc/ergocub-behavior/build/bin/run_bt" Enter
tmux select-pane -t $TMUX_NAME:0.0
tmux split-window -h -t $TMUX_NAME

# Bash for fun
tmux send-keys -t $TMUX_NAME "docker exec -it $DOCKER_CONTAINER_NAME bash" Enter
tmux send-keys -t $TMUX_NAME "source /ros_entrypoint.sh" Enter
tmux send-keys -t $TMUX_NAME "ros2 run groot Groot --mode monitor" Enter

# Attach
tmux a -t $TMUX_NAME

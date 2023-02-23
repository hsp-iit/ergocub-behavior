#!/bin/bash

TMUX_NAME=tmux_behavior
DOCKER_CONTAINER_NAME=ergocub_behavior_container

docker rm -f $DOCKER_CONTAINER_NAME
tmux kill-session -t $TMUX_NAME
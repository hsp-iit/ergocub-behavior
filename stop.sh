#!/bin/bash

TMUX_NAME=tmux_behavior
DOCKER_IMAGE_NAME=ergocub_behavior_container

docker rm -f $DOCKER_IMAGE_NAME
tmux kill-session -t $TMUX_NAME

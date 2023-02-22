#!/bin/bash

docker rm -f ergocub_behavior_container
tmux kill-session -t behavior-tmux

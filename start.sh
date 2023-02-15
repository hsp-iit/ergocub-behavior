#!/usr/bin/bash
conda_bin="/home/sberti/miniconda3/envs/ecub/bin/"

# Stop all existing docker containers and remove them
docker stop ergocub_container && docker rm ergocub_container

# Start the container with the right options
docker run --gpus=all -v $(pwd):/home/ecub -itd --rm --network host --name ergocub_container andrewr96/ecub-env:yarp bash

# Create tmux session
tmux new-session -d -s ecub-tmux

# Souce

# 0
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "/home/btuc/bt-assignment/build/bin/action_recognition" Enter
tmux split-window -v -t ecub-tmux

# 2
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "/home/btuc/bt-assignment/build/bin/object_detection" Enter
tmux split-window -v -t ecub-tmux


tmux a -t ecub-tmux
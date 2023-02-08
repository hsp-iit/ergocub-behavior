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
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && ${conda_bin}python scripts/source.py" Enter
tmux split-window -v -t ecub-tmux

# 2
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && python scripts/action_recognition_pipeline.py" Enter
tmux split-window -v -t ecub-tmux

#4
tmux send-keys -t ecub-tmux "${conda_bin}yarpserver --write" Enter

# 5
tmux split-window -h -t ecub-tmux
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && python scripts/manager.py" Enter

tmux split-window -h -t ecub-tmux

# 6
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && python scripts/action_recognition_rpc.py" Enter
tmux split-window -h -t ecub-tmux
#7
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && python scripts/object_detection_rpc.py" Enter
tmux split-window -h -t ecub-tmux
#8
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "components/action_recognition/build/bin/action_recognition" Enter

tmux split-window -h -t ecub-tmux
# 9
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "components/object_detection/build/bin/object_detection" Enter

tmux select-pane -t ecub-tmux:0.0
tmux split-window -h -t ecub-tmux
# 1
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && ${conda_bin}python scripts/sink.py" Enter

tmux select-pane -t ecub-tmux:0.2
tmux split-window -h -t ecub-tmux
#3
tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && python scripts/grasping_pipeline.py" Enter

#
#tmux split-window -v -p 66 -t ecub-tmux

#
#tmux split-window -v -p 66 -t ecub-tmux
#tmux send-keys -t ecub-tmux "cd ErgoCub-Visual-Perception && {$conda_python} scripts/sink.py" Enter
#
#tmux select-pane -t ecub-tmux:0.0
#tmux split-window -v -p 66 -t ecub-tmux
#tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
#
#tmux split-window -v -p 66 -t ecub-tmux
#tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter
#
#tmux split-window -v -p 66 -t ecub-tmux
#tmux send-keys -t ecub-tmux "docker exec -it ergocub_container bash" Enter

tmux a -t ecub-tmux
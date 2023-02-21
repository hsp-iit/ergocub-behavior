#!/bin/bash

docker run -itd --rm --network=host --ipc=host \
  --env DISPLAY=:0 --env QT_X11_NO_MITSHM=1 --env XDG_RUNTIME_DIR=/root/1000 --env XAUTHORITY=/home/btuc/.Xauthority \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw -v ${XAUTHORITY}:/home/btuc/.Xauthority:rw \
  -v /home/aros/projects/ergocub/ergocub-behavior:/home/btuc/ergocub-behavior \
  --name ergocub_behavior_container \
  ar0s/bt-assignment bash

# Create tmux session
tmux new-session -d -s behavior-tmux

# 0
  tmux send-keys -t behavior-tmux "docker exec -it ergocub_behavior_container bash" Enter
  tmux send-keys -t behavior-tmux "yarp conf 10.0.0.150 10000" Enter
  tmux send-keys -t behavior-tmux "yarp detect --write" Enter
  tmux send-keys -t behavior-tmux "/home/btuc/ergocub-behavior/build/bin/object_detection" Enter

tmux split-window -h -t behavior-tmux

# 2
  tmux send-keys -t behavior-tmux "docker exec -it ergocub_behavior_container bash" Enter
  tmux send-keys -t behavior-tmux "sleep 5" Enter
  tmux send-keys -t behavior-tmux "/home/btuc/ergocub-behavior/build/bin/run_bt" Enter

tmux select-pane -t behavior-tmux:0.0
tmux split-window -v -t behavior-tmux

#
  tmux send-keys -t behavior-tmux "docker exec -it ergocub_behavior_container bash" Enter

#  tmux send-keys -t behavior-tmux "sleep 5" Enter
#  tmux send-keys -t behavior-tmux "./grasp-demo /robotology-superbuild/build/install/share/iCub/robots/iCubGazeboV2_7/model.urdf" Enter
#
#tmux select-pane -t behavior-tmux:0.2
#tmux split-window -v -t behavior-tmux
#
#tmux send-keys -t behavior-tmux "docker exec -it ergocub_manipulation_container bash" Enter

tmux a -t behavior-tmux
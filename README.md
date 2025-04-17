# Ergocub Behavior Tree

Behavior tree for running the demo with the action recognition [ergocub-perception]()

## How to install
Requisite: you need to have already installed YARP.
Note: set `-DCMAKE_INSTALL_PREFIX=/path/to/install` to your designed install path.
On the robot it is $ROBOTOLOGY_SUPERBUILD_INSTALL_DIR.

Install the rpc-interfaces for the comunication between YARP modules and the BT:
```
git clone https://github.com/hsp-iit/ergocub-rpc-interfaces && cd ergocub-rpc-interfaces/ecub_perception/cpp_library && mkdir build &&\
    cd build && cmake -DCMAKE_INSTALL_PREFIX=/path/to/install .. && sudo make install -j4 && cd &&\
    cd ergocub-rpc-interfaces/ecub_gaze_controller/cpp_library && mkdir build &&\
    cd build && cmake -DCMAKE_INSTALL_PREFIX=/path/to/install .. && sudo make install -j4
```
Install the [BehaviorTree.CPP](https://github.com/BehaviorTree/BehaviorTree.CPP.git) dep to the designed commit (we don´t support the BTv4)
```
git clone https://github.com/BehaviorTree/BehaviorTree.CPP.git && cd BehaviorTree.CPP && git checkout 61c55ed &&\
    mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=/path/to/install .. && sudo make install -j8
```
Then compile this repo:
```
git clone https://github.com/hsp-iit/ergocub-behavior && cd ergocub-behavior && mkdir build && cd build && cmake .. && make -j4
```

## How to Run
1) Run the needed modules: [ergocub-perception](https://github.com/hsp-iit/ergocub-perception), [ergocub-gaze-controller](https://github.com/hsp-iit/ergocub-gaze-control), [ergocub-bimanual](https://github.com/hsp-iit/ergocub-bimanual).
2) Execute the binary in `/ergocub-behavior/build/bin/run_bt`.

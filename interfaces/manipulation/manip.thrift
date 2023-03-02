service ManipulationInterface {
    bool stop();
    bool home(1:bool block);
    bool wave(1:bool block);
    bool shake(1:bool block);
    bool ready(1:bool block);
    bool pose_grasp(1:bool block, 2:list<double> hand_frames);
    bool grasp(1:bool block);
    bool release(1:bool block);
    bool ins();
    bool out();
    bool up();
    bool down();
    bool fore();
    bool aft();
    string finished();
}

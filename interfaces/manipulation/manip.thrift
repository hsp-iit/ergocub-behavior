service ManipulationInterface {
    string stop();
    string home(1:bool block);
    string wave(1:bool block);
    string shake(1:bool block);
    string ready(1:bool block);
    string pose_grasp(1:bool block, 2:list<double> hand_frames);
    string testgrasp();
    string grasp(1:bool block);
    string release(1:bool block);
    string ins();
    string out();
    string up();
    string down();
    string fore();
    string aft();
    string finished();
}

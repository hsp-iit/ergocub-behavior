service ManipulationInterface {
    bool stop();
    bool home();
    bool wave();
    bool shake();
    bool ready();
    bool scripted_grasp();
    bool grasp(1:list<double> hand_frames);
    bool release();
    bool ins();
    bool out();
    bool up();
    bool down();
    bool fore();
    bool aft();
}

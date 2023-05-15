

service RPCServerInterface {
    bool look_at(1:list<double> point);
    bool set_gain(1:double gain);
}
class branch_predictor
{
public:
    branch_predictor(unsigned int t, unsigned int bp_size, unsigned int bt_size);
    bool predict();
    void update_state(bool taken);
    
private:
    int n_bits,bp_size,bt_size,max,state;
};

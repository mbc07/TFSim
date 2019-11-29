#include<iostream>
#include<ostream>
#include<cmath>

class branch_predictor
{
public:
    branch_predictor(unsigned int t, unsigned int bp_bits, unsigned int bt_size);
    bool predict(int tag);
    std::string get_state_ui(int tag);
    void update_state(int tag, bool taken);
    
private:
    struct bp_entry {
        unsigned int idx;
        int max, state;
        bp_entry(unsigned int i, int max_value, int state_value)
        {
            idx = i;
            max = max_value;
            state = state_value;
        }
    };

    unsigned int bp_size;
    bp_entry **bp_table;

    int n_bits,bp_bits,bt_size;
};

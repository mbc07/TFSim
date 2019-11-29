#include<iostream>
#include<ostream>
#include<cmath>

class branch_predictor
{
public:
    branch_predictor(unsigned int t, unsigned int bp_bits, unsigned int bt_size);
    ~branch_predictor();
    bool predict(int tag);
    std::string get_state_ui(int tag);
    void update_state(int tag, bool taken);
    int save_target(int pc, int target);
    int search_target(int pc);
    
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

    struct bt_entry {
        unsigned int idx;
        bool in_use;        
        int pc;
        int target;
        bt_entry(unsigned int id)
        {
            idx = id;
            in_use = false;
            pc = 0;
            target = 0;
        }
    };

    unsigned int bp_size;
    bp_entry **bp_table;

    unsigned int bt_size;
    bt_entry **bt_table;

    int n_bits,bp_bits;
};

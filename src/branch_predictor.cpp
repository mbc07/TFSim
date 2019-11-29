#include "branch_predictor.hpp"

branch_predictor::branch_predictor(unsigned int t, unsigned int bp_bits, unsigned int bt_size): n_bits(t)
{
    bp_size = std::pow(2, bp_bits);
    bp_table = new bp_entry*[bp_size];

    for (unsigned int i = 0; i < bp_size; i++)
        bp_table[i] = new bp_entry(i, (1 << n_bits) - 1, 0);
}

bool branch_predictor::predict(int tag)
{
    return bp_table[tag]->state & (1 << (n_bits - 1));
}

std::string branch_predictor::get_state_ui(int tag)
{
    std::string state = "";

    switch (bp_table[tag]->state)
    {
    case 0:
        state = "NT, NT";
        break;

    case 1:
        state = "NT, T";
        break;

    case 10:
        state = "T, NT";
        break;

    case 11:
        state = "T, T";
        break;
    }

    return state;
}

void branch_predictor::update_state(int tag, bool taken)
{
    if(taken)
    {
        if ((bp_table[tag]->state) + 1 > bp_table[tag]->max)
            bp_table[tag]->state = bp_table[tag]->max;
    }
    else if(bp_table[tag]->state)
    {
        if ((bp_table[tag]->state) - 1 < 0)
            bp_table[tag]->state = 0;
    }

    std::cout << "Atualizando tag " << tag << " do BPB com valor " << get_state_ui(tag) << std::endl << std::flush;
}

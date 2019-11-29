#include "branch_predictor.hpp"

branch_predictor::branch_predictor(unsigned int t, unsigned int bp_bits, unsigned int bt_size): n_bits(t)
{
    bp_size = std::pow(2, bp_bits);
    bp_table = new bp_entry*[bp_size];
    for (unsigned int i = 0; i < bp_size; i++)
        bp_table[i] = new bp_entry(i, (1 << n_bits) - 1, 0);

    bt_table = new bt_entry*[bt_size];
    for (unsigned int i = 0; i < bt_size; i++)
        bt_table[i] = new bt_entry(i);
}

branch_predictor::~branch_predictor()
{
    for (unsigned int i = 0; i < bp_size; i++)
        delete bp_table[i];
    for (unsigned int i = 0; i < bt_size; i++)
        delete bt_table[i];

    delete bp_table;
    delete bt_table;
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

int branch_predictor::save_target(int pc, int target)
{
    bool saved = false;
    int saved_index = -1;

    // Verifica se PC já não existe na tabela
    if (search_target(pc) == !pc)
    {
        for (unsigned int i = 0; i < bt_size && !saved; i++)
        {
            if (!bt_table[i]->in_use)
            {
                bt_table[i]->pc = pc;
                bt_table[i]->target = target;
                bt_table[i]->in_use = true;
                saved = true;
                std::cout << "Destino do branch em PC=" << pc << " armazenado na posicao " << (i + 1) << " do BTB" << std::endl << std::flush;
            }
        }

        if (!saved)
        {
            // BTB cheio, substitui aleatóriamente uma posição em uso
            saved_index = rand() % bt_size;
            bt_table[saved_index]->pc = pc;
            bt_table[saved_index]->target = target;
            std::cout << "BTB cheio, substituindo posicao " << (saved_index + 1) << " com destino do branch em PC=" << pc << std::endl << std::flush;
        }

        return saved_index;
    }
    else
        return search_target(pc);
}

int branch_predictor::search_target(int pc)
{
    for (unsigned int i = 0; i < bt_size; i++)
    {
        if (bt_table[i]->in_use)
        {
            if (bt_table[i]->pc == pc)
            {
                std::cout << "Destino do branch em PC=" << pc << " encontrado na posicao " << (i + 1) << " do BTB" << std::endl << std::flush;
                return bt_table[i]->target;
            }
        }
    }

    // Destino não encontrado
    return pc;
}
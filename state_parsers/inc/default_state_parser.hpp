#ifndef DEAFULT_STATE_PARSER
#define DEAFULT_STATE_PARSER

#include <torch/torch.h>
#include "reasoner.hpp"

class DefaultStateParser
{
public:
    static constexpr int input_size()
    {
        return reasoner::BOARD_SIZE + reasoner::NUMBER_OF_VARIABLES;
    }

    static torch::Tensor parse_game_state(
        const reasoner::game_state& game_state, 
        torch::Device device);
};

#endif  
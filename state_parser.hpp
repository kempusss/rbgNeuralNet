#ifndef STATE_PARSER
#define STATE_PARSER

#include <torch/torch.h>
#include "reasoner.hpp"

class DefaultStateParser
{
public:
    static torch::Tensor parse_game_state(
        const reasoner::game_state& game_state, 
        torch::Device device);

    static constexpr int input_channels();
};

#endif  
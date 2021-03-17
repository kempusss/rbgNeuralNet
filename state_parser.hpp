#ifndef STATE_PARSER
#define STATE_PARSER

#include <torch/torch.h>
#include "reasoner.hpp"

class StateParser
{
public:
    static torch::Tensor parseGameState(
        const reasoner::game_state& game_state, 
        torch::Device device);
};

#endif  
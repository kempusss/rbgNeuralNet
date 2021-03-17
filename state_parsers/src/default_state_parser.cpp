#include "reasoner.hpp"
#include "default_state_parser.hpp"
#include <torch/torch.h>
#include <cmath>
#include <vector>
#include <limits>

torch::Tensor DefaultStateParser::parse_game_state(const reasoner::game_state& game_state, 
                                          torch::Device device)
{
    constexpr int N = DefaultStateParser::input_size();
    float board[N];

    for (int i = 0; i < reasoner::BOARD_SIZE; ++i)
        board[i] = game_state.get_piece(i) / (float)reasoner::NUMBER_OF_PIECES;

    for (int i = 0; i < reasoner::NUMBER_OF_VARIABLES; ++i)
        board[i + reasoner::BOARD_SIZE] = game_state.get_variable_value(i) / (float)reasoner::get_bound(i);

    auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(true);
    return torch::from_blob(board, {N}, options).clone().to(device);
}

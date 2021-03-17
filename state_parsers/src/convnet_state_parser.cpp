#include "reasoner.hpp"
#include "convnet_state_parser.hpp"
#include <torch/torch.h>
#include <cmath>
#include <vector>
#include <limits>

torch::Tensor ConvnetStateParser::parse_game_state(const reasoner::game_state& game_state, 
                                          torch::Device device)
{
    constexpr int N = constceil(sqrt(reasoner::BOARD_SIZE));
    constexpr int V = reasoner::NUMBER_OF_VARIABLES;
    float board[N][N] = {};

    for (int i = 0; i < reasoner::BOARD_SIZE; ++i)
        board[i / N][i % N] = game_state.get_piece(i) / (float)reasoner::NUMBER_OF_PIECES;

    auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(true);
    std::array<torch::Tensor, V + 1> tensors;
    tensors[0] = torch::from_blob(board, {N, N}, options).clone().to(device);

    for (int i = 0; i < V; ++i)
    {
        float v = game_state.get_variable_value(i) / (float)reasoner::get_bound(i);
        tensors[i + 1] = torch::full_like(tensors[0], v, options.device(device));
    }

    return torch::stack(tensors, 0);
}


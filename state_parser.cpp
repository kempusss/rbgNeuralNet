#include "reasoner.hpp"
#include <torch/torch.h>
#include "state_parser.hpp"
#include <cmath>
#include <vector>
#include <limits>

double constexpr sqrt_newton_raphson(double x, double curr, double prev) noexcept
{
    return curr == prev
        ? curr
        : sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
}

double constexpr sqrt(int x) noexcept
{
	return sqrt_newton_raphson(x, x, 0);
}

int constexpr constceil(double x)
{
    return x != (int)x ? (int)(x + 1) : (int)x;
}

torch::Tensor DefaultStateParser::parse_game_state(const reasoner::game_state& game_state, 
                                          torch::Device device)
{
    constexpr int N = constceil(sqrt(reasoner::BOARD_SIZE));
    constexpr int V = reasoner::NUMBER_OF_VARIABLES;

    float board[N][N] = {};

    for (int i = 0; i < reasoner::BOARD_SIZE; ++i)
        board[i / N][i % N] = game_state.get_piece(i) / (float)reasoner::NUMBER_OF_PIECES;

    auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(true).device(device);
    std::array<torch::Tensor, V + 1> tensors;
    tensors[0] = torch::from_blob(board, {N, N}, options);

    for (int i = 0; i < V; ++i)
    {
        float v = game_state.get_variable_value(i) / (float)reasoner::get_bound(i);
        tensors[i + 1] = torch::full_like(tensors[0], v, options);
    }

    return torch::stack(tensors, 0).flatten();
}

constexpr int DefaultStateParser::input_channels()
{
    return reasoner::NUMBER_OF_VARIABLES + 1;
}

#ifndef CONVNET_STATE_PARSER
#define CONVNET_STATE_PARSER

#include "default_state_parser.hpp"

class ConvnetStateParser : public DefaultStateParser
{
private:
    static double constexpr sqrt_newton_raphson(double x, double curr, double prev) noexcept
    {
        return curr == prev
            ? curr
            : sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
    }

    static double constexpr sqrt(int x) noexcept
    {
        return sqrt_newton_raphson(x, x, 0);
    }

    static int constexpr constceil(double x)
    {
        return x != (int)x ? (int)(x + 1) : (int)x;
    }

public:
    static constexpr int input_channels()
    {
        return reasoner::NUMBER_OF_VARIABLES + 1;
    }

    static constexpr int input_size()
    {
        constexpr int N = constceil(sqrt(reasoner::BOARD_SIZE));
        return (reasoner::NUMBER_OF_VARIABLES + 1) * N * N;
    }

    static torch::Tensor parse_game_state(
        const reasoner::game_state& game_state, 
        torch::Device device);
};

#endif
#ifndef SHALLOW_NET
#define SHALLOW_NET

#include "net_module.hpp"
#include <torch/torch.h>

class ShallowNet : public NetModule
{
private:
    torch::nn::Linear hidden_l{nullptr}, hidden2_l{nullptr}, output_l{nullptr};

public:
    ShallowNet(int input_size, int neuron_count);
    torch::Tensor forward(torch::Tensor x) override;
};

#endif

#include "shallownet.hpp"
#include <torch/torch.h>

ShallowNet::ShallowNet(int input_size, int neuron_count)
{
    hidden_l = register_module("hidden", torch::nn::Linear(input_size, neuron_count));
    hidden2_l = register_module("hidden2", torch::nn::Linear(neuron_count, neuron_count));
    output_l = register_module("output", torch::nn::Linear(neuron_count, 1));
}

torch::Tensor ShallowNet::forward(torch::Tensor x)
{
    x = torch::relu(hidden_l->forward(x));
    x = torch::relu(hidden2_l->forward(x));
    x = torch::sigmoid(output_l->forward(x));
    return x;
}

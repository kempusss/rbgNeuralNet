#include "shallownet.hpp"
#include <torch/torch.h>

ShallowNet::Net::Net(int input_size)
{
    hidden_l = register_module("hidden", torch::nn::Linear(input_size, input_size));
    hidden2_l = register_module("hidden2", torch::nn::Linear(input_size, input_size));
    output_l = register_module("output", torch::nn::Linear(input_size, 1));
}

torch::Tensor ShallowNet::Net::forward(torch::Tensor x)
{
    x = torch::relu(hidden_l->forward(x));
    x = torch::relu(hidden2_l->forward(x));
    x = torch::sigmoid(output_l->forward(x));
    return x;
}

ShallowNet::ShallowNet(int input_size)
    : device(torch::cuda::is_available() ? at::kCUDA : at::kCPU)
{
    model = new ShallowNet::Net(input_size);
    model->to(device);
}

torch::Tensor ShallowNet::forward(torch::Tensor tensor)
{
    tensor = tensor.to(device);
    return model->forward(tensor);
}

ShallowNet::~ShallowNet()
{
    delete model;
}

void ShallowNet::train(torch::Tensor input, torch::Tensor target, float learning_rate, int iters)
{
    input = input.to(device);
    target = target.to(device);

    torch::optim::SGD optimizer(this->model->parameters(), learning_rate);

    for(int i = 0; i < iters; ++i)
    {
        optimizer.zero_grad();
        torch::Tensor pred = this->forward(input);
        torch::Tensor loss = torch::mse_loss(pred, target);
        loss.backward();
        optimizer.step();
    }

}

#ifndef SHALLOW_NET
#define SHALLOW_NET

#include "reasoner.hpp"
#include "neuralnet.hpp"
#include <torch/torch.h>

class ShallowNet : public NeuralNet
{
private:
    class Net : public torch::nn::Module 
    {
    private:
        torch::nn::Linear hidden_l{nullptr}, hidden2_l{nullptr}, output_l{nullptr};

    public:
        Net(int input_size);
        torch::Tensor forward(torch::Tensor x);
    };

    Net* model;
    torch::Device device;

public:

    ShallowNet(int input_size);
    ~ShallowNet();
    float eval(const reasoner::game_state& game_state) const override { return 0.0; };
    void update(const std::vector< std::pair<const reasoner::game_state&, float> >& to_learn) override {};

    torch::Tensor forward(torch::Tensor);
    void train(torch::Tensor input, torch::Tensor target, float learning_rate = 0.01f, int iters = 100);
};

#endif

#include "shallownet.hpp"
#include "neuralnet.hpp"
#include "state_parser.hpp"
#include "flat_exp_learner.hpp"
#include <torch/torch.h>
#include <iostream>
#include <vector>

int main()
{
    torch::Device device(torch::kCPU);

    float XORarr[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    float XORout[4][1] = {{0}, {1}, {1}, {0}};
    auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(true);
    
    torch::Tensor XORdataset = torch::from_blob(XORarr, {4, 2}, options);
    torch::Tensor XORtarget = torch::from_blob(XORout, {4, 1}, options);

    std::cout << "Dataset:" << std::endl;
    std::cout << XORdataset.to(at::kCUDA) << std::endl;

    NeuralNet<StateParser> net(std::make_unique<ShallowNet>(2,10), true);
    std::cout << "Network initialized" << std::endl;
    net.train(XORdataset, XORtarget, 0.02, 100000);

    std::vector <torch::Tensor> tensory;

    for (int i = 0; i < 4; ++i)
    {
        float test[2] = {0, 1};
        tensory.push_back(torch::from_blob(test, {2}, options));
    }

    std::cout << "Trained" << std::endl;

    auto stacktest = torch::stack(tensory);

    std::cout << net.forward(stacktest) << std::endl;

    net.save("siema.pt");

    NeuralNet<StateParser> net2(std::make_unique<ShallowNet>(2, 10), false);

    net2.load("siema.pt");

    std::cout << net.forward(XORdataset) << std::endl;


    FlatExpLearner learnerExp(0.5);

    return 0;
}

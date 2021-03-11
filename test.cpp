#include "shallownet.hpp"
#include <torch/torch.h>
#include <iostream>

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

    ShallowNet shallowNet(2);

    shallowNet.train(XORdataset, XORtarget, 0.02, 100000);
    
    std::cout << shallowNet.forward(XORdataset) << std::endl;

    return 0;
}

#include "Network.hpp"

Jimmy::Net::Net(std::vector<unsigned int> vec,const Jimmy::TransferFunction& transFoo,const Jimmy::LossFunction& lossFoo)
:transFunc(transFoo),
lossFunc(lossFoo)
{
    this->layers.push_back(Jimmy::LAYER(vec[0])); // initializes imput layer
    for(std::vector<unsigned int>::iterator iter = vec.begin()+1; iter != vec.end(); iter++){ // does the rest and sets theri connections
        this->layers.push_back(Jimmy::LAYER(*iter, this->layers.back()));
    }
}

void Jimmy::Net::feedForward(const std::vector<double>& inputs){
    for(int i = 0; i < this->layers[0].neurons.size(); i++){
        this->layers[0][i].outValue = inputs[i];
    }

    for(int i = 1; i < this->layers.size(); i++){
        for(int j = 0; j < this->layers[i].neurons.size(); j++){
            this->layers[i][j].think(this->transFunc);
        }
    }
//    for(int j = 0; j < this->layers[this->layers.size()-1].neurons.size(); j++){
//        std::cout << this->layers[this->layers.size()-1][j].outValue << std::endl;
//    }
}

void Jimmy::Net::backProp(const std::vector<double>& realValues){
    double error = this->lossFunc.run(realValues, this->layers.back());
}
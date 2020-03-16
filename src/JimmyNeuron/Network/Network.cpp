#include "Network.hpp"

Jimmy::Net::Net(std::vector<unsigned int> vec){
    for(std::vector<unsigned int>::iterator iter = vec.begin(); iter != vec.end(); iter++){
        this->layers.push_back(Jimmy::LAYER(*iter));
    }
}
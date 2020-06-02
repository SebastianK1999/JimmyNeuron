#include "Layer.hpp"

Jimmy::LAYER::LAYER(unsigned int size){
    for(int i = 0; i < size; i++){
        this->neurons.push_back(Jimmy::NEURON(i));
    }
}

Jimmy::LAYER::LAYER(unsigned int size, Jimmy::LAYER & layer){
    for(int i = 0; i < size; i++){
        this->neurons.push_back(Jimmy::NEURON(i,layer.neurons));
    }
}

Jimmy::NEURON& Jimmy::LAYER::operator[](int i){
    return this->neurons[i];
}
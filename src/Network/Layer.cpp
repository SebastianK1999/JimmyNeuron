#include "JimmyNeuron/Network/Layer.hpp"

Jimmy::Layer::Layer(int size){
    for(int i = 0; i < size; i++){
        this->neurons.push_back(Jimmy::Neuron(i));
    }
}

Jimmy::Layer::Layer(int size, Jimmy::Layer & layer){
    for(int i = 0; i < size; i++){
        this->neurons.push_back(Jimmy::Neuron(i,layer.neurons));
    }
}

Jimmy::Neuron& Jimmy::Layer::operator[](int i){
    return this->neurons[i];
}
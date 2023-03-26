class Layer
{
    vector<Neuron *> neurons;

public:
    Layer(int numOfNeurons, int numOfInputs)
    {
        for (int i = 0; i < numOfNeurons; i++)
        {
            neurons.push_back(new Neuron(numOfInputs));
        }
    }

    vector<Value *> parameters()
    {
        vector<Value *> res;
        for (Neuron *n : neurons)
        {
            vector<Value *> params = n->parameters();
            for (Value *p : params)
            {
                res.push_back(p);
            }
        }
        return res;
    }

    vector<Value> predict(vector<double> inputs)
    {
        vector<Value> res;
        for (Neuron *n : neurons)
        {
            res.push_back(n->predict(inputs));
        }
        return res;
    }

    vector<Value> predict(vector<Value> inputs)
    {
        vector<Value> res;
        for (Neuron *n : neurons)
        {
            res.push_back(n->predict(inputs));
        }
        return res;
    }

    friend ostream &operator<<(ostream &os, const Layer &l);
};

ostream &operator<<(ostream &os, const Layer &l)
{
    cout << "Layer: of [" << endl;
    for (Neuron *n : l.neurons)
    {
        cout << n;
        if (n != l.neurons.back())
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    return os;
}

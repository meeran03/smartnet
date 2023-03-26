double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

class Neuron
{
    vector<Value *> weights;
    Value bias;

public:
    Neuron(int numOfInputs)
    {
        for (int i = 0; i < numOfInputs; i++)
        {
            weights.push_back(new Value(fRand(-1, 1)));
            weights[i]->setLabel("w" + to_string(i));
        }
        bias = Value(fRand(-1, 1));
        bias.setLabel("bias");
    }

    Value predict(vector<double> inputs)
    {
        Value sum(0.0);
        for (int i = 0; i < inputs.size(); i++)
        {
            Value ans = *weights[i] * inputs[i];
            ans.setLabel(weights[i]->label + "*x" + to_string(i));
            sum = sum + ans;
            sum.setLabel("sum + " + ans.getLabel());
        }
        sum = sum + bias;
        sum.setLabel("sum + b");
        Value *out = new Value(sum.sigmoid());
        out->setLabel("out");
        return *out;
    }

    Value predict(vector<Value *> inputs)
    {
        Value sum(0.0);
        for (int i = 0; i < inputs.size(); i++)
        {
            Value ans = *weights[i] * *inputs[i];
            ans.setLabel(weights[i]->label + "*x" + to_string(i));
            sum = sum + ans;
            sum.setLabel("sum + " + ans.getLabel());
        }
        sum = sum + bias;
        sum.setLabel("sum + b");
        Value *out = new Value(sum.sigmoid());
        out->setLabel("out");
        return *out;
    }

    vector<Value *> parameters()
    {
        vector<Value *> res;
        for (Value *w : weights)
        {
            res.push_back(w);
        }
        res.push_back(&bias);
        return res;
    }

    friend ostream &operator<<(ostream &os, const Neuron &n);
};

ostream &operator<<(ostream &os, const Neuron &n)
{
    cout << "Neuron: of [" << n.weights.size() << " weights, bias: " << n.bias << "]" << endl;
    return os;
}
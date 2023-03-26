#include "Value.cpp"
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include "Utils.cpp"
#include "Neuron.cpp"

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

class MLP
{
    vector<Layer *> layers;

public:
    MLP(int numberOfInputs, vector<int> layersConfig)
    {
        vector<int> config = {numberOfInputs};
        for (int i : layersConfig)
        {
            config.push_back(i);
        }
        for (int i = 0; i < config.size() - 1; i++)
        {
            layers.push_back(new Layer(config[i + 1], config[i]));
        }
    }

    vector<Value *> parameters()
    {
        vector<Value *> res;
        for (Layer *l : layers)
        {
            vector<Value *> params = l->parameters();
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
        for (Layer *l : layers)
        {
            res = l->predict(inputs);
        }
        return res;
    }

    vector<Value> predict(vector<Value> inputs)
    {
        vector<Value> res;
        for (Layer *l : layers)
        {
            res = l->predict(inputs);
        }
        return res;
    }

    friend ostream &operator<<(ostream &os, const MLP &m);
};

ostream &operator<<(ostream &os, const MLP &m)
{
    cout << "MLP: of [" << endl;
    for (Layer *l : m.layers)
    {
        cout << l;
        if (l != m.layers.back())
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    return os;
}

int main()
{
    srand((unsigned)time(NULL));
    rand();
    rand();
    rand();

    MLP mlp(3, {4, 4, 1});

    vector<vector<double>> data = {
        {2.0, 3.0, -1.0},
        {3.0, -1.0, 0.5},
        {0.5, 1.0, 1.0},
        {1.0, 1.0, -1.0},
    };


    vector<double> labels = {1.0, -1.0, -1.0, 1.0};

    const int EPOCHS = 1000;
    const double LEARNING_RATE = 0.01;
    vector<vector<Value>> preds;
    for (int i = 0; i < EPOCHS; i++)
    {
        preds = {};
        Value loss;
        for (int j = 0; j < data.size(); j++)
        {
            vector<Value> pred = mlp.predict(data[j]);
            preds.push_back(pred);
            for (Value p : pred)
            {
                loss += (p - labels[j]) * (p - labels[j]);
            }

        }

        for (Value *param : mlp.parameters())
        {
            param->grad = 0;
        }

        loss.backward();

        for (Value *param : mlp.parameters())
        {
            param->data -= LEARNING_RATE * param->grad;
        }

        if (i % 100 == 0)
        {
            cout << "Loss: " << loss.data << endl;
        }
    }

    cout << "Predictions: " << endl;
    for (vector<Value> pred : preds)
    {
        for (Value p : pred)
        {
            cout << p.data << " ";
        }
        cout << endl;
    }
}
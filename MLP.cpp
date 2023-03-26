class MLP
{
    vector<Layer *> layers;

public:


    MLP(int nin, vector<int> nouts)
    {
        vector<int> sz = {nin };
        for (int i : nouts)
        {
            sz.push_back(i);
        }

        for (int i = 0; i < sz.size(); i++)
        {
            layers.push_back(new Layer(sz[i], sz[i + 1]));
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

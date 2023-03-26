#include "Value.cpp"
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include "Utils.cpp"
#include "Neuron.cpp"
#include "Layer.cpp"
#include "MLP.cpp"

void train(MLP &mlp, vector<vector<double>> &data, vector<double> &labels, int epochs, double learning_rate)
{
    vector<vector<Value>> preds;
    for (int i = 0; i < epochs; i++)
    {
        preds = {};
        Value loss(0.0);
        for (int j = 0; j < data.size(); j++)
        {
            vector<Value> pred = mlp.predict(data[j]);
            preds.push_back(pred);
            for (Value p : pred)
            {
                Value ans = (p - labels[j]) ^ 2;
                ans.setLabel("ans");
                loss = loss + ans;
            }

        }

        for (Value *param : mlp.parameters())
        {
            param->grad = 0;
        }

        loss._backward();
        for (Value *param : mlp.parameters())
        {
            cout << "Param: " << *param << endl;
            cout << "(LEARNING_RATE * param->grad)" << (learning_rate * param->grad) << endl;
            param->data = param->data - (learning_rate * param->grad);
            cout << "Param: " << *param << endl;
        }
        cout << "Loss: " << loss.data << endl;
    }
}


void exampleTrain() {
    MLP mlp(3, {4, 4, 1});

    vector<vector<double>> data = {
        {2.0, 3.0, -1.0},
        {3.0, -1.0, 0.5},
        {0.5, 1.0, 1.0},
        {1.0, 1.0, -1.0},
    };


    vector<double> labels = {1.0, -1.0, -1.0, 1.0};

    const int EPOCHS = 1;
    const double LEARNING_RATE = 0.01;

    train(mlp, data, labels, EPOCHS, LEARNING_RATE);
}


int main() {
    srand((unsigned)time(NULL));
    rand();
    rand();
    rand();

    Neuron n(1);
    vector<double> inputs = {2.0};
    Value yhat = n.predict(inputs);
    yhat.setLabel("yhat");
    yhat.printPrevious();
    // yhat._backward();
}
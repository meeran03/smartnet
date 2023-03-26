#include <iostream>
#include <stdlib.h>
#include <set>
#include <vector>
#include <ctime>
#include <string>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

enum OPERATION
{
    NONE,
    ADD,
    SUBTRACT,
    MULTIPLY,
    POWER,

    RELU,
    SIGMOID,
    TANH
};

class Value
{
public:
    double data;
    vector<Value *> prev;
    OPERATION _op;
    string label;
    double grad;
    string activationFunction;

    Value(double data = 0.0, vector<Value *> children = {}, OPERATION _op = NONE, string label = "")
    {
        this->data = data;
        this->_op = _op;
        this->grad = 0;
        this->label = label;
        this->activationFunction = activationFunction;

        for (Value *v : children)
        {
            this->prev.push_back(v);
        }

    }

    Value(const Value &v)
    {
        this->data = v.data;
        this->_op = v._op;
        this->grad = v.grad;
        this->label = v.label;
        this->activationFunction = v.activationFunction;
        // copy the children
        for (Value *child : v.prev)
        {
            this->prev.push_back(child);
        }
   }

    Value &operator=(const Value &v)
    {
        this->data = v.data;
        this->_op = v._op;
        this->grad = v.grad;
        this->label = v.label;
        this->activationFunction = v.activationFunction;

        // copy the children
        for (Value *child : v.prev)
        {
            this->prev.push_back(child);
        }
        return *this;
    }

    void setLabel(string label)
    {
        this->label = label;
    }

    string getLabel()
    {
        return this->label;
    }

    void backward()
    {
        for (Value *v : this->prev)
        {
            if (_op == ADD)
            {
                prev[0]->grad += this->grad;
                prev[1]->grad += this->grad;
            }
            else if (_op == MULTIPLY)
            {
                prev[0]->grad = prev[1]->data * this->grad;
                prev[1]->grad = prev[0]->data * this->grad;
            } else if (_op == TANH) {
                v->grad += (1 - this->data * this->data) * this->grad;
            } else if (_op == SIGMOID) {
                v->grad += (1 - this->data) * this->data * this->grad;
            } else if (_op == RELU) {
                v->grad += (this->data > 0 ? 1 : 0) * this->grad;
            } else if (_op == POWER) {
                prev[0]->grad = prev[1]->data * pow(prev[0]->data, prev[1]->data - 1) * this->grad;
                prev[1]->grad = pow(prev[0]->data, prev[1]->data) * log(prev[0]->data) * this->grad;
            }
        }
    }

    vector<Value *> topologicalSort()
    {
        vector<Value *> topo;
        unordered_set<Value *> visited;
        function<void(Value *)> dfs = [&](Value *v) {
            cout << "dfs: " << *v << endl;
            if (visited.find(v) != visited.end())
            {
                return;
            }
            visited.insert(v);
            for (Value *child : v->prev)
            {
                if (visited.find(child) == visited.end() && child->_op != NONE) {
                    dfs(child);
                }
            }
            topo.push_back(v);
        };
        dfs(this);
        return topo;
    }

    void _backward() {
        this->grad = 1.0;
        vector<Value *> topo = this->topologicalSort();
        reverse(topo.begin(), topo.end());
        for (Value *v : topo)
        {
            v->backward();
        }
    }

    Value relu()
    {
        Value *ptr = this;
        Value res(max(0.0, this->data), {ptr}, RELU);
        return res;
    }

    Value sigmoid()
    {
        Value *ptr = this;
        Value *res = new Value(1.0 / (1.0 + exp(-this->data)), {ptr}, SIGMOID);
        return *res;
    }

    Value tanh()
    {
        Value *ptr = this;
        Value res((exp(2 * this->data) - 1) / (exp(2 * this->data) + 1), {ptr}, TANH);
        return res;
    }

    /*
        operator overloadings
    */

    Value operator+(Value &other)
    {
        Value *ptr = &other;
        Value *res = new Value(other.data + this->data, {this, ptr}, ADD);
        return *res;
    }

    Value operator+(double other)
    {
        Value *ptr = new Value(other);
        Value *res = new Value(other + this->data, {this, ptr}, ADD);
        return *res;
    }

    Value operator-(Value &other)
    {
        Value *ptr = &other;
        Value *res = new Value(this->data - other.data, {this, ptr}, SUBTRACT);
        return *res;
    }

    Value operator-(double other)
    {
        Value *ptr = new Value(other);
        Value *res = new Value(this->data - other, {this, ptr}, SUBTRACT); 
        return *res;
    }

    Value operator*(Value &other)
    {
        Value *ptr = &other;
        Value *res = new Value(other.data * this->data, {this, ptr}, MULTIPLY);
        return *res;
    }

    Value operator*(const Value &other)
    {
        Value *ptr = (Value *)&other;
        Value *res = new Value(other.data * this->data, {this, ptr}, MULTIPLY);
        return *res;
    }

    Value operator*(double other)
    {
        Value *ptr = new Value(other);
        Value *res = new Value(other * this->data, {this, ptr}, MULTIPLY);
        return *res;
    }

    bool operator<(const Value &other) const
    {
        return this->data < other.data;
    }

    void printPrevious() {
        cout << "Previous: " << endl;
        for (Value *v : this->prev) {
            cout << *v << endl;
        }
    }

    // power operator
    Value operator^(double other)
    {
        Value* res = new Value(pow(this->data, other), {this}, POWER);
        return *res;
    }

    friend ostream &operator<<(ostream &out, const Value &obj);
};

ostream &operator<<(ostream &os, const Value &obj)
{
    os << "Value(label=" << obj.label << ",data=" << obj.data << " ,grad=" << obj.grad << ")";
    return os;
}

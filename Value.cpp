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
    MULTIPLY,

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

    Value() {

    }

    Value(double data, vector<Value *> children = {}, OPERATION _op = NONE, string label = "")
    {
        this->data = data;
        this->prev = children;
        this->_op = _op;
        this->grad = 0;
        this->label = label;
        this->activationFunction = activationFunction;
    }

    Value(const Value &v)
    {
        this->data = v.data;
        this->prev = v.prev;
        this->_op = v._op;
        this->grad = v.grad;
        this->label = v.label;
        this->activationFunction = v.activationFunction;
    }

    Value &operator=(const Value &v)
    {
        this->data = v.data;
        this->prev = v.prev;
        this->_op = v._op;
        this->grad = v.grad;
        this->label = v.label;
        this->activationFunction = v.activationFunction;
        return *this;
    }

    void setLabel(string label)
    {
        this->label = label;
    }

    void backward()
    {
        for (Value *v : this->prev)
        {
            if (_op == ADD)
            {
                v->grad += this->grad;
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
            }
        }
    }

    vector<Value *> topologicalSort()
    {
        vector<Value *> topo;
        unordered_set<Value *> visited;
        function<void(Value *)> dfs = [&](Value *v) {
            if (visited.find(v) != visited.end())
            {
                return;
            }
            visited.insert(v);
            for (Value *child : v->prev)
            {
                dfs(child);
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
        Value res(1.0 / (1.0 + exp(-this->data)), {ptr}, SIGMOID);
        return res;
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
        Value res(other.data + this->data, {this, ptr}, ADD);
        return res;
    }

    Value operator+(double other)
    {
        Value res(other + this->data, {this}, ADD);
        return res;
    }

    Value operator-(Value &other)
    {
        Value *ptr = &other;
        Value res(this->data - other.data, {this, ptr}, ADD);
        return res;
    }

    Value operator-(double other)
    {
        Value res(this->data - other, {this}, ADD);
        return res;
    }

    Value operator*(Value &other)
    {
        Value *ptr = &other;
        Value res(other.data * this->data, {this, ptr}, MULTIPLY);
        return res;
    }

    Value operator*(const Value &other)
    {
        Value *ptr = (Value *)&other;
        Value res(other.data * this->data, {this, ptr}, MULTIPLY);
        return res;
    }

    Value operator*(double other)
    {
        Value res(other * this->data, {this}, MULTIPLY);
        return res;
    }

    Value &operator+=(Value &other)
    {
        Value *ptr = &other;
        Value res(other.data + this->data, {this, ptr}, ADD);
        return res;
    }

    Value &operator+=(const Value &other)
    {
        Value *ptr = (Value *)&other;
        double ans = other.data + this->data;
        Value res(ans, {this, ptr}, ADD);
        return res;
    }

    Value &operator+=(double other)
    {
        Value res(other + this->data, {this}, ADD);
        return res;
    }

    bool operator<(const Value &other) const
    {
        return this->data < other.data;
    }

    void printPrevious()
    {
        cout << "[";
        for (Value *v : this->prev)
        {
            cout << *v << ",";
        }
        cout << "]\n";
    }

    friend ostream &operator<<(ostream &out, const Value &obj);
};

ostream &operator<<(ostream &os, const Value &obj)
{
    os << "Value(data=" << obj.data << ")";
    return os;
}

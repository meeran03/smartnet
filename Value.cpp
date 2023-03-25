#include <iostream>
#include <stdlib.h>
#include <set>
#include <vector>
#include <ctime>
#include <string>
#include <unordered_set>
#include <functional>
#include <algorithm>

using namespace std;

enum OPERATION
{
    NONE,
    ADD,
    MULTIPLY
};

class Value
{
public:
    double data;
    vector<Value *> prev;
    OPERATION _op;
    string label;
    double grad;
    Value(double data, vector<Value *> children = {}, OPERATION _op = NONE, string label = "")
    {
        this->data = data;
        this->prev = children;
        this->_op = _op;
        this->grad = 0;
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

    /*
        operator overloadings
    */

    Value operator+(Value &other)
    {
        Value *ptr = &other;
        Value res(other.data + this->data, {this, ptr}, ADD);
        return res;
    }

    Value operator*(Value &other)
    {
        Value *ptr = &other;
        Value res(other.data * this->data, {this, ptr}, MULTIPLY);
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

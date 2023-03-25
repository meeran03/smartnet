#include <iostream>
#include <stdlib.h>
#include <set>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

enum OPERATION
{
    NONE,
    ADD,
    MULTIPLY
};

template <typename T>
class Value
{
private:
    T data;
    set<Value> prev;
    OPERATION _op;
    string label;
    double grad;

public:
    Value(T data, vector<Value> children = {}, OPERATION _op = NONE, string label = "")
    {
        this->data = data;
        this->prev = set(children.begin(), children.end());
        this->_op = _op;
        this->grad = 0;
    }

    void setLabel(string label)
    {
        this->label = label;
    }

    /*
        operator overloadings
    */

    Value operator+(const Value &other)
    {
        Value res(other.data + this->data, {*this, other}, ADD);
        return res;
    }

    Value operator*(const Value &other)
    {
        Value res(other.data * this->data, {*this, other}, MULTIPLY);
        return res;
    }

    bool operator<(const Value &other) const
    {
        return this->data < other.data;
    }


    template <typename U>
    friend ostream & operator << (ostream &out, const Value<U> &obj);
};

template <typename T>
ostream &operator<<(ostream &os, const Value<T> & obj )
{
    os << "Value(data=" << obj.data << ")";
    return os;
}

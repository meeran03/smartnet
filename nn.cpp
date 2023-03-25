#include "Value.cpp"

int main()
{
    srand(time(0)); // randomness
    Value<int> a(2);
    a.setLabel("a");

    cout << a << endl;
}
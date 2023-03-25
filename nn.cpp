#include "Value.cpp"
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include "Utils.cpp"

int main()
{
    srand(time(0)); // randomness
    Value a(2);
    a.setLabel("a");

    Value b(-3);
    b.setLabel("b");

    Value c(10);
    c.setLabel("c");

    Value d = a*b;
    d.setLabel("d");

    Value e = d + c;
    e.setLabel("e");

    cout << e << endl;

    e._backward();

}
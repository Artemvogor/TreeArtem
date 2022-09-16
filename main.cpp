#include <iostream>
#include "source/source.cpp"
using namespace std;

int main() {
    Veb<6> a;
    a.insert(5);
    cout << a.get_max() << endl;
    return 0;
}

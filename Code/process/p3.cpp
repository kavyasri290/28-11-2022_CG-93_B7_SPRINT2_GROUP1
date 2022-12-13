#include <iostream>
using namespace std;

int main() {
    
    double n1=2, n2=3.5, n3=7;
    if(n1 >= n2 && n1 >= n3)
    cout << "Largest number: " << n1<<endl;;
    else if(n2 >= n1 && n2 >= n3)
    cout << "Largest number: " << n2<<endl;
    else 
    cout << "Largest number: " << n3<<endl;
    return 0;
}

#include <iostream>
using namespace std;

int main() {

    int n=12345, reversed_number = 0, remainder;
    while(n != 0) {
    remainder = n % 10;
    reversed_number = reversed_number * 10 + remainder;
    n /= 10;
  }

  cout << "Reversed Number = " << reversed_number;

  return 0;
}

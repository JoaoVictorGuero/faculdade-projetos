#include <iostream>
using namespace std;

int fibo(int n) {
    switch (n) {
    case 0:
        return 0;
        break;
    case 1:
        return 1;
        break;
    default:
        return fibo(n - 1) + fibo(n - 2);
        break;
    }
}

int main(){
    int numero;
    cin>>numero;
    cout<<"Fibonacci: ";
    for (int i = 1; i < numero; i++)
    {
        cout<<fibo(i)<<"; ";
    }
    

}
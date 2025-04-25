#include <iostream>

using namespace std;
float r;
float g;
float b;
float minimo(float a, float b)
{
    return a < b ? a : b;
}
float maximo(float a, float b)
{
    return a > b ? a : b;
}
float h1(float a, float b, float c, float cmax, float cmin, float delta)
{
    float h;
    if (cmax == a) h = 60*((int)((b-c)/delta)%6);
    if (cmax == b) h = 60*(((c-a)/delta)+2);
    if (cmax == c) h = 60*(((a-b)/delta)+4);
    if (delta == 0) h = 0;
    return h;
}
float s1(float a, float b, float c, float cmax, float cmin, float delta)
{
    float s;
    if (cmax == 0) s = 0;
    if (cmax != 0) s = delta/cmax;

    return s;
}

int main()
{
    cin>>r;
    cin>>g;
    cin>>b;
    r = r/255;
    g = g/255;
    b = b/255;
    float cmin = minimo(minimo(r,g),b);
    float cmax = maximo(maximo(r,g),b);
    float delta = cmax - cmin;
    float h = h1(r,g,b,cmax,cmin,delta);
    float s = s1(r,g,b,cmax,cmin,delta);
    float v = cmax;
    cout << fixed;
    cout.precision(2);
    cout<<"H: "<<h<<", S: "<<s*100<<", V: "<<v*100;
    return 0;
}

int IsPrime(int n){
    int x = 2;
    if(n>3) return 4;
    else return 9;


}
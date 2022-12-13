#include<iostream>
using namespace std;
int sum(int a,int b)
{
	int c;
	c=a+b;
	return c;

}

int main()
{
	int a,b,s;
	a=10;
	b=20;
	s=sum(a,b);
	cout<<"sum is:"<<s<<endl;
	return 0;
}


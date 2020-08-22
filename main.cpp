#include <lib/Clock.hpp>   
#include <iostream>

using namespace std;
int main(int argc, char** argv)
{  
    Clock clk;
	clk.start();
	cout<<"clock"<<endl;
	clk.stop();
    
    return 0;  
}  
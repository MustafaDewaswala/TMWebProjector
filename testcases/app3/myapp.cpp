#include<iostream>
#include<fstream>
#include<tmwp>
using namespace std;
using namespace tmwp;
void funOne(Request &request,Response &response)
{
cout<<"FUNCTION ONE GOT CALLED"<<endl;
request.setInt("abc",10);
request.forward("/two");
}
void funTwo(Request &request,Response &response)
{
cout<<"FUNCTION TWO GOT CALLED"<<endl;
request.setInt("pqr",20);
request.forward("/three");
}
void funThree(Request &request,Response &response)
{
cout<<"FUNCTION THREE GOT CALLED"<<endl;
int abc=request.getInt("abc");
cout<<"abc = "<<abc<<endl;
int pqr=request.getInt("pqr");
cout<<"pqr = "<<pqr<<endl;
request.forward("xyz.html");
}
int main()
{
TMWebProjector server(5050);
server.onRequest("/one",funOne);
server.onRequest("/two",funTwo);
server.onRequest("/three",funThree);
server.start();
return 0;
}
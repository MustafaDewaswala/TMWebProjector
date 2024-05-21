#include<iostream>
#include<tmwp>
#include<string.h>
#include<stdlib.h>
#include<ctime>
using namespace std;
using namespace tmwp;
void dispatchTimeFun(Request &request,Response &response)
{
time_t t=time(0);
char *now=ctime(&t);
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>The Clock</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>");
response.write(now);
response.write("</h1>");
response.write("<a href='now'>Refresh</a><br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}

void getCityView(Request &request,Response &response)
{
cout<<"getCityView is being processed"<<endl;
string cityCodeString=request.get("cityCode");
int cityCode=atoi(cityCodeString.c_str());
cout<<"("<<cityCode<<")"<<endl;
if(cityCode==1) request.forward("Ujjain.html");
else if(cityCode==2) request.forward("Indore.html");
else if(cityCode==3) request.forward("Dewas.html");
else request.forward("errorPage");
}

void createErrorPage(Request &request,Response &response)
{
/*
using response.write
Create an HTML that displays a page with 
some error message

*/
cout<<"ERROR PAGE FUNCTION GOT INVOKED"<<endl;
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>CITY</title>");
response.write("</head>");
response.write("<body>");
response.write("You have not selected a city<br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();

}

void saveEnquiry(Request &request,Response &response)
{
cout<<"SAVE ENQUIRY"<<endl;
string n=request.get("nm");
string c=request.get("ct");
string a=request.get("ad");
string m=request.get("msg");
cout<<"Data received"<<endl;
cout<<"Name : "<<n<<endl;
cout<<"City : "<<c<<endl;
cout<<"Address : "<<a<<endl;
cout<<"Query : "<<m<<endl;
// some logic to save data somewhere (maybe in a file or some database)
request.forward("SaveNotification.html");
}

int main()
{
TMWebProjector server(8080);
server.onRequest("/now",dispatchTimeFun);
server.onRequest("/getCity",getCityView);
server.onRequest("/addEnquiry",saveEnquiry);
server.onRequest("errorPage",createErrorPage);
server.start();
return 0;
}
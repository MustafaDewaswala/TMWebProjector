#include<iostream>
#include<fstream>
#include<tmwp>
#include<string.h>

using namespace std;
using namespace tmwp;

void addStudent(Request &request,Response &response);
void getAllStudents(Request &request,Response &response);
void sendStudentEditForm(Request &request,Response &response);
void editStudentData(Request &request,Response &response);
void sendStudentDeleteConfirmationForm(Request &request,Response &response);
void deleteStudentData(Request &request,Response &response);


int main()
{
TMWebProjector server(8080);

server.onRequest("/addStudent",addStudent);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",sendStudentEditForm);
server.onRequest("/EditStudent",editStudentData);
server.onRequest("/confirmDeleteStudent",sendStudentDeleteConfirmationForm);
server.onRequest("/deleteStudent",deleteStudentData);

server.start();

return 0;
}



// remaining code

//------------------------------------------------------------------------------------------------

// code to add data

void addStudent(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
cout<<"Roll Number : "<<rollNumber<<endl;
string name=request.get("nm");
cout<<"Name : "<<name<<endl;
fstream f;
f.open("student.dat",ios::app | ios::binary);
if(!f)
{
cout<<"Error while opening the file"<<endl;
request.forward("StudentNotFound.html");
return;
}
f<<rollNumber;
f<<endl;
f<<name;
f<<endl;
f.close();
response.write("<DOCTYPE HTML>");
response.write("<HTML lang='en'>");
response.write("<head>");
response.write("<TITLE>ABCL School, Ujjain</TITLE>");
response.write("<meta charset='utf-8'>");
response.write("</head>");
response.write("<body>");
response.write("Student Added");
response.write("<BR>");
response.write("<a href='index.html'>OK</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
//-------------------------------------------------------------------------------------------------

// code to view data

void getAllStudents(Request &request,Response &response)
{
fstream f;
f.open("student.dat",ios::in);
if(!f)
{
cout<<"The file does not exists"<<endl;
request.forward("StudentNotFound.html");
return;
}
string data;
string myLine;
if(f.is_open()) 
{
while(f)
{
getline(f,myLine);
if(myLine=="") break;
data+="Roll Number : ";
data+=myLine;
data+=", Name : ";
getline(f,myLine);
if(myLine=="") break;
data+=myLine;
data+="<BR>";
}
}
f.close();
response.write("<!DOCTYPE HTML>");
response.write("<HTML lang='en'>");
response.write("<head>");
response.write("<TITLE>ABCL School, Ujjain</TITLE>");
response.write("<meta charset='utf-8'>");
response.write("</head>");
response.write("<body>");
response.write("List Of Students : ");
response.write("<BR>");
response.write(data.c_str());
response.write("<BR>");
response.write("<a href='index.html'>BACK</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}

//-------------------------------------------------------------------------------------------------

// code to edit data

void sendStudentEditForm(Request &request,Response &response)
{
string name;
int found=0;
fstream f;
f.open("student.dat",ios::in);
if(!f)
{
cout<<"The file does not exists"<<endl;
return;
}
string myLine;
if(f.is_open()) 
{
while(f)
{
getline(f,myLine);
if(myLine=="") break;
if(strcmp(myLine.c_str(),request.get("rl"))==0)
{
found=1;
getline(f,name);
break;
}
getline(f,myLine);
if(myLine=="") break;
}
}
f.close();
cout<<"FOUND VALUE : "<<found<<endl;
if(!found) request.forward("StudentNotFound.html");
else
{
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)");
response.write("<form method='get' action='EditStudent'>");
response.write("<h1><--------------STUDENT DETAILS--------------></h1>");
response.write("Roll number : ");
response.write(request.get("rl"));
response.write("<br>Old Name : ");
response.write(name.c_str());
response.write("<input type='hidden' id='rl' name='rl' value='");
response.write(request.get("rl"));
response.write("'><br>");
response.write("New Name : <input type='text' id='nnm' name='nnm'><br>");
response.write("<button type='submit'>EDIT</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
}

void editStudentData(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
string newName=request.get("nnm");
cout<<endl<<"Roll Number : "<<rollNumber<<endl;
cout<<"NEW NAME : "<<newName<<endl;
fstream rd,wr;
string myLine,tmp;
rd.open("student.dat",ios::in);
if(!rd)
{
cout<<"The file does not exists"<<endl;
request.forward("StudentNotFound.html");
return;
}
wr.open("tmp.tmp",ios::out);
if(!wr)
{
cout<<"The file does not exists"<<endl;
request.forward("StudentNotFound.html");
return;
}
if(rd.is_open() && wr.is_open())
{
while(rd)
{
getline(rd,myLine);
if(myLine=="") break;
wr<<myLine;
wr<<endl;
if(rollNumber==atoi(myLine.c_str()))
{
getline(rd,tmp);
myLine=newName;
}
else
{
getline(rd,myLine);
}
if(myLine=="") break;
wr<<myLine;
wr<<endl;
}
rd.close();
wr.close();
// Remove the original input file
remove("student.dat");
// Rename the temporary output file to the original input file name
rename("tmp.tmp","student.dat");
}
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)<br>");
response.write("<h2>Student Updated</h2>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
cout<<"STUDENT DATA UPDATED !!!"<<endl;
}

//-------------------------------------------------------------------------------------------------

// code to delete data

void sendStudentDeleteConfirmationForm(Request &request,Response &response)
{
int found=0;
fstream f;
string name;
f.open("student.dat",ios::in);
if(!f)
{
cout<<"The file does not exists"<<endl;
return;
}
string myLine;
if(f.is_open()) 
{
while(f)
{
getline(f,myLine);
if(myLine=="") break;
if(strcmp(myLine.c_str(),request.get("rl"))==0)
{
found=1;
getline(f,name);
break;
}
getline(f,myLine);
if(myLine=="") break;
}
}
f.close();
cout<<"FOUND VALUE : "<<found<<endl;
if(!found) request.forward("StudentNotFound.html");
else
{
cout<<"Roll Number : "<<request.get("rl")<<", Name : "<<name<<endl;
response.write("<DOCTYPE HTML>");
response.write("<HTML lang='en'>");
response.write("<head>");
response.write("<TITLE>ABCL School, Ujjain</TITLE>");
response.write("<meta charset='utf-8'>");
response.write("</head>");
response.write("<body>");
response.write("<form method='get' action='deleteStudent'>");
response.write("Student (Delete Module)<BR>Student Ready For Deletion");
response.write("<h1><--------------STUDENT DETAILS--------------></h1>");
response.write("Roll number : ");
response.write(request.get("rl"));
response.write("<br>Name : ");
response.write(name.c_str());
response.write("<br><input type='hidden' id='rl' name='rl' value='");
response.write(request.get("rl"));
response.write("'><br>");
response.write("<button type='submit'>Delete</button>");
response.write("</form>");
response.write("<a href='index.html'>OK</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
}

void deleteStudentData(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
cout<<endl<<"Roll Number : "<<rollNumber<<endl;
fstream rd,wr;
string myLine,tmp;
rd.open("student.dat",ios::in);
if(!rd)
{
cout<<"The file does not exists"<<endl;
request.forward("StudentNotFound.html");
return;
}
wr.open("tmp.tmp",ios::out);
if(!wr)
{
cout<<"The file does not exists"<<endl;
request.forward("StudentNotFound.html");
return;
}
if(rd.is_open() && wr.is_open())
{
while(rd)
{
getline(rd,myLine);
if(myLine=="") break;
if(rollNumber==atoi(myLine.c_str()))
{
getline(rd,tmp);
continue;
}
else
{
wr<<myLine;
wr<<endl;
getline(rd,myLine);
if(myLine=="") break;
wr<<myLine;
wr<<endl;
}
}
rd.close();
wr.close();
// Remove the original input file
remove("student.dat");
// Rename the temporary output file to the original input file name
rename("tmp.tmp","student.dat");
}
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Delete Module)<br>");
response.write("<h3>Student Deleted</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
cout<<"STUDENT DATA DELETED !!!"<<endl;
}

//------------------------------------------------------------------------------------------------

// The End

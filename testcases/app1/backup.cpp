#include<iostream>
#include<fstream>
#include<tmwp>
#include<string.h>
using namespace std;
using namespace tmwp;

void addStudent(Request &request,Response &response)
{
int rollNo=atoi(request.get("rl"));
cout<<"Roll Number : "<<rollNo<<endl;
string name=request.get("nm");
cout<<"Name : "<<name<<endl;
fstream f;
f.open("student.dat",ios::app | ios::binary);
if(!f)
{
cout<<"Error while opening the file"<<endl;
return;
}
f<<rollNo;
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
void getAllStudents(Request &request,Response &response)
{
fstream f;
f.open("student.dat",ios::in);
if(!f)
{
cout<<"The file does not exists"<<endl;
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
void sendStudentEditForm(Request &request,Response &response)
{
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
if(strcmp(myLine.c_str(),request.get("rl"))==0) found=1;
getline(f,myLine);
if(myLine=="") break;
}
}
f.close();
cout<<"FOUND VALUE : "<<found<<endl;
if(found)
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
response.write("Roll number : ");
response.write(request.get("rl"));
response.write("<input type='hidden' id='rl' name='rl' value='");
response.write(request.get("rl"));
response.write("'><br>");
response.write("New Name <input type='text' id='nnm' name='nnm'><br>");
response.write("<button type='submit'>EDIT</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
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
response.write("<br>----------STUDENT NOT FOUND----------<br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
}
void editStudentData(Request &request,Response &response)
{
cout<<"ITS WORKING"<<endl;
int rollNo=atoi(request.get("rl"));
string name=request.get("nnm");
cout<<endl<<"Roll Number : "<<rollNo<<endl;
cout<<"NEW NAME : "<<name<<endl;
/*fstream f;
f.open("student.dat",ios::in | ios::out);
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
if(strcmp(myLine.c_str(),to_string(rollNo).c_str())==0)
{
f<<name;
break;
}
getline(f,myLine);
if(myLine=="") break;
}
}
f.close();*/
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)<br>");
response.write("<h3>Student updated</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
cout<<"STUDENT DATA UPDATED !!!"<<endl;
}
void sendStudentDeleteConfirmationForm(Request &request,Response &response)
{
// extract roll number from request (rl)
/*
file handling code to search roll number
and send html

<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>ABCL School, Ujjain</title>
</head>
<body>
Student (Delete Module)
<form method='get' action='deleteStudent'>
Roll Number : 101
<input type='hidden' id='rl' name='rl' value='101'><br>
Name : Sameer<br>
<button type='submit'>Delete</button>
</form>
<a href='index.html'>Home</a><br>
</body>
</html>

Roll Number : 101  in place of 101 value of rl should be placed
// Note : value='101'  in place of 101 value of rl should be placed
// Sameer              in place of Sameer the name against 101 should be placed


*/
/*
int rollNo=atoi(request.get("rl"));
gen=rollNo;
char c[200];
string n;
int r;
SqliteDB sqliteDB;
int found=0;
try
{
sqliteDB.open("student.dat");
string sql="select * from student;";
Rows rows;
rows=sqliteDB.selectRows(sql);
Row row;
while(rows.hasMoreRows())
{
row=rows.getRow();
r=row.getInt("roll_number");
n=row.getString("name");
if(r==rollNo)
{
found=1;
break;
}
}
}catch(SqliteException &sqliteException)
{
cout<<sqliteException.what()<<endl;
}
sqliteDB.close();
if(found)
{
cout<<"Roll Number : "<<r<<", Name : "<<n<<endl;
response.write("<DOCTYPE HTML>");
response.write("<HTML lang='en'>");
response.write("<head>");
response.write("<TITLE>ABCL School, Ujjain</TITLE>");
response.write("<meta charset='utf-8'>");
response.write("</head>");
response.write("<body>");
response.write("Student (Delete Module)<BR>Student Ready For Deletion");
response.write("<form method='get' action='deleteStudent'>");
response.write("<button type='submit'>Delete</button>");
response.write("</form>");
response.write("<a href='index.html'>OK</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}
else
{
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Delete Module)");
response.write("<br>----------STUDENT NOT FOUND----------<br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
}*/
}
void deleteStudentData(Request &request,Response &response)
{
/*
code to extract data against rl (roll number)
file Handling code to search roll number
and delete data

send back html


<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>ABCL School, Ujjain</title>
</head>
<body>
Student (Delete Module)<br>
<h3>Student deleted</h3>
<form method='get' action='index.html'>
<button type='submit'>OK</button>
</form>
<a href='index.html'>Home</a><br>
</body>
</html>

*/
/*
cout<<endl<<"Roll Number : "<<gen<<endl;
char c[300];
sprintf(c,"delete from student where roll_number=%d;",gen);
try
{
SqliteDB sqliteDB("student.dat");
string sql=c;
sqliteDB.executeDelete(sql);
sqliteDB.close();
}catch(SqliteException &sqliteException)
{
cout<<sqliteException.what()<<endl;
}
response.write("<DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Delete Module)<br>");
response.write("<h3>Student deleted</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");

cout<<"STUDENT DATA DELETED !!!"<<endl;*/
}

int main()
{
TMWebProjector server(5050);
server.onRequest("/addStudent",addStudent);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",sendStudentEditForm);
server.onRequest("/EditStudent",editStudentData);
//server.onRequest("/confirmDeleteStudent",sendStudentDeleteConfirmationForm);
//server.onRequest("/deleteStudent",deleteStudentData);
server.start();
return 0;
}
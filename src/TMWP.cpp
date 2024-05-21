#ifndef __$__TMWP__
#define __$__TMWP__123
#include<tmwp>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#define MAX_LINE_LENGTH 10000
using namespace tmwp;
int FindAtIndex(const char *,const char *);
char * findAndReplace(const char*,const char*,const char*);
int csd;
/*typedef struct _request
{
char *method;
char *resource;
char isClientSideTechnologyResource;
char *mimeType;
int dataCount;
char **data;   // p2p
}REQUEST;*/

int extensionEquals(const char *left,const char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90) a+=32;
if(b>=65 && b<=90) b+=32;
if(a!=b) return 0;
left++;
right++;
}
return *left==*right;
}
char * getMIMEType(char *resource)
{
char *mimeType=NULL;
int len=strlen(resource);
if(len<4) return mimeType;
int lastIndexOfDot=len-1;
while(lastIndexOfDot>0 && resource[lastIndexOfDot]!='.') lastIndexOfDot--;
if(lastIndexOfDot==0) return mimeType;
if(extensionEquals(resource+lastIndexOfDot+1,"html"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
if(extensionEquals(resource+lastIndexOfDot+1,"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
if(extensionEquals(resource+lastIndexOfDot+1,"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
if(extensionEquals(resource+lastIndexOfDot+1,"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastIndexOfDot+1,"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastIndexOfDot+1,"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
if(extensionEquals(resource+lastIndexOfDot+1,"ico"))
{
mimeType=(char *)malloc(sizeof(char)*13);
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}
char isClientSideResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0') return 'N';
return 'Y';
}

Request::Request(Request &other)
{
this->request=other.request;
this->requestSeparator=other.requestSeparator;
this->urlManager=other.urlManager;
this->isForwarding=0;
this->intStart=NULL;
}
Request::Request(REQUEST *req)
{
this->request=req;
this->requestSeparator=(char *)malloc(sizeof(char)*3);
strcpy(this->requestSeparator,"+");
this->requestSeparator[strlen(this->requestSeparator)]='\0';
this->urlManager=NULL;
this->isForwarding=0;
this->intStart=NULL;
}
Request & Request::operator=(REQUEST *request)
{
this->request=request;
return *this;
}
Request & Request::operator=(Request &other)
{
this->request=other.request;
this->requestSeparator=other.requestSeparator;
this->urlManager=other.urlManager;
this->isForwarding=other.isForwarding;
this->intStart=other.intStart;
return *this;
}
void Request::set(REQUEST *request)
{
*this=request;
}
void Request::setRequestSeparator(char *s)
{
free(this->requestSeparator);
this->requestSeparator=s;
}
void requestFilter(const char *dt)
{
dt=findAndReplace(dt,"%2C",",");
dt=findAndReplace(dt,"%21","!");
}
char * Request::get(const char *c) const
{
char *dt;
dt=(char *)malloc(sizeof(char)*2);
strcpy(dt,"");
for(int r=0;r<this->request->dataCount;r++)
{
if(FindAtIndex(this->request->data[r],c)==0)
{
int len;
int end;
int start;
len=strlen(c);
start=len+1; // to skip '=' also 
end=strlen(this->request->data[r])-len;
free(dt);
dt=(char *)malloc(sizeof(char)*end);
strncpy(dt,this->request->data[r]+start,end);
dt[strlen(dt)]='\0';
dt=findAndReplace(dt,this->requestSeparator," ");
requestFilter(dt); // for other separators : "!", "?", ",", etc.
return dt;
}
}
return dt;  // return empty string if string s does not exist in data
}
void Request::setUrlManager(URL *urlManager)
{
this->urlManager=urlManager;
}
void Request::forward(const char *url)
{
this->isForwarding=1;
Response rs;
Request rq(this->request);
for(URL *p=this->urlManager;p!=NULL;p=p->next)
{
if(strcmp(p->url,url)==0)
{
cout<<"We Found "<<url<<endl;
rq.setUrlManager(this->urlManager);
rq.appendIntData(this->intStart);
p->ptrOnRequest(rq,rs);
rs.setCSD(csd);
if(rs.isClosed()) rs.sendResponse();
csd=rs.getCSD();
return;
}
}
char responseBuffer[1024];
int clientSocketDescriptor=csd;
FILE *f;
f=fopen(url,"rb");
if(f==NULL)
{
cout<<url<<" not found"<<endl;
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:163\nConnection: close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
send(clientSocketDescriptor,responseBuffer,sizeof(responseBuffer),0);
printf("Sending 404 page\n");
this->isForwarding=0;
return;
}
cout<<"We Found "<<url<<endl;
fseek(f,0,2);  // move the internal pointer to the end of file
int length=ftell(f);
fseek(f,0,0);  // move the internal pointer to the start of file
//sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\nKeep-Alive: timeout=5, max=1000\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
int i=0;
while(i<length)
{
int rc=length-i;    // rc for read count
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
this->isForwarding=0;
}
void Request::setIsForwarding(int i)
{
this->isForwarding=i;
}
int Request::IsForwarding()
{
return this->isForwarding;
}
void Request::setIntData(char *dataName,int data)   // dataStructure : linked list
{
INTEGER *i,*j;
i=(INTEGER *)malloc(sizeof(INTEGER));
i->dataName=dataName;
i->data=data;
i->next=NULL;
if(this->intStart==NULL)
{
this->intStart=i;
return;
}
for(j=this->intStart;j->next!=NULL;j=j->next);
j->next=i;
}
int Request::getIntData(char *dataName)
{
INTEGER *j;
for(j=this->intStart;j!=NULL;j=j->next)
{
if(strcmp(j->dataName,dataName)==0) return j->data;
}
return 0;
}
void Request::setInt(string dataName,int data)
{
this->setIntData((char *)dataName.c_str(),data);
}
int Request::getInt(string dataName)
{
return this->getIntData((char *)dataName.c_str());
}
void Request::appendIntData(INTEGER *str)
{
if(this->intStart==NULL)
{
this->intStart=str;
return;
}
INTEGER *j;
for(j=this->intStart;j->next!=NULL;j=j->next);
j->next=str;
}

Response::Response()
{
this->isClose=0;
this->responseBuffer=(char *)malloc(sizeof(char)*1024);
this->tmp=(char *)malloc(sizeof(char)*501);
this->responseBuffer[0]='\0';
this->tmp[0]='\0';
}
Response::Response(Response &other)
{
this->isClose=0;
this->responseBuffer=other.responseBuffer;
this->tmp=other.tmp;
}
Response & Response::operator=(Response &other)
{
this->isClose=0;
this->responseBuffer=other.responseBuffer;
this->tmp=other.tmp;
return *this;
}
void Response::setCSD(int csd)
{
this->clientSocketDescriptor=csd;
}
int Response::getCSD()
{
return this->clientSocketDescriptor;
}
void Response::write(const char *s) const
{
if(this->isClose) return;
strcat(this->tmp,s);
}
char * Response::makeResponse()
{
sprintf(this->responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\n\n",strlen(this->tmp));
strcat(this->responseBuffer,this->tmp);
return this->responseBuffer;
}
void Response::sendResponse()
{
sprintf(this->responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\n\n",strlen(this->tmp));
strcat(this->responseBuffer,this->tmp);
send(this->clientSocketDescriptor,this->responseBuffer,strlen(this->responseBuffer),0);
}
void Response::sendResponse(const char *rb) const
{
strcpy(this->responseBuffer,rb);
strcat(this->responseBuffer,this->tmp);
send(this->clientSocketDescriptor,this->responseBuffer,strlen(this->responseBuffer),0);
}
void Response::sendResponse(const char *rb,const char *tmp) const
{
strcpy(this->responseBuffer,rb);
strcpy(this->tmp,tmp);
strcat(this->responseBuffer,this->tmp);
send(this->clientSocketDescriptor,this->responseBuffer,strlen(this->responseBuffer),0);
}
void Response::close()
{
closesocket(this->clientSocketDescriptor);
this->isClose=1;
}
int Response::isClosed()
{
return this->isClose;
}
Response::~Response()
{
free(this->responseBuffer);
free(this->tmp);
this->responseBuffer=NULL;
this->tmp=NULL;
}


REQUEST * parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
method[i]='\0';
i+=2;
char **data=NULL;
int dataCount=0;
if(strcmp(method,"GET")==0)
{
// for "whatever?fnsjkd=fsndjk&nfskdn=ncjsd&dsdji=bfsjdw"
for(j=0;bytes[i]!=' ';j++,i++)
{
if(bytes[i]=='?') break;
resource[j]=bytes[i];
}
resource[j]='\0';
if(bytes[i]=='?')
{
i++;
int si=i; // si for starting point
dataCount=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&') dataCount++;
i++;
}
dataCount++;
data=(char **)malloc(sizeof(char *)*dataCount);
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
int j=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],bytes+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}
}
} // method is of GET type
printf("Request arrived for %s\n",resource);
REQUEST *request=(REQUEST *)malloc(sizeof(REQUEST));
request->dataCount=dataCount;
request->data=data;
request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideResource(resource);
request->mimeType=getMIMEType(resource);
}
return request;
}
//----------------------------------------------------------------------------------------
TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
this->urlManager=NULL;
}
TMWebProjector::~TMWebProjector()
{
//if(this->url) delete [] this->url;
// something needs to be done here.
}

void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{
if(url==NULL || ptrOnRequest==NULL) return;
URL *tmp=(URL *)malloc(sizeof(URL));
tmp->url=(char *)malloc(sizeof(char)*strlen(url));
strcpy(tmp->url,url);
tmp->ptrOnRequest=ptrOnRequest;
tmp->next=NULL;
this->addInList(tmp);
}

void TMWebProjector::addInList(URL *url) // singly linked list code 
{
if(this->urlManager==NULL) // urlManager is treated as start of list
{
this->urlManager=url;
return;
}
URL *p;
for(p=urlManager;p->next!=NULL;p=p->next);
p->next=url;
}

// I replaced all return 0; with return; in the following method
void TMWebProjector::start()
{
FILE *f;
int i,rc;   // rc for read count
int length;
char g;
char responseBuffer[1024]; // a chunk of 1024 as fread will be used
char requestBuffer[8192]; // 8KB(8x1024)=8192
WORD ver;
WSADATA wsaData;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int bytesExtracted;
int successCode;
int len;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData); // socket library initialized
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
char message[101];
if(successCode<0)
{
sprintf(message,"Unable to bind socket to port %d",this->portNumber);
printf("%s\n",message);
WSACleanup();
return;
}
listen(serverSocketDescriptor,10);
len=sizeof(clientSocketInformation);
while(1)
{
sprintf(message,"TMServer is ready to accept request on port %d",this->portNumber);
printf("%s\n",message);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept client connection\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
csd=clientSocketDescriptor;
if(bytesExtracted<0)
{
// what to do is yet to be decided
}
else if(bytesExtracted==0)
{
// what to do is yet to be decided
}
else
{
requestBuffer[bytesExtracted]='\0';
REQUEST *request=parseRequest(requestBuffer);
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL) // if there is just '/' in request
{
f=fopen("index.html","rb");
if(f!=NULL) printf("Sending index.html\n");
if(f==NULL)
{
f=fopen("index.htm","rb");
if(f!=NULL) printf("Sending index.htm\n");
}
if(f==NULL)
{
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:163\nConnection: close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
send(clientSocketDescriptor,responseBuffer,sizeof(responseBuffer),0);
printf("Sending 404 page\n");
}
else
{
fseek(f,0,2);  // move the internal pointer to the end of file
length=ftell(f);
fseek(f,0,0);  // move the internal pointer to the start of file
//sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\nKeep-Alive: timeout=5, max=1000\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;    // rc for read count
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
}
}
else   // resource name is present
{
f=fopen(request->resource,"rb");
if(f!=NULL) printf("Sending %s\n",request->resource);
if(f==NULL)
{
// something needs to be done over here
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,sizeof(responseBuffer),0);
}
else
{
fseek(f,0,2);  // move the internal pointer to the end of file
length=ftell(f);
fseek(f,0,0);  // move the internal pointer to the start of file
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\nKeep-Alive: timeout=5, max=1000\n\n",request->mimeType,length);
//sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;    // rc for read count
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
}
}
}
else
{
// more changes required in context to server side resource
int anyNull=0;
for(URL *p=this->urlManager;p!=NULL;p=p->next)
{
if(p->url==NULL || p->ptrOnRequest==NULL) anyNull=1;
}
//if(this->url==NULL || this->ptrOnRequest==NULL)
if(anyNull)
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,sizeof(responseBuffer),0);
}
else
{
int ii=0;
int anyMatch=0;
URL *tmp;
//if(this->url[0]=='/') ii=1;
//if(strcmp(this->url+ii,request->resource)==0) 
for(tmp=this->urlManager;tmp!=NULL;tmp=tmp->next)
{
if(tmp->url[0]=='/') ii=1;
if(strcmp(tmp->url+ii,request->resource)==0)
{
anyMatch=1;
break;
}
}
if(anyMatch)
{
/*
Code to create object of Request class named as rq (you will have to create class Request)
It should act as wrapper to structure created from REQUEST whose address is in pointer
named as request and methods through which information from request structure can be extracted
It should also act as wrapper to dataCount and data
And it should have methods to extract data by name
get("nm"), if nm does not exist then return empty string

Code to create an object of Response class named as rs (you will have to create class Response)
wrap the necessary parts in response object so that
the write method of the response class should be capable of sending response to client
The close method of the response class should end the connection

Change the following line
this->ptrOnRequest(request->dataCount,request->data);
it should now be
this->ptrOnRequest(rq,rs);
*/
cout<<"URL : "<<tmp->url<<endl;
Request rq(request);
Response rs;
rq.setUrlManager(this->urlManager);
tmp->ptrOnRequest(rq,rs);
/*if(request->data!=NULL)
{
for(int k=0;k<request->dataCount;k++) free(request->data[k]);
free(request->data);
}*/
printf("Sending processed page\n");
char tmp[501];
/*
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s processed</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,sizeof(responseBuffer),0);
*/
rs.setCSD(clientSocketDescriptor);
if(rs.isClosed()) rs.sendResponse();
clientSocketDescriptor=rs.getCSD();
}
else
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,sizeof(responseBuffer),0);
}
}
}
}
}    // the infinite loop related to accept method ends here 
/*if(successCode>0)
{
printf("Response sent\n");
}
else
{
printf("Unable to send response\n");
}*/
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}




int FindAtIndex(const char *p,const char *q)
{
const char *a,*b,*j;
j=p;
while(1)
{
a=j;
b=q;
while(*a && *a!=*b) a++;
if(*a=='\0') return -1; // not found
j=a;
while(*a && *b && *a==*b)
{
a++;
b++;
}
if(*b=='\0') return j-p;
if(*a=='\0') return -1; // not found
j++;
}
return -1; // not found
}

char * findAndReplace(const char* oldData,const char* toReplace,const char* replaceWith)
{
char* newData;
int i,cnt=0;
int lr=strlen(replaceWith);
int lt=strlen(toReplace); 
for(i=0;oldData[i]!='\0';i++) 
{
if(strstr(&oldData[i],toReplace)==&oldData[i])
{
cnt++;
i+=lt-1;
}
}
newData=(char*)malloc(i+cnt*(lr-lt)+1);
i=0;
while(*oldData)
{
if(strstr(oldData,toReplace)==oldData)
{
strcpy(&newData[i],replaceWith);
i+=lr;
oldData+=lt;
}
else
newData[i++]=*oldData++;
}
newData[i]='\0';
return newData;
}

void findAndReplaceOnlyOnce(const char *p,const char *q,const char *r)
{
const char *p1,*p2,*p3,*p4,*q1,*r1;
int e;
p1=p;
char result[MAX_LINE_LENGTH];
while(*p1)
{
q1=q;
r1=r;
e=0;
while(*p1 && *p1!=*q1) p1++;
p2=p1;
while(*p1==*q1 && *p1 && *q1)
{
p1++;
q1++;
}
if(*q1=='\0')
{
p3=p;
while(p3<p2)
{
result[e]=*p3;
e++;
p3++;
}
while(*r1)
{
result[e]=*r1;
r1++;
e++;
}
while(*p1)
{
result[e]=*p1;
e++;
p1++;
}
result[e]='\0';
}
}
strcpy((char *)p,result);
}
int findOccurenceOfString(const char *e,const char *f)
{
const char *p,*q;
int count=0;
p=e;
while(*p)
{
q=f;
while(*p && *p!=*q) p++;
while(*p && *q && *p==*q)
{
p++;
q++;
}
if(*q=='\0') count++;
}
return count;
}
void searchAndReplaceinFile(const char *inputFile,const char *outputFile,const char *pattern,const char *replacement)
{
FILE *input=fopen(inputFile,"r");
FILE *output=fopen(outputFile,"w");
if(input==NULL || output==NULL) 
{
printf("Error opening file");
return;
}
char line[MAX_LINE_LENGTH];
while(fgets(line,sizeof(line),input)) 
{
int i=findOccurenceOfString(line,pattern);
while(i>0)
{
findAndReplaceOnlyOnce(line,pattern,replacement);
i--;
}
fputs(line,output);
}
fclose(input);
fclose(output);
// Remove the original input file
//remove(inputFile);
// Rename the temporary output file to the original input file name
//rename(outputFile,inputFile);
}
#endif
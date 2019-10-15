#include <bits/stdc++.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/stat.h>
#include <openssl/sha.h>
using namespace std;
pthread_t thread1[10],thread2[10];
int listenfd = 0,connfd[10]={0};
struct sockaddr_in serv_addr; 
char readBuff[512];
char sendBuff[512];
vector<int>lgout;
int temp; 
map<string,string> logi;
map<int,string> conn;
map<string,int>conn2;
map<string,string> listreq;
map<string,string>userfilename;
map<string,string>curruser;
vector< pair <string,string> > grp; 
vector< pair <string,string> > req; 
  
void createuser(char *pnt,int *p)
{
	 ofstream fout; 
	 char split[]=" ";
	 pnt=strtok(NULL,split);
	 fout.open("Password.txt",ios::app); 
	 string s(pnt);
    // fout<<s<<" "; 
	 conn.insert(make_pair(connfd[*p],s));
	 conn2.insert(make_pair(s,connfd[*p]));
	 pnt=strtok(NULL,split);
	 string k(pnt);
	 //fout<<k<<endl;
	 string z=s+" "+k;
	 fout<<z<<endl;
	 
	 logi.insert(make_pair(s,k));
	 write(connfd[*p],"created user\n",20);
	 
     
}
void create_group(char *pnt,int *p)
{
	ofstream fout,fout1;
	char split[]=" ";
	pnt=strtok(NULL,split);
	fout.open("Group.txt",ios::app);
	string s(pnt);
	int z1=connfd[*p];
	string z=conn[z1];
	string t=z+" "+s;
	fout<<t<<endl;
	
	string k=s+"mem"+".txt";
	 k=k.substr(0,k.size()-1);
	grp.push_back(make_pair(z,s));
	//k=k.substr(0,k.size()-1);
	fout1.open(k,ios::app);
	z=z+" "+"owner";
	fout1<<z<<endl;
	//fout<<z;
	write(connfd[*p],"created Group\n",20);
	fout.close();
	fout1.close();
	//fout1.open(s,ios::app);
	/*int z1=connfd[*p];
	string z=conn[z1];
	fout<<z<<endl;*/
}
void join_group(char *pnt,int *p)
{
	ofstream fin;
	fin.open("ListRequest.txt",ios::app);
	
		
	char split[]=" ";
	pnt=strtok(NULL,split);
	string s(pnt);
	//fin.open(s,ios::app);
	string z=conn[connfd[*p]];
	string k=z+" "+s;
	 fin<<k<<endl;
	req.push_back(make_pair(z,s));
	 fin.close();
	//fin<<z<<endl;
}
void accept_request(char *pnt,int *p)
{
	char split[]=" ";
	char buf[512];
	pnt=strtok(NULL,split);
	string gid(pnt);
	pnt=strtok(NULL,split);
	string uid(pnt);
	grp.push_back(make_pair(uid,gid));
	string z=gid+"mem"+".tx";
	ofstream fin;
	fin.open(z,ios::app);
	fin<<uid<<endl;
	grp.push_back(make_pair(uid,gid));
	 vector< pair <string,string> > ::iterator it;
	
	string z2(pnt);
	string name=conn[connfd[*p]];
	 it=find(req.begin(),req.end(),make_pair(uid,gid));
	 req.erase(it);
}
	
void leave_group(char *pnt,int *p)
{
	char split[]=" ";char buf[512];
	pnt=strtok(NULL,split);
	
    vector< pair <string,string> > ::iterator it;
	//char split[]=" ";
	//pnt=strtok(NULL,split);
	string z2(pnt);
	string name=conn[connfd[*p]];
	 it=find(grp.begin(),grp.end(),make_pair(name,z2));
	 req.erase(it);
	 write(connfd[*p],"left group Successfully\n",40);
}
void list_requests(char *pnt,int *p)
{
	char split[]=" ";char char_Array[200];
	pnt=strtok(NULL,split);
	string z(pnt);string k="";
	vector< pair <string,string> > ::iterator it;
	for(it=req.begin();it!=req.end();it++)
	{
		if(it->second==z)
			k=k+it->first+" ";
	}
	strcpy(char_Array, k.c_str());
	 write(connfd[*p],char_Array,200);
}
void lists_files(char *pnt,int *p)	
{
	char split[]=" ";char char_Array[200];
	pnt=strtok(NULL,split);
	string z(pnt);string temp="",s="",k="",name="";
	z=z+".txt";
	ifstream fin;
	fin.open(z,ios::in);
	while(fin)
	{
		//cout<<"hellolistfiles\n";
		getline(fin,s);
		int l=s.length();
		int i;
		for(i=0;i<l;i++)
		{
			if(s[i]!=' ')
			{
				k=k+s[i];
			}
			else
				break;
		}
		cout<<k<<endl;
		int len=k.length();string temp="";
		for(i=len-1;i>=0;i--)
		{
			 if(s[i]!='/')
				temp=temp+k[i];
			else
				break;
		}
		reverse(temp.begin(),temp.end());
		name=name+temp+'\n';
		k="";
		temp="";
		s="";
	}
	strcpy(char_Array,name.c_str());
	 write(connfd[*p],char_Array,500);
	
	
}
	
	

		
void download_file(char *pnt,int *p)
{
	         //cout<<"lol\n";
	         ifstream fin,fin2;
			 
	         char split[]=" ";
	         pnt=strtok(NULL,split);
			 string s(pnt);
			 pnt=strtok(NULL,split);
			 string fname(pnt);
			 pnt=strtok(NULL,split);
			 cout<<fname<<endl;
			 string destpath(pnt);
			 s=s+".txt";
			 fin.open(s,ios::in);
			// cout<<s<<endl;
			 string k,z="",temp="",finame="";int i,j,f=0,flg=0;
			 while(fin)
			 {
				// cout<<"hello\n"<<endl;
				 getline(fin,z);
				 int l=z.length();
				 for(i=0;i<l;i++)
				 {
					 if(z[i]==' ')
						 break;
					 if(z[i]!=' ')
						 k=k+z[i];
				 }
				 
				 finame=k;
				 int len=k.length();
				 string name="";
				
				   for(int i=len-1;i>=0;i--)
                   {
					   if(k[i]=='/')
						   break;
                      if(k[i]!='/')
                      name=name+k[i];
                    }
					//cout<<"lol"<<endl;
					//cout<<name<<endl;
                    reverse(name.begin(),name.end());
					//cout<<name<<endl;
				   if(fname==name)
				   {
					   for(j=i+1;j<l;j++)
					   {
						   if(z[j]==' ')
							   break;
						   f=1;
						   if(z[j]!=' ')
						   temp=temp+z[j];
					   }
				   }
				   if(f==1)
					   break;
				   k="";
				   z="";
				   name="";
			 }
			 if(f==0)
			 {
				  write(connfd[*p],"GROUP NOT CONTAIN FILE\n",40);
				  return;
			 }
			// close(fin);
			 cout<<"temp=\n"<<temp<<endl;			 
			 k="";
			 f=0;
			 string comp="",final1="";
			 
        //string k,s;
    //getline(fin,k);
             string jus;
	         fin2.open("tracker_info.txt",ios::in);
			 getline(fin2,comp);
			// cout<<jus<<endl;
			int m=1;
			 while(fin2)
			 {
				 string c10;
				// cout<<"hellowhatsup\n"<<endl;
				 
				 //cout<<"common\n"<<comp<<endl;
				 int l=comp.length();
				int i,j;string temp1="";
				m=2;
				for(i=0;i<l;i++)
				{
					
					if(comp[i]!=' ')
						temp1=temp1+comp[i];
					else
						break;
				}
				//cout<<"lol\n"<<temp1<<endl;
				  if(temp1==temp)
				  {
					//  cout<<"hii\n";
					  for(j=i+1;j<l;j++)
					  {
						  if(comp[j]!=' ')
						  {
							  f=1;
							  final1=final1+comp[j];
						  }
						 
					  }
					  cout<<final1<<endl;
						 if(f==1)
						 break;
				  }
						  temp1="";
						getline(fin2,comp);
			 }						  
					  
					 
             //string match;			 
	         //string k,s;
		     //getline(fin,k);
			 
			 
			 // int i=0;
			
			int k5=conn2[temp];
			 cout<<"filename"<<fname<<endl;
			 string nam="";
			 string test="filename";
			 nam=nam+test+" "+finame;
			
			// cout<<conn2[uname]<<endl;
			 char arr[512];
			 strcpy(arr,nam.c_str());
			  write(connfd[k5],arr,512);
			
			
			 z="";
			// string z="";
			 z=z+"port"+" "+final1+" "+destpath;
			// cout<<z<<" HELLO\n";
			 int l2=z.length();
			 char char_array[l2+1]; 
             strcpy(char_array, z.c_str()); 
  
			  write(connfd[*p],char_array,30);
			  
			  
}

void logout1(char *pnt,int *p)
{
	//cout<<"logout\n";
	int n=connfd[*p];
	string z=conn[n];
	logi.erase(z);
	 lgout.push_back(n);
	write(connfd[*p],"logoutSuccessfully\n",30);
}
void login1(char *pnt,int *p)
{
	char split[]=" ";
	pnt=strtok(NULL,split);
	string s(pnt);
	pnt=strtok(NULL,split);
    string k(pnt);
	if(logi.find(s)!=logi.end())
	{
	  
      conn.insert(make_pair(connfd[*p],s));
	  write(connfd[*p],"Successfull Login\n",30);
	}
    else
	write(connfd[*p],"Unsuccessfull login\n",30);
  
}	
void list_groups(char *pnt,int *p)
{
	//cout<<"hello2\n";
	struct stat file;
	stat("Group.txt",&file);
	int size=file.st_size;
	char *buff=(char *)(malloc(size));
	int fd=open("Group.txt",O_RDWR,0666);
	read(fd,buff,size);
	close(fd);
	//send(connfd[*p],buff,size,0);
	write(connfd[*p],buff,size);
}
			
void upload(char *pnt,int *p)
{
	int temp=*p;
  //char split[]=" ";
  string name="";
  char split[]=" ";
  pnt=strtok(NULL,split);
  string s(pnt);
  int len=s.length();
  for(int i=len-1;i>=0;i--)
  {
	  if(s[i]!='/')
	  name=name+s[i];
  }
       SHA_CTX ctx;
       SHA1_Init(&ctx);

         unsigned char hash[SHA_DIGEST_LENGTH];
         char char_array[512],buf[512],chararr[512];
         strcpy(char_array, s.c_str());
	     int sour = open(char_array,O_RDONLY);
	  while (1)
	  {
	    //cout<<"COMMON INDI\n";
		int in = read(sour, buf,512);
		SHA1_Update(&ctx,buf,in);
		if (in <= 0) break;
		//write(connfd, buf,in);
	   
	  }
	  struct stat file;
	  strcpy(chararr,s.c_str());
	  stat(chararr,&file);
	  int size=file.st_size;
	  string leng=to_string(size);
	  SHA1_Final(hash, &ctx);//string mytext;
	  reverse(name.begin(),name.begin());
	  string mytext(reinterpret_cast<char*>(hash));
	  pnt=strtok(NULL,split);
	  string t(pnt);
	 
	  cout<<t<<endl;
	  cout<<t.size()<<endl;
	  t=t.substr(0,t.size()-1);
	  t=t+".txt";
	   
	  ofstream fout;
	  fout.open(t,ios::app);
	 // int z1=connfd[*p];
	  string z=conn[connfd[temp]];
	//  cout<<"maing"<<endl<<z<<endl;
	  string t1=s+" "+z+" "+leng+" "+mytext;
	  
	  fout<<t1<<endl;
	  fout.close();
	  write(connfd[*p],"uploadedsuccessfully\n",30);
  
  
}

void *rd(void *arg)
{
	vector<int>::iterator it; 
	 
	char split[]=" ", filepath[100];
	char *pnt;
	int *p=(int *)(arg);
	int f=0;
	while(1)
	{
		it=find (lgout.begin(),lgout.end(),connfd[*p]);
		read(connfd[*p],readBuff, 100);
	//	int n=connfd[*p];
	  //  string z=conn[n];
	   
			
		printf("Client send:%s\n", readBuff);
		pnt = strtok(readBuff, split);
		//cout<<pnt;
		printf("%s\n", pnt);
		
		if(conn.empty()==1&&strcmp(pnt,"login")!=0&&strcmp(pnt,"create_user")!=0||(it!=lgout.end()&&strcmp(pnt,"login")!=0))
		{
			//cout<<"lol";
			write(connfd[*p],"Errorloginfirst\n",30);
			continue;
			//f=0;
		}
	
		if(strcmp(pnt,"upload_file")==0)
		{
			upload(pnt,p);
			bzero(readBuff,255);
			
		}
		
			
		else if(strcmp(pnt,"create_user")==0)
		{
			createuser(pnt,p);
			bzero(readBuff,255);
			
		}
		else if(strcmp(pnt,"login")==0)
		{
			//cout<<"hello\n";
			login1(pnt,p);
			bzero(readBuff,255);
			continue;
		}
		else if(strcmp(pnt,"logout")==0)
		{
			cout<<"logout\n";
			logout1(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"create_group")==0)
		{
			//cout<<"HELLO\n";
			create_group(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"join_group")==0)
		{
			join_group(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"download_file")==0)
		{
			download_file(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"list_groups")==0)
		{
			list_groups(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"list_files")==0)
		{
			lists_files(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"leave_group")==0)
		{
			leave_group(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"accept_request")==0)
		{
			accept_request(pnt,p);
			bzero(readBuff,255);
		}
		else if(strcmp(pnt,"list_requests")==0)
		{
			list_requests(pnt,p);
			bzero(readBuff,255);
		}
		else
		{
			write(connfd[*p],"Command not Found\n",30);
			//cout<<"heelo";
			
			//int k=stoi(s);
		//s	write(connfd[],readBuff,100);
		     bzero(readBuff,255);
	    }
	}
}
void *wrt(void *arg)
{
	while(1)
	{
		int *p=(int *)(arg);
		fgets(sendBuff,255,stdin);
		//write(connfd[*p],sendBuff,10);
		if(strcmp(readBuff,"Bye")!=0)
		{
			//close(connfd[*p]);
		}
	}
}

int main(int argc, char *argv[])
{
	remove("tracker_info.txt");
	ifstream fin,fout;
	fin.open("Password.txt", ios::in);
	string s="",t="",z="";
	map<string,string>::iterator itr;
	itr=logi.begin();
	while(fin)
	{
		getline(fin,s);
		//cout<<s<<endl;
		
		for(int i=0; i<s.length(); i++)
		{
			if(s[i]!=' ')
				t+=s[i];
			else
			{
				z=t;
				t="";
			}
		}
		if(s.length()>1&&s!=" ")
		logi.insert(make_pair(z,t));
	     //cout<<itr->first<<endl;
		 //itr++;
		 s="";
		 z="";
		 t="";
	}
	string a="",b="",c="";
	fout.open("ListRequest.txt",ios::in);
	//itr2=req.begin();
	
	//fout.close();
	//map<string,string>::iterator itr;
	/*for(itr=logi.begin();itr!=logi.end();itr++)
	{
		//fflush(stdout);
		cout<<itr->first<<" "<<itr->second<<endl;
	}*/
	
	pthread_attr_t initi1,initi2;
	pthread_attr_init(&initi1);
	pthread_attr_init(&initi2);
	int *p;int portno;
   	listenfd = socket(AF_INET, SOCK_STREAM, 0);
   	memset(&serv_addr, '0', sizeof(serv_addr));
   	memset(sendBuff, '0', sizeof(sendBuff)); 
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	portno=atoi(argv[1]);
    serv_addr.sin_port = htons(portno); 
	
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	
    listen(listenfd, 10); 
	int i=0;
	while(i<10)
	{
	 connfd[i] = accept(listenfd, (struct sockaddr*)NULL, NULL);
	 long *p=(long *)malloc(sizeof(long));
	 p[0]=i;
	 pthread_create(&thread1[i], &initi1,rd,(void *)p);
	pthread_create(&thread2[i], &initi2,wrt,(void *)p);
	 i++;
	}
	     i=0;
		while(i<10)
		{
	
       	 pthread_join(thread1[i],NULL);
	     pthread_join(thread2[i],NULL);
		
         close(connfd[i]);
		}
}
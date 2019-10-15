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
#include <openssl/sha.h>
#define SIZE 512
using namespace std;
int sockfd = 0,sockfd1, n = 0,connfd=0;
char recvBuff[512];int temp,serport;
struct sockaddr_in serv_addr;
char sendBuff[512];char *ipadd="127.0.0.1";
pthread_t thread0,thread1,thread2,thread3;string file_name,file_name1,dest_path;
void *client1(void *arg);

void *rd(void *arg)
{
	while(1)
	{

	 char *ptr, split[] = " ";
	 read(sockfd,recvBuff,512);
	 printf("Server send:%s\n",recvBuff);
	 ptr=strtok(recvBuff,split);
	 if( strcmp(ptr, "port") == 0 )
	 {
		    int *p;
			pthread_attr_t initi3;
			pthread_t thread3;
			pthread_attr_init(&initi3);
			string s(recvBuff);
			cout<<s<<endl;
			ptr = strtok(NULL, split);
			int port = atoi(ptr);
            
			p=&port;

			pthread_create(&thread3,&initi3,client1,(void *)p);
			pthread_join(thread3,NULL);
	   
	 }
	 if(strcmp(ptr,"filename")==0)
	 {	
       // cout<<"amrit\n";
		//file_name="Group.txt";
		ptr=strtok(NULL,split);

		file_name=string(ptr);
		cout<<file_name<<endl;
	
	  }
	    bzero(sendBuff,SIZE);
		bzero(recvBuff,SIZE);
		


	}
}

void *rdfile(void *arg)
{
	int f=1;
	
	
	char char_array[512];
	cout<<file_name1<<endl;
	string k=dest_path+'/'+file_name1;
	
	

    
	strcpy(char_array, k.c_str());
	int destin = creat(char_array,0666);

	while(1)
	{
	   cout<<"DOWNLOADING\n";
	    int inp=read(sockfd1, recvBuff, SIZE);
		
		if(inp<=0)
		break;
		int outp=write(destin,recvBuff,inp);
		if(outp<=0)
		break;
	}
	 
        cout<<"Downloaded Successfully\n";
	close(sockfd1);

}
void *client1(void *arg)
{
	int *param = (int *)arg;
	int port = param[0];

	cout<<port<<endl;
	struct sockaddr_in serv_addr;
	
	pthread_attr_t initi1,initi2;
	pthread_attr_init(&initi1);
   
    int *p;
    if((sockfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
    }

      memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	
	printf("Server address used is: %s\n", ipadd);
    if(inet_pton(AF_INET, ipadd, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
    }

    if( connect(sockfd1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
         printf("\n Error : Connect Failed \n");
    }

	pthread_create(&thread1,&initi1,rdfile,(void *)p);
	

	pthread_join(thread1,NULL);
	


}

void *wrt(void *arg)
{
	char split[]=" ", filepath[100];
	char *ptr;
	int in,out,sour;
	while(1)
	{
		fgets(sendBuff,100,stdin);

		write(sockfd,sendBuff,100);
		ptr = strtok(sendBuff, split);

		if(strcmp(ptr,"login")==0)
		{
				ofstream fout;
				char split[]=" ";
				ptr=strtok(NULL,split);
				fout.open("tracker_info.txt",ios::app);
				string s(ptr);
				fout<<s<<" ";
				 // int n=5000;
				string k=to_string(serport);
				fout<<k<<" "<<endl;
		}

		else if(strcmp(ptr, "download_file") == 0 )
		{
			ptr = strtok(NULL, split);
			ptr=strtok(NULL,split);
			string z(ptr);
			file_name1=z;
			ptr=strtok(NULL,split);
			string k(ptr);
			dest_path=k;
			cout<<dest_path<<endl;
		}

    }
	//close(sockfd);
}
void *writefile(void *arg)
{
		//cout<<"hello\n";
		char buf[512]; char char_array[512];
	    strcpy(char_array, file_name.c_str());
	    int sour = open("char_array",O_RDONLY);
	  while (1)
	  {
	    cout<<"SENDING\n";
		int inp = read(sour, buf,512);
		if (inp <= 0) break;
		write(connfd, buf,inp);
	   
	  }
	  cout<<"SENT Successfully\n";
      close(connfd);
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
		int inp = read(sour, buf,512);
		SHA1_Update(&ctx,buf,inp);
		if (inp <= 0) break;
		
	   
	  }
	    SHA1_Final(hash, &ctx);
}

void *client(void *arg)
{
	struct sockaddr_in serv_addr;
	int *port= (int *)arg;
	

     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
    }

	
    memset(&serv_addr, '0', sizeof(serv_addr));
    pthread_attr_t initi1,initi2;
	pthread_attr_init(&initi1);
    pthread_attr_init(&initi2);
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(*port);
	printf("Server address used is: %s\n", ipadd);
    if(inet_pton(AF_INET, ipadd, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
      printf("\n Error : Connect Failed \n");
    }
	int *p;
	pthread_create(&thread1,&initi1,rd,(void *)p);
	pthread_create(&thread2,&initi2,wrt,(void *)p);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);


}
int main(int argc, char *argv[])
{

	pthread_attr_t clien,thread1;
	pthread_attr_init(&clien);
	srand( time(0) );
	serport = rand()%10000+1024;
    int port = atoi(argv[2]);
	int *por=&port;
	pthread_create(&thread0,&clien,client,(void *)por);
	
	
	
	
    pthread_t threadc;
	struct sockaddr_in serv_addr;
	
	pthread_attr_t initi1,initi2;
	pthread_attr_init(&initi1);
	pthread_attr_init(&initi2);

     int i = 0;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(serport);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    int *p;
	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		pthread_create(&threadc, &initi1,writefile,(void *)p);
	}
     pthread_join(threadc,NULL);
     close(connfd);
	pthread_join(thread0,NULL);

	
}
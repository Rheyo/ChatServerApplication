#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<ctype.h>
using namespace std;

#define BUFFER 100

char mysocket[] = "mysocket";
int cfd=-1,flag=0,f=0;

int parse(char arr[],int len, char str1[20], char str2[20], char str3[20]){
	int space=0;
	int i,j=0,k=0,l=0;
	for(i=0;i<len;i++){
		if(arr[i]==' ')space++;
		if(space==0)str1[j++]=arr[i];
		if(space==1)str2[k++]=arr[i+1];
		if(space>=2)str3[l++]=arr[i+1];
	}
	return space;
}

void registerme(char com[100],int myfd)
{

	if(f==1)
		cout<<"Already registered"<<endl;
	else
	{
		
		write(myfd,com,strlen(com));
		read(myfd,com,BUFFER);
		if(strcmp(com,"0")==0){
			cout<<"Username already existing"<<endl;	
			return;
		}
		f=1;		
		cout<<com<<endl;
		return;
		
	
	}

}

void loginme(char com[BUFFER], int myfd)
{

	if(flag==1)
	{
		cout<<"Already Logged in"<<endl;
		return;
	}
	
	write(myfd,com,BUFFER);

	read(myfd,com,BUFFER);
	if(strcmp(com,"0")==0){
			cout<<"Username/password didnt match or banned"<<endl;	
	
	}
	if(strcmp(com,"1")==0){
		flag=1;
		cout<<"successfully logged in"<<endl;
	}
}	
int main(){

	cout<<"\033[2J\033[1;1H";//clrscr() for both linux and windows
	char str1[20],str2[20],str3[20];
	int myfd=socket(AF_LOCAL,SOCK_STREAM,0);
	struct sockaddr_un servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	strcpy(servaddr.sun_path,mysocket);
	cfd=connect(myfd,(sockaddr*) &servaddr, sizeof(servaddr));
	if(cfd==-1){
		cerr<<"Cannot connect"<<endl;
	}
	bzero(&str1,sizeof(str1));
	bzero(&str2,sizeof(str2));
	bzero(&str3,sizeof(str3));
	int len=0,space=0;
	char command[BUFFER];
	int max_fds;
	int res;
	cout<<"Hello Client!"<<endl;
	cout<<"WELCOME TO XSERVER ROOM"<<endl;
	cerr<<"*****COMMANDS ALLOWED*****"<<endl;
	cerr<<"register\nlogin\nsend\nlobbystatus\nlogout"<<endl;
	while(true){
		bzero(&str1,sizeof(str1));
		bzero(&str2,sizeof(str2));
		bzero(&str3,sizeof(str3));
		fd_set readset;
		max_fds=myfd;

		FD_ZERO(&readset);
		FD_SET(0,&readset);
		FD_SET(myfd,&readset);
			res=select(max_fds+1,&readset,0,0,0);
			if(res==-1)
				cout<<"error in select"<<endl;
			else{
				if(FD_ISSET(myfd,&readset))
				{
					bzero(&command,sizeof(command));
					read(myfd,command,sizeof(command));
						fputs(command,stdout);
						cout<<endl;
						if(strcmp(command,"close")==0){
							//cout<<endl;
							close(myfd);
							exit(EXIT_SUCCESS);
						}

				}
				if(FD_ISSET(0,&readset)){
					bzero(&command,sizeof(command));
					read(0,&command,sizeof(command));
					len=strlen(command);
					space=parse(command,len,str1,str2,str3);
					if(strcmp(str1,"register")==0)
					{	
						if(space!=2)
							cout<<"Invalid no. of parameters"<<endl;
						else
						{
							registerme(command,myfd);
							bzero(&command,BUFFER);
						}
					}
	
					else if(strcmp(str1,"login")==0)
					{
					
						if(space!=2)
							cout<<"Invalid no. of parameters"<<endl;
						else
							loginme(command,myfd);
			
					}		
					
					else if(strcmp(str1,"send")==0 && flag==1)
					{	
						if(space<2)
							cout<<"Invalid no. of parameters"<<endl;
						else
							write(myfd,command,sizeof(command));
					}
					else if((strcmp(str1,"lobbystatus\n")==0) && space==0 && flag==1)
					{
						write(myfd,command,sizeof(command));

					}
					else if(strcmp(str1,"logout\n")==0 && space==0 && flag==1){
						write(myfd,command,sizeof(command));						
					}
					
					else
						cout<<"Cannot execute this command"<<endl;
				}
				
			}
	}	

return 0;
}

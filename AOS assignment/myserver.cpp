#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/time.h>
using namespace std;

#define BUFFER 100
#define MAXMSG  512
#define pb push_back
#define FILENAME "users.data"
struct mydata{
	char username[20];
	char password[20];
	int status;
}data[100];

int ind=0;

vector<int> cons;
vector<int> onl_fd;
vector<string> uname;

char buf[BUFFER];

char mysocket[] = "mysocket";

void log(string tag,string msg)
{
   time_t currentTime=time(0);
   ofstream f;
   f.open("logfile.txt",ios::out|ios::app);

   char* dateTime = ctime(&currentTime);

   f<<tag<<": "<<msg<<" "<<dateTime;
   f.close(); 

}

int parse(char arr[],int len, char str1[20], char str2[20], char str3[20])
{
	int space=0;
	int i,j=0,k=0,l=0;
	for(i=0;i<len;i++){
		if(arr[i]==' ')space++;
		if(space==0)str1[j++]=arr[i];
		if(space==1)str2[k++]=arr[i+1];
		if(space>=2)str3[l++]=arr[i+1];
	}
	str3[l]='\0';
	str2[k]='\0';
	return space;
}
void lobbyst(int i)
{
	strcpy(buf,"Lobby Status:");
	write(i,buf,BUFFER);
	for(int j=0;j<onl_fd.size();j++){
		
		strcpy(buf,uname[j].c_str());
		//strcat(buf,"\n");
		write(i,buf,BUFFER);
	}
}
void display_registered_users()
{
	ifstream f;
	f.open(FILENAME,ios::binary);
	struct mydata usr;
	int FLAG=1;
	while(f.read((char*)&usr,sizeof(usr))>0)
	{
		cerr<<" "<<usr.username<<" "<<usr.password<<" "<<usr.status<<endl;
		FLAG=0;
	}
	if(FLAG==1)
		cerr<<"NO REGISTERED USER"<<endl;
	f.close();
}
void file_update(){
	ofstream f;
	f.open(FILENAME,ios::binary|ios::trunc);
	for(int i=0;i<ind;i++){
		f.write((char*)&data[i],sizeof(mydata));
	}
	f.close();
	return;
}
void initdb(){
	ifstream f;
	f.open(FILENAME,ios::binary);
	struct mydata usr;
	int i=0;
	while(f.read((char*)&usr,sizeof(usr))>0)
	{
		strcpy(data[i].username,usr.username);
		strcpy(data[i].password,usr.password);
		data[i].status=usr.status;
		i++;
	}
	ind=i;
	f.close();
	return;
}
void display(){
	int i=0;
	for(i=0;i<ind;i++)
	{
		cout<<" "<<data[i].username<<"  "<<data[i].password<<"  "<<data[i].status<<endl;
	}
	
	return;
}
void register_me(int index){
	ofstream f;
	f.open(FILENAME,ios::binary|ios::app);
	f.write((char*)&data[index],sizeof(mydata));
	f.close();
	return;
}
int main() {
				cout<<"XSERVER ROOM"<<endl;
				cout<<" Getting started......"<<endl;
				sleep(2);
				cerr<<"*****COMMANDS ALLOWED*****"<<endl;
				cerr<<"kick\nban\nunban\nsend\nbroadcast\nlogout "<<endl;

			    char tempo[20];
				int length;
				for(int x=0;x<100;x++)
					data[x].status=0;
				char str1[20];
				char str2[20],str3[20];
				onl_fd.reserve(10);
				uname.reserve(10);
				char tempvar2[BUFFER];
				int flagg=0;
				char tempvariable[BUFFER];
				cout<<"\033[2J\033[1;1H";//clrscr() for both linux and windows
				int k=1,space=0,temp=0,tempp,j=0,flag1=0,flag2=0,s=0,tempvar1=0;
				cons.pb(0);
				struct sockaddr_un servaddr,cliaddr;
				fd_set readset;
				int num_fds=0,i,max_fds,sd;
				int listenfd=socket(AF_LOCAL,SOCK_STREAM,0);
				int check;
				bzero(&servaddr, sizeof(servaddr));
				servaddr.sun_family=AF_LOCAL;
				strcpy(servaddr.sun_path,mysocket);

				unlink(mysocket);

				bind(listenfd,(sockaddr *) &servaddr, sizeof(servaddr));
				log("server","online");
				initdb();

				listen(listenfd,10);
				socklen_t len=sizeof(cliaddr);
				int connfd;

	
	while(true)
	{
			FD_ZERO(&readset);
			FD_SET(0,&readset);
			FD_SET(listenfd,&readset);
			max_fds=listenfd;
			
			for(i=0;i<cons.size();i++){
				if(i!=0){
					FD_SET(cons[i],&readset);
				}
				if(max_fds<cons[i])
					max_fds=cons[i];
			}
			
			int res=select(max_fds+1,&readset,0,0,0);
				if(res==-1)
					cout<<"error in select"<<endl;
				else
				{

					if(FD_ISSET(listenfd,&readset))
					{
						connfd=accept(listenfd,(sockaddr *) &servaddr, &len);
						cons.pb(connfd);k++;
		
					}
					
					if(FD_ISSET(0,&readset))
					{
						bzero(&buf,sizeof(buf));
						read(0,buf,sizeof(buf));
						bzero(&str1,sizeof(str1));
						bzero(&str2,sizeof(str2));
						bzero(&str3,sizeof(str3));
				 		space=parse(buf,strlen(buf),str1,str2,str3);
							if(strcmp(str1,"broadcast")==0)
							{
								bzero(&buf,sizeof(buf));
								char tempvarr[BUFFER];
								bzero(&tempvarr,sizeof(tempvarr));
								strcpy(tempvarr,"Server :");
								strcat(tempvarr,str2);
								strcat(tempvarr,str3);
								strcpy(buf,tempvarr);
								for(j=0;j<onl_fd.size();j++){
									write(onl_fd[j],buf,sizeof(buf));
								}
							}
							else if(strcmp(str1,"ban")==0)
							{
										bzero(&tempvariable,sizeof(tempvariable));
										flagg=0;
										length=strlen(str2);
										str2[length-1]='\0';
										for(j=0;j<ind;j++){
											strcpy(tempvariable,data[j].username);
											length=strlen(tempvariable);
											tempvariable[length-1]='\0';
											if(strcmp(str2,tempvariable)==0){
												data[j].status=1;
												flagg=1;
												break;
											}
										}
										bzero(&buf,sizeof(buf));
										if(flagg==0){
										
											strcpy(buf," SORRY!! No such user exists");
											cout<<buf<<endl;
											
											}
										

										else{
											bzero(&tempvar2,sizeof(tempvar2));
											strcpy(tempvar2,data[j].username);
											strcat(tempvar2," Banned");
											strcat(tempvar2,str3);
											s=0;
											tempvar1=0;
											
											for(j=0;j<uname.size();j++){
												strcpy(tempo,uname[j].c_str());
												length=strlen(tempo);
												tempo[length-1]='\0';
												if(strcmp(tempvariable,tempo)==0){
													tempvar1=onl_fd[j];
													break;
												}
											}
											if(tempvar1!=0){
												
												for(s=0;s<cons.size();s++){
													if(tempvar1==cons[s])
														break;	
												}
														strcpy(buf,"You are banned\n");
														write(tempvar1,buf,sizeof(buf));
														bzero(buf,sizeof(buf));
														strcpy(buf,"close");
														write(tempvar1,buf,sizeof(buf));
														FD_CLR(tempvar1,&readset);
								 						close(tempvar1);
								 						log(str2,str1);	
								 						cons.erase(cons.begin()+s);
							 							onl_fd.erase(onl_fd.begin()+j);
							 							uname.erase(uname.begin()+j);
			
											}
														file_update();	
								 					//	cout<<"file updated"<<endl;
								 					//	display_registered_users();
														for(j=0;j<onl_fd.size();j++){
															bzero(buf,sizeof(buf));
															strcpy(buf,tempvar2);
															write(onl_fd[j],buf,sizeof(buf));
															lobbyst(onl_fd[j]);									
														}
														
										}
						}

						else if(strcmp(str1,"unban")==0)
						{
							length=strlen(str2);
							str2[length-1]='\0';
							bzero(&tempvariable,sizeof(tempvariable));
							flagg=0;
						
							for(j=0;j<ind;j++){
								strcpy(tempvariable,data[j].username);
								length=strlen(tempvariable);
								tempvariable[length-1]='\0';
								if(strcmp(str2,tempvariable)==0 && data[j].status==1){
									data[j].status=0;
									flagg=1;
									break;
								}
							}
							bzero(&buf,sizeof(buf));
							if(flagg==0){
							
								strcpy(buf,"No such user exists/Not banned\n");
								write(0,buf,sizeof(buf));
							}
							else{
								tempvar2[BUFFER];
								bzero(&tempvar2,sizeof(tempvar2));
								strcpy(tempvar2,data[j].username);
								strcat(tempvar2," unbanned\n");
								cout<<tempvar2;
								
										for(j=0;j<onl_fd.size();j++){
											bzero(buf,sizeof(buf));
											strcpy(buf,tempvar2);
											write(onl_fd[j],buf,sizeof(buf));	
																					
										}
										file_update();	
								 		//cout<<"file updated"<<endl;
								 		//display_registered_users();
								 		log(str2,str1);		
									}
									
							
						}
						else if(strcmp(str1,"kick")==0)
						{
									tempvar1=0;
									tempo[20];
									length=strlen(str2);
									str2[length-1]='\0';
									for(j=0;j<onl_fd.size();j++){
										strcpy(tempo,uname[j].c_str());
										length=strlen(tempo);
										tempo[length-1]='\0';	
										if(strcmp(str2,tempo)==0){
											tempvar1=onl_fd[j];
											break;
										}
									}
									bzero(&buf,sizeof(buf));
									if(tempvar1==0){
									
										strcpy(buf,"No such user online");
										cout<<buf<<endl;
										}									
									else
									{	
										bzero(&tempvar2,sizeof(tempvar2));
										strcpy(tempvar2,uname[j].c_str());
										strcat(tempvar2," kicked out");
										strcat(tempvar2,str3);
										s=0;
										for(s=0;s<cons.size();s++){
											if(tempvar1==cons[s])
												break;
										}
										
										strcpy(buf,"You are kicked ");
										strcat(buf,str3);
										write(tempvar1,buf,BUFFER);
										bzero(buf,sizeof(buf));
										strcpy(buf,"close");
										write(tempvar1,buf,sizeof(buf));	
										FD_CLR(tempvar1,&readset);
							 			close(tempvar1);
							 			cout<<"Connection closed with client "<<str2<<endl;
							 			cons.erase(cons.begin()+s);
							 			onl_fd.erase(onl_fd.begin()+j);
							 			uname.erase(uname.begin()+j);
					 					for(j=0;j<onl_fd.size();j++){
											bzero(buf,sizeof(buf));
											strcpy(buf,tempvar2);
											write(onl_fd[j],buf,sizeof(buf));	
											lobbyst(onl_fd[j]);								
										}
								
									}
							
						}

						else if(strcmp(str1,"send")==0)
						{
									tempvar1=0;
									for(j=0;j<onl_fd.size();j++){
										if(strcmp(str2,uname[j].c_str())==0){
											tempvar1=onl_fd[j];
											break;
										}
									}
									bzero(&buf,sizeof(buf));
									if(tempvar1==0){
										strcpy(buf,"No such user online");
										cout<<buf<<endl;
										
									}
									else{
										char tempvar2[BUFFER];
										bzero(&tempvar2,sizeof(tempvar2));
										strcpy(tempvar2,"Server: ");
										strcat(tempvar2,str3);
										strcpy(buf,tempvar2);
										write(tempvar1,buf,sizeof(buf));
									}
						}
						else if(strcmp(str1,"logout\n")==0)
						{
										for(j=1;j<cons.size();j++){
											bzero(&buf,sizeof(buf));
						 					strcpy(buf,"Server logged out");
						 					write(cons[j],buf,sizeof(buf));
						 					bzero(buf,sizeof(buf));
											strcpy(buf,"close");
											write(cons[j],buf,sizeof(buf));	
						 					close(cons[j]);
						 				}
						 				close(listenfd);
						 				exit(EXIT_SUCCESS);

						}
					}
					for(i=1;i<cons.size();i++)
					{		
				
						if(FD_ISSET(cons[i],&readset))
						{	
									read(cons[i],buf,BUFFER);
									bzero(&str1,sizeof(str1));
									bzero(&str2,sizeof(str2));
									bzero(&str3,sizeof(str3));

							 		space=parse(buf,strlen(buf),str1,str2,str3);

							 		
							
							 		flag1=0,flag2=0;
							 		if(strcmp(str1,"register")==0)
							 		{
						
									 			for(j=0;j<ind;j++){
									 				if(strcmp(data[j].username,str2)==0)
									 				{
									 					strcpy(buf,"0");
									 					write(cons[i],buf,BUFFER);
									 					flag1=1;
									 					break;
									 				}
									 			}
									 			if(flag1==0){
									 				strcpy(data[ind].username,str2);
									 				strcpy(data[ind].password,str3);
									 				data[ind].status=0;
									 				register_me(ind);
									 				ind++;
									 				bzero(&buf,sizeof(buf));
									 				strcpy(buf,"Successfully registered");
									 				write(cons[i],buf,BUFFER);
													log(str2,str1);	
												//	cout<<"file:"<<endl;
													//display_registered_users();		
													//cout<<"data:"<<endl;	
													//display();		
													//cout<<"out"<<endl;		
									 			}
									 			
					 				}
					 		
					 		else if(strcmp(str1,"login")==0)
					 		{	
							 			for(j=0;j<ind;j++){
							 				if((strcmp(data[j].username,str2)==0) && (strcmp(data[j].password,str3)==0) && (data[j].status==0))
							 				{
							 					
							 					flag2=1;
							 					break;
							 				}
							 			}
							 			if(flag2==0){
							 				bzero(&buf,sizeof(buf));
							 				strcpy(buf,"0");
							 				write(cons[i],buf,sizeof(buf));
							 			}
							 			if(flag2==1)
							 			{
							 				onl_fd.pb(cons[i]);
							 				uname.pb(str2);
							 				strcpy(buf,"1");
							 				write(cons[i],buf,sizeof(buf));
							 				for(j=0;j<onl_fd.size();j++){
												lobbyst(onl_fd[j]);								
											}
											log(str2,str1);		
							 			}

					 		}
							else if(strcmp(str1,"send")==0)
							{
											tempvar1=0;
											for(j=0;j<onl_fd.size();j++){
												if(strcmp(str2,uname[j].c_str())==0){
													tempvar1=onl_fd[j];
													break;
												}
											}
											bzero(&buf,sizeof(buf));
											if(tempvar1==0){
												
												strcpy(buf,"No such user online");
												write(cons[i],buf,sizeof(buf));
											}
											else{
												char tempvar2[BUFFER];
												bzero(&tempvar2,sizeof(tempvar2));
												for(j=0;j<onl_fd.size();j++){
													if(onl_fd[j]==cons[i]){
														strcpy(tempvar2,uname[j].c_str());
														break;
													}
												}

												strcat(tempvar2,":");
												strcat(tempvar2,str3);
												strcpy(buf,tempvar2);
												write(tempvar1,buf,sizeof(buf));
											}
							}
						
							else if(strcmp(str1,"lobbystatus\n")==0)
							{	
								for(j=0;j<onl_fd.size();j++){
									if(onl_fd[j]==cons[i])
										break;
								}
								lobbyst(onl_fd[j]);
						
							}
							else if(strcmp(str1,"logout\n")==0)
							{
										for(j=0;j<onl_fd.size();j++){
											if(cons[i]==onl_fd[j])
												break;
										}
										bzero(&buf,sizeof(buf));
							 			strcpy(buf,"Successfully logged out");
							 			write(cons[i],buf,sizeof(buf));
							 			bzero(buf,sizeof(buf));
										strcpy(buf,"close");
										write(cons[i],buf,sizeof(buf));	
							 		  	FD_CLR(cons[i],&readset);
							 			close(cons[i]);
							 			log(uname[j],str1);		
							 			cons.erase(cons.begin()+i);
							 			onl_fd.erase(onl_fd.begin()+j);
							 			uname.erase(uname.begin()+j);
							 			
    									
    								for(j=0;j<onl_fd.size();j++){
    									lobbyst(onl_fd[j]);
    								}	

					 		
							}
						}
					}	
				
			}	
		
	}
	return 0;
}
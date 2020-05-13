#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int mathbot(char problem[200]){
  char num1[11];
    char num2[11];
    char op;
    int i = 13;
    int j = 0;
    while(problem[i]!=' '){
        num1[j] = problem[i];
        i++;
        j++;
    }
    i++;
    op = problem[i];
    j=0;
    i++;
    i++;
    while(problem[i]!='\n'){
        num2[j] = problem[i];
        i++;
        j++;
    }
    int exspression1 = atoi(num1);
    int exspression2 = atoi(num2);
    switch (op) {
      case '+':
        return exspression1 + exspression2;
      case '-':
        return exspression1 - exspression2;
      case '/':
        return exspression1 / exspression2;
      case '*':
        return exspression1 * exspression2 ;
    }
}

int flagcheck(char flag[200]){
    int i = 6;
    while(flag[i]!=' '){
        i++;
    }
    if(flag[i+1]=='B'){
       if(flag[i+2]=='Y'){
            if(flag[i+3]=='E'){
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc , char *argv[])
{
  if(argc != 4){
    printf("Not enough arguments\n");
    exit(1);
  }

  int socket_desc;
	struct sockaddr_in server;
  char request[200];
  char* reply;
  char* ans;
  char* username = argv[1];
  int port = atoi(argv[2]);
  char* ip = argv[3];

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("connect error\n");
		return 1;
	}

	printf("Connected\n");

  //request first problem
  strcpy(request, "cs230 HELLO <");
  strcat(request, username);
  strcat(request, ">@umass.edu\n");
  if( send(socket_desc , request , strlen(request) , 0) < 0)
  {
    printf("Send failed\n");
    return 1;
  }
  printf("Data Send\n");



  while (1) {
    //Receive a reply from the server
    char reply_buf[200];
    if( recv(socket_desc, reply_buf , 200 , 0) < 0)
    {
      printf("recv failed\n");
    }
    printf("Reply received\n");


    //get reply -- print reply
    reply = reply_buf;
    printf("%s",reply);

    //check if flag is received
    if(flagcheck(reply_buf)){break;}

    //getting solution

    char mes_buf[200];
    int x = mathbot(reply_buf);
    int length = snprintf( NULL, 0, "%d", x );
    char str[length+1];
    snprintf( str, length + 1, "%d", x );
    sprintf(ans, "%i",mathbot(reply_buf));
    strcpy(mes_buf, "cs230 ");
    strcat(mes_buf, str);
    strcat(mes_buf, "\n");
    printf("%s",mes_buf );

    if( send(socket_desc , mes_buf , strlen(mes_buf) , 0) < 0)
    {
      printf("Send failed\n");
      return 1;
    }
    printf("Data Send\n");


  }
  close(socket_desc);
	return 0;
}

#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<sys/socket.h>  
#include<sys/types.h>  
#include<unistd.h>  
#include<netinet/in.h>  
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


static char *input_file_path;
static char *server_ip_addr;
static int  listen_port = 0;    

void check_args_client(int argc,char **argv)
{
	if (4 != argc)
	{
		printf("usage: %s <ip> <port> <input_file>\n", argv[0]);
		exit(0);
	}
	
	server_ip_addr = argv[1];
	listen_port = atoi(argv[2]);
	input_file_path = argv[3];
	printf("%s : %d : %s\n", server_ip_addr, listen_port, input_file_path);
}

int main(int argc,char **argv)  
{  
	check_args_client(argc, argv);
	
	
    int sockfd;  
    int err;  
    struct sockaddr_in addr_ser;  
    char sendline[1024];
	int send_n;
	
	FILE *fp = fopen(input_file_path, "rb+");
	if (!fp)
	{
		printf("can't open : %s, %s\n", input_file_path, strerror(errno));
		return -1;
	}	
      
    sockfd=socket(AF_INET,SOCK_STREAM,0);  
    if(sockfd==-1)  
    {  
        printf("socket error\n");  
        return -1;  
    }  
      
    bzero(&addr_ser,sizeof(addr_ser));  
    addr_ser.sin_family=AF_INET;  
    addr_ser.sin_addr.s_addr=inet_addr(server_ip_addr);  
    addr_ser.sin_port=htons(listen_port);  
    err=connect(sockfd,(struct sockaddr *)&addr_ser,sizeof(addr_ser));  
    if(err==-1)  
    {  
        perror("connect error ");  
        return -1;  
    }  
      
    printf("connected with server...\n");  
    
	int total_bytes = 0;
    while(1)  
    {  
        send_n = fread(sendline, 1, 1024, fp);
		if (send_n == 0)
		{
			printf("data send done: %d bytes sent\n", total_bytes);
			break;
		}
		else if (send_n < 0)
		{
			printf("file read err : %s\n", strerror(errno));
			return -1;
		}
		else
		{  	
			total_bytes += send_n; 
			printf("progress : %d, %d\n", send_n, total_bytes);
			if (send_n != send(sockfd,sendline,send_n,0))
			{
				printf("send error. %s\n", strerror(errno));
				return -1;
			}
		}
		
    }  
      
    fclose(fp);
	close(sockfd);
    return 0;  
}  
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

static char *output_file_path;
static int  listen_port = 0;    

void check_args_server(int argc,char **argv)
{
	if (3 != argc)
	{
		printf("usage: %s <port> <input_file>\n", argv[0]);
		exit(0);
	}
	
	listen_port = atoi(argv[1]);
	output_file_path = argv[2];
	printf("%d : %s\n", listen_port, output_file_path);
}

int main(int argc,char **argv)  
{
	check_args_server(argc, argv);
	
    int ser_sockfd, cli_sockfd;  
    int err, n;  
    socklen_t addlen;  
    struct sockaddr_in ser_addr;  
    struct sockaddr_in cli_addr;  
    char recvline[1024];
	
	FILE *fp = fopen(output_file_path, "wb+");
	if (!fp)
	{
		printf("can't open : %s, %s\n", output_file_path, strerror(errno));
		return -1;
	}

      
    ser_sockfd=socket(AF_INET,SOCK_STREAM,0);  
    if(ser_sockfd==-1)  
    {  
        printf("socket error:%s\n",strerror(errno));  
        return -1;  
    }
	
	int reuse0=1;
	if (setsockopt(ser_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse0, sizeof(reuse0))==-1) 
	{
		printf("set SO_REUSEADDR err : %s\n", strerror(errno));
		return -1;
	}	
      
    bzero(&ser_addr,sizeof(ser_addr));  
    ser_addr.sin_family=AF_INET;  
    ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
    ser_addr.sin_port=htons(listen_port);
	
    err=bind(ser_sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));  
    if(err==-1)  
    {  
        printf("bind error:%s\n",strerror(errno));  
        return -1;  
    }  
      
    err=listen(ser_sockfd,5);  
    if(err==-1)  
    {  
        printf("listen error\n");  
        return -1;  
    }  
      
    printf("now, wait for file\n");  
      
    while(1)  
    {     
        addlen=sizeof(struct sockaddr);  
        cli_sockfd=accept(ser_sockfd,(struct sockaddr *)&cli_addr,&addlen);  
        if(cli_sockfd == -1)  
        {  
            printf("accept error, try again...\n");
			continue;
        }
		else
		{
			printf("client : %s\n", inet_ntoa(cli_addr.sin_addr));
		}
		
		int total_bytes = 0;
        while(1)  
        {  
			printf("\n\n");
            printf("waiting for client...\n");  
            n=recv(cli_sockfd,recvline,1024,0);  
            if(n==-1)  
            {  
                printf("recv error\n");
				return -1;
            }
			else if (n == 0)
			{
				printf("data received done: %d bytes received\n", total_bytes);
				break;
			}
		
			total_bytes += n; 
			printf("progress : %d, %d\n", n , total_bytes);
			
			if (n != fwrite(recvline, 1, n, fp))
			{
				printf("fwrite error. %s\n", strerror(errno));
				return -1;
			}
        }
		
        close(cli_sockfd); 
		break;		
    }  
      
	fclose(fp);  
    close(ser_sockfd);  
      
    return 0;  
} 
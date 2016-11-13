/**********************************
tcp_ser.c: the source file of the server in tcp transmission 
***********************************/


#include "headsock.h"

#define BACKLOG 10

void str_ser(int sockfd, int errProb);          // transmitting and receiving function

int main(int argc, char **argv)
{
	int sockfd, con_fd, ret;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	unsigned int sin_size;
	int errProb = 0;

//	char *buf;
	pid_t pid;

	if (argc != 2) {
		printf("Wrong number of parameters!");
	}

	errProb = atoi(argv[1]);							// Error probability


	sockfd = socket(AF_INET, SOCK_STREAM, 0); 			//create socket
	if (sockfd <0)
	{
		printf("error in socket!");
		exit(1);

	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYTCP_PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);       //inet_addr("172.0.0.1");
	bzero(&(my_addr.sin_zero), 8);
	ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));    //bind socket
	if (ret <0)
	{
		printf("error in binding");
		exit(1);
	}
	
	ret = listen(sockfd, BACKLOG);                       //listen
	if (ret <0) {
		printf("error in listening");
		exit(1);
	}

	while (1)
	{
		printf("waiting for data\n");
		sin_size = sizeof (struct sockaddr_in);

		con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);       //accept the packet
		if (con_fd <0)
		{
			printf("error in accept\n");
			exit(1);
		}

		if ((pid = fork())==0)					// create acception process
		{
			close(sockfd);
			str_ser(con_fd, errProb);			//receive packet and response
			close(con_fd);
			exit(0);
		}
		else close(con_fd);						//parent process
	}
	close(sockfd);
	exit(0);
}

void str_ser(int sockfd, int errProb)
{
	char buf[BUFSIZE];
	FILE *fp;
	char recvs[DATALEN];
	struct ack_so ack;
	int end, n, p, seqnum = 0;
	long lseek=0;
	end = 0;
	int NACK = 2;
	ack.len = 1;

	if (errProb > 95) errProb = 95;
	
	printf("receiving data!\n");

	while(!end)
	{
		if ((n= recv(sockfd, &recvs, DATALEN, 0))==-1)      //receive the packet
		{
			printf("error when receiving\n");
			exit(1);
		}
		else if (rand()%100 < errProb) // Error, send NACK
		{
			ack.num = NACK;
			if ((p = send(sockfd, &ack, 2, 0))== -1 )
			{
				printf("NACK send error!");		     	
				exit(1);
			}
		}
		else // No error, send ACK
		{
			ack.num = seqnum;
			if ((p = send(sockfd, &ack, 2, 0))== -1 )
			{
				printf("ACK send error!");		     	
				exit(1);
			}
			seqnum = 1 - seqnum;

			if (recvs[n-1] == '\0')		// it is the end of the file
			{
				end = 1;
				n --;
			}
			// Copy to the result
			memcpy((buf+lseek), recvs, n);
			lseek += n;
		}

	}
	

	if ((fp = fopen ("myTCPreceive.txt","wt")) == NULL)
	{
		printf("File doesn't exit\n");
		exit(0);
	}

	fwrite (buf , 1 , lseek , fp);					//write data into file
	fclose(fp);
	printf("a file has been successfully received!\nthe total data received is %d bytes\n", (int)lseek);

  
	

}

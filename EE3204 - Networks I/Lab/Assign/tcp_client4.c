/*******************************
tcp_client.c: the source file of the client in tcp transmission 
********************************/

#include "headsock.h"

float str_cli(FILE *fp, int sockfd, long *len, int *nrErrors);	//transmission function
void tv_sub(struct  timeval *out, struct timeval *in);			//calc the time interval between out and in

int main(int argc, char **argv)
{
	int sockfd, ret;
	float time, rate;
	long len;
	struct sockaddr_in ser_addr;
	char ** pptr;
	struct hostent *sh;
	struct in_addr **addrs;
	FILE *fp;
	int nrErrors = 0;

	if (argc != 2) {
		printf("parameters not match");
	}

	sh = gethostbyname(argv[1]);	                        //get host's information
	if (sh == NULL) {
		printf("error when gethostby name");
		exit(0);
	}

	printf("canonical name: %s\n", sh->h_name);	       	//print the remote host's information
	for (pptr=sh->h_aliases; *pptr != NULL; pptr++)
		printf("the aliases name is: %s\n", *pptr);
	switch(sh->h_addrtype)
	{
		case AF_INET:
			printf("AF_INET\n");
		break;
		default:
			printf("unknown addrtype\n");
		break;
	}
        
	addrs = (struct in_addr **)sh->h_addr_list;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);                //create the socket
	if (sockfd <0)
	{
		printf("error in socket");
		exit(1);
	}
	ser_addr.sin_family = AF_INET;                                                      
	ser_addr.sin_port = htons(MYTCP_PORT);
	memcpy(&(ser_addr.sin_addr.s_addr), *addrs, sizeof(struct in_addr));
	bzero(&(ser_addr.sin_zero), 8);
	ret = connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr));   //connect the socket with the host
	if (ret != 0) {
		printf ("connection failed\n"); 
		close(sockfd); 
		exit(1);
	}
	
	if((fp = fopen ("myfile.txt","r+t")) == NULL)
	{
		printf("File doesn't exit\n");
		exit(0);
	}

	time = str_cli(fp, sockfd, &len, &nrErrors);	//perform the transmission and receiving

	rate = (len/(float)time);						//caculate the average transmission rate

	printf("Time(ms) : %.3f, Data sent(byte): %d\nData rate: %f (Kbytes/s)\n", time, (int)len, rate);

	printf("Number of errors: %1i\n", nrErrors);

	close(sockfd);
	fclose(fp);
//}
	exit(0);
}

float str_cli(FILE *fp, int sockfd, long *len, int *nrErrors)
{
	char *buf;
	long lsize, ci;
	char sends[DATALEN];
	struct ack_so acks;
	int n, slen, seqnum = 0;
	float time_inv = 0.0;
	struct timeval sendt, recvt;
	ci = 0;

	fseek (fp , 0 , SEEK_END);
	lsize = ftell (fp);
	rewind (fp);
	printf("The file length is %d bytes\n", (int)lsize);
	printf("The packet length is %d bytes\n",DATALEN);

	// allocate memory to contain the whole file.
	buf = (char *) malloc (lsize);
	if (buf == NULL) exit (2);

	// copy the file into the buffer.
	fread (buf,1,lsize,fp);

	/*** the whole file is loaded in the buffer. ***/
	buf[lsize] ='\0';					//append the end byte
	gettimeofday(&sendt, NULL);			//get the current time

	// Start transmission
	while(ci<= lsize) // Until the whole msg has been sent
	{
	    // Divide into packets and copy into sending structure
		if ((lsize+1-ci) <= DATALEN)
			slen = lsize+1-ci;
		else 
			slen = DATALEN;

		// TODO: Add redundency check?

		memcpy(sends, (buf+ci), slen);

	    // Send packet
		n = send(sockfd, &sends, slen, 0);
		if(n == -1) {
			printf("send error!");			    
			exit(1);
		}

		// TODO: implement Stop-and-wait without Timeout

		if ((n=recv(sockfd, &acks, 2, 0)) == -1 )    // Wait for ACK or NACK    
		{
		   printf("error when receiving\n");
		   exit(1);
		}
		else if (acks.num != seqnum || acks.len != 1) // Recieved NACK, resend, count errors
		{
			(*nrErrors)++;
		}
		else // Recieved ACK, send next frame
		{
			ci += slen;
			seqnum = 1 - seqnum;
		}

	}
	
	gettimeofday(&recvt, NULL);                         //get current time
	*len= ci;                                                  
	tv_sub(&recvt, &sendt);                            // get the whole trans time
	time_inv += (recvt.tv_sec)*1000.0 + (recvt.tv_usec)/1000.0;
	return(time_inv);
}

void tv_sub(struct  timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) <0)
	{
		--out ->tv_sec;
		out ->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

#include "mes_fonctions.h"

struct addrinfo *srv_addr;

int connectTo(char *host,char *port_char)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_protocol=IPPROTO_UDP;
	int get_addr_status= getaddrinfo((char*)host,(char*) port_char,&hints,&srv_addr);

	int sfd=socket(srv_addr->ai_family,srv_addr->ai_socktype,srv_addr->ai_protocol);
	int connect_status;
	connect_status=(sfd,srv_addr->ai_addr,srv_addr->ai_addrlen);
	if (connect_status !=-1)
		{
		printf("Connection succesful \n");
		}
	return sfd;	
}
void sendRRQ(char*argv,int sockfd,int mode)
{    
     int RRQ_size=strlen(argv)+4; //Don't use strlen(RRQ_PACKET)
     char *RRQ_packet = calloc(128, sizeof(char));
    RRQ_packet[0]=0;
	RRQ_packet[1]=1;
	strcpy(RRQ_packet+2,argv);
	switch(mode)
		{case OCTET :
			strcpy(RRQ_packet+3+strlen(argv),"octet");
			RRQ_size=RRQ_size+strlen("octet");
			break;
		case NETASCII :
			strcpy(RRQ_packet+3+strlen(argv),"netascii");
			RRQ_size=RRQ_size+strlen("netascii");
			break;
		
		}
     int sent_val=sendto(sockfd,RRQ_packet,RRQ_size,0,srv_addr->ai_addr,srv_addr->ai_addrlen);
     if(sent_val==-1)
     {printf("RRQ not sent \n");}
     else
     {printf("RRQ sent \n");}
     free(RRQ_packet);
}
void receive_data(int sockfd, char *filename)
{	char ACK_packet[4]={0,4,0,0};
	char* buffer;
	buffer=calloc(DATA_PACKET_SIZE,sizeof(char)); 
	int received_size;
	int file_fd=open(PATH_TO_FILE,O_WRONLY|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
	FILE *fp=fopen(filename,"w");
	int write_status;
	do
	{
		 received_size=recvfrom(sockfd,buffer,DATA_PACKET_SIZE,0,srv_addr->ai_addr,&srv_addr->ai_addrlen);
		if(received_size==-1)
			{printf("Could not receive data \n");
			exit(EXIT_FAILURE);
			}
		if(buffer[0]=='0' && buffer[1]=='5')
			{printf("Packet %d is an error packet\n", buffer[3]);
			exit(EXIT_FAILURE);
			}
		write_status=fwrite(buffer+4,sizeof(char),received_size-4,fp);
		if(write_status==1)
			{printf("Can't write data to file\n");}
		else
			{printf("Written packet n° %d \n",buffer[3]);}

		printf("Packet n° %d \n",buffer[3]);
		ACK_packet[2]=buffer[2];
		ACK_packet[3]=buffer[3];
		printf("ACK : ");
		for(int j=0;j<4;j++)
			{printf("%x",ACK_packet[j]);}
			printf("\n");
		int sent_ack=sendto(sockfd,ACK_packet,4,0,srv_addr->ai_addr,srv_addr->ai_addrlen);
		if(sent_ack==-1)printf("ACK error");
		
	}while(received_size==DATA_PACKET_SIZE); 
	free(buffer);
}
void sendWRQ(char*argv,int sockfd,int mode)
{ 
    int WRQ_size=strlen(argv)+4; 
    char *WRQ_packet = calloc(128, sizeof(char));
    
    WRQ_packet[0]=0;
	WRQ_packet[1]=2;
	strcpy(WRQ_packet+2,argv);
	switch(mode)
		{case OCTET :
			strcpy(WRQ_packet+3+strlen(argv),"octet");
			WRQ_size=WRQ_size+strlen("octet");
			break;
		case NETASCII :
			strcpy(WRQ_packet+3+strlen(argv),"netascii");
			WRQ_size=WRQ_size+strlen("netascii");
			break;
		
		}
     int sent_val=sendto(sockfd,WRQ_packet,WRQ_size,0,srv_addr->ai_addr,srv_addr->ai_addrlen);
     if(sent_val==-1)
     {printf("WRQ not sent \n");}
     else
     {printf("WRQ sent \n");}
     free(WRQ_packet);
 }
void send_data(int sockfd, char *filename)
{	char packet_header[4]={0,3,0,0}; 
	char ack_packet[4]={0,4,0,0};
	char* buffer;
	char* ack_buffer;
	ack_buffer=calloc(ACK_PACKET_SIZE,sizeof(char));
	buffer=calloc(516,sizeof(char)); 
	FILE *fp=fopen(filename,"r");
	int read_size;
	int sent_status;
	int packount=0;

	do
	{
		recvfrom(sockfd,ack_buffer,ACK_PACKET_SIZE,0,srv_addr->ai_addr,&srv_addr->ai_addrlen);
		packount++;
		printf("Receiving packet n° %x %x\n",ack_buffer[2],ack_buffer[3]);
		if(ack_buffer[2]==ack_packet[2] && ack_buffer[3]==ack_packet[3]) 
			{
			ack_packet[2]=packount/256;
			ack_packet[3]=packount%256;
			packet_header[2]=packount/256;
			packet_header[3]=packount%256;
			}
			for(int i=0;i<4;i++)
				{buffer[i]=packet_header[i];} //adding header to data packets
			
		read_size=fread(buffer+4, sizeof(char),512,fp); //reading data from file
		
		printf("Sending   packet n° %x %x of size %i \n",packet_header[2],packet_header[3],read_size);
		sent_status=sendto(sockfd,buffer,read_size+4,0,srv_addr->ai_addr,srv_addr->ai_addrlen); //sending data to server
		if(sent_status==-1)
		{printf("Couldn't send data");}
	}while(read_size>0);
	free(buffer);
	free(ack_buffer);
	
}


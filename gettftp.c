#include "mes_fonctions.h"

int main (int argc, char *argv[])
{
if (argc < 3){
		printf(ERR_MES_ARG);
		exit(EXIT_FAILURE);
	}
	//user input
 char *host = strtok(argv[1],":");
 char *port_char=strtok(NULL,":");
 char *file = argv[2];
	
int sfd=connectTo((char*)host,(char*)port_char);

sendRRQ(argv[2],sfd,0);
receive_data(sfd,argv[2]);
exit(EXIT_SUCCESS);
return 0;
}

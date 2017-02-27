#include "IO.h"
#include <stdlib.h>
using namespace std;
IOSocket::IOSocket(string path)
{
	sock=socket(AF_UNIX, SOCK_STREAM, 0);	
	char* p=&path[0];
	serv_addr.sun_family=AF_UNIX;
	strncpy(serv_addr.sun_path, p, sizeof(serv_addr.sun_path));
	if(connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))<0)
	{
		cerr<<"Could not connect to server"<<endl;
		exit(0);
	}
}
void IOSocket::Write(double* data,int len)
{
	write(sock, &len, sizeof(len));
	write(sock, data, len);
	}
void IOSocket::Recv(double * data)
{
	int len;
	char buf[48+sizeof(int)];
	bzero(&buf[0], 48+sizeof(int));
	recv(sock, &buf[0], sizeof(int), NULL);
	memcpy(&len, &buf[0], sizeof(int));
	recv(sock, &buf[sizeof(int)], len*sizeof(double), NULL);
	memcpy(data, &buf[sizeof(int)], len*sizeof(double));
}
IOSocket::~IOSocket()
{
	close(sock);
}


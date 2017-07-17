#ifndef IO_H
#define IO_H
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string> 
#include <iostream>
#include <sys/un.h>
#include <vector>
#include <stdlib.h>
class IOSocket 
{
	public: 
		IOSocket(std::string path);
		void Write(double* data, int len);
		void Recv(double* data);
		~IOSocket();
	private:
		int sock, len;
		struct sockaddr_un serv_addr;
};
#endif // IO_H

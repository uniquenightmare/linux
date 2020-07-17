#pragma  once
#include<cstdio>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;
class UDPSvr
{
  public:
    UDPSvr()
      :_sock(-1)
    {
    }
    ~UDPSvr()
    {

    }
    bool CreatSocket()
    {
      _sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(_sock < 0)
      {
        perror("CreatSocket\n");
        return false;
      }
      return true;
    }
    bool Bind(string& ip,uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = bind(_sock,(struct sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("Bind\n");
        return false;

      }
      return true;
    }
    bool Send(string& data,struct sockaddr_in* destaddr)
    {
      int sendsize = sendto(_sock,data.c_str(),data.size(),0,(struct sockaddr*)destaddr,sizeof(struct sockaddr_in));
      if(sendsize < 0)
      {
        perror("Send\n");
        return false;
      }
      return true;
    }
    bool Recv(string* buf,struct sockaddr_in* srcaddr)
    {
      char tmp[1024] = {0};
      socklen_t socklen = sizeof(struct sockaddr_in);
      int recvsize = recvfrom(_sock,tmp,sizeof(tmp)-1,0,(struct sockaddr*)srcaddr,&socklen);
      if(recvsize < 0)
      {
        perror("Recv\n");
        return false;
      }
      (*buf).assign(tmp,recvsize);
      return true;
    }
    void Close()
    {
      close(_sock);
      _sock = -1;
    }
  private:
    int _sock;
};

#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<iostream>
#include<arpa/inet.h>
#include<string>
using namespace std;
class TCPSvr
{
  public:
    TCPSvr()
      :_sockfd(-1)
    {

    }
    ~TCPSvr()
    {

    }
    bool CreatSocket()
    {
      _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(_sockfd < 0)
      {
        perror("CreatSocket");
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
      int ret = bind(_sockfd,(struct sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("Bind");
        return false;
      }
      return true;
    }
    bool Listen(int backlog = 5)
    {
      int ret = listen(_sockfd,backlog);
      if(ret < 0)
      {
        perror("Listen");
        return false;
      }
      return true;
    }
    bool Accept(struct sockaddr_in* peeraddr,TCPSvr *ts)
    {
      socklen_t addrlen = sizeof(struct sockaddr);
      int serverfd = accept(_sockfd,(struct sockaddr*)peeraddr,&addrlen);
      if(serverfd < 0)
      {
        perror("Accept");
        return false;
      }
      ts->_sockfd = serverfd;
      return true;
    }
    bool Connect(string& ip,uint16_t port)
    {
      struct sockaddr_in destaddr;
      destaddr.sin_family = AF_INET;
      destaddr.sin_port = htons(port);
      destaddr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = connect(_sockfd,(struct sockaddr*)&destaddr,sizeof(destaddr));
      if(ret < 0)
      {
        perror("Connect");
        return false;
      }
      return true;
    }
    bool Send(string& data)
    {
      int sendsize = send(_sockfd,data.c_str(),data.size(),0);
      if(sendsize < 0)
      {
        perror("Send");
        return false;
      }
      return true;
    }
    bool Recv(string* data)
    {
      char buf[1024] = { 0 };

      int revsize = recv(_sockfd,buf,sizeof(buf) - 1,0);
      if(revsize < 0)
      {
        perror("Recv");
        return false;
      }
      else if(!revsize)
      {
        printf("peer shutdown\n");
        return false;
      }
      (*data).assign(buf,revsize);
      return true;
    }
    void Close()
    {
      close(_sockfd);
      _sockfd = -1;
    }
  private:
    int _sockfd;
};

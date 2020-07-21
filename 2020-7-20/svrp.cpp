#include"tcpsvr.hpp"
#include<pthread.h>

void* Threadstart(void* arg)
{
  pthread_detach(pthread_self());
  TCPSvr* ts = (TCPSvr*)arg;
  while(1)
  {
    string buf;
    ts->Recv(&buf);
    printf("client did say :%s\n",buf.c_str());
    printf("please return msg to client:");
    fflush(stdout);
    cin>>buf;
    ts->Send(buf);
  }
  delete ts;
  ts->Close();
}
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("Start ./svrp[ip][port]\n");
    return 0;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TCPSvr ts;
  if(!ts.CreatSocket())
  {
    return 0;
  }
  if(!ts.Bind(ip,port))
  {
    return 0;
  }
  if(!ts.Listen())
  {
    return 0;
  }
  while(1)
  {
    TCPSvr* newts = new TCPSvr();
    struct sockaddr_in peeraddr;
    if(!ts.Accept(&peeraddr,newts))
    {
      continue;
    }
    printf("Have a new connection ip = %s,port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
    pthread_t ptid;
    int ret = pthread_create(&ptid,NULL,Threadstart,(void*)newts);
    if(ret < 0)
    {
      perror("pthread_create");
      return 0;
    }
  }
}

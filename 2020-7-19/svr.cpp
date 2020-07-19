#include"tcpsvr.hpp"
#include<sys/wait.h>
void sigcb(int signo)
{
  (void)signo;
  while(1)
  {
    waitpid(-1,NULL,WNOHANG);
  }
}
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("start server: ./svr[ip][port]\n");
    return 0;
  }
  signal(SIGCHLD,sigcb);
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TCPSvr ts;
  if(!ts.CreatSocket())
  {
    perror("CreatSocket");
    return 0;
  }
  if(!ts.Bind(ip,port))
  {
    perror("Bind");
    return 0;
  }
  if(!ts.Listen())
  {
    perror("Listen");
    return 0;
  }
  while(1)
  {
    TCPSvr newts;
    struct sockaddr_in peeraddr;
    if(!ts.Accept(&peeraddr,&newts))
    {
      continue;
    }
    printf("Have a new connection ip = %s,port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
    int pid = fork();
    if(pid < 0)
    {
      perror("fork");
      return 0;
    }
    else if(pid == 0)
    {
      while(1)
      {
        string buf;
        newts.Recv(&buf);
        printf("client did say:%s\n",buf.c_str());
        printf("pelase return msg to client:");
        fflush(stdout);
        cin>>buf;
        newts.Send(buf);
      }
      newts.Close();
      exit(1);
    }
    else
    {
      newts.Close();
    }
  }
  return 0;
}

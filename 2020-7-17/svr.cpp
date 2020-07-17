#include"udpsvr.hpp"
#include<stdlib.h>
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("Use tihs process ,please\"./svr[ip][port]\"");
    return 0;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  UDPSvr us;
  if(!us.CreatSocket())
  {
    return 0;
  }
  if(!us.Bind(ip,port))
  {
    return 0;
  }
  while(1)
  {
    string buf;
    struct sockaddr_in peeraddr;
    us.Recv(&buf,&peeraddr);
    printf("cli say:%s\n",buf.c_str());
    printf("server say:");
    fflush(stdout);
    cin>>buf;
    us.Send(buf,&peeraddr);
  }
  us.Close();
  return 0;
}


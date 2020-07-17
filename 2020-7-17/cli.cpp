#include"udpsvr.hpp"
#include<stdlib.h>
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("./cli[svr_ip][svr_port]\n");
    return 0;
  }
  string svr_ip = argv[1];
  uint16_t svr_port = atoi(argv[2]);
  UDPSvr us;
  if(!us.CreatSocket())
  {
    perror("CreatSocket");
    return 0;
  }
  while(1)
  {
    string buf;
    printf("client say:");
    fflush(stdout);
    cin>>buf;
    struct sockaddr_in destaddr;
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(svr_port);
    destaddr.sin_addr.s_addr = inet_addr(svr_ip.c_str());
    us.Send(buf,&destaddr);
    struct sockaddr_in peeraddr;
    us.Recv(&buf,&peeraddr);
    printf("svr say:%s\n",buf.c_str());
  }
  us.Close();
  return 0;
}

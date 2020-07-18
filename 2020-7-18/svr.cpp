#include"tcpsvr.hpp"

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("Start Server:./svr[ip][port]\n");
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
  TCPSvr serverts;
  struct sockaddr_in clientaddr;
  if(!ts.Accept(&clientaddr,&serverts))
  {
    return 0;
  }
  while(1)
  {
    string buf;
    serverts.Recv(&buf);
    printf("client say: %s\n",buf.c_str());
    printf("please return msg:");
    fflush(stdout);
    cin>>buf;
    serverts.Send(buf);
  }
  serverts.Close();
  ts.Close();
  return 0;
}

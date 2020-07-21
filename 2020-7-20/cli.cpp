#include"tcpsvr.hpp"

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("Start Client:./cli[ip][port]\n");
    return 0;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TCPSvr ts;
  if(!ts.CreatSocket())
  {
    return 0;
  }
  if(!ts.Connect(ip,port))
  {
    return 0;
  }
  while(1)
  {
    printf("please enter msg to client :");
    fflush(stdout);
    string buf;
    cin>>buf;
    ts.Send(buf);
    ts.Recv(&buf);
    printf("Sever say:%s\n",buf.c_str());
  }
  ts.Close();
  return 0;
}

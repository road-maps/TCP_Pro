// TCPServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream.h"
#include "stdio.h"
#include "WINSOCK2.H"

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
	WSADATA wsd;		//WSADATA变量，存有套接字库得版本信息
	SOCKET sServer;		//服务器套接字
	int iRetVal;		//返回值
	char buf[BUF_SIZE];

	//初始化套接字动态库，MAKEWORD宏返回一个DWORD型变量，指定版本号
	//调用成功后，wsd结构体得wVersion成员变量将含有版本信息
	if (WSAStartup(MAKEWORD(1,1),&wsd)!=0)
	{
		cout<<"初始化套接字动态库失败"<<endl;
		return -1;
	}
	//判断版本是否正确加载，如果不是1.1版本则返回
	if (LOBYTE(wsd.wVersion)!=1||HIBYTE(wsd.wVersion)!=1)
	{
		WSACleanup();		//清理套接字资源
		return -1;
	}
	//利用socket()函数创建套接字，注意是流式套接字
	sServer=socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET==sServer)
	{
		cout<<"创建套接字失败"<<endl;
		WSACleanup();
		return -1;
	}
	//服务器套接字地址
	SOCKADDR_IN addrServ;		//服务器地址
	addrServ.sin_family=AF_INET;		//AF_INET代表是Internet协议
	addrServ.sin_port=htons(6666);		//指定服务器端口，htons()函数
	addrServ.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	//绑定套接字
	iRetVal=bind(sServer,(SOCKADDR *)&addrServ,sizeof(SOCKADDR));
	if (SOCKET_ERROR==iRetVal)
	{
		cout<<"绑定套接字失败"<<endl;
		closesocket(sServer);		//关闭套接字
		WSACleanup();				//释放套接字资源
		return -1;
	}
	//监听
	iRetVal=listen(sServer,3);
	if (SOCKET_ERROR==iRetVal)
	{
		cout<<"监听失败"<<endl;
		WSACleanup();
		return -1;
	}
	cout<<"服务器已经启动，并监听客户端连接"<<endl;
	SOCKADDR_IN addrClient;				//客户端地址
	int len=sizeof(addrClient);
	//接收客户端数据
	while(1)
	{
		ZeroMemory(buf,BUF_SIZE);
		SOCKET sClient;				//客户端套接字
		//接受客户端请求，建立连接
		sClient=accept(sServer,(SOCKADDR *)&sClient,&len);
		if (INVALID_SOCKET==sClient)
		{
			cout<<"接收客户端连接失败"<<endl;
			closesocket(sServer);		//关闭套接字
			WSACleanup();				//释放套接字资源
			return -1;
		}
		//接收客户端数据
		iRetVal=recv(sClient,buf,BUF_SIZE,0);
		if (SOCKET_ERROR==iRetVal)
		{
			cout<<"接收数据失败"<<endl;
			closesocket(sServer);		//关闭服务器套接字
			closesocket(sClient);		//关闭客户端套接字
			WSACleanup();
			return -1;
		}
		cout<<buf<<endl;				//输出数据
		//给客户端的应答数据
		char sendBuf[100];
		sprintf(sendBuf,"Welcome %s to Server",inet_ntoa(addrClient.sin_addr));
		send(sClient,sendBuf,strlen(sendBuf)+1,0);
		//关闭套接字资源
		closesocket(sClient);
	}	
		//退出
		closesocket(sServer);		//关闭服务器套接字
		WSACleanup();				//清空winsock dll
	
	
	return 0;
}

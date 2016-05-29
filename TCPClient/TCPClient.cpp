// TCPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream.h>
#include <stdio.h>
#include <WINSOCK2.H>

#define BUF_SIZE 1024
int main(int argc, char* argv[])
{
	WSADATA wsd;		//存有套接字的版本信息
	SOCKET sClient;		//客户端套接字
	int iRetVal;		//返回值
	char buf[BUF_SIZE];	//数据缓冲区
	//初始化套接字动态库，我们用1.1版本
	if (WSAStartup(MAKEWORD(1,1),&wsd)!=0)
	{
		cout<<"初始化套接字动态库失败"<<endl;
		return -1;
	}
	if (LOBYTE(wsd.wVersion)!=1||HIBYTE(wsd.wVersion)!=1)
	{
		WSACleanup();
		return -1;
	}
	//创建套接字
	sClient=socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET==sClient)
	{
		cout<<"创建套接字失败"<<endl;
		WSACleanup();
		return -1;
	}
	cout<<"客户端启动..."<<endl;

	//服务器套接字地址
	SOCKADDR_IN addrServ;		//服务器地址结构
	addrServ.sin_family=AF_INET;
	addrServ.sin_port=htons(6666);
	addrServ.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");	//服务器地址

	//向服务器发送请求
	iRetVal=connect(sClient,(SOCKADDR *)&addrServ,sizeof(SOCKADDR));
	if (SOCKET_ERROR==iRetVal)
	{
		cout<<"服务器连接失败"<<endl;
		closesocket(sClient);
		WSACleanup();
		return -1;
	}
	cout<<"连接服务器成功..."<<endl;
	//向服务器发送数据
	iRetVal=send(sClient,"Client",strlen("Client")+1,0);
	if (SOCKET_ERROR==iRetVal)
	{
		cout<<"数据发送失败"<<endl;
		closesocket(sClient);
		WSACleanup();
		return -1;
	}
	//接收服务器回传数据
	iRetVal=recv(sClient,buf,BUF_SIZE,0);
	if (SOCKET_ERROR==iRetVal)
	{
		cout<<"接收数据失败"<<endl;
		closesocket(sClient);
		WSACleanup();
		return -1;
	}
	cout<<buf<<endl;
	closesocket(sClient);
	WSACleanup();
	return 0;
}

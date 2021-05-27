#include<iostream>
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>
//#define ASIO_STANDALONE 1
//standalone version of asio as we are not using the boost library

int main()
{
	//asio is good at handling errors
	asio::error_cocde ec; //this asio variable is for error correction
	
	asio::io_context context;// this line is used for making it work on any platform it operates. i.e., takes care of the platform.
	
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34",ec),80);
	//create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);
	
	//Tell socket to try and connect
	
	socket.connect(endpoint,ec);
	
	if(!ec)
	{
		std::cout<<"Connected"<<std::endl;
	}
	
	else
	{
		std::cout<<"Failed to connect to address:\n"<<ec.message()<<std::endl;
	}
	
	return 0;
}

	
	

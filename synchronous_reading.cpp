#include<iostream>
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>

int main()
{
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34",ec),80);
	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint,ec);
	if(!ec)
	{
		std::cout<<"Connected"<<std::endl;
	}
	
	else
	{
		std::cout<<"Failed to connect to address:\n"<<ec.message()<<std::endl;
	}
	//to see if the connection is successful we call the "is_open()" function
	
	if(socket.is_open())
	{
		std::string sRequest=
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.cpm\r\n"
			"Connection: close\r\n\r\n";
		socket.write_some(asio::buffer(sRequest.data(),sRequest.size()),ec);
		socket.wait(socket.wait_read);
		size_t bytes=socket.available();
		if(bytes)
		{
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(),vBuffer.size()),ec);
			for(auto x:vBuffer)
			{
				std::cout<<x;
			}
		}
	}
	return 0;
}
//compile using flags for thread (g++ progname.cpp -lpthread)

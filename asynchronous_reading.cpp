#include<chrono>
#include<iostream>
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>

/* What actually happens on this code
 * we are creating a context where asio can do its work
 * but we need to prime that context
 * with jobs for asio to do
 * if asio doesn't have anything to do
 * it is going to exit immediately
 * in the meantime we give it some fake jobs to do to prevent it's exit
 * we allow that context to run in its own thread
 * so it doesn't block the main program execution
 * connection to tcp server
 * prime tcp socket to read some content
 * when it is primed
 * we read some http request
 * and we wait 
*/
std::vector<char> vBuffer(20*1024);
void GrabSomeData(asio::ip::tcp::socket& socket)
{
        socket.async_read_some(asio::buffer(vBuffer.data(),vBuffer.size()),[&](std::error_code ec, std::size_t length)
                {
                        if(!ec)
                        {
                                for(int i=0;i<length;++i)
                                {
                                        std::cout<<vBuffer[i];
                                }

                        GrabSomeData(socket);
                        //async read_some will prime some context and the above statement should not be confused with recursion
                        }
                }

        );
}



int main()
{
        asio::error_code ec;
        asio::io_context context;
        //Give some fake tasks to asio so the context doesn't finish
        asio::io_context::work idleWork(context);
        // the above is a fake work for the context to do
        //Start the context
        std::thread thrContext= std::thread([&](){context.run();});
        //the run function will return as soon as the context has returned out of the things to do
        //
        asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34",ec),80);
        asio::ip::tcp::socket socket(context);
        //run the asio context in its own thread
        //this gives the context some temporal space within
        //which one can execute this instrutions without blocking the main program
        //
        socket.connect(endpoint,ec);

        if (!ec)
        {
                std::cout<<"Connected"<<std::endl;
        }

        else
        {
                std::cout<<"Failed to connet "<<ec.message()<<std::endl;
        }


        if(socket.is_open())
        {
                GrabSomeData(socket);
                //prime asio to read bytes before sending some data
                std::string sRequest=
                        "GET /index.html HTTP/1.1.\r\n"
                        "Host: david-barr.co.uk\r\n"
                        "Connection: close \r\n\r\n";
                socket.write_some(asio::buffer(sRequest.data(),sRequest.size()),ec);


                using namespace std::chrono_literals;
                std::this_thread::sleep_for(20000ms);
                context.stop();
                if(thrContext.joinable()) thrContext.join();

        }

        return 0;
}


//compile using -lpthread 

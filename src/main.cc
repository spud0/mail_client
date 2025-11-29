#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/system_error.hpp>
#include <boost/lexical_cast.hpp>


namespace asio = boost::asio; 
namespace ip = boost::asio::ip;

#if 0
void connect_to_server (const std::string& hostname, unsigned short port) {
	return;
}

void perform_handshake () {
	return;
}	
# endif 

int main (void) {

	// Create a local scope
	{
		asio::io_service io_serv; 
		asio::ssl::context context (asio::ssl::context::sslv3_client); 
		asio::ssl::stream <ip::tcp::socket> secure_socket (io_serv, context); 
		ip::tcp::socket& socket (secure_socket.next_layer()); 

		// Resolve the hostname, try all entries in list
		auto connect_to_server = [&](std::string& hostname, unsigned short port) {
		  ip::tcp::resolver::query 
				query(hostname, boost::lexical_cast<std::string>(port));	
		};

		auto perform_handshake = [&]() {

		}; 

	}

	std::cout << "EHLO" << std::endl;

	return 0;	 
}

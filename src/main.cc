#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/system_error.hpp>
#include <boost/lexical_cast.hpp>

namespace asio = boost::asio; 
namespace ip = boost::asio::ip;
namespace sys = boost::system;

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
		std::string server_id; 
		std::string message = ""; 
		asio::io_service io_serv; 
		asio::ssl::context context (asio::ssl::context::sslv3_client); 
		asio::ssl::stream <ip::tcp::socket> secure_socket (io_serv, context); 
		ip::tcp::socket& socket (secure_socket.next_layer()); 
		bool using_tls; 

		auto perform_handshake = [&]() {
			std::string server_name = socket.remote_endpoint().address().to_string(); 	
			if (using_tls) {
				sys::error_code err; 
				using_tls = false; 
				message += std::string{"EHLO "} + server_name + std::string{"\r\n"}; 
				message += std::string{"STARTTLS"} + "\r\n"; 
				secure_socket.handshake (asio::ssl::stream<ip::tcp::socket>::client, err); 
				if (err) {
					std::cerr << "Error occurred during handshake: " << err.message(); 
				}
				using_tls = true; 
			} else {
				// server_id
			} 
			return; 
		}; 

		// Resolve the hostname, try all entries in list
		auto connect_to_server = [&](std::string& hostname, unsigned short port) {
			sys::error_code err = asio::error::host_not_found; 	
			ip::tcp::resolver resolver (io_serv); 	
			ip::tcp::resolver::query query (hostname, boost::lexical_cast<std::string>(port));	
			ip::tcp::resolver::iterator iter = resolver.resolve(query); 	
			ip::tcp::resolver::iterator end; 	

			while (err && iter != end) {
				socket.close(); 	
				socket.connect (*iter++, err); 

				boost::asio::streambuf response;
				boost::system::error_code ec;
				boost::asio::read_until(socket, response, "\r\n", ec);

				if (!ec) {
					std::istream is(&response);
					std::string line;
					std::getline(is, line);
					std::cout << "Server greeting: " << line << std::endl;
					// expect something starting with "220"
				} else {
					std::cerr << "Failed to read greeting: " << ec.message() << std::endl;
				}

			} 

			if (err) std::cerr << "Error: " << err.message(); 
			else perform_handshake(); 
		};

	
		unsigned short port = 234; 	
		std::string hostname = "somehost"; 
		connect_to_server(hostname, port); 
	}

	// std::cout << "EHLO" << std::endl;

	return 0;	 
}

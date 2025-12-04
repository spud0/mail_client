#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/system_error.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <tuple>
#include <thread>
#include <cstdlib>

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



std::tuple<std::string, unsigned short int> get_env_vars () {
	std::tuple<std::string, unsigned short int> result; 

	std::string host_env_var = std::string{std::getenv("MC_HOSTNAME")}; 
	std::string port_env_var = std::string{std::getenv("MC_HOSTPORT")};  
	if (host_env_var.length() == 0) 
		return {}; 

	try {
		unsigned int port_number = std::stoul(port_env_var); 

		if (port_number > std::numeric_limits<unsigned short>::max()) 
			return {}; 
		else 
			return {host_env_var, static_cast<unsigned short int>(port_number)}; 
	} catch (const std::out_of_range& oor) {
        std::cerr << "Out of range error for port number: " << oor.what() << std::endl;
		return {}; 
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument error for port number: " << ia.what() << std::endl;
		return {}; 
    }	

	return result; 
}

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

		// Formats the message address to send
		auto send_mail = [&] (const std::string& address, const std::vector<std::string>& recipients, const std::string& body) -> std::string {

			if ((!address.length ()) || (!recipients.size ())) {
				std::cerr << "Couldn't send email, either sender or recipients not provided." << std::endl; 
				return {}; 
			}
			
			// Start formatting string to send to mail server. 
			std::string result; 	
			

			return result;
		};

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

		std::tuple<std::string, unsigned short int> env_vars = get_env_vars (); 	
		auto [hostname, port] = env_vars; 
		std::cout << "Connected to mail server @ " <<  hostname << ":" << port << std::endl; 
		connect_to_server (hostname, port); 
		send_mail		
	}

	// std::cout << "EHLO" << std::endl;

	return 0;	 
}

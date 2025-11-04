#include "features/impl/ddos_protection.h"

#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "config/impl/config_parser.h"
#include "util/impl/logger.h"
#include "util/impl/misc.h"
#include "util/impl/misc.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

const std::string error_connect_message =
    "Failed to connect to the service behind ProtectThing!\nIf you are the owner of this site, please check your configuration.";
const std::string portal_log_name = "Portal";
const std::string guard_log_name = "Guard";

int DDoSProtection::start_server(const ConfigParser::ProtectThingConfig& config, const std::string& portal_name) {
    const std::vector<std::string> split_address = Misc::split_string(config.portal_bind, ':');
    const std::string& port_str = split_address.back();
    const auto port = static_cast<short>(std::stoi(port_str));
    const std::string target_host = Misc::get_hostname_from_url(config.hostname);
    short target_port = config.port;

    try {
        net::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        Logger::info(std::format("{} running on http://{}", portal_name, config.portal_bind), guard_log_name);
        Logger::info(std::format("{} using {}:{}", portal_name, target_host, target_port), guard_log_name);

        while(true) {
            tcp::socket client_socket(io_context);
            acceptor.accept(client_socket);

            boost::system::error_code error_code;
            auto remote_endpoint = client_socket.remote_endpoint(error_code);

            if(!error_code) {
                unsigned short client_port = 0;
                std::string client_ip = remote_endpoint.address().to_string();
                client_port = remote_endpoint.port();
                Logger::info(std::format("Client connected: [IP: {}:{}]", client_ip, client_port), guard_log_name);
            } else {
                Logger::error(std::format("Failed to get remote endpoint: {}", error_code.message()), guard_log_name);
            }

            beast::flat_buffer buffer;
            http::request<http::string_body> req;

            http::request_parser<http::string_body> parser;
            parser.body_limit(100ULL * 1024 * 1024 * 1024);

            http::read(client_socket, buffer, parser, error_code);
            if(!error_code)
                req = parser.release();

            if(error_code) {
                Logger::error(std::format("Error reading client request: {}", error_code.message()), guard_log_name);
                client_socket.close();
                continue;
            }

            tcp::resolver resolver(io_context);
            auto const results = resolver.resolve(target_host, std::to_string(target_port), error_code);

            if(error_code) {
                Logger::error(std::format("Failed to resolve target: {}", error_code.message()), portal_log_name);

                http::response<http::string_body> error_res{http::status::bad_gateway, req.version()};
                error_res.set(http::field::server, portal_log_name);
                error_res.set(http::field::content_type, "text/plain");
                error_res.body() = error_connect_message;
                error_res.prepare_payload();
                http::write(client_socket, error_res, error_code);

                client_socket.close();
                continue;
            }

            tcp::socket target_socket(io_context);
            net::connect(target_socket, results.begin(), results.end(), error_code);

            if(error_code) {
                Logger::error(std::format("Failed to connect to target: {}", error_code.message()), portal_log_name);

                http::response<http::string_body> error_res{http::status::bad_gateway, req.version()};
                error_res.set(http::field::server, portal_log_name);
                error_res.set(http::field::content_type, "text/plain");
                error_res.body() = error_connect_message;
                error_res.prepare_payload();
                http::write(client_socket, error_res, error_code);

                client_socket.close();
                continue;
            }

            req.set(http::field::host, std::format("{}:{}", target_host, target_port));

            http::write(target_socket, req, error_code);
            if(error_code) {
                Logger::error(std::format("Error writing to target: {}", error_code.message()), portal_log_name);
                target_socket.close();
                client_socket.close();
                continue;
            }

            beast::flat_buffer response_buffer;
            http::response<http::dynamic_body> res;

            http::response_parser<http::dynamic_body> response_parser;
            response_parser.body_limit(100ULL * 1024 * 1024 * 1024);

            http::read(target_socket, response_buffer, response_parser, error_code);
            if(!error_code)
                res = response_parser.release();

            if(error_code) {
                Logger::error(std::format("Error reading from target: {}", error_code.message()), portal_log_name);
                target_socket.close();
                client_socket.close();
                continue;
            }

            http::write(client_socket, res, error_code);
            if(error_code)
                Logger::error(std::format("Error writing to client: {}", error_code.message()), portal_log_name);

            error_code = target_socket.shutdown(tcp::socket::shutdown_both, error_code);
            if(error_code)
                continue;
            target_socket.close();

            error_code = client_socket.shutdown(tcp::socket::shutdown_both, error_code);
            if(error_code)
                Logger::error(std::format("Error occurred shutting down client_socket: {}", error_code.message()),
                              guard_log_name);
            client_socket.close();
        }
    } catch(std::exception& exception) {
        Logger::error(std::format("Exception: {}", exception.what()), guard_log_name);
    }

    return 0;
}

void DDoSProtection::poll_traffic() {}

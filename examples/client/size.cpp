#include <ftps/client.hpp>

int main() {

    std::map<std::string, std::string> options =
            {
                    { "ftps_hostname", "https://ftps.server.ru" },
                    { "ftps_login", "{ftps_login}" },
                    { "ftps_password", "{ftps_password}" }
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));
    auto free_size = client->free_size();
    std::cout << "Free size: " << free_size << " bytes" << std::endl;
}

/// Free size: 8234213123 bytes

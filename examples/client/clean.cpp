#include <ftps/client.hpp>

int main() {

    std::map<std::string, std::string> options =
            {
                    { "ftps_hostname", "https://ftps.server.ru" },
                    { "ftps_login", "{ftps_login}" },
                    { "ftps_password", "{ftps_password}" }
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));
    bool is_connected = client->check();

    auto remote_resources = {
            "existing_file.dat",
            "not_existing_file.dat",
            "existing_directory",
            "existing_directory/",
            "not_existing_directory",
            "not_existing_directory/"
    };

    for (auto remote_resource : remote_resources) {
        bool is_clean = client->check(remote_resource);
        std::cout << "Resource: " << remote_resource << " is " << is_clean ? "" : "not " << "clean" << std::endl;
    }
}

/// Resource: existing_file.dat is clean
/// Resource: not_existing_file.dat is clean
/// Resource: existing_file.dat is clean
/// Resource: existing_directory is clean
/// Resource: existing_directory/ is clean
/// Resource: not_existing_directory is clean
/// Resource: not_existing_directory/ is clean

#include <ftps/client.hpp>

int main() {

    std::map<std::string, std::string> options =
            {
                    { "ftps_hostname", "https://ftps.server.ru" },
                    { "ftps_login", "{ftps_login}" },
                    { "ftps_password", "{ftps_password}" }
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_directories = {
            "existing_directory",
            "existing_directory/new_directory",
            "not_existing_directory/new_directory",
    };

    for (auto remote_directory : remote_directories) {
        bool is_created = client->create_directory(remote_directory);
        std::cout << "Directory: " << remote_directory << " is " << is_existed ? "" : "not " << "created" << std::endl;
    }

    auto remote_directory = "not_existing_directory/new_directory";
    bool recursive = true;
    bool is_created = client->create_directory("not_existing_directory/new_directory", recursive);
    std::cout << "Directory: " << remote_directory << " is " << is_existed ? "" : "not " << "created" << std::endl;
}

/// Directory: existing_directory is created
/// Directory: existing_directory/new_directory is created
/// Directory: not_existing_directory/new_directory is not created
/// Directory: not_existing_directory/new_directory is created

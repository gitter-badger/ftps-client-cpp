#include <ftps/client.hpp>

std::ostream resources_to_string(std::vector<std::string> & resources)
{
    std::ostream stream;
    for (auto resource : resources)
    {
        stream << "\t" << "- " << resource << std::endl;
    }
    return stream;
}

int main() {

    std::map<std::string, std::string> options =
            {
                    { "ftps_hostname", "https://ftps.server.ru" },
                    { "ftps_login", "{ftps_login}" },
                    { "ftps_password", "{ftps_password}" }
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_resources = {
            "existing_file.dat",
            "not_existing_file.dat",
            "existing_directory",
            "not_existing_directory"
    };

    for (auto remote_resource : remote_resources) {
        auto resources = client->check(remote_resource);
        std::cout << remote_resource << " resource contain:" << std::endl;
        std::cout << resources_to_string(resources);
        std::cout << std::endl;
    }
}

/// existing_file.dat resource contain:
///
/// not_existing_file.dat resource contain:
///
/// existing_directory resource contain:
///  - dir/
///  - file.dat
///
/// not_existing_directory resource contain:
///

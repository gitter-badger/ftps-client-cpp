#include <ftps/client.hpp>

std::ostream info_to_string(std::map<std::string, std::string> & info)
{
    std::ostream stream;
    for (auto option :info)
    {
        stream << "/t" << option.first << ": " << option.second << std::endl;
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

    for (auto remote_resource : remote_resources)
    {
        auto info = client->info(remote_resource);
        std::cout << "Information about " << remote_resource << ":" << std::endl;
        std::cout << info_to_string(info);
        std::cout << std::endl;
    }
}

///Information about existing_file:
/// created:
/// name: existing_file
/// modified:
/// size:
/// type:
///
///Information about not_existing_file.dat:
///
///Information about existing_directory:
/// created:
/// name:
/// modified:
/// size: 0
/// type: d:collection
///
///Information about not_existing_directory:
///

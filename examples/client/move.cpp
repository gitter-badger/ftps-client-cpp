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

    auto remote_file = "file.dat";
    auto remote_directory = "dir/";
    auto new_remote_file = "file2.dat";
    auto new_remote_directory = "dir2/";

    auto resources = client->list();
    std::cout << "\"/\" resource contain:" << std::endl;
    std::cout << resources_to_string(resources) << std::endl;

    client->move(remote_file, new_remote_file);
    client->move(remote_directory, new_remote_directory);

    resources = client.list();
    std::cout << "\"/\" resource contain:" << std::endl;
    std::cout << resources_to_string(resources) << std::endl;
}

/// "/" resource contain:
/// - dir/
/// - file.dat
///
/// "/" resource contain:
/// - dir2/
/// - file2.dat

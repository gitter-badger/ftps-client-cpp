#include <ftps/client.hpp>

//! [upload_from_file]

void upload_from_file()
{
    std::map<std::string, std::string> options =
            {
                    {"ftps_hostname", "https://ftps.server.ru"},
                    {"ftps_login", "{ftps_login}"},
                    {"ftps_password", "{ftps_password}"}
            };


	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_file = "dir/file.dat";
    auto local_file = "/home/user/Downloads/file.dat";

    bool is_uploaded = client->upload(remote_file, local_file);

    std::cout << remote_file << " resource is" << is_uploaded ? "" : "not" << "uploaded" << std::endl;
}

/// dir/file.dat resource is uploaded

//! [upload_from_file]

//! [async_upload_from_file]

void async_upload_from_file()
{
    std::map<std::string, std::string> options =
            {
                    {"ftps_hostname", "https://ftps.server.ru"},
                    {"ftps_login", "{ftps_login}"},
                    {"ftps_password", "{ftps_password}"}
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_file = "dir/file.dat";
    auto local_file = "/home/user/Downloads/file.dat";

    client->async_upload(remote_file, local_file, [&remote_file](bool is_uploaded)
    {
        std::cout << remote_file << " resource is" << is_uploaded ? "" : "not" << "uploaded" << std::endl;
    });
}

/// dir/file.dat resource is uploaded

//! [async_upload_from_file]

//! [upload_from_buffer]

void upload_from_buffer()
{
    std::map<std::string, std::string> options =
            {
                    {"ftps_hostname", "https://ftps.server.ru"},
                    {"ftps_login", "{ftps_login}"},
                    {"ftps_password", "{ftps_password}"}
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_file = "dir/file.dat";
    char * buffer_ptr = nullptr;
    long long int buffer_size = 0;

    bool is_uploaded = client->upload(remote_file, buffer_ptr, buffer_size);

    std::cout << remote_file << " resource is" << is_uploaded ? "" : "not" << "uploaded" << std::endl;
}

/// dir/file.dat resource is uploaded

//! [upload_from_buffer]

//! [async_upload_from_buffer]

void async_upload_from_buffer()
{
    std::map<std::string, std::string> options =
            {
                    {"ftps_hostname", "https://ftps.server.ru"},
                    {"ftps_login", "{ftps_login}"},
                    {"ftps_password", "{ftps_password}"}
            };

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_file = "dir/file.dat";
    char * buffer_ptr = nullptr;
    long long int buffer_size = 0;

    client->async_upload(remote_file, buffer_ptr, buffer_size, [&remote_file](bool is_uploaded)
    {
        std::cout << remote_file << " resource is" << is_uploaded ? "" : "not" << "uploaded" << std::endl;
    });
}

/// dir/file.dat resource is uploaded

//! [async_upload_from_buffer]

//! [upload_from_stream]

void upload_from_stream()
{
    std::map<std::string, std::string> options =
            {
                    {"ftps_hostname", "https://ftps.server.ru"},
                    {"ftps_login", "{ftps_login}"},
                    {"ftps_password", "{ftps_password}"}
            };

    std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

    auto remote_file = "dir/file.dat";
    std::ifstream stream("/home/user/Downloads/file.dat");

    bool is_uploaded = client->upload_from(remote_file, stream);

    std::cout << remote_file << " resource is" << is_uploaded ? "" : "not" << "uploaded" << std::endl;
}

/// dir/file.dat resource is uploaded

//! [upload_from_stream]
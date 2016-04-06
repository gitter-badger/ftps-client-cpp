FTPS Client
===
[![Gitter](https://badges.gitter.im/designerror/ftps-client-cpp.svg)](https://gitter.im/designerror/ftps-client-cpp?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Build Status](https://travis-ci.org/designerror/ftps-client-cpp.svg?branch=master)](https://travis-ci.org/designerror/ftps-client-cpp)

Package ```FTPS Client``` provides easy and convenient to work with FTPS-servers.

Install
===

```bash
> git clone https://github.com/designerror/ftps-client-cpp.git
> cd ftps-client-cpp
> clion .
```

Documentation
===

```bash
> cd docs
> doxygen doxygen.conf
> firefox html/index.html
```

Usage examples
===

```c++
#include <iostream>
#include <memory>
#include <ftps/client.hpp>

int main()
{
  std::map<std::string, std::string> options =
  {
    {"ftps_hostname", "https://ftps.server.ru"},
    {"ftps_login", "ftps_login"},
    {"ftps_password", "ftps_password"}
  };
            
  std::shared_ptr<FTPS::Client> client(FTPS::Client::Init(options));
  
  auto check_connection = client->check();
  std::cout << "test connection with FTPS-server is " 
            << (check_connection ? "" : "not ")
            << "successful"<< std::endl;
  
  auto is_directory = client->is_dir("/path/to/remote/resource");
  std::cout << "remote resource is " 
            << (is_directory ? "" : "not ") 
            << "directory" << std::endl;
  
  client->create_directory("/path/to/remote/directory/");
  client->clean("/path/to/remote/directory/");
  
  std::cout << "On FTPS-server available free space: " 
            << client->free_size() 
            << std::endl;
  
  std::cout << "remote_directory_name";
  for(auto resource_name : client->list("/path/to/remote/directory/"))
  {
    std::cout << "\t" << "-" << resource_name;
  }
  std::cout << std::endl;
  
  client->download("/path/to/remote/file", "/path/to/local/file");
  client->clean("/path/to/remote/file");
  client->upload("/path/to/remote/file", "/path/to/local/file");
  
  auto meta_info = client->info("/path/to/remote/resource");
  for(auto field : meta_info)
  {
    std::cout << field.first << ":" << "\t" << field.second;
  }
  std::cout << std::endl;

  client->copy("/path/to/remote/file1", "/path/to/remote/file2");
  client->move("/path/to/remote/file1", "/path/to/remote/file3");
  
  client->async_upload("/path/to/remote/file", "/path/to/local/file");
  client->async_download("/path/to/remote/file", "/path/to/local/file");
}
```

Acknowledgments
===
Thanks to the `JetBrains` company for

<a href="https://www.jetbrains.com/clion/"><img src="https://confluence.jetbrains.com/download/attachments/54342479/CLION?version=3&modificationDate=1449748289000&api=v2" width="100"></a>

//#define WITH_PROXY

#include <map>

static std::map<std::string, std::string> options_with_proxy = 
{ 
	{ "ftps_hostname", 	"ftps.server.ru" },
	{ "ftps_login", 	"ftps.test.login" },
	{ "ftps_password", 	"ftps.test.password" },
	{ "proxy_hostname", "{proxy_hostname}" },
	{ "proxy_login", 	"{proxy_login}" },
	{ "proxy_password", "{proxy_password}" }
};

static std::map<std::string, std::string> options_without_proxy =
{
	{ "ftps_hostname", 	"ftps.server.ru" },
	{ "ftps_root", 		"/" },
	{ "ftps_login", 	"ftps.test.login" },
	{ "ftps_password", 	"ftps.test.password" }
};

#ifdef WITH_PROXY
static std::map<std::string, std::string> options = options_with_proxy;
#else
static std::map<std::string, std::string> options = options_without_proxy;
#endif

static std::string file_content = "static std::wstring file_content = L\"static std::wstring file_content = ...\";";
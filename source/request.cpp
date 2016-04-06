
#include "stdafx.h"
#include "request.hpp"
#include "fsinfo.hpp"

bool inline check_code(CURLcode code)
{
	return code == CURLE_OK;
}

namespace FTPS
{
	Request::Request(std::map<std::string, std::string> options) noexcept
	{
		this->options = options;

		auto ftps_hostname = options["ftps_hostname"];
		auto ftps_login = options["ftps_login"];
		auto ftps_password = options["ftps_password"];

		auto proxy_hostname = options["proxy_hostname"];
		auto proxy_login = options["proxy_login"];
		auto proxy_password = options["proxy_password"];

		auto cert_path = options["cert_path"];
		auto key_path = options["key_path"];

		this->handle = curl_easy_init();

		this->set(CURLOPT_DEFAULT_PROTOCOL, "ftps");
		this->set(CURLOPT_SSL_VERIFYHOST, 0);
		this->set(CURLOPT_SSL_VERIFYPEER, 0);

#ifdef _DEBUG
		this->set(CURLOPT_VERBOSE, 0);
#else
		this->set(CURLOPT_VERBOSE, 0);
#endif
		if (this->cert_required()){

			this->set(CURLOPT_SSLCERTTYPE, "PEM");
			this->set(CURLOPT_SSLKEYTYPE, "PEM");
			this->set(CURLOPT_SSLCERT, (char *)cert_path.c_str());
			this->set(CURLOPT_SSLKEY, (char *)key_path.c_str());

		}
		
		this->set(CURLOPT_URL, (char *)ftps_hostname.c_str());
		this->set(CURLOPT_HTTPAUTH, (int)CURLAUTH_BASIC);
		auto token = ftps_login + ":" + ftps_password;
		this->set(CURLOPT_USERPWD, (char *)token.c_str());

		if (!this->proxy_enabled()) return;

		this->set(CURLOPT_PROXY, (char *)proxy_hostname.c_str());
		this->set(CURLOPT_PROXYAUTH, (int)CURLAUTH_BASIC);

		if (proxy_login.empty()) return;

		if (proxy_password.empty())
		{
			this->set(CURLOPT_PROXYUSERNAME, (char *)proxy_login.c_str());
		}
		else
		{
			token = proxy_login + ":" + proxy_password;
			this->set(CURLOPT_PROXYUSERPWD, (char *)token.c_str());
		}
	}

	Request::~Request() noexcept
	{
		if (this->handle != nullptr) curl_easy_cleanup(this->handle);
	}

	bool
	Request::set(CURLoption option, size_t value) noexcept
	{
		if (this->handle == nullptr) return false;
		return check_code(curl_easy_setopt(this->handle, option, value));
	}

	bool
	Request::set(CURLoption option, long value) noexcept
	{
		if (this->handle == nullptr) return false;
		return check_code(curl_easy_setopt(this->handle, option, value));
	}

	bool
	Request::set(CURLoption option, int value) noexcept
	{
		if (this->handle == nullptr) return false;
		return check_code(curl_easy_setopt(this->handle, option, value));
	}

	bool
	Request::set(CURLoption option, struct curl_slist * value) noexcept
	{
		if (this->handle == nullptr) return false;
		return check_code(curl_easy_setopt(this->handle, option, value));
	}

	bool
	Request::set(CURLoption option, curl_off_t value) noexcept
	{
		if (this->handle == nullptr) return false;
		return check_code(curl_easy_setopt(this->handle, option, value));
	}

	bool
	Request::set(CURLoption option, const char * value) noexcept
	{
		if (this->handle == nullptr) return false;
		return check_code(curl_easy_setopt(this->handle, option, value));
	}

	bool Request::perform() noexcept
	{
		if (this->handle == nullptr) return false;
		auto is_performed = check_code(curl_easy_perform(this->handle));
		if (!is_performed) return false;
		long http_code = 0;
		curl_easy_getinfo(this->handle, CURLINFO_RESPONSE_CODE, &http_code);
		if (http_code < 200 || http_code > 299) return false;
		return true;
	}

	bool Request::proxy_enabled() noexcept
	{
		auto proxy_hostname = options["proxy_hostname"];
		auto proxy_login = options["proxy_login"];
		auto proxy_password = options["proxy_password"];
		bool proxy_hostname_presented = !proxy_hostname.empty();
		if (!proxy_hostname_presented) return false;
		bool proxy_login_presented = !proxy_login.empty();
		bool proxy_password_presented = !proxy_password.empty();
		if (proxy_password_presented && !proxy_login_presented) return false;
		return true;
	}

	bool Request::cert_required() noexcept
	{
		auto cert_path = options["cert_path"];
		auto key_path = options["key_path"];
		if (cert_path.empty()) return false;
		bool cert_is_existed = FileInfo::exists(cert_path);
		if (!cert_is_existed) return false;
		if (key_path.empty()) return false;
		return FileInfo::exists(key_path);
	}
}
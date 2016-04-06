#include "stdafx.h"
#include <ftps/client.hpp>
#include "header.hpp"
#include "request.hpp"
#include "urn.hpp"
#include "fsinfo.hpp"
#include "callback.hpp"

namespace FTPS
{
	typedef int(* progress_funptr)(void *context, size_t dltotal, size_t dlnow, size_t ultotal, size_t ulnow);

	class ClientImpl : public Client
	{
	public:

		void init() noexcept;

		std::string ftps_hostname;
		std::string ftps_root;
		std::string ftps_login;
		std::string ftps_password;

		std::string proxy_hostname;
		std::string proxy_login;
		std::string proxy_password;

		std::string cert_path;
		std::string key_path;

		dict_t options() noexcept;

		bool sync_download(
			std::string remote_file, 
			std::string local_file, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_download_to(
			std::string remote_file, 
			char * & buffer_ptr, 
			unsigned long long int & buffer_size, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_download_to(
			std::string remote_file, 
			std::ostream & stream,
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_upload(
			std::string remote_file, 
			std::string local_file, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_upload_from(
			std::string remote_file, 
			char * buffer_ptr, 
			unsigned long long int buffer_size, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_upload_from(
			std::string remote_file, 
			std::istream & stream, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		ClientImpl(dict_t & options) noexcept;
	};

	
	inline ClientImpl * GetImpl(Client * ptr) { return (ClientImpl *)ptr; }
	inline const ClientImpl * GetImpl(const Client * ptr) { return (const ClientImpl *)ptr; }

	ClientImpl::ClientImpl(dict_t & options) noexcept
	{
		this->ftps_hostname = options["ftps_hostname"];
		this->ftps_root = options["ftps_root"];
		this->ftps_login = options["ftps_login"];
		this->ftps_password = options["ftps_password"];

		this->proxy_hostname = options["proxy_hostname"];
		this->proxy_login = options["proxy_login"];
		this->proxy_password = options["proxy_password"];

		this->cert_path = options["cert_path"];
		this->key_path = options["key_path"];

		this->init();
	}

	void
	ClientImpl::init() noexcept
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

	dict_t
	ClientImpl::options() noexcept
	{
		return dict_t
		{
			{ "ftps_hostname", this->ftps_hostname },
			{ "ftps_root", this->ftps_root },
			{ "ftps_login", this->ftps_login },
			{ "ftps_password", this->ftps_password },
			{ "proxy_hostname", this->proxy_hostname },
			{ "proxy_login", this->proxy_login },
			{ "proxy_password", this->proxy_password },
			{ "cert_path", this->cert_path },
			{ "key_path", this->key_path },
		};
	}

	bool
	ClientImpl::sync_download(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->ftps_root, true);
		auto file_urn = root_urn + remote_file;

		std::ofstream file_stream(local_file, std::ios::binary);

		Request request(this->options());

		auto url = this->ftps_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&file_stream);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Write::stream);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool
	ClientImpl::sync_download_to(
		std::string remote_file, 
		char * & buffer_ptr, 
		unsigned long long int & buffer_size, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->ftps_root, true);
		auto file_urn = root_urn + remote_file;

		Data data = { 0, 0, 0 };

		Request request(this->options());

		auto url = this->ftps_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();
		if (callback != nullptr) callback(is_performed);
		if (!is_performed) return false;

		buffer_ptr = data.buffer;
		buffer_size = data.size;
		return true;
	}

	bool
	ClientImpl::sync_download_to(
		std::string remote_file, 
		std::ostream & stream, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->ftps_root, true);
		auto file_urn = root_urn + remote_file;

		Request request(this->options());

		auto url = this->ftps_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&stream);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Write::stream);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();
		if (callback != nullptr) callback(is_performed);
		if (!is_performed) return false;

		return true;
	}

	bool
	ClientImpl::sync_upload(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		bool is_existed = FileInfo::exists(local_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->ftps_root, true);
		auto file_urn = root_urn + remote_file;

		std::ifstream file_stream(local_file, std::ios::binary);
		auto size = FileInfo::size(local_file);

		Request request(this->options());

		auto url = this->ftps_hostname + file_urn.quote(request.handle);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&file_stream);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::stream);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
		request.set(CURLOPT_BUFFERSIZE, (long)Client::buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool
	ClientImpl::sync_upload_from(
		std::string remote_file, 
		char * buffer, 
		unsigned long long int buffer_size, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto root_urn = Urn(this->ftps_root, true);
		auto file_urn = root_urn + remote_file;

		Data data = { buffer, 0, buffer_size };

		Request request(this->options());

		auto url = this->ftps_hostname + file_urn.quote(request.handle);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&data);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::buffer);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)buffer_size);
		request.set(CURLOPT_BUFFERSIZE, (long)Client::buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}
	
		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool
	ClientImpl::sync_upload_from(
		std::string remote_file, 
		std::istream & stream, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto root_urn = Urn(this->ftps_root, true);
		auto file_urn = root_urn + remote_file;

		Request request(this->options());

		auto url = this->ftps_hostname + file_urn.quote(request.handle);
		stream.seekg(0, std::ios::end);
		size_t stream_size = stream.tellg();
		stream.seekg(0, std::ios::beg);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&stream);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::stream);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)stream_size);
		request.set(CURLOPT_BUFFERSIZE, (long)Client::buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}
	
		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	Client * Client::Init(dict_t & options) noexcept
	{
		return new ClientImpl(options);
	}

	Client::~Client() noexcept
	{
		curl_global_cleanup();
	}

	unsigned long long
	Client::free_size() noexcept
	{
		Header header = {
				"Accept: */*",
				"Depth: 0",
				"Content-Type: text/xml"
		};

		
		Data data = { 0, 0, 0 };

		Request request(GetImpl(this)->options());

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		//request.set(CURLOPT_POSTFIELDS, document_print.c_str());
		//request.set(CURLOPT_POSTFIELDSIZE, (long)size);
		request.set(CURLOPT_HEADER, 0);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		auto is_performed = request.perform();
		if (!is_performed) return 0;

		// TODO
	}

	bool
	Client::check(std::string remote_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		auto root_urn = Urn(clientImpl->ftps_root, true);
		auto resource_urn = root_urn + remote_resource;

		Header header = {
				"Accept: */*",
				"Depth: 1"
		};

		Data data = { 0, 0, 0 };

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + resource_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		return request.perform();
	}

	dict_t
	Client::info(std::string remote_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		auto root_urn = Urn(clientImpl->ftps_root, true);
		auto target_urn = root_urn + remote_resource;

		Header header = {
				"Accept: */*",
				"Depth: 1"
		};

		Data data = { 0, 0, 0 };

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + target_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		bool is_performed = request.perform();

		if (!is_performed) return dict_t();

		// TODO

		return dict_t();
	}

	bool 
	Client::is_dir(std::string remote_resource) noexcept
	{
		auto information = this->info(remote_resource);
		auto resource_type = information["type"];
		bool is_directory = resource_type.compare("d:collection") == 0;
		return is_directory;
	}

	strings_t
	Client::list(std::string remote_directory) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_directory);
		if (!is_existed) return strings_t();

		bool is_directory = this->is_dir(remote_directory);
		if (!is_directory) return strings_t();
		auto target_urn = Urn(clientImpl->ftps_root, true) + remote_directory;
		target_urn = Urn(target_urn.path(), true);

		Header header = {
				"Accept: */*",
				"Depth: 1"
		};

		Data data = { 0, 0, 0 };

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + target_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_HEADER, 0);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		bool is_performed = request.perform();

		if (!is_performed) return strings_t();

		strings_t resources;

		// TODO

		return resources;
	}

	bool Client::download(
		std::string remote_file, 
		std::string local_file, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_download(remote_file, local_file, nullptr, progress);
	}

	void
	Client::async_download(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		std::thread downloading([&](){ clientImpl->sync_download(remote_file, local_file, callback, progress); });
		downloading.detach();
	}

	bool
	Client::download_to(
		std::string remote_file, 
		char * & buffer_ptr, 
		unsigned long long int & buffer_size, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_download_to(remote_file, buffer_ptr, buffer_size, nullptr, progress);
	}

	bool
	Client::download_to(
		std::string remote_file, 
		std::ostream & stream, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_download_to(remote_file, stream, nullptr, progress);
	}

	bool
	Client::create_directory(std::string remote_directory, bool recursive) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_directory);
		if (is_existed) return true;

		bool resource_is_dir = true;
		Urn directory_urn(remote_directory, resource_is_dir);

		if (recursive) {
			auto remote_parent_directory = directory_urn.parent();
			bool is_created = this->create_directory(remote_parent_directory, true);
			if (!is_created) return false;
		}

		Header header = {
				"Accept: */*",
				"Connection: Keep-Alive"
		};

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + directory_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "MKCOL");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}

	bool 
	Client::move(std::string remote_source_resource, std::string remote_destination_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_source_resource);
		if (!is_existed) return false;

		Urn root_urn(clientImpl->ftps_root, true);

		auto source_resource_urn = root_urn + remote_source_resource;
		auto destination_resource_urn = root_urn + remote_destination_resource;

		Header header = {
				"Accept: */*",
				"Destination: " + destination_resource_urn.path()
		};

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + source_resource_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "MOVE");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}

	bool 
	Client::copy(std::string remote_source_resource, std::string remote_destination_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_source_resource);
		if (!is_existed) return false;

		Urn root_urn(clientImpl->ftps_root, true);

		auto source_resource_urn = root_urn + remote_source_resource;
		auto destination_resource_urn = root_urn + remote_destination_resource;

		Header header = {
				"Accept: */*",
				"Destination: " + destination_resource_urn.path()
		};

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + source_resource_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "COPY");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}

	bool
	Client::upload(
		std::string remote_file,
		std::string local_file,
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_upload(remote_file, local_file, nullptr, progress);
	}

	void
	Client::async_upload(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		std::thread uploading([&](){ clientImpl->sync_upload(remote_file, local_file, callback, progress); });
		uploading.detach();
	}

	bool
	Client::upload_from(
		std::string remote_file, 
		std::istream & stream, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_upload_from(remote_file, stream, nullptr, progress);
	}

	bool
	Client::upload_from(
		std::string remote_file, 
		char * buffer_ptr,
		unsigned long long int buffer_size, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_upload_from(remote_file, buffer_ptr, buffer_size, nullptr, progress);
	}

	bool
	Client::clean(std::string remote_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_resource);
		if (!is_existed) return true;

		auto root_urn = Urn(clientImpl->ftps_root, true);
		auto resource_urn = root_urn + remote_resource;

		Header header = {
				"Accept: */*",
				"Connection: Keep-Alive"
		};

		Request request(clientImpl->options());

		auto url = clientImpl->ftps_hostname + resource_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "DELETE");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}
}
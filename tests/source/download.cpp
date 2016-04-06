#include "stdafx.h"
#include "catch.hpp"

SCENARIO("Client must download into buffer", "[download][buffer]") {

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

	GIVEN("A buffer") {

		std::string source_buffer = "content of the buffer";
		std::string remote_resource = "file.dat";

		auto buffer_pointer = const_cast<char *>(source_buffer.c_str());
		unsigned long long buffer_size = (source_buffer.length() + 1)* sizeof(source_buffer.c_str()[0]);

		auto is_success = client->upload_from(remote_resource, buffer_pointer, buffer_size);
		REQUIRE(is_success);

		WHEN("Download into the buffer") {

			REQUIRE(client->check(remote_resource));

			auto is_success = client->download_to(remote_resource, buffer_pointer, buffer_size);

			THEN("buffer must be downloaded") {

				CHECK(is_success);
				std::string destination_buffer(buffer_pointer);
				CHECK(destination_buffer == source_buffer);
			}
		}
	}
}

SCENARIO("Client must download stream", "[download][stream]") {

	std::unique_ptr<FTPS::Client> client(FTPS::Client::Init(options));

	GIVEN("A stream") {

		std::stringstream destination_stream;

		std::stringstream source_stream("content of the stream");
		std::string remote_resource = "file.dat";

		auto is_success = client->upload_from(remote_resource, source_stream);
		REQUIRE(is_success);

		WHEN("Upload the stream") {

			REQUIRE(client->check(remote_resource));

			auto is_success = client->download_to(remote_resource, destination_stream);

			THEN("stream must be uploaded") {

				CHECK(is_success);

				std::string source_buffer = source_stream.str();
				std::string destination_buffer = source_stream.str();

				CHECK(destination_buffer == source_buffer);

			}
		}
	}
}  
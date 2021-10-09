#pragma once
#include <string>
#include <map>

class RequestParser
{
	public:
		enum MethodType
		{
			GET,
			POST,
			DELETE,
			OTHER
		};

		enum ParseResult
		{
			HEADER_INCOMPLETE,
			HEADER_COMPLETE,
			REQUEST_COMPLETE,
			BAD_REQUEST
		};

		struct HttpVersion
		{
			HttpVersion();
			HttpVersion(int maj, int min);

			int major;
			int minor;
		};

		typedef std::map<std::string, std::string> header_field_t;

	public:

		int					parseHeader(std::string const &request);
		enum MethodType		getMethod() const;
		const std::string&	getTargetResource() const;
		HttpVersion			getHttpVersion() const;
		header_field_t&		getHeaderFields();
		const std::string&	getMessageBody() const;
	
	private:
		int parseRequestLine(std::string const & request, std::size_t max_size);
		int parseMethod(std::string const & s);
		int parseTargetResource(std::string const & s);
		int parseVersion(std::string const & s);

	private:
		enum MethodType	_method;
		std::string		_target_resource;
		HttpVersion		_version;
		header_field_t	_header_fields;
		std::string		_message_body;

		std::size_t		_index;
};

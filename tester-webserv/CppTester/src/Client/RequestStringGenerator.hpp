#pragma once

#include "DataStructures/Request.hpp"
#include <string>

class RequestStringGenerator {
	private:
		enum class State {
			REQUEST_LINE,
			HEADER_FIELD,
			MESSAGE_BODY,
			COMPLETE,
			ERROR
		};

	public:
		RequestStringGenerator();

		int generate(const Request::Pointer request);
		const std::string& getRequestString() const;
		void eraseBytes(std::size_t n);

		bool isComplete() const;
		bool isError() const;
		bool isFull() const;
		void resetState();

	private:
		void generateRequestLine(const Request::Pointer request);
		void generateHeaderField(const Request::Pointer request);
		void generateMessageBody(const Request::Pointer request);
		void setState(State new_state);

	private:
		State state;
		std::string request_string;
		std::size_t pos;
};

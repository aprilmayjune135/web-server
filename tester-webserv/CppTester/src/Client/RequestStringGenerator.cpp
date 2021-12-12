#include "RequestStringGenerator.hpp"
#include "settings.hpp"

RequestStringGenerator::RequestStringGenerator() {
	resetState();
}

int RequestStringGenerator::generate(const Request::Pointer request) {
	while (request_string.size() < BUFFER_SIZE) {
		switch (state) {
			case State::REQUEST_LINE:
				generateRequestLine(request);
				break;
			case State::HEADER_FIELD:
				generateHeaderField(request);
				break;
			case State::MESSAGE_BODY:
				generateMessageBody(request);
				break;
			case State::COMPLETE:
				return OK;
			case State::ERROR:
				return ERR;
		}
	}
	return OK;
}

void RequestStringGenerator::generateRequestLine(const Request::Pointer request) {
	request_string.append(request->request_line + CRLF);
	setState(State::HEADER_FIELD);
}

void RequestStringGenerator::generateHeaderField(const Request::Pointer request) {
	for (auto it = request->header_fields.begin(); it != request->header_fields.end(); ++it) {
		request_string.reserve(it->first.size() + it->second.size() + 4);
		request_string.append(it->first);
		request_string.append(": ");
		request_string.append(it->second);
		request_string.append(CRLF);
	}
	request_string.append(CRLF);
	if (request->message_body.size() > 0) {
		setState(State::MESSAGE_BODY);
	} else {
		setState(State::COMPLETE);
	}
}

void RequestStringGenerator::generateMessageBody(const Request::Pointer request) {
	std::size_t to_append = std::min(BUFFER_SIZE - request_string.size(), request->message_body.size() - pos);
	request_string.append(request->message_body, pos, to_append);
	pos += to_append;
	if (pos == request->message_body.size()) {
		setState(State::COMPLETE);
	}
}

const std::string& RequestStringGenerator::getRequestString() const {
	return request_string;
}

void RequestStringGenerator::eraseBytes(std::size_t n) {
	request_string.erase(0, n);
}

bool RequestStringGenerator::isComplete() const {
	return state == State::COMPLETE;
}

bool RequestStringGenerator::isError() const {
	return state == State::ERROR;
}

bool RequestStringGenerator::isFull() const {
	return request_string.size() >= BUFFER_SIZE;
}

void RequestStringGenerator::resetState() {
	state = State::REQUEST_LINE;
	pos = 0;
}

void RequestStringGenerator::setState(State new_state) {
	state = new_state;
}

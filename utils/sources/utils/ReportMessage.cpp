//
// Created by alexus on 29.12.23.
//


#include <EngineUtils/utils/ReportMessage.h>
#include <cstring>
#include <utility>

n::engine::utils::ReportMessage::ReportMessage(std::stacktrace pStacktrace)
	: stacktrace(std::move(pStacktrace)), exceptionPtr(std::current_exception()) {

	addInfoLine("Current errno status: {}", strerror(errno));
}
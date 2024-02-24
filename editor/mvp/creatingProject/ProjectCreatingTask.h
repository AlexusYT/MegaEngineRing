//
// Created by alexus on 25.01.24.
//

#ifndef PROJECTCREATINGTASK_H
#define PROJECTCREATINGTASK_H

namespace mer::editor::mvp {
struct ProjectCreatingTask {
	using MessageCallback = std::function<void(const std::string &pMessage)>;
	using TaskFunc = std::function<sdk::utils::ReportMessagePtr(const MessageCallback &pCallback)>;

	TaskFunc function;
	std::string tabName;

	//void appendToBuffer(const Glib::ustring &pMarkup) const { buffer->insert_markup(buffer->end(), pMarkup + "\n"); }
};
} // namespace mer::editor::mvp


#endif //PROJECTCREATINGTASK_H

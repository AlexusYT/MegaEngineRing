//
// Created by alexus on 30.12.23.
//

#ifndef REPORTMESSAGEFWD_H
#define REPORTMESSAGEFWD_H
#include <memory>

namespace n::engine::utils {
class ReportMessage;

using ReportMessageUPtr = std::unique_ptr<ReportMessage>;
} // namespace n::engine::utils

#endif //REPORTMESSAGEFWD_H

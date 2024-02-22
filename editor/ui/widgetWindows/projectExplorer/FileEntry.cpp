//
// Created by alexus on 29.01.24.
//

#include "FileEntry.h"

#include <mvp/main/editors/CppEditor.h>

namespace n::core::ui {
std::shared_ptr<mvp::IEditorPresenter> FileEntry::openEditor() {
	return /*std::make_shared<mvp::CppEditor>(path);*/ nullptr;
}
} // namespace n::core::ui
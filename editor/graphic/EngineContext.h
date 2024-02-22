//
// Created by alexus on 08.01.24.
//

#ifndef ENGINECONTEXT_H
#define ENGINECONTEXT_H

#include <EngineSDK/main/context/IContext.h>

namespace n::core::graphics {
class EngineContext : public sdk::main::IContext {

	Gtk::GLArea* area;

public:
	explicit EngineContext(Gtk::GLArea* const pArea) : area(pArea) {}

	void makeCurrent() override {
		area->make_current();
		engine::utils::Logger::out("Test");
	}

	[[nodiscard]] Gtk::GLArea* getArea() const { return area; }
};
} // namespace n::core::graphics


#endif //ENGINECONTEXT_H

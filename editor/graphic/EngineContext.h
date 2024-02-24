//
// Created by alexus on 08.01.24.
//

#ifndef ENGINECONTEXT_H
#define ENGINECONTEXT_H

#include <EngineSDK/main/context/IContext.h>

namespace mer::editor::graphics {
class EngineContext : public sdk::main::IContext {

	Gtk::GLArea* area;

public:
	explicit EngineContext(Gtk::GLArea* const pArea) : area(pArea) {}

	void makeCurrent() override {
		area->make_current();
		sdk::utils::Logger::out("Test");
	}

	[[nodiscard]] Gtk::GLArea* getArea() const { return area; }
};
} // namespace mer::editor::graphics


#endif //ENGINECONTEXT_H

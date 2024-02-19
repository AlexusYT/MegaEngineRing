//
// Created by alexus on 31.01.24.
//

#ifndef WINDOW_H
#define WINDOW_H
#ifndef EDITOR_SDK
	#include <EngineUtils/utils/ReportMessageFwd.h>

	#include "IContext.h"

typedef struct GLFWwindow GLFWwindow;

namespace n::sdk::main {

class Window : IContext {
	GLFWwindow* native{};
	std::shared_ptr<Window> sharedWindow;
	int width = 800;
	int height = 600;
	std::string title = "Untitled121";

protected:
	Window() = default;

public:
	~Window() override;

	void show();

	void setVisible(bool pState = true) const;

	bool isVisible() const;

	void setCloseRequest(bool pState) const;

	engine::utils::ReportMessagePtr setContextVersion(int pMajor, int pMinor) const;

	bool isCloseRequest() const;

	[[nodiscard]] const std::shared_ptr<Window> &getSharedWindow() const { return sharedWindow; }

	void setSharedWindow(const std::shared_ptr<Window> &pSharedWindow) { sharedWindow = pSharedWindow; }

	IContext* getContext() { return this; }

private:
	virtual void onResize(int pWidth, int pHeight);

	engine::utils::ReportMessagePtr init() override;

	void makeCurrent() override;

	void swapBuffers() const override;
};

} // namespace n::sdk::main
#endif //WINDOW_H

#endif
//
// Created by alexus on 31.01.24.
//

#ifndef WINDOW_H
#define WINDOW_H
#ifndef EDITOR_SDK
	#include <EngineUtils/utils/ReportMessageFwd.h>

	#include "IContext.h"

typedef struct GLFWwindow GLFWwindow;

namespace mer::sdk::main {

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

	sdk::utils::ReportMessagePtr setContextVersion(int pMajor, int pMinor) const;

	bool isCloseRequest() const;

	[[nodiscard]] const std::shared_ptr<Window> &getSharedWindow() const { return sharedWindow; }

	void setSharedWindow(const std::shared_ptr<Window> &pSharedWindow) { sharedWindow = pSharedWindow; }

	IContext* getContext() { return this; }

	[[nodiscard]] int getWidth() const { return width; }

	void setWidth(const int pWidth) { width = pWidth; }

	[[nodiscard]] int getHeight() const { return height; }

	void setHeight(const int pHeight) { height = pHeight; }

private:
	virtual void onSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyChanged(int pKey, int pScancode, int pAction, int pMods);

	sdk::utils::ReportMessagePtr init() override;

	void makeCurrent() override;

	void swapBuffers() const override;
};

} // namespace mer::sdk::main
#endif //WINDOW_H

#endif
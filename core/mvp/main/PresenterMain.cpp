//
// Created by alexus on 26.01.24.
//

#include "PresenterMain.h"

#include <future>
#include <mvp/main/IModelMain.h>
#include <mvp/main/IViewMain.h>
#include <project/Project.h>

namespace MVP_CORE {
PresenterMain::PresenterMain(const std::shared_ptr<IViewMain> &pViewMain, const std::shared_ptr<IModelMain> &pModelMain)
	: viewMain(pViewMain), modelMain(pModelMain) {
	viewMain->connectReloadCmakeClickedSignal(
		bind(mem_fun(*this, &PresenterMain::generateCmakeFiles), sigc::slot<void(int pExitCode)>{}));

	viewMain->connectRunClickedSignal([this] {
		std::thread([this] {
			build([this](int pExitCode) {
				if (pExitCode) return;
				run();
			});
		}).detach();
	});
	viewMain->setWindowTitle("Game engine editor - " + modelMain->getProject()->getProjectName());

	auto r =
		viewMain->connectKeyPressedSignal([this](const guint pKeyVal, guint pKeyCode, const Gdk::ModifierType pState) {
			if (pKeyCode == 39 && (pState & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK |
											 Gdk::ModifierType::ALT_MASK)) == Gdk::ModifierType::CONTROL_MASK) {
				std::thread([this] {
					engine::utils::Logger::info("Saving project...");
					if (const auto msg = modelMain->getProject()->saveProject()) engine::utils::Logger::error(msg);
					else
						engine::utils::Logger::info("Project saved");
				}).detach();

				return true;
			}
			return false;
		});


	viewMain->addActionGroup("win", modelMain->getProject()->getActionGroups());
}

void PresenterMain::generateCmakeFiles(const sigc::slot<void(int pExitCode)> &pOnFinish) const {
	viewMain->executeInMainThread([this](const std::promise<void> & /*pPromise*/) {
		viewMain->switchLogPage(0);
		viewMain->clearLogMessage(0);
	});

	std::thread([this, pOnFinish] {
		pOnFinish(modelMain->getProject()->reloadCMake([this](const std::string &pLogLine) { logMessage(0, pLogLine); },
													   [this](const std::string &pLogLine) { logError(0, pLogLine); }));
	}).detach();
}

void PresenterMain::build(const sigc::slot<void(int pExitCode)> &pOnFinish) const {
	viewMain->executeInMainThread([this](const std::promise<void> & /*pPromise*/) {
		viewMain->switchLogPage(1);
		viewMain->clearLogMessage(1);
	});

	std::thread([this, pOnFinish] {
		pOnFinish(modelMain->getProject()->build([this](const std::string &pLogLine) { logMessage(1, pLogLine); },
												 [this](const std::string &pLogLine) { logError(1, pLogLine); }));
	}).detach();
}

void PresenterMain::run(const sigc::slot<void(int pExitCode)> &pOnFinish) const {
	viewMain->executeInMainThread([this](const std::promise<void> & /*pPromise*/) {
		viewMain->switchLogPage(2);
		viewMain->clearLogMessage(2);
	});
	std::thread([this, pOnFinish] {
		pOnFinish(modelMain->getProject()->run([this](const std::string &pLogLine) { logMessage(2, pLogLine); },
											   [this](const std::string &pLogLine) { logError(2, pLogLine); }));
	}).detach();
}

void PresenterMain::logMessage(int pId, const std::string &pMessage) const {
	viewMain->executeInMainThread([pMessage, this, pId](const std::promise<void> & /*pPromise*/) {
		viewMain->addLogMessage(pId, Glib::Markup::escape_text(pMessage));
	});
}

void PresenterMain::logError(int pId, const std::string &pMessage) const {
	viewMain->executeInMainThread([pMessage, this, pId](const std::promise<void> & /*pPromise*/) {
		viewMain->addLogMessage(pId, "<span foreground=\"red\">" + Glib::Markup::escape_text(pMessage) + "</span>\n");
	});
}
} // namespace MVP_CORE
//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.01.24.
//

#include "ModelMain.h"

#include <nlohmann/json.hpp>

#include "Globals.h"
#include "IPresenterMain.h"
#include "StringInputStream.h"

namespace mer::editor::mvp {
void ModelMain::loadLayoutsFile() {
	const auto layoutFile = Globals::getConfigPath() / "paned-layouts/Layouts.json";
	const auto file = Gio::File::create_for_path(layoutFile);

	//According to the documentation of the Gio::File::query_exists method, it is best to try to create the file.
	//If it already exists, an exception will be thrown, which means we can read its contents.
	file->create_file_async(sigc::bind(sigc::mem_fun(*this, &ModelMain::tryCreateLayoutFile), file));
}

const std::vector<PanedLayoutTab> &ModelMain::getPanedLayoutTabs() const { return panedLayoutTabs; }

void ModelMain::setPanedLayoutTabs(const std::vector<PanedLayoutTab> &pPanedLayoutTabs) {
	panedLayoutTabs = pPanedLayoutTabs;
	if (presenter) presenter->onPanedLayoutTabsChanged();
}

void ModelMain::setCurrentTab(const int32_t pCurrentTab) {
	currentTab = pCurrentTab;
	if (presenter) presenter->onCurrentTabChanged();
}

void ModelMain::tryCreateLayoutFile(const std::shared_ptr<Gio::AsyncResult> &pResult,
									const std::shared_ptr<Gio::File> &pFile) {

	try {
		std::shared_ptr<Gio::FileOutputStream> streamOut = pFile->create_file_finish(pResult);
		readInternalLayouts(streamOut, pFile);
	} catch (Gio::Error &error) {
		if (error.code() == Gio::Error::Code::EXISTS) {
			auto strStream = utils::StringInputStream::create(pFile->read());
			strStream->readAllAsync(sigc::bind(sigc::mem_fun(*this, &ModelMain::userLayoutsReadFinished), pFile));
		} else {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to copy Layouts file to user config directory");
			msg->setMessage("Exception occurred");
			msg->addInfoLine("Path to user Layouts file: {}", pFile->get_path());
			msg->addInfoLine("Using internal file.");
			presenter->displayError(msg);
			readInternalLayouts();
		}
	}
}

void ModelMain::userLayoutsReadFinished(const std::string &pStrResult, const std::exception_ptr &pException,
										const std::shared_ptr<Gio::File> &pFile) {
	if (pException) {
		const auto msg = sdk::utils::ReportMessage::create();
		msg->setExceptionPtr(pException);
		msg->setTitle("Failed to read user-defined Layouts file ");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Path to user Layouts file: {}", pFile->get_path());
		msg->addInfoLine("Using internal file.");
		presenter->displayError(msg);
		readInternalLayouts();
		return;
	}
	parseLayoutsContent(pStrResult, true);
}

void ModelMain::readInternalLayouts(const std::shared_ptr<Gio::OutputStream> &pSaveTo,
									const std::shared_ptr<Gio::File> &pUserFile) {
	auto resourceStream = Gio::Resource::open_stream_global("/paned-layouts/Layouts.json");
	auto strStream = utils::StringInputStream::create(resourceStream);
	strStream->readAllAsync(
		sigc::bind(sigc::mem_fun(*this, &ModelMain::internalLayoutsReadFinished), pSaveTo, pUserFile));
}

void ModelMain::internalLayoutsReadFinished(const std::string &pStrResult, const std::exception_ptr &pException,
											const std::shared_ptr<Gio::OutputStream> &pSaveTo,
											const std::shared_ptr<Gio::File> &pUserFile) {
	if (pException) {
		const auto msg = sdk::utils::ReportMessage::create();
		msg->setExceptionPtr(pException);
		msg->setTitle("Failed to read internal Layouts file ");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Cannot display anything.");
		presenter->displayError(msg);
		presenter->onLayoutLoadFatal();
		return;
	}
	parseLayoutsContent(pStrResult, false);
	if (!pSaveTo || !pUserFile) return;
	pSaveTo->write_all_async(pStrResult.c_str(), pStrResult.size(),
							 [pSaveTo, pUserFile, this](const std::shared_ptr<Gio::AsyncResult> &pResult) {
								 gsize size = 0;
								 try {
									 pSaveTo->write_all_finish(pResult, size);
								 } catch (...) {
									 const auto msg = sdk::utils::ReportMessage::create();
									 msg->setTitle("Failed to save internal Layouts file to user directory");
									 msg->setMessage("Exception occurred");
									 msg->addInfoLine("Path to user Layouts file: {}", pUserFile->get_path());
									 msg->addInfoLine("Bytes written: {}", size);
									 msg->addInfoLine("File may be corrupted.");
									 presenter->displayError(msg);
								 }
							 });
}

void ModelMain::parseLayoutsContent(const std::string &pContent, const bool pUser) {
	try {
		nlohmann::json json = nlohmann::json::parse(pContent);
		std::vector<PanedLayoutTab> tabs;
		json.at("Tabs").get_to(tabs);
		setPanedLayoutTabs(tabs);
		int32_t curTab = 0;
		json.at("CurrentTab").get_to(curTab);
		setCurrentTab(curTab);
	} catch (...) {
		const auto msg = sdk::utils::ReportMessage::create();
		msg->setMessage("Exception occurred");
		if (pUser) {
			msg->setTitle("Failed to parse user Layouts file");
			msg->addInfoLine("Trying to parse internal Layouts file.");
			readInternalLayouts();
		} else {
			msg->setTitle("Failed to parse internal Layouts file");
			msg->addInfoLine("Cannot display anything.");
			presenter->onLayoutLoadFatal();
		}
		presenter->displayError(msg);
	}
}
} // namespace mer::editor::mvp
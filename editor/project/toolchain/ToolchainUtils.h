//
// Created by alexus on 11.12.23.
//

#ifndef TOOLCHAINUTILS_H
#define TOOLCHAINUTILS_H

namespace mer::editor::project {
class ToolchainUtils {
public:
	static void execute(const std::filesystem::path &pRootPath, const std::filesystem::path &path,
						const std::string &pArgs, const sigc::slot<void(const std::string &log)> &pCoutCallback,
						const sigc::slot<void(const std::string &log)> &pCerrCallback,
						const sigc::slot<void(int pReturnValue)> &pOnFinish = {});
	static int executeSync(const std::filesystem::path &pRootPath, const std::filesystem::path &pPath,
						   const std::string &pArgs, const sigc::slot<void(const std::string &pLog)> &pCoutCallback,
						   const sigc::slot<void(const std::string &pLog)> &pCerrCallback);

	static sdk::utils::ReportMessagePtr generateCMakeListsFile(const std::filesystem::path &pPath);

	static sdk::utils::ReportMessagePtr generateCMakePresetsFile(const std::filesystem::path &pPath);

	static sdk::utils::ReportMessagePtr generateCMakeUserPresetsFile(const std::filesystem::path &pPath);

	static sdk::utils::ReportMessagePtr generateVcpkgManifestFile(const std::filesystem::path &pPath);

	static sdk::utils::ReportMessagePtr generateCMakeDirectory(const std::filesystem::path &pPath);

	static sdk::utils::ReportMessagePtr writeFile(const std::filesystem::path &pPath, const std::string &pContents);
};
} // namespace mer::editor::project

#endif //TOOLCHAINUTILS_H

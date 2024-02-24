//
// Created by alexus on 27.01.24.
//

#ifndef SCENESINFO_H
#define SCENESINFO_H
#include "GeneratedFileEntry.h"
#include "SceneInfo.h"

namespace mer::editor::project {
class ScenesInfo : public GeneratedFileEntry {


protected:
	explicit ScenesInfo(const std::shared_ptr<Project> &pProject);

public:
	static std::shared_ptr<ScenesInfo> create(const std::shared_ptr<Project> &pProject) {

		return Glib::make_refptr_for_instance(new ScenesInfo(pProject));
	}

	void getActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup);

private:
	sdk::utils::ReportMessagePtr onLoadDatabase() override;

	sdk::utils::ReportMessagePtr onSaveDatabase() const override;

	sdk::utils::ReportMessagePtr createTable() const;
};
} // namespace mer::editor::project


#endif //SCENESINFO_H

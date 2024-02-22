//
// Created by alexus on 29.01.24.
//

#include "PresenterAppInfo.h"

#include <project/Project.h>
#include <project/generatedFiles/ApplicationInfo.h>

#include "IModelAppInfo.h"
#include "IViewAppInfo.h"

namespace MVP_CORE {

PresenterAppInfo::PresenterAppInfo(const std::shared_ptr<IViewAppInfo> &pViewAppInfo,
								   const std::shared_ptr<IModelAppInfo> &pModelAppInfo)
	: viewAppInfo(pViewAppInfo), modelAppInfo(pModelAppInfo) {
	viewAppInfo->setSlotCreateModel(
		[this](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
			auto col = std::dynamic_pointer_cast<project::AppInfoProperty>(pItem);
			Glib::RefPtr<Gio::ListStore<project::AppInfoProperty>> result;
			if (!col) { result = modelAppInfo->getInfo()->getPropertiesList(); }
			// If result is empty, it's a leaf in the tree, i.e. an item without children.
			// Returning an empty RefPtr (not a RefPtr with an empty Gio::ListModel)
			// signals that the item is not expandable.
			return result;
			//	return (result->get_n_items() > 0) ? result : Glib::RefPtr<Gio::ListModel>();
		});
	viewAppInfo->setValueChangedSlot(
		[this](const std::shared_ptr<project::AppInfoProperty> &pInfoProperty, const std::string &pNewValue) {
			pInfoProperty->setValue(pNewValue);
			tabHeaderChanged("*" + modelAppInfo->getInfo()->getName());
		});
}

PresenterAppInfo::operator Gtk::Widget&() { return viewAppInfo->getMainWidget(); }
} // namespace MVP_CORE
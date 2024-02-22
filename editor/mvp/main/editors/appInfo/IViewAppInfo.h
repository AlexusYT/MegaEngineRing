//
// Created by alexus on 29.01.24.
//

#ifndef IVIEWAPPINFO_H
#define IVIEWAPPINFO_H
#include <mvp/IView.h>
#include <ui/customWidgets/TreeWidget.h>

namespace n::core::project {
class AppInfoProperty;
} // namespace n::core::project

namespace MVP_CORE {

class IViewAppInfo : public IView {
public:
	using ValueChangedSlot =
		sigc::slot<void(const std::shared_ptr<project::AppInfoProperty> &pInfoProperty, const std::string &pNewValue)>;
	virtual void setSlotCreateModel(const ui::TreeWidget::SlotCreateModel &pSlotCreateModel) = 0;

	virtual Gtk::Widget &getMainWidget() = 0;

	[[nodiscard]] virtual const ValueChangedSlot &getValueChangedSlot() const = 0;

	virtual void setValueChangedSlot(const ValueChangedSlot &pValueChangedSlot) = 0;
};

} // namespace MVP_CORE

#endif //IVIEWAPPINFO_H

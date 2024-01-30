//
// Created by alexus on 08.01.24.
//

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <EngineUtils/utils/UUID.h>
#include <ranges>
#include <uuid/uuid.h>

class SceneObject : public Glib::Object {
	UUID uuid;
	SceneObject* parent{};
	using ChildrenMap = std::unordered_map<UUID, std::shared_ptr<SceneObject>>;
	ChildrenMap children;
	Glib::RefPtr<Gio::ListStore<SceneObject>> childrenUi;
	std::string name;

protected:
	SceneObject() {
		childrenUi = Gio::ListStore<SceneObject>::create();
		UUID::create(uuid);
	}

public:
	SceneObject(const SceneObject &pOther) = delete;

	SceneObject(SceneObject &&pOther) noexcept
		: uuid(std::move(pOther.uuid)), parent(pOther.parent), children(std::move(pOther.children)),
		  name(std::move(pOther.name)) {}

	SceneObject &operator=(const SceneObject &pOther) = delete;

	SceneObject &operator=(SceneObject &&pOther) noexcept {
		if (this == &pOther) return *this;
		uuid = std::move(pOther.uuid);
		parent = pOther.parent;
		children = std::move(pOther.children);
		name = std::move(pOther.name);
		return *this;
	}

	~SceneObject() override = default;

	template<typename T, typename... Args>
	static std::shared_ptr<T> create(Args... pArgs) {
		return Glib::make_refptr_for_instance<T>(new (std::nothrow) T(pArgs...));
	}

	[[nodiscard]] const UUID &getUuid() const { return uuid; }

	void setUuid(const UUID &pUuid) { uuid = pUuid; }

	[[nodiscard]] SceneObject* getParent() const { return parent; }

	void setParent(SceneObject* pParent) { parent = pParent; }

	[[nodiscard]] const ChildrenMap &getChildren() const { return children; }

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<SceneObject>> &getChildrenUi() const { return childrenUi; }

	void addChild(std::shared_ptr<SceneObject> pChild) {
		pChild->setParent(this);
		children.emplace(pChild->getUuid(), pChild);
		childrenUi->append(pChild);
	}

	bool removeChild(const UUID &pUuid) {
		if (const auto iter = children.find(pUuid); iter != children.end()) {
			children.erase(iter);
			childrenUi->remove(childrenUi->get_n_items() - 1);
			return true;
		}
		return false;
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	virtual std::string getType() { return "sss"; }
};

/*
class SceneObject {
	UUID uuid;
	SceneObject* parent{};
	using ChildrenMap = std::unordered_map<UUID, std::shared_ptr<SceneObject>>;
	ChildrenMap children;
	std::string name;

protected:
	SceneObject() { UUID::create(uuid); }

public:
	SceneObject(const SceneObject &pOther) = delete;

	SceneObject(SceneObject &&pOther) noexcept
		: uuid(std::move(pOther.uuid)), parent(pOther.parent), children(std::move(pOther.children)),
		  name(std::move(pOther.name)) {}

	SceneObject &operator=(const SceneObject &pOther) = delete;

	SceneObject &operator=(SceneObject &&pOther) noexcept {
		if (this == &pOther) return *this;
		uuid = std::move(pOther.uuid);
		parent = pOther.parent;
		children = std::move(pOther.children);
		name = std::move(pOther.name);
		return *this;
	}

	virtual ~SceneObject() = default;

	template<typename T, typename... Args>
	static std::shared_ptr<T> create(Args... pArgs) {
		return std::shared_ptr<T>(new (std::nothrow) T(pArgs...));
	}

	[[nodiscard]] const UUID &getUuid() const { return uuid; }

	void setUuid(const UUID &pUuid) { uuid = pUuid; }

	[[nodiscard]] SceneObject* getParent() const { return parent; }

	void setParent(SceneObject* pParent) { parent = pParent; }

	[[nodiscard]] const ChildrenMap &getChildren() const { return children; }

	void addChild(std::shared_ptr<SceneObject> pChild) {
		pChild->setParent(this);
		children.emplace(pChild->getUuid(), pChild);
	}

	bool removeChild(const UUID &pUuid) {
		if (const auto iter = children.find(pUuid); iter != children.end()) {
			children.erase(iter);
			return true;
		}
		return false;
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	SceneObject* findByUuid(const UUID &pUuid) { return nullptr; }

	virtual std::string getType() = 0;
};*/

template<>
struct std::formatter<SceneObject> : std::formatter<string_view> {
	auto format(const SceneObject &col, std::format_context &ctx) const {
		std::string temp;
		std::format_to(std::back_inserter(temp), "Name: {}\nUUID: {}", col.getName(), col.getUuid());
		return std::formatter<string_view>::format(temp, ctx);
	}
};

#endif //SCENEOBJECT_H

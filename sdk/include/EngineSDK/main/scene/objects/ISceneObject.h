//
// Created by alexus on 12.02.24.
//

#ifndef ISCENEOBJECT_H
#define ISCENEOBJECT_H
#include <EngineUtils/utils/ReportMessageFwd.h>

namespace mer::sdk::utils {
class ModifierKeys;
enum class KeyboardKey;
} // namespace mer::sdk::utils

namespace mer::sdk::main {
class IScene;

class ISceneObject {
public:
	virtual ~ISceneObject() = default;

	virtual utils::ReportMessagePtr init() = 0;

	virtual void render() const = 0;

	virtual void onWindowSizeChanged(int pWidth, int pHeight) const = 0;

	virtual void onCursorPosChanged(double pX, double pY) const = 0;

	virtual void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) const = 0;

	virtual void setScene(IScene* pScene) = 0;
};

} // namespace mer::sdk::main

#endif //ISCENEOBJECT_H

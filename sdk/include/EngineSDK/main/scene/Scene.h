//
// Created by alexus on 02.02.24.
//

#ifndef SCENE_H
#define SCENE_H
#include <EngineSDK/main/buffers/ProgramWideShaderBuffer.h>
#include <EngineSDK/main/resources/LazyResource.h>

#include "EngineSDK/main/resources/Resources.h"
#include "IScene.h"

namespace mer::sdk::main {
class Camera;
class IActor;
class ISceneObject;
} // namespace mer::sdk::main

namespace mer::sdk::main {
class Scene : public IScene {
	std::shared_ptr<Resources> resources;
	std::shared_ptr<ProgramWideShaderBuffer> programBuffer;
	std::vector<std::shared_ptr<IActor>> objects;
	std::shared_ptr<Camera> camera;

public:
	Scene();

	[[nodiscard]] const std::shared_ptr<Resources> &getResources() const final { return resources; }

	/**
	 * \copydoc Resources::hasResource
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->hasResource(pName);
	 * \endcode
	 */
	[[nodiscard]] bool hasResource(const std::string &pName) const { return resources->hasResource(pName); }

	/**
	 * \copydoc Resources::isLazyResource
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->isLazyResource(pName);
	 * \endcode
	 */
	bool isLazyResource(const std::string &pName) const { return resources->isLazyResource(pName); }

	/**
	 * \copydoc Resources::get
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->get<ClassT>(pName);
	 * \endcode
	 *
	 * Если загрузка ресурса была запрошена в ленивом режиме, то следует использовать один из следующих методов:
	 *  - getLazyResource()
	 *  - getLazyResourceSync()
	 *  - getLazyResourceAsync()
	 *  - getLazyResourceFuture()
	 */
	template<typename ClassT>
	std::shared_ptr<ClassT> getResource(const std::string &pName) const {
		return resources->get<ClassT>(pName);
	}

	template<RequestRegularConcept ClassT>
	auto getResourceByRequest(const std::shared_ptr<ClassT> &pRequest) const {
		return resources->getByRequest(pRequest);
	}

	template<RequestLazyConcept ClassT>
	auto getResourceByRequest(const std::shared_ptr<ClassT> &pRequest) const {
		return resources->getByRequest(pRequest);
	}

	/**
	 * \copydoc Resources::getLazyResource
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->getLazyResource(pName);
	 * \endcode
	 *
	 * Если загрузка ресурса была запрошена в обычном режиме, то следует использовать метод getResource().
	 */
	std::shared_ptr<LazyResource> getLazyResource(const std::string &pName) const {
		return resources->getLazyResource(pName);
	}

	/**
	 * \copybrief Resources::getLazyResource
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->getLazyResource()->getSync<ClassT>();
	 * \endcode
	 *
	 * Если загрузка ресурса была запрошена в обычном режиме, то следует использовать метод getResource().
	 *
	 * Эта версия метода выполняет загрузку синхронно (т.е. с блокированием потока). Асинхронная версия -
	 * getLazyResourceAsync()
	 * \tparam ClassT Класс ресурса, генерируемый классом, проиводным от ResourceLoader
	 * \param pName Название ресурса, которое было указано в запросе ранее
	 * \return Указатель на ClassT ресурса или на nullptr в случае, если ресурс не найден или невозможно преобразовать
	 * ресурс к ClassT. Для проверки существования ресурса, используйте isResourceLoaded()
	 */
	template<typename ClassT>
	std::shared_ptr<ClassT> getLazyResourceSync(const std::string &pName) const {
		return getLazyResource(pName)->getSync<ClassT>();
	}

	/**
	 * \copybrief Resources::getLazyResource
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->getLazyResource()->getAsync<ClassT>(pSlot);
	 * \endcode
	 *
	 * Если загрузка ресурса была запрошена в обычном режиме, то следует использовать метод getResource().
	 *
	 * Эта версия метода выполняет загрузку асинхронно (т.е. без блокирования потока). Синхронная версия -
	 * getLazyResourceSync()
	 * \tparam ClassT Класс ресурса, генерируемый классом, проиводным от ResourceLoader
	 * \param pName Название ресурса, которое было указано в запросе ранее
	 * \param pSlot Функция обратного вызова, которая будет вызвана после завершения загрузки
	 */
	template<typename ClassT>
	void getLazyResourceAsync(const std::string &pName,
							  const sigc::slot<void(std::shared_ptr<ClassT> pResource)> &pSlot) const {
		return getLazyResource(pName)->getAsync<ClassT>(pSlot);
	}

	/**
	 * \copybrief Resources::getLazyResource
	 *
	 * Вызов этого метода эквивалентен вызову
	 *
	 * \code
	 *	getResources()->getLazyResource()->get<ClassT>();
	 * \endcode
	 *
	 * Если загрузка ресурса была запрошена в обычном режиме, то следует использовать метод getResource().
	 *
	 * Эта версия метода выполняет загрузку асинхронно (т.е. без блокирования потока) и записывает результат в
	 * std::future.
	 * \tparam ClassT Класс ресурса, генерируемый классом, проиводным от ResourceLoader
	 * \param pName Функция обратного вызова, которая будет вызвана после завершения загрузки
	 * \return std::future c указатель на ClassT ресурса или на nullptr в случае, если ресурс не найден или невозможно
	 * преобразовать ресурс к ClassT. Для проверки существования ресурса, используйте isResourceLoaded()
	 *
	 */
	template<typename ClassT>
	std::future<std::shared_ptr<ClassT>> getLazyResourceFuture(const std::string &pName) const {
		return getLazyResource(pName)->get<ClassT>();
	}

	void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest, const sdk::utils::ReportMessagePtr& pError) override;


protected:
	virtual void beforeRender();

	virtual void afterRender() {}

	sdk::utils::ReportMessagePtr initScene() override;

	sdk::utils::ReportMessagePtr preloadScene(const std::shared_ptr<ResourceRequests>& pRequests) override;

	void addObject(const std::shared_ptr<IActor> &pObject) ;

private:
	void setResources(const std::shared_ptr<Resources> &pResources) final;

	void render() final;

	void resize(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(utils::KeyboardKey pKey, bool pPressed, utils::ModifierKeys pMods) override;

};
} // namespace mer::sdk::main


#endif //SCENE_H
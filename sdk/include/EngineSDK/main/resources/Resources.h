// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 01.02.24.
//

#ifndef RESOURCES_H
#define RESOURCES_H
#include <memory>
#include <string>
#include <unordered_map>

#include "IResource.h"

namespace mer::sdk::main {
class RegularRequest;
class LazyRequest;
} // namespace mer::sdk::main

namespace mer::sdk::main {
class LazyResource;
class IResource;


template<class T>
concept RequestRegularConcept = requires(T) {
	requires std::derived_from<T, RegularRequest>;
	typename T::ResourceT;
	requires std::derived_from<typename T::ResourceT, IResource>;
};

template<class T>
concept RequestLazyConcept = requires(T pX) {
	requires std::derived_from<T, LazyRequest>;
	//requires pX.getPolicy() == ResourceLoadingPolicy::LAZY;
};

class Resources {
	friend class LoadedResources;
	std::unordered_map<std::string, std::shared_ptr<IResource>> resources;

public:
	/**
	 * \brief Выполняет поиск ресурса по имени
	 * \param pName Название ресурса для поиска
	 * \return указатель на IResource в случае успеха или на nullptr, если ресурс не найден.
	 */
	std::shared_ptr<IResource> find(const std::string &pName) const {
		if (const auto iter = resources.find(pName); iter == resources.end()) return nullptr;
		else
			return iter->second;
	}

	/**
	 * \brief Проверяет, существует ли ресурс с таким именем.
	 *
	 * \note Независимо от того, "ленивый" ли ресурс или же обычный, метод вернет `true`. Для проверки способа загрузки
	 * используйте isLazyResource().
	 * \param pName Название ресурса для проверки
	 * \return `true` если такой ресурс существует, `false` если нет
	 */
	bool hasResource(const std::string &pName) const { return find(pName) != nullptr; }

	/**
	 * \brief Проверят, является ли ресурс "ленивым".
	 *
	 * \note Метод вернет `false` если:
	 * - ресурс с таким именем не существует;
	 * - ресурс не "ленивый".
	 * \note Для того, чтобы различить причины, используйте этот метод в паре с hasResource()
	 * \code
	 * if(isLazyResource(name)) //Ленивый
	 * else if(hasResource(name)) //обычный
	 * else //Не существует
	 * \endcode
	 * \param pName Название ресурса для проверки
	 * \return `true` если ресурс "ленивый", `false` если нет или не существует
	 */
	bool isLazyResource(const std::string &pName) const;

	/**
	 * \brief Возвращает ранее загруженный ресурс
	 * \tparam ClassT Класс ресурса, генерируемый классом, проиводным от ResourceLoader
	 * \param pName Название ресурса, которое было указано в запросе ранее
	 * \return Указатель на ClassT ресурса или на nullptr в случае, если ресурс не найден или невозможно преобразовать
	 * ресурс к ClassT. Для проверки существования ресурса, используйте hasResource()
	 */
	template<typename ClassT>
	std::shared_ptr<ClassT> get(const std::string &pName) const {
		if (const auto res = find(pName)) return std::dynamic_pointer_cast<ClassT>(res);
		return nullptr;
	}

	template<RequestRegularConcept ClassT>
	std::shared_ptr<typename ClassT::ResourceT> getByRequest(const std::shared_ptr<ClassT> &pRequest) const {
		return get<typename ClassT::ResourceT>(pRequest->getName());
	}

	template<RequestLazyConcept ClassT>
	std::shared_ptr<LazyResource> getByRequest(const std::shared_ptr<ClassT> &pRequest) const {
		return getLazyResource(pRequest->getName());
	}

	/**
	 * \brief Возвращает "ленивый" ресурс, который содержит требуемый ресурс, либо запрос на его загрузку.
	 * \param pName Название ресурса, которое было указано в запросе ранее
	 * \return Указатель на LazyResource или на nullptr в случае, если ресурс не найден или невозможно преобразовать
	 * ресурс к LazyResource. Для проверки существования ресурса, используйте hasResource()
	 */
	std::shared_ptr<LazyResource> getLazyResource(const std::string &pName) const;

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<IResource>> &getResources() const {
		return resources;
	}

	void clear() {resources.clear();}
};

} // namespace mer::sdk::main


#endif //RESOURCES_H

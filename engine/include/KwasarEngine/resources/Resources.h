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
// Created by alexus on 01.02.24.
//

#ifndef RESOURCES_H
#define RESOURCES_H
#include <memory>
#include <string>
#include <unordered_map>

#include "IResource.h"

namespace ke {
class IResource;

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
	 * \param pName Название ресурса для проверки
	 * \return `true` если такой ресурс существует, `false` если нет
	 */
	bool hasResource(const std::string &pName) const { return find(pName) != nullptr; }

	/**
	 * \brief Возвращает ранее загруженный ресурс
	 * \tparam ClassT Класс ресурса, генерируемый классом, проиводным от ResourceLoader
	 * \param pName Название ресурса, которое было указано в запросе ранее
	 * \return Указатель на ClassT ресурса или на nullptr в случае, если ресурс не найден или невозможно преобразовать
	 * ресурс к ClassT. Для проверки существования ресурса, используйте hasResource()
	 */
	template <typename ClassT>
	std::shared_ptr<ClassT> get(const std::string &pName) const {
		if (const auto res = find(pName)) return std::dynamic_pointer_cast<ClassT>(res);
		return nullptr;
	}

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<IResource>> &getResources() const {
		return resources;
	}

	void clear() { resources.clear(); }
};
} // namespace ke


#endif //RESOURCES_H

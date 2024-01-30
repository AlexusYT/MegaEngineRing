//
// Created by Alexus on 02.09.2022.
//

#ifndef ELECTRICITY_SIMULATOR_UIUTILS_H
#define ELECTRICITY_SIMULATOR_UIUTILS_H


#include <glm/gtx/euler_angles.hpp>
#include <vector>

class UIUtils {
public:
	static void getCubeVertices(std::vector<glm::vec3> &vertices, const glm::vec3 &minSize, const glm::vec3 &maxSize);
	static void getCubeVertices(std::vector<glm::vec3> &vertices, const glm::vec3 &size);
	static void getCubeVerticesOffset(std::vector<glm::vec3> &vertices, const glm::vec3 &size, const glm::vec3 &pos);

	static void getCubeIndices(std::vector<uint16_t> &indices, uint16_t offset = 0);

	static glm::vec3 calculateNormal(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {

		const glm::vec3 A = v2 - v1;
		const glm::vec3 B = v3 - v1;

		return glm::normalize(glm::vec3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x));
	}

	static constexpr glm::vec3 getEntityRotationInGUI() {
		return {glm::radians(-49.5), glm::radians(207.4), glm::radians(28.5)};
	}

	static glm::mat4 getModelViewMatrixForSlot(const glm::vec3 &size, const glm::vec3 &pos, int slotSize) {

		glm::mat4 rotationMatrix = UIUtils::eulerAngleYXZ(glm::mat4(1), getEntityRotationInGUI());
		glm::vec3 meshSize = rotationMatrix * glm::vec4(size, 1);

		float scale = float(slotSize) / std::max(meshSize.x, std::max(meshSize.y, meshSize.z));

		return glm::translate(glm::mat4(1), pos) * rotationMatrix * glm::scale(glm::mat4(1), {scale, scale, scale});
	}

	/**
	 * @brief Calculates ModelView matrix using only given position.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param position of mesh.
	 */
	static glm::mat4 buildModelViewMatrixP(const glm::mat4 &mat, const glm::vec3 &position);

	/**
	 * @brief Calculates ModelView matrix using only given position.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param scale of mesh.
	 */
	static glm::mat4 buildModelViewMatrixS(const glm::mat4 &mat, const glm::vec3 &scale);

	/**
	 * @brief Calculates ModelView matrix using only given rotation.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param rotation of mesh.
	 */
	static glm::mat4 buildModelViewMatrixR(const glm::mat4 &mat, const glm::vec3 &rotation);

	/**
	 * @brief Calculates ModelView matrix using given position and rotation.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param position of mesh.
	 * @param rotation of mesh.
	 */
	static glm::mat4 buildModelViewMatrixPR(const glm::mat4 &mat, const glm::vec3 &position, const glm::vec3 &rotation);

	/**
	 * @brief Calculates ModelView matrix using given position and scale.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param position of mesh.
	 * @param scale of mesh.
	 */
	static glm::mat4 buildModelViewMatrixPS(const glm::mat4 &mat, const glm::vec3 &position, const glm::vec3 &scale);

	/**
	 * @brief Calculates ModelView matrix using given rotation and scale.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param rotation of mesh.
	 * @param scale of mesh.
	 */
	static glm::mat4 buildModelViewMatrixRS(const glm::mat4 &mat, const glm::vec3 &rotation, const glm::vec3 &scale);

	/**
	 * @brief Calculates ModelView matrix using given position, rotation and scale.
	 * @param mat matrix to transform. Usually View-Projection matrix or identity matrix.
	 * @param position of mesh.
	 * @param rotation of mesh.
	 * @param scale of mesh.
	 */
	static glm::mat4 buildModelViewMatrixPRS(const glm::mat4 &mat, const glm::vec3 &position, const glm::vec3 &rotation,
											 const glm::vec3 &scale);

	template<typename T>
	static glm::mat<4, 4, T, glm::defaultp> eulerAngleYXZ(glm::mat<4, 4, T, glm::defaultp> in,
														  const glm::vec<3, T, glm::defaultp> &angles) {
		return in * glm::eulerAngleYXZ(angles.x, angles.y, angles.z);
	}
};


#endif //ELECTRICITY_SIMULATOR_UIUTILS_H

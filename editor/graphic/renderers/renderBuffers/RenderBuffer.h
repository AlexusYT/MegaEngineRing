//
// Created by alexus on 25.07.23.
//

#ifndef MAIN_RENDERBUFFER_H
#define MAIN_RENDERBUFFER_H

#include <Render/opengl/buffers/IndexBufferObject.h>
#include <Render/opengl/buffers/VertexBufferObject.h>

template<typename ElementType, typename BufferType>
class RenderBuffer {
public:
	using ElementT = ElementType;
	using BufferT = BufferType;

	struct BufferInfo {
		std::vector<ElementType> data;
		std::mutex mutex;
		BufferType buffer;
		bool raw{};
	};

	BufferInfo info;

	virtual ~RenderBuffer() = default;

protected:
	template<typename = std::enable_if<std::is_same<BufferT, rel::opengl::VertexBufferObjectSPtr>::value>>
	[[nodiscard]] BufferType::element_type::Builder getBufferBuilder(uint32_t indexInShader) const {

		return typename BufferType::element_type::Builder()
			.setBufferData(getBufferData())
			.setUsage(rel::opengl::DYNAMIC_DRAW)
			.addVertexAttribPointer(indexInShader, ElementT::length())
			.enableAttrib(indexInShader);
	}

	template<typename = std::enable_if<std::is_same<BufferType, rel::opengl::VertexBufferObjectSPtr>::value>>
	[[nodiscard]] BufferType::element_type::Builder getBufferBuilder() const {

		return typename BufferType::element_type::Builder()
			.setBufferData(getBufferData())
			.setUsage(rel::opengl::STATIC_DRAW);
	}

	void updateBuffer() {
		if (!info.raw) return;
		const std::lock_guard<std::mutex> lockGuard(info.mutex);
		info.buffer->setBufferData(info.data, rel::opengl::DYNAMIC_DRAW);
		info.raw = false;
	}

	void setBuffer(BufferT buffer) { info.buffer = std::move(buffer); }

	virtual void invalidate() {
		const std::lock_guard<std::mutex> lockGuard(info.mutex);
		info.data.clear();
		getData(info.data);
		info.raw = true;
		onInvalidated();
	}

	virtual void getData(std::vector<ElementType> &data) = 0;

	virtual void onInvalidated() = 0;

	[[nodiscard]] const std::vector<ElementT> &getBufferData() const { return info.data; }
};


#endif //MAIN_RENDERBUFFER_H

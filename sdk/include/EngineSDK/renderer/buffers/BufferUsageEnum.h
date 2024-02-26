//
// Created by alexus on 04.02.24.
//

#ifndef BUFFERUSAGEENUM_H
#define BUFFERUSAGEENUM_H

namespace mer::sdk::renderer {

enum BufferUsageEnum : uint32_t {
	STREAM_DRAW = 0x88E0,
	STREAM_READ = 0x88E1,
	STREAM_COPY = 0x88E2,
	STATIC_DRAW = 0x88E4,
	STATIC_READ = 0x88E5,
	STATIC_COPY = 0x88E6,
	DYNAMIC_DRAW = 0x88E8,
	DYNAMIC_READ = 0x88E9,
	DYNAMIC_COPY = 0x88EA
};
}


#endif //BUFFERUSAGEENUM_H

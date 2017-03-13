#pragma once

#include "base.h"
#include "framebuffer.h"
#include "scene.h"

namespace Tracer {

int trace(Scene* scene, Viewer& view, BufferInfo info);

};

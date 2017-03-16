#pragma once

#include "linmath.h"
#include "base.h"
#include "framebuffer.h"
#include "scene.h"
#include "geometries.h"
#include "materials.h"

namespace Tracer {

int trace(Scene* scene, BufferInfo info);

};

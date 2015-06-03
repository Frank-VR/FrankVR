//
//  RenderableLineEntityItem.cpp
//  libraries/entities-renderer/src/
//
//  Created by Seth Alves on 5/11/15.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include <glm/gtx/quaternion.hpp>

#include <gpu/GPUConfig.h>
#include <gpu/Batch.h>
#include <GeometryCache.h>

#include <DeferredLightingEffect.h>
#include <PerfStat.h>

#include "RenderableLineEntityItem.h"

EntityItemPointer RenderableLineEntityItem::factory(const EntityItemID& entityID, const EntityItemProperties& properties) {
    return EntityItemPointer(new RenderableLineEntityItem(entityID, properties));
}

void RenderableLineEntityItem::render(RenderArgs* args) {
    PerformanceTimer perfTimer("RenderableLineEntityItem::render");
    Q_ASSERT(getType() == EntityTypes::Line);
    glm::vec3 p1 = ENTITY_ITEM_ZERO_VEC3;
    glm::vec3 p2 = getDimensions();
    glm::vec4 lineColor(toGlm(getXColor()), getLocalRenderAlpha());

    Q_ASSERT(args->_batch);
    gpu::Batch& batch = *args->_batch;
    batch.setModelTransform(getTransformToCenter());

    glLineWidth(getLineWidth());
    auto geometryCache = DependencyManager::get<GeometryCache>();
    if (_lineVerticesID == GeometryCache::UNKNOWN_ID) {
        _lineVerticesID = geometryCache ->allocateID();
    }

    //TODO: Figure out clean , efficient way to do relative line positioning. For now we'll just use absolute positioning.
    //glTranslatef(position.x, position.y, position.z);
    //glm::vec3 axis = glm::axis(rotation);
    //glRotatef(glm::degrees(glm::angle(rotation)), axis.x, axis.y, axis.z);
    if (_pointsChanged) {
      geometryCache->updateVertices(_lineVerticesID, getLinePoints(), lineColor);
        _pointsChanged = false;
    }
    geometryCache->renderVertices(gpu::LINE_STRIP, _lineVerticesID);
    
    RenderableDebugableEntityItem::render(this, args);
};

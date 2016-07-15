//
//  GLBackendQuery.cpp
//  libraries/gpu/src/gpu
//
//  Created by Sam Gateau on 7/7/2015.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#include "GLBackend.h"
#include "GLQuery.h"

using namespace gpu;
using namespace gpu::gl;

void GLBackend::do_beginQuery(Batch& batch, size_t paramOffset) {
    auto query = batch._queries.get(batch._params[paramOffset]._uint);
    GLQuery* glquery = syncGPUObject(*query);
    if (glquery) {
        glQueryCounter(glquery->_beginqo, GL_TIMESTAMP);
        (void)CHECK_GL_ERROR();
    }
}

void GLBackend::do_endQuery(Batch& batch, size_t paramOffset) {
    auto query = batch._queries.get(batch._params[paramOffset]._uint);
    GLQuery* glquery = syncGPUObject(*query);
    if (glquery) {
        glQueryCounter(glquery->_endqo, GL_TIMESTAMP);
        (void)CHECK_GL_ERROR();
    }
}

void GLBackend::do_getQuery(Batch& batch, size_t paramOffset) {
    auto query = batch._queries.get(batch._params[paramOffset]._uint);
    GLQuery* glquery = syncGPUObject(*query);
    if (glquery) { 
        glGetQueryObjectui64v(glquery->_endqo, GL_QUERY_RESULT_AVAILABLE, &glquery->_result);
        if (glquery->_result == GL_TRUE) {
            GLuint64 start, end;
            glGetQueryObjectui64v(glquery->_beginqo, GL_QUERY_RESULT, &start);
            glGetQueryObjectui64v(glquery->_endqo, GL_QUERY_RESULT, &end);
            glquery->_result = end - start;
            query->triggerReturnHandler(glquery->_result);
        }
        (void)CHECK_GL_ERROR();
    }
}

void GLBackend::resetQueryStage() {
}

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#include <string.h>

#include "Vertex.h"
#include "modelLoader.h"
#include "model.h"

void *getBufferOffset(cgltf_buffer_view *bufferView) {
    return (uint8_t *)bufferView->buffer->data + bufferView->offset;
}

void *getAccessorOffset(cgltf_accessor *accessor) {
    return (uint8_t *)getBufferOffset(accessor->buffer_view) + accessor->offset;
}

void applySparce(cgltf_accessor *accessor, void *dest, size_t size) {
    uint16_t *indices = NULL;
    uint8_t *newVal = NULL;

    if (accessor->is_sparse) {
        indices = getBufferOffset(accessor->sparse.indices_buffer_view);
        newVal = getBufferOffset(accessor->sparse.values_buffer_view);

        for (uint16_t i = 0; i < accessor->sparse.count; i += 1) {
            memcpy((uint8_t *)dest + size * indices[i], newVal + size * i, size);
        }
    }
}

static void loadModel(const char *filePath, struct Model *model) {
    cgltf_options options = { 0 };
    cgltf_data *data = NULL;

    if (cgltf_result_success == cgltf_parse_file(&options, filePath, &data))
    if (cgltf_result_success == cgltf_load_buffers(&options, data, filePath)) {
        model->meshQuantity = data->meshes_count;
        model->mesh = malloc(sizeof(struct Mesh) * model->meshQuantity);
        for (uint32_t j = 0; j < data->meshes_count; j += 1) {
            cgltf_primitive* primitive = data->meshes[j].primitives;

            cgltf_accessor *index_accessor = primitive->indices;
            cgltf_accessor *vertex_accessor = NULL;

            for (size_t i = 0; i < primitive->attributes_count; i += 1) {
                if (primitive->attributes[i].type == cgltf_attribute_type_position) {
                    vertex_accessor = primitive->attributes[i].data;
                    break;
                }
            }
            model->mesh[j].indicesQuantity = index_accessor->count;
            model->mesh[j].verticesQuantity = vertex_accessor->count;
            model->mesh[j].vertices = malloc(sizeof(struct Vertex) * model->mesh[j].verticesQuantity);
            model->mesh[j].indices = malloc(sizeof(uint16_t) * model->mesh[j].indicesQuantity);

            memcpy(model->mesh[j].indices, getAccessorOffset(index_accessor), sizeof(uint16_t) * model->mesh[j].indicesQuantity);
            float localVertices[model->mesh[j].verticesQuantity][3];
            memcpy(localVertices, getAccessorOffset(vertex_accessor), sizeof(float) * model->mesh[j].verticesQuantity * 3);

            applySparce(vertex_accessor, localVertices, sizeof(float[3]));
            applySparce(index_accessor, model->mesh[j].indices, sizeof(uint16_t));

            for (size_t i = 0; i < model->mesh[j].verticesQuantity; i += 1) {
                model->mesh[j].vertices[i] = (struct Vertex) {
                    .pos = {
                        [0] = localVertices[i][0],
                        [1] = localVertices[i][1],
                        [2] = localVertices[i][2]
                    },
                    .color = {
                        [0] = localVertices[i][0],
                        [1] = localVertices[i][1],
                        [2] = localVertices[i][2]
                    },
                    .texCoord = {
                        0, 0
                    }
                };
            }
        }

        /*
        cgltf_accessor *time_accessor = data->animations[0].channels[0].sampler->input;
        cgltf_accessor *rotation_accessor = data->animations[0].channels[0].sampler->output;

        uint16_t timeCount = time_accessor->count;
        uint16_t rotationCount = rotation_accessor->count;

        float *time = getAccessorOffset(time_accessor);
        float (*rotation)[4] = getAccessorOffset(rotation_accessor);

        for (uint16_t i = 0; i < timeCount; i += 1) {
            printf("%f\n", time[i]);
        }

        for (uint16_t i = 0; i < rotationCount; i += 1) {
            printf("%f %f %f %f\n", rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3]);
        }
        */

        cgltf_free(data);
    }
}

struct ModelBuilder gltfLoader(struct ModelBuilder a) {
    a.loadModel = loadModel;

    return a;
}

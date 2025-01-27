#include "unity.h"

#include "model.h"
#include "Vertex.h"
#include "modelBuilder.h"
#include "modelFunctions.h"
#include "modelBuilder.h"
#include "modelLoader.h"

struct ModelBuilder gltf;
struct ModelBuilder obj;

struct Model a;

[[maybe_unused]]
static void prepare_test() {
    gltf = gltfLoader(gltf);
    obj = objLoader(obj);
}

[[maybe_unused]]
static void cleanup_test() {

}

void test_objLoader(void) {
    prepare_test();

    obj.loadModel("models/my_floor.obj", &a, NULL, NULL, NULL);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[0].pos[0], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[0].pos[1], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[0].pos[2], -0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[1].pos[0],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[1].pos[1], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[1].pos[2], -0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[2].pos[0],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[2].pos[1],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[2].pos[2], -0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[3].pos[0], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[3].pos[1],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[3].pos[2], -0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[4].pos[0], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[4].pos[1], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[4].pos[2],  0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[5].pos[0],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[5].pos[1], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[5].pos[2],  0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[6].pos[0],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[6].pos[1],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[6].pos[2],  0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[7].pos[0], -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[7].pos[1],  0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, a.mesh[0].vertices[7].pos[2],  0.5f);
}



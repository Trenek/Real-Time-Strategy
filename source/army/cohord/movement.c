#include <time.h>
#include <string.h>

#include <cglm.h>
#include <GLFW/glfw3.h>

#include "instanceBuffer.h"
#include "VulkanTools.h"
#include "model.h"

#include "cohord.h"

void giveAcceleration(float deltaTime, struct cohort this) {
    vec2 center;
    vec2 deltaX;

    if (deltaTime != 0)
    for (uint32_t i = 0; i < this.info.instanceCount; i += 1) {
        glm_vec2_add(this.center, this.warriorPos[i], center);

        glm_vec2_sub(center, this.info.instance[i].pos, deltaX);
        float distance = glm_vec2_norm(deltaX);
        if (distance < this.info.instance[i].scale[0] / 2) { // vf2 = vi2 + 2ad
            glm_vec2_mul(this.info.instance[i].velocity, this.info.instance[i].velocity, this.info.instance[i].acceleration);
            glm_vec2_scale(this.info.instance[i].acceleration, -0.5f, this.info.instance[i].acceleration);
            glm_vec2_div(this.info.instance[i].acceleration, deltaX, this.info.instance[i].acceleration);
            if (isinf(this.info.instance[i].acceleration[0]) || isnan(this.info.instance[i].acceleration[0])) this.info.instance[i].acceleration[0] = 0.0f;
            if (isinf(this.info.instance[i].acceleration[1]) || isnan(this.info.instance[i].acceleration[1])) this.info.instance[i].acceleration[1] = 0.0f;
        }
        else { // vf = vi + at
            vec2 vf2; {
                glm_vec2_lerp((vec2) { 0.0f, 0.0f }, deltaX, this.info.instance[i].maxVelocity / distance, vf2);
            }
            glm_vec2_sub(vf2, this.info.instance[i].velocity, vf2);
            glm_vec2_divs(vf2, 1, this.info.instance[i].acceleration);
        }

        this.info.instance[i].acceleration[2] = -9.81f;
    }
}

static void maxVelocity(vec2 v, float max) {
    float d = glm_vec2_norm(v);

    if (d > max) {
        glm_vec2_lerp((vec2) { 0.0f, 0.0f }, v, max / d, v);
    }
}

void calculateVelocity(float deltaTime, uint32_t num, struct instance models[num]) {
    vec3 temp;

    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec3_scale(models[i].acceleration, deltaTime, temp);
        glm_vec3_add(models[i].velocity, temp, models[i].velocity);
        maxVelocity(models[i].velocity, models[i].maxVelocity);
    }
}

static void calculateNewVelocity(vec2 newVelocity, struct instance a1, struct instance a2) {
    vec2 deltaV;
    vec2 deltaX;
    vec2 velocityChange;

    glm_vec2_sub(a1.velocity, a2.velocity, deltaV);
    glm_vec2_sub(a1.pos, a2.pos, deltaX);

    float dotProduct = glm_vec2_dot(deltaV, deltaX);
    float norm = glm_vec2_norm(deltaX);

    glm_vec2_scale(deltaX, dotProduct / (norm * norm), velocityChange);
    glm_vec2_sub(a1.velocity, velocityChange, newVelocity);
}

void actualCollision(struct instance *this, struct instance *that) {
    float minDistance = (this->scale[0] + that->scale[0]) / 2.0f;
    float distance = glm_vec2_distance(that->pos, this->pos);

    if (distance < minDistance) {
        vec2 n; {
            float ratio = 0.5;
            float responseCoef = 0.75f;
            float delta = 0.5f * responseCoef * (distance - minDistance);

            glm_vec2_sub(this->pos, that->pos, n);
            glm_vec2_scale(n, ratio * delta / distance, n);
        }
        glm_vec2_sub(this->pos, n, this->pos);
        glm_vec2_add(that->pos, n, that->pos);

        vec2 a1;
        vec2 a2;

        calculateNewVelocity(a1, *this, *that);
        calculateNewVelocity(a2, *that, *this);

        glm_vec2_copy(a1, this->velocity);
        glm_vec2_copy(a2, that->velocity);
        maxVelocity(that->velocity, that->maxVelocity);
        maxVelocity(this->velocity, this->maxVelocity);
    }
}

int compInstance(const void* a, const void* b) {
    return 1000 * (((struct instance *)a)->pos[0] - ((struct instance *)b)->pos[0]);
}

float compInstance2(const void* a, const void* b) {
    const struct instance *aa = a;
    const struct instance *bb = b;

    float result = aa->pos[1] - bb->pos[1];
    return 0 == result ? aa->pos[0] - bb->pos[0] : result;
}

void printInstance(const void *a) {
    printf("[%f %f]", ((struct instance *)a)->pos[0], ((struct instance *)a)->pos[1]);
}

struct node {
    struct instance *key;
    uint16_t index;

    struct node *left;
    struct node *right;
};

struct node *rightRotate(struct node *x) {
    struct node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

struct node *leftRotate(struct node *x) {
    struct node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

struct node *splay(struct node *root, struct instance *key, float comp(const void* a, const void* b)) {
    if (root == NULL || comp(root->key, key) == 0)
        return root;

    if (comp(root->key, key) > 0) {
        if (root->left == NULL) return root;

        if (comp(root->left->key, key) > 0) {
            root->left->left = splay(root->left->left, key, comp);

            root = rightRotate(root);
        }
        else if (comp(root->left->key, key) < 0) {
            root->left->right = splay(root->left->right, key, comp);

            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL) ? root : rightRotate(root);
    }
    else {
        if (root->right == NULL) return root;

        if (comp(root->right->key, key) > 0) {
            root->right->left = splay(root->right->left, key, comp);

            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (comp(root->right->key, key) < 0) {
            root->right->right = splay(root->right->right, key, comp);
            root = leftRotate(root);
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

struct node *insert(struct node *root, struct node *k, float comp(const void* a, const void* b)) {
    if (root == NULL) return k;

    root = splay(root, k->key, comp);

    if (comp(root->key, k->key) == 0) return root;

    if (comp(root->key, k->key) > 0) {
        k->right = root;
        k->left = root->left;
        root->left = NULL;
    }
    else {
        k->left = root;
        k->right = root->right;
        root->right = NULL;
    }

    return k;
}

void preOrder(struct node *root, void (*print)(const void *)) {
    if (root != NULL) {
        preOrder(root->left, print);
        print(root->key);
        preOrder(root->right, print);
    }
}

void checkColisionInOrder(struct node *root, struct node *currRoot) {
    if (currRoot != NULL && fabsf(root->key->pos[1] - currRoot->key->pos[1]) < 2 * root->key->scale[1]) {
        if (root != currRoot) actualCollision(root->key, currRoot->key);
        checkColisionInOrder(root, currRoot->left);
        checkColisionInOrder(root, currRoot->right);
    }
}

struct node* NodeSearch(struct node *root, struct instance *key, struct node** parrent, float comp(const void* a, const void* b)) {
    struct node* result = root;
    struct node* parent = NULL;

    while (result != NULL && comp(result->key, key) != 0) {
        parent = result;
        result = comp(key, result->key) < 0 ? result->left : result->right;
    }

    if (parrent != NULL) {
        *parrent = parent;
    }
    return result;
}

bool Delete(struct node *root, struct instance *key, float comp(const void* a, const void* b)) {
    bool result = false;
    struct node * parent = NULL;
    struct node * toDelete = NodeSearch(root, key, &parent, comp);
    struct node * parentRight = NULL;
    struct node * right = NULL;

    if (toDelete != NULL) {
        result = true;
        right = toDelete->left;
        if (toDelete->right != NULL) {
            right = toDelete->right;
            if (toDelete->left != NULL) {
                if (right->left != NULL) {
                    while (right->left != NULL) {
                        parentRight = right;
                        right = right->left;
                    }
                    parentRight->left = right->right;
                    right->right = toDelete->right;
                }
                right->left = toDelete->left;
            }
        }

        if (parent == NULL) {
            root = right;
        }
        else {
            if (parent->right == toDelete) {
                parent->right = right;
            }
            else {
                parent->left = right;
            }
        }
    }

    return result;
}

int compNode(const void *a, const void *b) {
    const struct node *aa = a;
    const struct node *bb = b;

    return 10000 * (aa->key->pos[0] - bb->key->pos[0]);
}

void collision(uint32_t num, struct instance models[num], struct instance floor) {
    struct node mem[num];
    [[maybe_unused]]struct node *tree = NULL;
    [[maybe_unused]]struct node *farthest = mem;

    for (uint32_t this = 0; this < num; this += 1) {
        mem[this] = (struct node) {
            .key = models + this,
            .index = this,
            .left = NULL,
            .right = NULL
        };
    }
    qsort(mem, num, sizeof(struct node), compNode);

    for (uint32_t this = 0; this < num; this += 1) {
#if 0
        tree = insert(tree, &mem[this], compInstance2);
        while (farthest != tree && fabsf(tree->key->pos[0] - farthest->key->pos[0]) > 2 * tree->key->scale[0]) {
            Delete(tree, farthest->key, compInstance2);

            farthest += 1;
        }

        checkColisionInOrder(tree, tree);

   //     preOrder(tree, printInstance);
   //     printf("\n\n");
#else
        for (uint32_t that = this + 1; that < num; that += 1) {
            actualCollision(&models[this], &models[that]);
        }
#endif

        if (floor.pos[2] + floor.scale[1] / 2 > models[this].pos[2]) {
            models[this].pos[2] = floor.pos[2] + floor.scale[1] / 2;
            models[this].velocity[2] = 0.0f;
        }
    }
}

void move(float deltaTime, uint32_t num, struct instance models[num]) {
    vec3 temp2;

    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec3_scale(models[i].velocity, deltaTime, temp2);
        glm_vec3_add(models[i].pos, temp2, models[i].pos);
    }
}

void setPosition(struct Model model, uint32_t num, vec2 pos[num]) {
    for (uint32_t z = 0; z < num; z += 1) {
        model.instance[z] = (struct instance) {
            .textureIndex = z % model.texturesQuantity,
            .pos[2] = 0.5f,
            .maxVelocity = 5.0f
        };

        glm_vec2_copy(pos[z], model.instance[z].pos);
        glm_vec3_fill(model.instance[z].rotation, 0.0f);
        glm_vec3_fill(model.instance[z].scale, 2.0f);
        glm_vec3_fill(model.instance[z].velocity, 0.0f);
        glm_vec3_fill(model.instance[z].acceleration, 0.0f);
    }
}

void getCoordinate(vec3 result, struct VulkanTools vulkan);
bool isClicked(struct VulkanTools *vulkan, struct cohort *this) {
    bool result = false;
    vec3 pos;

    getCoordinate(pos, *vulkan);

    vec2 diff;
    for (uint16_t i = 0; result == false &&  i < this->info.instanceCount; i += 1) {
        glm_vec2_sub(pos, this->info.instance[i].pos, diff);
        if (fabsf(glm_vec2_norm(diff)) < glm_vec2_norm(this->info.instance[i].scale)) {
            result = true;
        }
    }

    return result;
}

void highlightCohort(struct cohort *cohort, bool isOn) {
    for (uint16_t i = 0; i < cohort->info.instanceCount; i += 1) {
        cohort->info.instance[i].shadow = isOn;
    }
}

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include "GL\glew.h"
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"
#include "gtc\matrix_transform.hpp"

#define DEF_WIDTH		800
#define DEF_HEIGHT		600
#define DEF_FPS			60

extern int screen_width;
extern int screen_height;
extern glm::mat4 model_matrix;
extern glm::mat4 view_matrix;
extern glm::mat4 projection_matrix;
extern glm::mat4 translation_matrix;
extern glm::mat4 rotation_matrix;
extern glm::mat4 scaling_matrix;

void SetupMatrix();

#endif // !COMMON_H


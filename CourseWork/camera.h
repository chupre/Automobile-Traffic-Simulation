#pragma once

vec2 cameraFocusPostion;
GLfloat cameraZoom = 1.0f;

GLvoid getProjectionMatrix(mat4 dest);
GLvoid getModelMatrix(mat4 dest);
GLvoid initCam();

GLvoid getProjectionMatrix(mat4 dest)
{	
	GLfloat left = cameraFocusPostion[0] - WINDOW_WIDTH / 2.0f;
	GLfloat right = cameraFocusPostion[0] + WINDOW_WIDTH / 2.0f;
	GLfloat bottom = cameraFocusPostion[1] + WINDOW_HEIGHT / 2.0f;
	GLfloat top = cameraFocusPostion[1] - WINDOW_HEIGHT / 2.0f;

	mat4 orthoMatrix;
	
	//glm_ortho(left, right, top, bottom, 0.01f, 100.0f, orthoMatrix);
	glm_ortho(left, right, bottom, top, 0.01f, 100.0f, orthoMatrix);
	//glm_mat4_scale(orthoMatrix, cameraZoom);

	glm_mat4_copy(orthoMatrix, dest);
}


GLvoid getModelMatrix(mat4 dest)
{
	vec3 position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0 };
	vec3 scale = { 400, 200, 1 };

	mat4 transMatrix, scaleMatrix;

	glm_mat4_identity(transMatrix);
	glm_mat4_identity(scaleMatrix);

	glm_translate(transMatrix, position);
	glm_translate(scaleMatrix, scale);

	glm_mat4_mul(scaleMatrix, transMatrix, dest);
}


GLvoid initCam()
{
	cameraFocusPostion[0] = WINDOW_WIDTH / 2.0f;
	cameraFocusPostion[1] = WINDOW_HEIGHT / 2.0f;
}
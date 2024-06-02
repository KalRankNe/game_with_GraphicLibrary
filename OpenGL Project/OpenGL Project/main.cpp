#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")

// ����� �簢���� ��ġ
float x = 0.0f;
float y = 0.0f;

// �簢���� ������ �����ϴ� ����ü
struct Rectangle {
    float x, y; // ��ġ
    float dx, dy; // ����
};

// �簢���� ���
std::vector<Rectangle> rectangles;

bool gameOver = false;

void addRectangle() {
    // ������ ��ġ���� �簢���� �����մϴ�.
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.141592f;
    float distance = 1.2f;
    float tx = cosf(angle) * distance;
    float ty = sinf(angle) * distance;

    // �簢���� ����� �簢���� ���� ��ǥ�� ���ϵ��� ������ �����մϴ�.
    float speed = 0.005f; // �������� �ӵ��� ���Դϴ�.
    float dx = (x - tx) * speed;
    float dy = (y - ty) * speed;

    // �簢���� ��Ͽ� �߰��մϴ�.
    rectangles.push_back({ tx, ty, dx, dy });
}

void updateRectangles() {
    // ��� �簢���� ������Ʈ�մϴ�.
    for (auto& r : rectangles) {
        r.x += r.dx;
        r.y += r.dy;

        // �浹 �˻�
        if (fabs(r.x - x) < 0.05f && fabs(r.y - y) < 0.05f) {
            gameOver = true;
        }
    }
}

void drawRectangles() {
    // ��� �簢���� �׸��ϴ�.
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // ������
    for (const auto& r : rectangles) {
        glVertex2f(r.x - 0.02f, r.y - 0.02f); // �簢���� ��ġ
        glVertex2f(r.x + 0.02f, r.y - 0.02f); // �簢���� ũ�⸦ Ű��ϴ�.
        glVertex2f(r.x + 0.02f, r.y + 0.02f); // �簢���� ũ�⸦ Ű��ϴ�.
        glVertex2f(r.x - 0.02f, r.y + 0.02f); // �簢���� ũ�⸦ Ű��ϴ�.
    }
    glEnd();
}

void drawSquare() {
    // ����� �簢���� �׸��ϴ�.
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f); // �����
    glVertex2f(x - 0.02f, y - 0.02f);
    glVertex2f(x + 0.02f, y - 0.02f);
    glVertex2f(x + 0.02f, y + 0.02f);
    glVertex2f(x - 0.02f, y + 0.02f);
    glEnd();
}

void resetGame() {
    // ������ �����մϴ�.
    x = 0.0f;
    y = 0.0f;
    rectangles.clear();
    gameOver = false;
}

int main(void)
{
    GLFWwindow* window;

    // GLFW ���̺귯�� �ʱ�ȭ
    if (!glfwInit())
        return -1;

    // ������ ����
    window = glfwCreateWindow(700, 700, "Avoidance", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // ���ؽ�Ʈ�� ���� �����忡 ����
    glfwMakeContextCurrent(window);

    // GLEW �ʱ�ȭ
    if (glewInit() != GLEW_OK)
        return -1;

    // ���� ����
    while (!glfwWindowShouldClose(window))
    {
        // �׸��� ���� �����
        glClear(GL_COLOR_BUFFER_BIT);

        if (gameOver) {
            // "Game Over" ������ ����մϴ�.
            //printf("Game Over\n");

            // 3�� ���� ����մϴ�.
            std::this_thread::sleep_for(std::chrono::seconds(3));

            // ������ �����մϴ�.
            resetGame();
        }
        else {
            // �簢�� �߰�
            if (rand() % 100 < 2.5) // 2% Ȯ���� �簢�� �߰�
                addRectangle();

            // �簢�� ������Ʈ
            updateRectangles();

            // �簢�� �׸���
            drawRectangles();

            // �簢�� �׸���
            drawSquare();

            // Ű���� �Է� ó��
            const float speed = 0.01f; // �������� �ӵ��� ���Դϴ�.
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) y += speed;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) y -= speed;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) x -= speed;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) x += speed;
        }

        // ���� ���� ��ȯ
        glfwSwapBuffers(window);

        // �̺�Ʈ ����
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

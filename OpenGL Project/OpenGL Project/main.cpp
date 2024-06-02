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

// 노란색 사각형의 위치
float x = 0.0f;
float y = 0.0f;

// 사각형의 정보를 저장하는 구조체
struct Rectangle {
    float x, y; // 위치
    float dx, dy; // 방향
};

// 사각형의 목록
std::vector<Rectangle> rectangles;

bool gameOver = false;

void addRectangle() {
    // 랜덤한 위치에서 사각형을 생성합니다.
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.141592f;
    float distance = 1.2f;
    float tx = cosf(angle) * distance;
    float ty = sinf(angle) * distance;

    // 사각형이 노란색 사각형의 순간 좌표를 향하도록 방향을 설정합니다.
    float speed = 0.005f; // 움직임의 속도를 줄입니다.
    float dx = (x - tx) * speed;
    float dy = (y - ty) * speed;

    // 사각형을 목록에 추가합니다.
    rectangles.push_back({ tx, ty, dx, dy });
}

void updateRectangles() {
    // 모든 사각형을 업데이트합니다.
    for (auto& r : rectangles) {
        r.x += r.dx;
        r.y += r.dy;

        // 충돌 검사
        if (fabs(r.x - x) < 0.05f && fabs(r.y - y) < 0.05f) {
            gameOver = true;
        }
    }
}

void drawRectangles() {
    // 모든 사각형을 그립니다.
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // 빨간색
    for (const auto& r : rectangles) {
        glVertex2f(r.x - 0.02f, r.y - 0.02f); // 사각형의 위치
        glVertex2f(r.x + 0.02f, r.y - 0.02f); // 사각형의 크기를 키웁니다.
        glVertex2f(r.x + 0.02f, r.y + 0.02f); // 사각형의 크기를 키웁니다.
        glVertex2f(r.x - 0.02f, r.y + 0.02f); // 사각형의 크기를 키웁니다.
    }
    glEnd();
}

void drawSquare() {
    // 노란색 사각형을 그립니다.
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f); // 노란색
    glVertex2f(x - 0.02f, y - 0.02f);
    glVertex2f(x + 0.02f, y - 0.02f);
    glVertex2f(x + 0.02f, y + 0.02f);
    glVertex2f(x - 0.02f, y + 0.02f);
    glEnd();
}

void resetGame() {
    // 게임을 리셋합니다.
    x = 0.0f;
    y = 0.0f;
    rectangles.clear();
    gameOver = false;
}

int main(void)
{
    GLFWwindow* window;

    // GLFW 라이브러리 초기화
    if (!glfwInit())
        return -1;

    // 윈도우 생성
    window = glfwCreateWindow(700, 700, "Avoidance", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // 컨텍스트를 현재 스레드에 연결
    glfwMakeContextCurrent(window);

    // GLEW 초기화
    if (glewInit() != GLEW_OK)
        return -1;

    // 루프 조건
    while (!glfwWindowShouldClose(window))
    {
        // 그리기 버퍼 지우기
        glClear(GL_COLOR_BUFFER_BIT);

        if (gameOver) {
            // "Game Over" 문구를 출력합니다.
            //printf("Game Over\n");

            // 3초 동안 대기합니다.
            std::this_thread::sleep_for(std::chrono::seconds(3));

            // 게임을 리셋합니다.
            resetGame();
        }
        else {
            // 사각형 추가
            if (rand() % 100 < 2.5) // 2% 확률로 사각형 추가
                addRectangle();

            // 사각형 업데이트
            updateRectangles();

            // 사각형 그리기
            drawRectangles();

            // 사각형 그리기
            drawSquare();

            // 키보드 입력 처리
            const float speed = 0.01f; // 움직임의 속도를 줄입니다.
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) y += speed;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) y -= speed;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) x -= speed;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) x += speed;
        }

        // 더블 버퍼 교환
        glfwSwapBuffers(window);

        // 이벤트 폴링
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

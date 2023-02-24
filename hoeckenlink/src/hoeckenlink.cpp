// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "ball.h"
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <math.h>

//---------------------------------------------------------------
std::vector<Point> points = {
	{{140, 600}, 10}};

//----------- system dynamic parameters --------------------

float r = 1.0;

//---------------------------------------------------------------

std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>> simulation()
{

	std::vector<float> dx;
	std::vector<float> dy;

	std::vector<float> ax;
	std::vector<float> ay;

	std::vector<float> cx;
	std::vector<float> cy;

	float dt = M_PI / 180.0;
	float Ox = 0.0;
	float Oy = 0.0;

	float Ax = 0.0;
	float Ay = 0.0;
	float Bx = 0.0;
	float By = 0.0;
	float Cx = 0.0;
	float Cy = 0.0;
	float Dx = 0.0;
	float Dy = 0.0;

	for (float t = 0.0; t < 2 * M_PI; t = t + dt)
	{

		Ax = r * std::cos(t);
		Ay = r * std::sin(t);

		Bx = 2 * r;
		By = 0.0;

		Cx = (2 + std::cos(t)) * r * 0.5 + r * std::sin(t) * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));
		Cy = std::sin(t) * r * 0.5 + r * (2 - std::cos(t)) * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));

		Dx = 2 * r + 2 * r * std::sin(t) * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));
		Dy = (2 - std::cos(t)) * r * 2.0 * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));

		dx.push_back(Dx);
		dy.push_back(Dy);

		ax.push_back(Ax);
		ay.push_back(Ay);

		cx.push_back(Cx);
		cy.push_back(Cy);
	}

	std::vector<float> dx2(dx);
	dx2.insert(dx2.end(), dx.begin(), dx.end());

	std::vector<float> dy2(dy);
	dy2.insert(dy2.end(), dy.begin(), dy.end());

	std::vector<float> ax2(ax);
	ax2.insert(ax2.end(), ax.begin(), ax.end());

	std::vector<float> ay2(ay);
	ay2.insert(ay2.end(), ay.begin(), ay.end());

	std::vector<float> cx2(cx);
	cx2.insert(cx2.end(), cx.begin(), cx.end());

	std::vector<float> cy2(cy);
	cy2.insert(cy2.end(), cy.begin(), cy.end());

	return std::make_tuple(dx2, dy2, ax2, ay2, cx2, cy2);
}

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	int w = 1280;
	int h = 720;
	float scale_x = 50.0;
	float scale_y = 50.0;
	std::string title = "Bouncing ball";
	initImgui(w, h, title);

	// simulation state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	// compute ball path
	auto sim = simulation();

	// Main loop
	int ii = 0;
	bool flag = true;
	std::vector<ImVec2> tail;
	std::vector<ImVec2> tail_circle;
	std::vector<ImVec2> tail_arc;

	int LOOPS = 5;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		//----------------------------------------

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		ImVec2 D = {(float)points[0].position.x + std::get<0>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<1>(sim)[ii] * scale_y};

		ImVec2 XY0 = {(float)points[0].position.x, (float)points[0].position.y};
		ImVec2 B = {(float)points[0].position.x + 2 * r * scale_x, (float)points[0].position.y + 2 * r * scale_y};

		ImVec2 A = {(float)points[0].position.x + std::get<2>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<3>(sim)[ii] * scale_y};
		ImVec2 C = {(float)points[0].position.x + std::get<4>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<5>(sim)[ii] * scale_y};

		float radius = 5;
		float size_line = 3;

		draw_list->AddCircleFilled(D, radius, ImColor(ray_color));
		draw_list->AddLine(XY0, A, ImColor(ray_color), 2.0);
		draw_list->AddLine(A, D, ImColor(ray_color), 2.0);
		draw_list->AddLine(B, C, ImColor(ray_color), 2.0);

		tail.push_back(D);
		tail_circle.push_back(A);
		tail_arc.push_back(C);

		for (int ii = 0; ii < tail.size(); ii++)
		{
			draw_list->AddCircleFilled(tail[ii], 0.5, ImColor(ray_color));
			draw_list->AddCircleFilled(tail_arc[ii], 0.5, ImColor(ray_color));
			draw_list->AddCircleFilled(tail_circle[ii], 0.5, ImColor(ray_color));
		}

		ii++;
		if (ii >= std::get<0>(sim).size())
		{
			flag = false;
		}
		ImGui::End();

		//----------------------------------------

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}

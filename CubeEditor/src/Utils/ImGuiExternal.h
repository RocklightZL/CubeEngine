#pragma once

#include <functional>
#include <string>
#include <imgui/imgui.h>

void addDashLine(ImDrawList* drawList, const ImVec2& start, const ImVec2& end, const ImU32& color, float thickness = 1.0f, float segmentLen = 10.0f, float intervalLen = 10.0f);

class ModalPopup {
public:
	ModalPopup(const std::string& title, const std::function<void()>& content, const std::function<void()>& confirm, const std::function<void()>& clear);
	~ModalPopup() = default;

	void render();
	void open(){ isOpen = true; }
	void close(){ isOpen = false; }

private:
	std::string title;
	bool isOpen = false;
	std::function<void()> content;
	std::function<void()> confirm;
	std::function<void()> clear;
	ImVec4 borderColor;
	ImVec4 originalBorderColor;
	ImVec4 highLightColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
};
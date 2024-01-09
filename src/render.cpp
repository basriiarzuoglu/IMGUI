#include <array>
#include <cmath>
#include <vector>
#include <iostream>
#include <set>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawSelection();
    DrawPlot();

    ImGui::End();
}

void WindowClass::DrawSelection()
{
    for (const auto func_name : functionNames)
    {
        const auto curr_function = functionNameMapping(func_name);
        auto selected = selectedFunctions.contains(curr_function);

        if (ImGui::Checkbox(func_name.data(), &selected))
        {
            if (selected)
                selectedFunctions.insert(curr_function);
            else
                selectedFunctions.erase(curr_function);
        }
    }
}

void WindowClass::DrawPlot()
{
    static constexpr auto num_points = 10'000;
    static constexpr auto x_min = -100.0;
    static constexpr auto x_max = 100.0;
    static const auto x_step = (std::abs(x_max) + std::abs(x_min)) / num_points;

    static auto xs = std::array<double, num_points>{};
    static auto ys = std::array<double, num_points>{};

    if (selectedFunctions.size() == 0 ||
        (selectedFunctions.size() == 1 &&
         *selectedFunctions.begin() == Function::NONE))
    {
        ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
        ImPlot::EndPlot();
        return;
    }

    ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);

    for (const auto &function : selectedFunctions)
    {
        auto x = x_min;
        for (std::size_t i = 0; i < num_points; ++i)
        {
            xs[i] = x;
            ys[i] = evaluateFunction(function, x);
            x += x_step;
        }

        const auto plot_label =
            fmt::format("##function{}", static_cast<int>(function));
        ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), num_points);
    }

    ImPlot::EndPlot();
}

WindowClass::Function WindowClass::functionNameMapping(
    std::string_view function_name)
{
    if (std::string_view{"sin(x)"} == function_name)
        return WindowClass::Function::SIN;

    if (std::string_view{"cos(x)"} == function_name)
        return WindowClass::Function::COS;

    return WindowClass::Function::NONE;
}

double WindowClass::evaluateFunction(const Function function, const double x)
{
    switch (function)
    {
    case Function::SIN:
    {
        return std::sin(x);
    }
    case Function::COS:
    {
        return std::cos(x);
    }
    case Function::NONE:
    default:
    {
        return 0.0;
    }
    }
}


bool show_another_window = true;
bool show_another_window2 = true;
ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
ImGuiWindowFlags childframe_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

float main_frame_width = 0.0f;
float main_frame_height = 0.0f;
float child_frame_width = 0.0f;
float child_frame_height = 0.0f;
float child2_frame_width = 0.0f;
float child2_frame_height = 0.0f;
 WindowClass w;
void render(GLFWwindow *const window)
{
 if (show_another_window)
    {
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);


        ImGui::Begin("Bad MotherFucker", NULL, window_flags);
        main_frame_height = ImGui::GetContentRegionAvail().y;
        main_frame_width = ImGui::GetContentRegionAvail().x;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    {
        ImGui::BeginChildFrame(ImGui::GetID("Child"), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 3.f), childframe_window_flags);
        child_frame_height = ImGui::GetContentRegionAvail().y;
        child_frame_width = ImGui::GetContentRegionAvail().x;
        static float f;
        ImGui::PushItemWidth(-100);
        ImGui::DragFloat("float##2a", &f);
        static bool animate = true;
        ImGui::Checkbox("Animate", &animate);
                // Fill an array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
        // and the sizeof() of your structure in the "stride" parameter.
        static float values[90] = {};
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 60.0f;
        }


        // Plots can display overlay texts
        // (in this example, we will display an average value)
        {

             float average = 0.0f;
             for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                 average += values[n];
             average /= (float)IM_ARRAYSIZE(values);
             char overlay[32];
             sprintf_s(overlay, "avg %f", average);

             ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 50.0f));
        }


        static float x_data[1000] = {};
        static float y_data[1000] = {};
        static int count = 0;
        static bool isPlotting = true;

        // Add a button to toggle plotting
        std::string label = isPlotting ? "Stop " : "Resume";
        if (ImGui::Button(label.data(), ImVec2(100, 30))) {
            isPlotting = !isPlotting;
        }

        ImGui::SameLine();

        if (ImPlot::BeginPlot("Real-Time Plot", NULL, NULL, ImGui::GetContentRegionAvail(), ImPlotFlags_YAxis2)) {


            // Generate some data (replace with your actual data source)
            if(isPlotting)
            for (int i = 0; i < 1000; i++) {
                x_data[i] = static_cast<float>(i);
                y_data[i] = sinf(i * 0.1f + static_cast<float>(ImGui::GetTime()) );
            }

                ImPlot::PlotLine("Sine Wave", x_data, y_data, 1000);
        }
        ImPlot::EndPlot();

        ImGui::EndChildFrame();

    }

    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::BeginChild(ImGui::GetID("Child2"), ImVec2(ImGui::GetWindowWidth() / 6.f, ImGui::GetContentRegionAvail().y- 20), ImGuiChildFlags_Border, childframe_window_flags);
        child2_frame_height = ImGui::GetContentRegionAvail().y;
        child2_frame_width = ImGui::GetContentRegionAvail().x;
        ImVec4 originalColor = style.Colors[ImGuiCol_Button]; // Save the original color
        ImVec4 originalTextColor = style.Colors[ImGuiCol_Text]; // Save the original color

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Set the color to red
        ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_Text]);

        // Your text here
        ImGui::Text("Hello, motherfucker!");

        ImGui::PopStyleColor();

        style.Colors[ImGuiCol_Text] = originalTextColor; // Reset to the original color

        ImGui::Text("Hello, world!");

        style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Set the color to red
        ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);

        // Your button here
        if (ImGui::Button("Click me", ImVec2(ImGui::GetContentRegionAvail().x, 50)))
        {
            // Handle button click
        }

        ImGui::PopStyleColor();

        style.Colors[ImGuiCol_Button] = originalColor;

        if (ImGui::Button("Close Me", ImVec2(ImGui::GetContentRegionAvail().x, 50)))
        {
            show_another_window = false;
            glfwSetWindowShouldClose(window, true);
        }

        style.Colors[ImGuiCol_Button] = ImVec4(12.0f, 10.0f, 0.0f, 1.0f); // Set the color to red
        ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);

        if (ImGui::Button("Click me", ImVec2(ImGui::GetContentRegionAvail().x, 50)))
        {
            // Handle button click
        }

        ImGui::PopStyleColor();

        style.Colors[ImGuiCol_Button] = originalColor;

        // Add a vertical spacer
        ImGui::Dummy(ImVec2(0.0f, ImGui::GetContentRegionAvail().y - 50 - ImGui::GetFrameHeight() - ImGui::GetStyle().ItemSpacing.y* 2)); // 20.0f is the height of the spacer

        ImGui::Text("PORT:");
        ImGui::SameLine(); // Place the next widget on the same line
        const char* items[] = { "Item 1", "Item 2", "Item 3" };
        static int item_current = 0;
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo("##combo", &item_current, items, IM_ARRAYSIZE(items)); // Create the combo box without a label


        if (ImGui::Button("Connect", ImVec2(ImGui::GetContentRegionAvail().x / 2.f, 50)))
        {
            // Handle button click
        }

        ImGui::SameLine();
        if (ImGui::Button("Disconnect", ImVec2(ImGui::GetContentRegionAvail().x, 50)))
        {
            // Handle button click
        }

        ImGui::EndChild();

    }



    ImGui::End();
    }
}
    // if(show_another_window2)
    // {
    //     ImGui::Begin("Another Window2", &show_another_window2);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //     ImGui::Text("Hello from another window!");
    //     if (ImGui::Button("Close Me"))
    //         show_another_window2 = false;
    //     ImGui::End();
    // }}

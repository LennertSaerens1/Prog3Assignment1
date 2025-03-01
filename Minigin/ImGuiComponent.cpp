#include "ImGuiComponent.h"
#include "imgui_plot.h"
#include <iostream>
#include <chrono>
#include <vector>
using namespace std::chrono;


dae::ImGuiComponent::ImGuiComponent(dae::GameObject& owner)
    :Component(owner)
{

}

void dae::ImGuiComponent::TrashWithInts(int amount) const
{
    m_timeVector.clear();
    std::vector<int> Vector{};
    for (int idx{}; idx < 1000000; idx++)
    {
        Vector.push_back(2);
    }
    int stepSize{};
    m_timeVector.reserve(11);
    for (int i{}; i < amount ;++i)
    {
        int idx{ 0 };
        for (stepSize = 1; stepSize <= 1024; stepSize *= 2)
        {
            auto start = high_resolution_clock::now();
            for (int v{}; v < Vector.size(); v += stepSize)
            {
                Vector[v] *= 2;
            }
            auto end = high_resolution_clock::now();
            std::chrono::duration<float, std::milli> duration(end - start);
            std::cout << "The Elapsed timefor stepsize: " << stepSize << " is: " << duration.count() << std::endl;
            m_timeVector.push_back(duration.count());
            m_timeVector[idx] += duration.count();
            ++idx;
        }
    }
    for (auto totalDuration : m_timeVector)
    {
        totalDuration /= amount;
    }
}

void dae::ImGuiComponent::TrashWithGameObject(int amount) const
{
    m_timeVectorGameObject.clear();

    std::vector<GameObject3D> Vector{};
    for (int idx{}; idx < 1000000; idx++)
    {
        Vector.push_back(GameObject3D());
    }
    int stepSize{};
    m_timeVectorGameObject.reserve(11);
    for (int i{}; i < amount; ++i)
    {
        int idx{ 0 };
        for (stepSize = 1; stepSize <= 1024; stepSize *= 2)
        {
            auto start = high_resolution_clock::now();
            for (int v{}; v < Vector.size(); v += stepSize)
            {
                Vector[v].id *= 2;
            }
            auto end = high_resolution_clock::now();
            std::chrono::duration<float, std::milli> duration(end - start);
            std::cout << "The Elapsed timefor stepsize: " << stepSize << " is: " << duration.count() << std::endl;
            m_timeVectorGameObject.push_back(duration.count());
            m_timeVectorGameObject[idx] += duration.count();
            ++idx;
        }
    }
    for (auto totalDuration : m_timeVectorGameObject)
    {
        totalDuration /= amount;
    }
}

void dae::ImGuiComponent::TrashWithGameObjectPtr(int amount) const
{
    m_timeVectorGameObjectPtr.clear();

    std::vector<GameObject3DPtr> Vector{};
    for (int idx{}; idx < 1000000; idx++)
    {
        Vector.push_back(GameObject3DPtr());
    }
    int stepSize{};
    m_timeVectorGameObjectPtr.reserve(11);
    for (int i{}; i < amount; ++i)
    {
        int idx{ 0 };
        for (stepSize = 1; stepSize <= 1024; stepSize *= 2)
        {
            auto start = high_resolution_clock::now();
            for (int v{}; v < Vector.size(); v += stepSize)
            {
                Vector[v].id *= 2;
            }
            auto end = high_resolution_clock::now();
            std::chrono::duration<float, std::milli> duration(end - start);
            std::cout << "The Elapsed timefor stepsize: " << stepSize << " is: " << duration.count() << std::endl;
            m_timeVectorGameObjectPtr.push_back(duration.count());
            m_timeVectorGameObjectPtr[idx] += duration.count();
            ++idx;
        }
    }
    for (auto totalDuration : m_timeVectorGameObjectPtr)
    {
        totalDuration /= amount;
    }
}

void dae::ImGuiComponent::Update(float)
{
}

void dae::ImGuiComponent::FixedUpdate(float)
{
}

void dae::ImGuiComponent::Render(float, float) const
{
    static int number = 10; // Variable to hold the numeric value

    // Begin a new ImGui window
    ImGui::Begin("Exercise 1");

    // Create a horizontal layout for the input field and buttons
    ImGui::PushItemWidth(200); // Set width for the input field
    ImGui::InputInt("##number", &number, 0, 0, ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();

    // Keep the buttons on the same line as the input field
    ImGui::SameLine();

    // Create the increment and decrement buttons
    if (ImGui::Button("+"))
    {
        number++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-"))
    {
        number--;
    }
    ImGui::SameLine();
    ImGui::Text("#Samples");

    // Add some vertical spacing
    ImGui::Spacing();
    ImGui::Spacing();

    // Create the action button
    if (ImGui::Button("Trash the cache"))
    {
        TrashWithInts(number);
    }

    if (!m_timeVector.empty()) {
        const unsigned int size{ 11 };
        auto maxElement = std::max_element(m_timeVector.begin(), m_timeVector.end());

        // Plot the data using imgui-plot
        ImGui::PlotConfig config;
        config.values.ys = m_timeVector.data();
        config.values.count = size;
        config.scale.min = 0;
        config.scale.max = *maxElement;
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(250, 250);
        config.line_thickness = 2.f;

        ImGui::Plot("Trash with Ints", config);
    }

    // End the ImGui window
    ImGui::End();
    static int numberGameObject = 100; // Variable to hold the numeric value


    // Begin a new ImGui window
    ImGui::Begin("Exercise 2");

    // Create a horizontal layout for the input field and buttons
    ImGui::PushItemWidth(200); // Set width for the input field
    ImGui::InputInt("##number", &numberGameObject, 0, 0, ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();

    // Keep the buttons on the same line as the input field
    ImGui::SameLine();

    // Create the increment and decrement buttons
    if (ImGui::Button("+"))
    {
        numberGameObject++;
    }
    ImGui::SameLine();
    if (ImGui::Button("-"))
    {
        numberGameObject--;
    }
    ImGui::SameLine();
    ImGui::Text("#Samples");

    // Add some vertical spacing
    ImGui::Spacing();
    ImGui::Spacing();

    // Create the action button
    if (ImGui::Button("Trash the cache with GameObject3D"))
    {
        TrashWithGameObject(numberGameObject);
    }

    if (!m_timeVectorGameObject.empty()) {
        const unsigned int size{ 11 };
        auto maxElement = std::max_element(m_timeVectorGameObject.begin(), m_timeVectorGameObject.end());

        // Plot the data using imgui-plot
        ImGui::PlotConfig config;
        config.values.ys = m_timeVectorGameObject.data();
        config.values.count = size;
        config.scale.min = 0;
        config.scale.max = *maxElement;
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(250, 250);
        config.line_thickness = 2.f;
        config.values.color = IM_COL32(0, 255, 0, 255);

        ImGui::Plot("Trash the cache with GameObject3D plot", config);
    }

    // Add some vertical spacing
    ImGui::Spacing();
    ImGui::Spacing();

    // Create the action button
    if (ImGui::Button("Trash the cache with GameObject3DAlt"))
    {
        TrashWithGameObjectPtr(numberGameObject);
    }

    if (!m_timeVectorGameObjectPtr.empty()) {
        const unsigned int size{ 11 };
        auto maxElement = std::max_element(m_timeVectorGameObjectPtr.begin(), m_timeVectorGameObjectPtr.end());

        // Plot the data using imgui-plot
        ImGui::PlotConfig config;
        config.values.ys = m_timeVectorGameObjectPtr.data();
        config.values.count = size;
        config.scale.min = 0;
        config.scale.max = *maxElement;
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(250, 250);
        config.line_thickness = 2.f;
        config.values.color = IM_COL32(0, 128, 128, 255);


        ImGui::Plot("Trash the cache with GameObject3DAlt plot", config);
    }
    // Add some vertical spacing
    ImGui::Spacing();
    ImGui::Spacing();
    if (!m_timeVectorGameObject.empty() && !m_timeVectorGameObjectPtr.empty())
    {
        const unsigned int size{ 11 };

        auto maxElement = std::max_element(m_timeVectorGameObject.begin(), m_timeVectorGameObject.end());
        auto maxElementPtr = std::max_element(m_timeVectorGameObjectPtr.begin(), m_timeVectorGameObjectPtr.end());

        auto biggestNumber = std::max(maxElement, maxElementPtr);

        ImGui::Text("Combined:");

        // Plot the data using imgui-plot
        ImGui::PlotConfig config;
        config.values.ys_list = new const float* [2] { m_timeVectorGameObject.data(), m_timeVectorGameObjectPtr.data() };        
        config.values.ys_count = 2;  // Number of datasets
        config.values.count = size;
        config.scale.min = 0;
        config.scale.max = *biggestNumber;
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(250, 250);
        config.line_thickness = 2.f;
        config.values.colors = new ImU32[2]{ IM_COL32(0, 255, 0, 255), IM_COL32(0, 128, 128, 255) };
        ImGui::Plot("Combined line", config);

        // Clean up dynamic memory
        delete[] config.values.ys_list;
        delete[] config.values.colors;
    }


    // End the ImGui window
    ImGui::End();
}

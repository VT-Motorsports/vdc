#ifndef GUI_BACKEND_HPP
#define GUI_BACKEND_HPP

inline GLFWwindow* graphics_window;
inline ImGuiWindowFlags window_flags;
inline int gui_width = 1280;
inline int gui_height = 720;
inline string gui_title = "VTMotorsports' Vehicle Dynamics Collection";

inline void start_window() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create graphics window
    graphics_window = glfwCreateWindow(gui_width, gui_height, gui_title.c_str(), NULL, NULL);
    if (graphics_window == NULL)
        exit(EXIT_FAILURE);
    glfwMakeContextCurrent(graphics_window); // Tell opengl to target this window

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        exit(EXIT_FAILURE);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    // Configure font
    ImFontConfig config;
    config.SizePixels = 18.0f; // --- FONT SIZE ---
    io.Fonts->AddFontDefault(&config);
    // io.Fonts->Build(); // --- DEPRECATED IN NEWER IMGUI

    // Dark mode
    ImGui::StyleColorsDark();

    // Parent window flags
    window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    /* === ImPlot Setup === */
    ImPlot::CreateContext();
    ImPlot::StyleColorsDark();

    /* === Platform/renderer bindings === */
    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(graphics_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

}
inline void end_window(){
    // Shut down ImPlot
    ImPlot::DestroyContext();

    // Shut down ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Shut down GLFW
    glfwDestroyWindow(graphics_window);
    glfwTerminate();
}
inline void new_frame(){
    // Create new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set up parent window
    glfwGetFramebufferSize(graphics_window, &gui_width, &gui_height);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once, ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(gui_width), static_cast<float>(gui_height)));

    // Start parent window (here for convenience)
    ImGui::Begin("Parent Window", nullptr, window_flags);
}
inline void render_frame(){
    // End parent window (here for convenience)
    ImGui::End();

    // Render frame
    ImGui::Render();

    // OpenGL buffer swap
    glViewport(0, 0, 960, 720);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwPollEvents(); // Handle event triggers
    glfwSwapBuffers(graphics_window);
}

#endif
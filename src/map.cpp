#include <project/common.h>

unsigned int VBO, VAO, EBO;
unsigned int map_setup()
{
    float vertices[] = {
        // positions                   // colors           // texture coords
        1.0f, (11.0f / 12.0f), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -(17.0f / 18.0f), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -(17.0f / 18.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, (11.0f / 12.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VAO;
}

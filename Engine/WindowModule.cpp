#include "WindowModule.h"
#include "Engine.h"

void WindowModule::Create()
{

    window = new sf::RenderWindow(sf::VideoMode({ 1280, 720 }), "Tiny wings - Reskin");

}

void WindowModule::Update(float dt)
{

    while (const std::optional event = window->pollEvent())
    {
        if (event->is<sf::Event::Closed>()) {
            Engine::GetInstance()->Quit();
            window->close();
        }

    }

    window->clear();

}

void WindowModule::Render()
{

    window->display();

}

sf::RenderWindow* WindowModule::GetRenderWindow()
{

    return window;

}
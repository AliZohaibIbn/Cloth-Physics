#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "particle.h"
#include "constraint.h"
#include "input_handler.h"

using namespace std;

const int WIDTH = 1080;
const int HEIGHT = 640;
const float PARTICLE_RADIUS = 10.0f;
const float GRAVITY = 10.0f;
const float tS = 0.1f; // time step abbreviation 
const int row = 10;
const int col = 20;
const float rDist = 30.0f;

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth with Verlet Integration");
    window.setFramerateLimit(60);

    // Particle vector ( to store the particles ) & Constraint Vecotr ( to store all the contraints )
    vector<Particle> particles;
    vector<Constraint> constraints;

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            float x = c * rDist + WIDTH/3;
            float y = r * rDist + HEIGHT/3;
            bool pinned = (r == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

    // Initialize Contraints: 
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            if (c < col - 1) {
                // Horizontal :
                constraints.emplace_back(&particles[r * col + c], &particles[r * col + c + 1]);
            }
            if (r < row - 1) {
                // vertical: 
                constraints.emplace_back(&particles[r * col + c], &particles[(r + 1) * col + c]);
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // handle the mouse events: 
            InputHandler::handle_mouse_click(event, particles, constraints);
        }

        for (auto& particle : particles) {
            // Apply gravity here: 
            particle.apply_force(sf::Vector2f(0, GRAVITY));
            particle.update(tS);
            particle.constraintToBounds(WIDTH, HEIGHT);
        }

        for (size_t i = 0; i < 5; i++) {
            for (auto& constraint : constraints) {
                constraint.satisfy();
            }
        }

        window.clear(sf::Color::Black);

        // Draw particles as points: 
        for (const auto& particle : particles) {
            sf::Vertex point(particle.position, sf::Color::Blue);
            window.draw(&point, 1, sf::Points);
        }

        // Draw contraints as lines: 
        for (const auto& constraint : constraints) {

            if (!constraint.active) {
                continue;
            }
            sf::Vertex line[] = {
                sf::Vertex(constraint.p1->position, sf::Color::Blue),
                sf::Vertex(constraint.p2->position, sf::Color::Blue),
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}
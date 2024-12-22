#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

class Particle {

    public: 
        sf::Vector2f position;
        sf::Vector2f previous_position;
        sf::Vector2f acceleration;
        bool isPinned;

        Particle(float x, float y, bool pinned = false) : position(x, y), previous_position(x, y), acceleration(0, 0), isPinned(pinned) {}

        void apply_force(const sf::Vector2f& force) {
            if (!isPinned) {
                acceleration += force;
            }
        }

        void update(float tS) {
            // the verlet integration is done here: 
            if (!isPinned) {
                sf::Vector2f velocity = position - previous_position;
                previous_position = position;
                position += velocity + acceleration * tS * tS;
                acceleration = sf::Vector2f(0, 0);
            }
        }

        void constraintToBounds(float width, float height) {
            if (position.x < 0) position.x = 0;
            if (position.x  > width) position.x = width;
            if (position.y < 0) position.y = 0;
            if (position.y > height) position.y = height;
        }

};

#endif
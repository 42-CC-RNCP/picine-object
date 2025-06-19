#pragma once
#include <iostream>

// start(): starts the engine
// stop(): stops the engine and applies the brakes
// accelerate(speed): increases the speed of the car by a specified value
// shift_gears_up(): shifts up to the next gear
// shift_gears_down(): shifts down to the previous gear
// reverse(): puts the transmission in reverse gear
// turn_wheel(angle): turns the wheels by a specified angle
// straighten_wheels(): returns the wheels to a straight-ahead position
// apply_force_on_brakes(force): applies a specified force to the brakes
// apply_emergency_brakes(): applies the brakes with maximum force for an emergency stop


/*
S: Everything should just do one thing.
    - benefit: can be able to change 1 thing without changing the rest

A `Car` changes if...
    - a basic property changes (e.g. color, model, etc.)
    - a part of the car changes (e.g. engine, transmission, etc.)
        - e.g. if the engine is replaced with a more powerful one, the car's acceleration and speed capabilities change

which function is doing more than a car should do?
    1. `start()`, `stop()` and `accelerate(speed)`: should belong to the engine, not the car?
    2. `shift_gears_up()`, `shift_gears_down()`, `reverse()`: should belong to the transmission, not the car?
    3. `turn_wheel(angle)`, `straighten_wheels()`: should belong to the steering system, not the car?
    4. `apply_force_on_brakes(force)`, `apply_emergency_brakes()`: should belong to the braking system, not the car?

By the duck typing philosophy, we can say that a `Car` is a collection of parts that can be replaced or changed without affecting the rest of the car.
    - e.g. if the engine is replaced with a more powerful one, the car's acceleration and speed capabilities change, but the car itself remains the same.

*/

class Engine
{
    public:
        void start() {
            std::cout << "Engine started." << std::endl;
        }
        void stop() {
            std::cout << "Engine stopped." << std::endl;
        }
        void accelerate(int speed) {
            std::cout << "Accelerating by " << speed << " km/h." << std::endl;
        }
};

class Transmission
{
    public:
        void shift_gears_up() {
            std::cout << "Shifting up to the next gear." << std::endl;
        }
        void shift_gears_down() {
            std::cout << "Shifting down to the previous gear." << std::endl;
        }
        void reverse() {
            std::cout << "Putting the transmission in reverse gear." << std::endl;
        }
    private:
};

class SteeringSystem
{
    public:
        void turn_wheel(int angle) {
            std::cout << "Turning the wheels by " << angle << " degrees." << std::endl;
        }
        void straighten_wheels() {
            std::cout << "Straightening the wheels to a straight-ahead position." << std::endl;
        }
    private:
};

class BrakingSystem
{
    public:
        void apply_force_on_brakes(int force) {
            std::cout << "Applying " << force << " N of force to the brakes." << std::endl;
        }
        void apply_emergency_brakes() {
            std::cout << "Applying emergency brakes with maximum force!" << std::endl;
        }
    private:
};


class Car
{
    public:
        Engine engine;
        Transmission transmission;
        SteeringSystem steering_system;
        BrakingSystem braking_system;
};

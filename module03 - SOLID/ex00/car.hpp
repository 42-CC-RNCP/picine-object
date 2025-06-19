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

class ILogger
{
    public:
        virtual void log(const std::string &message) = 0;
        virtual ~ILogger() {}
};

class ConsoleLogger : public ILogger
{
    public:
        void log(const std::string &message) {
            std::cout << "Log: " << message << std::endl;
        }
};

class Engine
{
    public:
        Engine(ILogger& logger) : _logger(logger) {
            _logger.log("Engine initialized.");
        }

        void start() {
            _logger.log("Engine started.");
        }
        void stop() {
            _logger.log("Engine stopped.");
        }
        void accelerate(int speed) {
            _logger.log("Accelerating to " + std::to_string(speed) + " km/h.");
        }
    
    private:
        ILogger& _logger;
};

class Transmission
{
    public:
        Transmission(ILogger& logger) : _logger(logger) {
            _logger.log("Transmission initialized.");
        }

        void shift_gears_up() {
            _logger.log("Shifting up to the next gear.");
        }
        void shift_gears_down() {
            _logger.log("Shifting down to the previous gear.");
        }
        void reverse() {
            _logger.log("Putting the transmission in reverse gear.");
        }

    private:
        ILogger& _logger;
};

class SteeringSystem
{
    public:
        SteeringSystem(ILogger& logger) : _logger(logger) {
            _logger.log("Steering system initialized.");
        }

        void turn_wheel(int angle) {
            _logger.log("Wheel turned by " + std::to_string(angle) + " degrees.");
        }
        void straighten_wheels() {
            _logger.log("Wheels straightened to the straight-ahead position.");
        }

    private:
        ILogger& _logger;
};

class BrakingSystem
{
    public:
        BrakingSystem(ILogger& logger) : _logger(logger) {
            _logger.log("Braking system initialized.");
        }
        void apply_force_on_brakes(int force) {
            _logger.log("Applying " + std::to_string(force) + " force on brakes.");
        }
        void apply_emergency_brakes() {
            _logger.log("Applying emergency brakes with maximum force.");
        }
    
    private:
        ILogger& _logger;
};


class Car
{
    public:
        void start() {
            _engine.start();
        }

        void stop() {
            _braking_system.apply_emergency_brakes();
            _engine.stop();
        }

        void accelerate(int speed) {
            _engine.accelerate(speed);
        }

        void shift_gears_up() {
            _transmission.shift_gears_up();
        }

        void shift_gears_down() {
            _transmission.shift_gears_down();
        }

        void reverse() {
            _transmission.reverse();
        }

        void turn_wheel(int angle) {
            _steering_system.turn_wheel(angle);
        }

        void straighten_wheels() {
            _steering_system.straighten_wheels();
        }

        void apply_force_on_brakes(int force) {
            _braking_system.apply_force_on_brakes(force);
        }

        void apply_emergency_brakes() {
            _braking_system.apply_emergency_brakes();
        }

    private:
        Engine _engine;
        Transmission _transmission;
        SteeringSystem _steering_system;
        BrakingSystem _braking_system;
};

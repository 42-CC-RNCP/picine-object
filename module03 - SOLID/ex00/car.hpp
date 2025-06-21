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
    1. `start()`, `stop()` and `accelerate(speed)`: should belong to the engine
    2. `shift_gears_up()`, `shift_gears_down()`, `reverse()`: should belong to the transmission
    3. `turn_wheel(angle)`, `straighten_wheels()`: should belong to the steering system
    4. `apply_force_on_brakes(force)`, `apply_emergency_brakes()`: should belong to the braking system

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

class IEngine
{
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void accelerate(int speed) = 0;
        virtual ~IEngine() {}
};

class Engine : public IEngine
{
    public:
        Engine(ILogger& logger) : _logger(logger), _is_active(false) {
            _logger.log("Engine initialized.");
        }

        void start() {
            _logger.log("Engine started.");
            _is_active = true;
        }
        void stop() {
            _logger.log("Engine stopped.");
            _is_active = false;
        }
        void accelerate(int speed) {
            if (!_is_active) {
                _logger.log("Cannot accelerate. Engine is not running.");
                return;
            }
            _logger.log("Accelerating to " + std::to_string(speed) + " km/h.");
        }

        bool is_active() const {
            return _is_active;
        }
    
    private:
        ILogger& _logger;
        bool _is_active;
};

class ITransmission
{
    public:
        virtual void shift_gears_up() = 0;
        virtual void shift_gears_down() = 0;
        virtual void reverse() = 0;
        virtual ~ITransmission() {}
};

enum Gear { P, N, D, R };

class Transmission : public ITransmission
{
    public:
        Transmission(ILogger& logger) : _logger(logger) {
            _current_gear = P; // Assuming P is the initial gear (Park)
            _logger.log("Transmission initialized in gear " + std::to_string(_current_gear) + ".");
        }

        void shift_gears_up() {
            if (_current_gear == P) {
                _logger.log("Cannot shift up from Park gear.");
                return;
            }
            if (_current_gear == N) {
                _logger.log("Shifting to Drive gear.");
                _current_gear = D; // Set to Drive
            } else if (_current_gear == D) {
                _logger.log("Already in Drive gear. Cannot shift up further.");
            } else {
                _logger.log("Already in Reverse gear. Cannot shift up further.");
            }
        }

        void shift_gears_down() {
            if (_current_gear == P) {
                _logger.log("Cannot shift down from Park gear.");
                return;
            }
            if (_current_gear == N) {
                _logger.log("Shifting to Drive gear.");
                _current_gear = D; // Set to Drive
            } else if (_current_gear == D) {
                _logger.log("Shifting to Neutral gear.");
                _current_gear = N; // Set to Neutral
            } else {
                _logger.log("Already in the lowest gear. Cannot shift down further.");
            }
        }

        void reverse() {
            if (_current_gear != P) {
                _logger.log("Cannot reverse. Please shift to Park gear first.");
                return;
            }
            _current_gear = R; // Set to Reverse
            _logger.log("Shifting to reverse gear.");
        }

        Gear get_current_gear() const {
            return _current_gear;
        }

        void set_current_gear(Gear gear) {
            _current_gear = gear;
            _logger.log("Current gear set to " + std::to_string(_current_gear) + ".");
        }

        bool is_in_park() const {
            return _current_gear == P;
        }

    private:
        ILogger& _logger;
        Gear _current_gear;
};

class ISteeringSystem
{
    public:
        virtual bool turn_wheel(int angle) = 0;
        virtual void straighten_wheels() = 0;
        virtual ~ISteeringSystem() {}
};

class SteeringSystem : public ISteeringSystem
{
    public:
        SteeringSystem(ILogger& logger) : _logger(logger), _current_angle(0) {
            _logger.log("Steering system initialized with wheels straightened.");
        }

        bool turn_wheel(int angle) {
            if (angle < -MAX_TURN_ANGLE || angle > MAX_TURN_ANGLE) {
                _logger.log("Invalid angle. Must be between " + std::to_string(-MAX_TURN_ANGLE) + " and " + std::to_string(MAX_TURN_ANGLE) + ".");
                return false;
            }
            _current_angle = angle;
            _logger.log("Wheels turned to " + std::to_string(angle) + " degrees.");
            return true;
        }
        void straighten_wheels() {
            _current_angle = 0;
            _logger.log("Wheels straightened to the straight-ahead position.");
        }

    private:
        static const int MAX_TURN_ANGLE = 45;
        ILogger& _logger;
        int _current_angle;
};

class IBrakingSystem
{
    public:
        virtual bool apply_force_on_brakes(int force) = 0;
        virtual void apply_emergency_brakes() = 0;
        virtual ~IBrakingSystem() {}
};

class BrakingSystem : public IBrakingSystem
{
    public:
        BrakingSystem(ILogger& logger) : _logger(logger) {
            _current_force = 0;
            _logger.log("Braking system initialized.");
        }
        bool apply_force_on_brakes(int force) {
            if (force < 0 || force > MAX_BRAKE_FORCE) {
                _logger.log("Invalid force. Must be between 0 and " + std::to_string(MAX_BRAKE_FORCE) + ".");
                return false;
            }
            _current_force = force;
            _logger.log("Brakes applied with force: " + std::to_string(force));
            return true;
        }
        void apply_emergency_brakes() {
            _current_force = MAX_BRAKE_FORCE;
            _logger.log("Emergency brakes applied with maximum force: " + std::to_string(MAX_BRAKE_FORCE));
        }

        int get_current_force() const {
            return _current_force;
        }

        bool is_braking() const {
            return _current_force > 0;
        }
    
    private:
        ILogger& _logger;
        static const int MAX_BRAKE_FORCE = 100; // Example maximum force
        int _current_force;
};

class Car
{
    public:
        Car(ILogger& logger,
            IEngine& eng,
            ITransmission& trans,
            ISteeringSystem& ss,
            IBrakingSystem& bs)
            : _logger(logger), _engine(eng), _transmission(trans), _steering_system(ss), _braking_system(bs) {
            _logger.log("Car initialized with all systems ready.");
        }

        void start() {
            _engine.start();
            _logger.log("Car started.");
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
        ILogger& _logger;
        IEngine& _engine;
        ITransmission& _transmission;
        ISteeringSystem& _steering_system;
        IBrakingSystem& _braking_system;
};

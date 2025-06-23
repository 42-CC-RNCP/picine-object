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

namespace ansi {
    static const char* RESET = "\033[0m";
    static const char* RED   = "\033[31m";
    static const char* GREEN = "\033[32m";
    static const char* YEL   = "\033[33m";
    static const char* BLUE  = "\033[34m";
    static const char* MAG   = "\033[35m";
    static const char* CYN   = "\033[36m";
    static const char* WHT   = "\033[37m";
}

template <typename T> struct LogColor {
    static const char* color() {
        return ansi::RESET; // Default color
    }
};

template <> struct LogColor<class Engine> {
    static const char* color() {
        return ansi::YEL; // Yellow for Engine
    }
};

template <> struct LogColor<class Transmission> {
    static const char* color() {
        return ansi::CYN; // Cyan for Transmission
    }
};

template <> struct LogColor<class SteeringSystem> {
    static const char* color() {
        return ansi::GREEN; // Green for SteeringSystem
    }
};

template <> struct LogColor<class BrakingSystem> {
    static const char* color() {
        return ansi::RED; // Red for BrakingSystem
    }
};

class ILogger
{
    public:
        virtual void log(const std::string &message) const = 0;
        virtual ~ILogger() {}
};

class ConsoleLogger : public ILogger
{
    public:
        void log(const std::string &message) const {
            std::cout << message << std::endl;
        }
};

template <typename Derived>
class LoggerMixin : public ILogger
{
    public:
        void log(const std::string& m) const {
            if (_logger) {
                std::string class_name = Derived::class_name;
                std::string color = LogColor<Derived>::color();
                _logger->log(color + class_name + ": " + m + ansi::RESET);
            } else {
                std::cerr << "Logger is not set!" << std::endl;
            }
        }
    protected:
        ILogger* _logger;

    protected:
        LoggerMixin(ILogger* logger) : _logger(logger) {
            if (!_logger) {
                throw std::runtime_error("Logger cannot be null");
            }
        }
};

class IEngine
{
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void accelerate(int speed) = 0;
        virtual bool is_active() const = 0;
        virtual ~IEngine() {}
};

class Engine : public IEngine, public LoggerMixin<Engine>
{
    public:
        static const std::string class_name;

        Engine(ILogger* logger) : LoggerMixin<Engine>(logger), _is_active(false) {
            log("Initialized.");
        }

        void start() {
            log("Started.");
            _is_active = true;
        }
        void stop() {
            log("Stopped.");
            _is_active = false;
        }
        void accelerate(int speed) {
            if (!_is_active) {
                log("Cannot accelerate. Engine is not running.");
                return;
            }
            log("Accelerating to " + std::to_string(speed) + " km/h.");
        }

        bool is_active() const {
            return _is_active;
        }
    
    private:
        bool _is_active;
};
const std::string Engine::class_name = "Engine";


enum Gear { P, D, R };

inline const std::string gear_to_string(Gear g)
{
    switch (g) { case P: return "P";
                 case D: return "D";
                 case R: return "R";
                 default: return "?"; }
}

class ITransmission
{
    public:
        virtual bool to_park() = 0;
        virtual bool to_drive() = 0;
        virtual bool to_reverse() = 0;
        virtual bool is_in_park() const = 0;
        virtual Gear get_current_gear() const = 0;
        virtual ~ITransmission() {}
};


class Transmission : public ITransmission, public LoggerMixin<Engine>
{
    public:
        static const std::string class_name;

        Transmission(ILogger* logger) : LoggerMixin<Engine>(logger), _current_gear(P) {
            log("Initialized in gear " + gear_to_string(_current_gear) + ".");
        }

        bool to_park() {
            return _try_set(P);
        }

        bool to_drive() {
            return _try_set(D);
        }

        bool to_reverse() {
            return _try_set(R);
        }

        Gear get_current_gear() const {
            return _current_gear;
        }

        bool is_in_park() const {
            return _current_gear == P;
        }

    private:
        Gear _current_gear;
    
    private:
        bool _try_set(Gear gear) {
            if (gear == _current_gear) {
                return false; // Already in the desired gear
            }
            _current_gear = gear;
            log("Gear -> " + gear_to_string(_current_gear) + ".");
            return true; // Can shift to the desired gear
        }
};
const std::string Transmission::class_name = "Transmission";

class ISteeringSystem
{
    public:
        virtual bool turn_wheel(int angle) = 0;
        virtual void straighten_wheels() = 0;
        virtual ~ISteeringSystem() {}
};

class SteeringSystem : public ISteeringSystem, public LoggerMixin<SteeringSystem>
{
    public:
        static const std::string class_name;
        
        SteeringSystem(ILogger* logger) : LoggerMixin<SteeringSystem>(logger), _current_angle(0) {
            log("system initialized with wheels straightened.");
        }

        bool turn_wheel(int angle) {
            if (angle < -MAX_TURN_ANGLE || angle > MAX_TURN_ANGLE) {
                log("Invalid angle. Must be between " + std::to_string(-MAX_TURN_ANGLE) + " and " + std::to_string(MAX_TURN_ANGLE) + ".");
                return false;
            }
            _current_angle = angle;
            log("Wheels turned to " + std::to_string(angle) + " degrees.");
            return true;
        }
        void straighten_wheels() {
            _current_angle = 0;
            log("Wheels straightened to the straight-ahead position.");
        }

    private:
        static const int MAX_TURN_ANGLE = 45;
        int _current_angle;
};
const std::string SteeringSystem::class_name = "SteeringSystem";

class IBrakingSystem
{
    public:
        virtual bool apply_force_on_brakes(int force) = 0;
        virtual void apply_emergency_brakes() = 0;
        virtual int get_current_force() const = 0;
        virtual bool is_braking() const = 0;
        virtual ~IBrakingSystem() {}
};

class BrakingSystem : public IBrakingSystem, public LoggerMixin<BrakingSystem>
{
    public:
        static const std::string class_name;

        BrakingSystem(ILogger* logger) : LoggerMixin<BrakingSystem>(logger){
            _current_force = 0;
            log("Braking system initialized.");
        }
        bool apply_force_on_brakes(int force) {
            if (force < 0 || force > MAX_BRAKE_FORCE) {
                log("Invalid force. Must be between 0 and " + std::to_string(MAX_BRAKE_FORCE) + ".");
                return false;
            }
            _current_force = force;
            log("Brakes applied with force: " + std::to_string(force));
            return true;
        }
        void apply_emergency_brakes() {
            _current_force = MAX_BRAKE_FORCE;
            log("Emergency brakes applied with maximum force: " + std::to_string(MAX_BRAKE_FORCE));
        }

        int get_current_force() const {
            return _current_force;
        }

        bool is_braking() const {
            return _current_force > 0;
        }
    
    private:
        static const int MAX_BRAKE_FORCE = 100; // Example maximum force
        int _current_force;
};
const std::string BrakingSystem::class_name = "BrakingSystem";

class ICarPolicy
{
    public:
        virtual bool can_start(const IEngine& engine, const ITransmission& transmission, const IBrakingSystem& braking_system) const = 0;
        virtual bool can_stop(const IEngine& engine, const ITransmission& transmission) const = 0;
        virtual bool can_accelerate(const IEngine& engine, const ITransmission& transmission, const IBrakingSystem& braking_system) const = 0;
        virtual bool can_reverse(const ITransmission& transmission, const IBrakingSystem& braking_system) const = 0;
        virtual ~ICarPolicy() {}
};

class DefaultCarPolicy : public ICarPolicy
{
    public:
        bool can_start(const IEngine& engine, const ITransmission& transmission, const IBrakingSystem& braking_system) const {
            if (engine.is_active()) {
                return false; // Engine is already running
            }
            if (!transmission.is_in_park()) {
                return false; // Transmission must be in Park gear to start
            }
            if (braking_system.is_braking()) {
                return false; // Brakes must be applied before starting
            }
            return true; // Can start the engine
        }

        bool can_stop(const IEngine& engine, const ITransmission& transmission) const {
            if (!engine.is_active()) {
                return false; // Engine must be running to stop
            }
            if (transmission.is_in_park()) {
                return false; // Cannot stop if already in Park gear
            }
            return true; // Can stop the engine
        }

        bool can_accelerate(const IEngine& engine, const ITransmission& transmission, const IBrakingSystem& braking_system) const {
            if (!engine.is_active()) {
                return false; // Engine must be running to accelerate
            }
            if (transmission.is_in_park()) {
                return false; // Transmission must be in Park gear to accelerate
            }
            if (braking_system.is_braking()) {
                return false; // Cannot accelerate while brakes are applied
            }
            return true; // Can accelerate
        }

        bool can_reverse(const ITransmission& transmission, const IBrakingSystem& braking_system) const {
            if (transmission.get_current_gear() != R) {
                return false; // Transmission must be in Reverse gear to reverse
            }
            if (!braking_system.is_braking()) {
                return false; // Brakes must be applied before reversing
            }
            return true; // Can reverse
        }
};

class Car : public LoggerMixin<Car>
{
    public:
        static const std::string class_name;

        Car(ILogger* logger,
            IEngine& eng,
            ITransmission& trans,
            ISteeringSystem& ss,
            IBrakingSystem& bs,
            ICarPolicy& policy)
            : LoggerMixin<Car>(logger), _engine(eng), _transmission(trans), _steering_system(ss), _braking_system(bs), _policy(policy) {
            log("Initialized with all systems ready.");
        }

        void start() {
            _braking_system.apply_emergency_brakes(); // Ensure brakes are applied before starting
            if (!_policy.can_start(_engine, _transmission, _braking_system)) {
                log("Start rejected by policy.");
                return;
            }
            _engine.start();
            log("Started, braking system holding emergency brakes.");
        }

        void stop() {
            if (!_policy.can_stop(_engine, _transmission)) {
                log("Stop rejected by policy.");
                return;
            }
            _braking_system.apply_emergency_brakes();
            _engine.stop();
            log("Stopped and transmission set to Park.");
        }

        void accelerate(int speed) {
            if (!_policy.can_accelerate(_engine, _transmission, _braking_system)) {
                log("Acceleration rejected by policy.");
                return;
            }
            _engine.accelerate(speed);
        }

        void shift_gears_up() {
            _transmission.to_park();
        }

        void shift_gears_down() {
            _transmission.to_drive();
        }

        void reverse() {
            if (!_policy.can_reverse(_transmission, _braking_system)) {
                log("Reverse rejected by policy.");
                return;
            }
            _braking_system.apply_emergency_brakes();
            _transmission.to_reverse();
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
        IEngine& _engine;
        ITransmission& _transmission;
        ISteeringSystem& _steering_system;
        IBrakingSystem& _braking_system;
        ICarPolicy& _policy;
};

const std::string Car::class_name = "Car";

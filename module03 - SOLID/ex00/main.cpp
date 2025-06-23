#include "car.hpp"


int main() {
    ConsoleLogger console;

    console.log("\n==== Initializing Car Components ====");
    Engine engine(&console);
    Transmission transmission(&console);
    SteeringSystem steering_system(&console);
    BrakingSystem braking_system(&console);
    DefaultCarPolicy policy;
    Car car(&console, engine, transmission, steering_system, braking_system, policy);

    console.log("\n==== Car Simulation ====");
    car.start();
    car.shift_gears_up();
    car.shift_gears_down();
    car.reverse();
    car.turn_wheel(30);
    car.straighten_wheels();
    car.apply_force_on_brakes(50);
    car.apply_emergency_brakes();
    car.stop();

    console.log("\n==== Car test policy====");
    car.stop();

    return 0;
}

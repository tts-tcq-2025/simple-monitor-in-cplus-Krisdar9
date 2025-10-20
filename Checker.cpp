#include <iostream>
#include <string>
#include <cassert>
using namespace std;

//  Enum for breach type
enum class BreachType {
    NORMAL,
    TOO_LOW,
    TOO_HIGH
};

//  Pure function to check value against limits, returns breach type
BreachType checkBreach(float value, float lowerLimit, float upperLimit) {
    if (value < lowerLimit) return BreachType::TOO_LOW;
    if (value > upperLimit) return BreachType::TOO_HIGH;
    return BreachType::NORMAL;
}

//  Pure functions for each parameter returning breach
BreachType checkTemperature(float temperature) {
    return checkBreach(temperature, 0, 45);
}

BreachType checkSoc(float soc) {
    return checkBreach(soc, 20, 80);
}

BreachType checkChargeRate(float chargeRate) {
    return (chargeRate > 0.8) ? BreachType::TOO_HIGH : BreachType::NORMAL;
}

//  Function to convert breach enum to string for messages
string breachTypeToString(BreachType breach) {
    switch (breach) {
        case BreachType::TOO_LOW: return "too low";
        case BreachType::TOO_HIGH: return "too high";
        default: return "normal";
    }
}

//  I/O function to print breach messages (side effect separated)
void printBreachMessage(const string& parameter, BreachType breach) {
    if (breach != BreachType::NORMAL) {
        cout << parameter << " is " << breachTypeToString(breach) << "!" << endl;
    }
}

//  Checks battery and prints messages, returns overall OK status
bool batteryIsOk(float temperature, float soc, float chargeRate) {
    BreachType tempBreach = checkTemperature(temperature);
    BreachType socBreach = checkSoc(soc);
    BreachType chargeRateBreach = checkChargeRate(chargeRate);

    printBreachMessage("Temperature", tempBreach);
    printBreachMessage("State of Charge", socBreach);
    printBreachMessage("Charge Rate", chargeRateBreach);

    return (tempBreach == BreachType::NORMAL) && 
           (socBreach == BreachType::NORMAL) && 
           (chargeRateBreach == BreachType::NORMAL);
}

//  --- Tests ---

void testCheckBreach() {
    assert(checkBreach(5, 0, 10) == BreachType::NORMAL);
    assert(checkBreach(-1, 0, 10) == BreachType::TOO_LOW);
    assert(checkBreach(11, 0, 10) == BreachType::TOO_HIGH);
}

void testBatteryIsOk() {
    //  All normal values
    assert(batteryIsOk(25, 70, 0.7) == true);

    //  Temperature too high
    assert(batteryIsOk(50, 70, 0.7) == false);

    //  Temperature too low
    assert(batteryIsOk(-1, 70, 0.7) == false);

    //  SOC too low
    assert(batteryIsOk(25, 10, 0.7) == false);

    //  SOC too high
    assert(batteryIsOk(25, 90, 0.7) == false);

    //  Charge rate too high
    assert(batteryIsOk(25, 70, 0.9) == false);

    //  Multiple breaches
    assert(batteryIsOk(-5, 10, 0.9) == false);
}

int main() {
    testCheckBreach();
    testBatteryIsOk();

    cout << "All tests passed!" << endl;

    // Example run to see printout
    batteryIsOk(50, 85, 0.9); // should print all breach messages

    return 0;
}

#pragma once
#include <memory>
#include "PaymentStrategy.h"

class Payment {
public:
    explicit Payment(std::unique_ptr<PaymentStrategy> strategy)
        : strategy(std::move(strategy)) {}
    bool process(double amount, const std::string& details) {
        return strategy->pay(amount, details);
    }
private:
    std::unique_ptr<PaymentStrategy> strategy;
};

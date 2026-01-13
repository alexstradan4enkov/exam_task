#pragma once
#include <string>

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount, const std::string& details) = 0;
};

// Card, e-wallet, SBP
class CardPaymentStrategy : public PaymentStrategy {
public:
    bool pay(double amount, const std::string& details) override;
};

class WalletPaymentStrategy : public PaymentStrategy {
public:
    bool pay(double amount, const std::string& details) override;
};

class SBPPaymentStrategy : public PaymentStrategy {
public:
    bool pay(double amount, const std::string& details) override;
};

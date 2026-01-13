#include "PaymentStrategy.h"
#include <iostream>

bool CardPaymentStrategy::pay(double amount, const std::string& details) {
    std::cout << "[CARD] Оплата " << amount << " успешно выполнена. " << details << "\n";
    return true;
}

bool WalletPaymentStrategy::pay(double amount, const std::string& details) {
    std::cout << "[WALLET] Оплата " << amount << " успешно выполнена. " << details << "\n";
    return true;
}

bool SBPPaymentStrategy::pay(double amount, const std::string& details) {
    std::cout << "[SBP] Оплата " << amount << " успешно выполнена. " << details << "\n";
    return true;
}

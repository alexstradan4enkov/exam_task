#pragma once
#include "User.h"
#include "DatabaseConnection.h"
#include "Payment.h"

class Customer : public User {
public:
    Customer(int id, const std::string& name,
             DatabaseConnection<std::string>& db)
        : User(id, name), db(db) {}

    void createOrder() override;
    void viewOrderStatus() override;
    void cancelOrder() override;

    void addToOrder();
    void removeFromOrder();
    void makePayment();

private:
    DatabaseConnection<std::string>& db;
};

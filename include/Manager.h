#pragma once
#include "User.h"
#include "DatabaseConnection.h"

class Manager : public User {
public:
    Manager(int id, const std::string& name,
            DatabaseConnection<std::string>& db)
        : User(id, name), db(db) {}

    void createOrder() override;
    void viewOrderStatus() override;
    void cancelOrder() override;

    void approveOrder();
    void updateStock();

private:
    DatabaseConnection<std::string>& db;
};

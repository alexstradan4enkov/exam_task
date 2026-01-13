#pragma once
#include "User.h"
#include "DatabaseConnection.h"

class Admin : public User {
public:
    Admin(int id, const std::string& name,
          DatabaseConnection<std::string>& db)
        : User(id, name), db(db) {}

    void createOrder() override;
    void viewOrderStatus() override;
    void cancelOrder() override;

    void addProduct();
    void updateProduct();
    void deleteProduct();
    void viewAllOrders();
    void updateOrderStatus();

private:
    DatabaseConnection<std::string>& db;
};

#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Order.h"

class User {
public:
    User(int id, std::string name) : id(id), name(std::move(name)) {}
    virtual ~User() = default;

    virtual void createOrder() = 0;
    virtual void viewOrderStatus() = 0;
    virtual void cancelOrder() = 0;

    void addOrder(const std::shared_ptr<Order>& order) {
        orders.push_back(order);
    }

    int getId() const { return id; }
    const std::string& getName() const { return name; }

protected:
    int id;
    std::string name;
    std::vector<std::shared_ptr<Order>> orders;
};

#pragma once
#include <vector>
#include <memory>
#include <string>
#include <chrono>

class OrderItem {
public:
    OrderItem(int productId, int quantity, double price)
        : productId(productId), quantity(quantity), price(price) {}
    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
private:
    int productId;
    int quantity;
    double price;
};


class Payment;

enum class OrderStatus { Pending, Completed, Canceled, Returned };

class Order {
public:
    Order(int id, int userId)
        : id(id), userId(userId), status(OrderStatus::Pending) {}

    void addItem(int productId, int qty, double price) {
        items.emplace_back(std::make_unique<OrderItem>(productId, qty, price));
    }

    void removeItem(int productId);

    double calculateTotal() const;

    void setStatus(OrderStatus s) { status = s; }
    OrderStatus getStatus() const { return status; }
    int getId() const { return id; }
    int getUserId() const { return userId; }

    void setPayment(std::unique_ptr<Payment> p) {
        payment = std::move(p);
    }

private:
    int id;
    int userId;
    OrderStatus status;
    std::vector<std::unique_ptr<OrderItem>> items;
    std::unique_ptr<Payment> payment;
};

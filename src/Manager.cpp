#include "Manager.h"
#include <iostream>

void Manager::createOrder() {
    std::cout << "Менеджер обычно не создаёт заказы напрямую.\n";
}

void Manager::viewOrderStatus() {
    int orderId;
    std::cout << "Введите order_id: ";
    std::cin >> orderId;
    auto res = db.executeQuery(
        "SELECT getOrderStatus(" + std::to_string(orderId) + ");");
    if (!res.empty() && !res[0].empty()) {
        std::cout << "Статус заказа: " << res[0][0] << "\n";
    }
}

void Manager::cancelOrder() {
    std::cout << "Операция отмены может быть ограничена правами.\n";
}

void Manager::approveOrder() {
    int orderId;
    std::cout << "order_id для утверждения: ";
    std::cin >> orderId;
    std::string sql =
        "SELECT change_order_status(" +
        std::to_string(orderId) + ", 'completed', " +
        std::to_string(getId()) + ");";
    db.executeNonQuery(sql);
    std::cout << "Заказ утверждён (completed)\n";
}

void Manager::updateStock() {
    int id, qty;
    std::cout << "product_id и новое количество: ";
    std::cin >> id >> qty;
    std::string sql =
        "UPDATE products SET stock_quantity = " +
        std::to_string(qty) +
        " WHERE product_id = " + std::to_string(id) + ";";
    db.executeNonQuery(sql);
    std::cout << "Склад обновлён\n";
}

#include "Admin.h"
#include "Order.h"
#include <iostream>

void Admin::createOrder() {
    int userId;
    std::cout << "Введите user_id: ";
    std::cin >> userId;

    // Упрощённый пример: один товар
    int productId, qty;
    std::cout << "product_id, quantity: ";
    std::cin >> productId >> qty;

    std::string sql =
        "SELECT createOrder(" +
        std::to_string(userId) + ", ARRAY[" + std::to_string(productId) +
        "], ARRAY[" + std::to_string(qty) + "]);";

    auto res = db.executeQuery(sql);
    if (!res.empty() && !res[0].empty()) {
        int orderId = std::stoi(res[0][0]);
        auto order = std::make_shared<Order>(orderId, userId);
        addOrder(order);
        std::cout << "Создан заказ с id=" << orderId << "\n";
    }
}

void Admin::viewOrderStatus() {
    int orderId;
    std::cout << "Введите order_id: ";
    std::cin >> orderId;
    std::string sql = "SELECT getOrderStatus(" + std::to_string(orderId) + ");";
    auto res = db.executeQuery(sql);
    if (!res.empty() && !res[0].empty()) {
        std::cout << "Статус заказа: " << res[0][0] << "\n";
    } else {
        std::cout << "Заказ не найден\n";
    }
}

void Admin::cancelOrder() {
    int orderId;
    std::cout << "Введите order_id для отмены: ";
    std::cin >> orderId;
    std::string sql =
        "SELECT change_order_status(" +
        std::to_string(orderId) + ", 'canceled', " +
        std::to_string(getId()) + ");";
    db.executeNonQuery(sql);
    std::cout << "Заказ отменён\n";
}

void Admin::addProduct() {
    std::string name;
    double price;
    int stock;
    std::cout << "Название, цена, количество: ";
    std::cin >> name >> price >> stock;

    std::string sql =
        "INSERT INTO products(name, price, stock_quantity) VALUES (" +
        db.getRawConnection().quote(name) + ", " +
        std::to_string(price) + ", " +
        std::to_string(stock) + ");";
    db.executeNonQuery(sql);
    std::cout << "Продукт добавлен\n";
}

void Admin::updateProduct() {
    int id;
    double price;
    int stock;
    std::cout << "product_id, новая цена, количество: ";
    std::cin >> id >> price >> stock;

    std::string sql =
        "UPDATE products SET price = " + std::to_string(price) +
        ", stock_quantity = " + std::to_string(stock) +
        " WHERE product_id = " + std::to_string(id) + ";";
    db.executeNonQuery(sql);
    std::cout << "Продукт обновлён\n";
}

void Admin::deleteProduct() {
    int id;
    std::cout << "product_id для удаления: ";
    std::cin >> id;
    std::string sql =
        "DELETE FROM products WHERE product_id = " + std::to_string(id) + ";";
    db.executeNonQuery(sql);
    std::cout << "Продукт удалён\n";
}

void Admin::viewAllOrders() {
    auto res = db.executeQuery(
        "SELECT order_id, user_id, status, total_price FROM orders;");
    for (const auto& row : res) {
        std::cout << "order_id=" << row[0]
                  << " user_id=" << row[1]
                  << " status=" << row[2]
                  << " total=" << row[3] << "\n";
    }
}

void Admin::updateOrderStatus() {
    int orderId;
    std::string newStatus;
    std::cout << "order_id, новый статус (pending/completed/canceled/returned): ";
    std::cin >> orderId >> newStatus;
    std::string sql =
        "SELECT change_order_status(" +
        std::to_string(orderId) + ", " +
        db.getRawConnection().quote(newStatus) + ", " +
        std::to_string(getId()) + ");";
    db.executeNonQuery(sql);
    std::cout << "Статус обновлён\n";
}

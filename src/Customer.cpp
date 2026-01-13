#include "Customer.h"
#include "Order.h"
#include "PaymentStrategy.h"
#include "Payment.h"
#include <iostream>
#include <algorithm>
#include <numeric>

void Customer::createOrder() {
    // Создаём пустой заказ в БД через createOrder с одним фиктивным товаром
    int productId, qty;
    std::cout << "product_id, quantity: ";
    std::cin >> productId >> qty;

    std::string sql =
        "SELECT createOrder(" +
        std::to_string(getId()) + ", ARRAY[" +
        std::to_string(productId) + "], ARRAY[" +
        std::to_string(qty) + "]);";

    auto res = db.executeQuery(sql);
    if (!res.empty() && !res[0].empty()) {
        int orderId = std::stoi(res[0][0]);
        auto order = std::make_shared<Order>(orderId, getId());
        addOrder(order);
        std::cout << "Создан заказ с id=" << orderId << "\n";
    }
}

void Customer::viewOrderStatus() {
    int orderId;
    std::cout << "Введите order_id: ";
    std::cin >> orderId;

    // доступ только к своим заказам
    auto it = std::find_if(orders.begin(), orders.end(),
        [orderId](const std::shared_ptr<Order>& o) {
            return o->getId() == orderId;
        });
    if (it == orders.end()) {
        std::cout << "Нет доступа к этому заказу\n";
        return;
    }

    auto res = db.executeQuery(
        "SELECT getOrderStatus(" + std::to_string(orderId) + ");");
    if (!res.empty() && !res[0].empty()) {
        std::cout << "Статус: " << res[0][0] << "\n";
    }
}

void Customer::cancelOrder() {
    int orderId;
    std::cout << "order_id для отмены: ";
    std::cin >> orderId;
    std::string sql =
        "SELECT change_order_status(" +
        std::to_string(orderId) + ", 'canceled', " +
        std::to_string(getId()) + ");";
    db.executeNonQuery(sql);
    std::cout << "Заказ отменён\n";
}

void Customer::addToOrder() {
    int orderId, productId, qty;
    std::cout << "order_id, product_id, quantity: ";
    std::cin >> orderId >> productId >> qty;

    // очень упрощённо: добавляем новую строку в order_items
    std::string sql =
        "INSERT INTO order_items(order_id, product_id, quantity, price) "
        "SELECT " + std::to_string(orderId) + ", product_id, " +
        std::to_string(qty) + ", price FROM products "
        "WHERE product_id = " + std::to_string(productId) + ";";
    db.executeNonQuery(sql);

    // пересчёт total_price
    db.executeNonQuery(
        "UPDATE orders SET total_price = ("
        "  SELECT COALESCE(SUM(quantity * price),0) "
        "  FROM order_items WHERE order_id = " + std::to_string(orderId) +
        ") WHERE order_id = " + std::to_string(orderId) + ";");

    std::cout << "Товар добавлен в заказ\n";
}

void Customer::removeFromOrder() {
    int orderId, productId;
    std::cout << "order_id, product_id для удаления: ";
    std::cin >> orderId >> productId;

    std::string sql =
        "DELETE FROM order_items WHERE order_id = " +
        std::to_string(orderId) + " AND product_id = " +
        std::to_string(productId) + ";";
    db.executeNonQuery(sql);

    db.executeNonQuery(
        "UPDATE orders SET total_price = ("
        "  SELECT COALESCE(SUM(quantity * price),0) "
        "  FROM order_items WHERE order_id = " + std::to_string(orderId) +
        ") WHERE order_id = " + std::to_string(orderId) + ";");

    std::cout << "Товар удалён из заказа\n";
}

void Customer::makePayment() {
    int orderId;
    std::cout << "order_id для оплаты: ";
    std::cin >> orderId;

    auto res = db.executeQuery(
        "SELECT total_price FROM orders WHERE order_id = " +
        std::to_string(orderId) + ";");
    if (res.empty() || res[0].empty()) {
        std::cout << "Заказ не найден\n";
        return;
    }

    double amount = std::stod(res[0][0]);

    std::cout << "Выберите способ оплаты:\n"
              << "1. Карта\n2. Электронный кошелёк\n3. СБП\n";
    int choice;
    std::cin >> choice;

    std::unique_ptr<PaymentStrategy> strategy;
    if (choice == 1) strategy = std::make_unique<CardPaymentStrategy>();
    else if (choice == 2) strategy = std::make_unique<WalletPaymentStrategy>();
    else strategy = std::make_unique<SBPPaymentStrategy>();

    Payment payment(std::move(strategy));
    if (payment.process(amount, "order #" + std::to_string(orderId))) {
        // проставляем статус completed через хранимую процедуру
        std::string sql =
            "SELECT change_order_status(" +
            std::to_string(orderId) + ", 'completed', " +
            std::to_string(getId()) + ");";
        db.executeNonQuery(sql);
    }
}

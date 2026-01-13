#include "Menu.h"
#include <iostream>

void adminMenu(std::shared_ptr<Admin> a) {
    bool run = true;
    while (run) {
        std::cout << "\nМеню администратора:\n"
                  << "1. Добавить продукт\n"
                  << "2. Обновить продукт\n"
                  << "3. Удалить продукт\n"
                  << "4. Просмотр всех заказов\n"
                  << "5. Изменить статус заказа\n"
                  << "6. Создать заказ\n"
                  << "7. Просмотр статуса заказа\n"
                  << "8. Отменить заказ\n"
                  << "9. Выход\n";
        int c; std::cin >> c;
        switch (c) {
            case 1: a->addProduct(); break;
            case 2: a->updateProduct(); break;
            case 3: a->deleteProduct(); break;
            case 4: a->viewAllOrders(); break;
            case 5: a->updateOrderStatus(); break;
            case 6: a->createOrder(); break;
            case 7: a->viewOrderStatus(); break;
            case 8: a->cancelOrder(); break;
            default: run = false;
        }
    }
}

void managerMenu(std::shared_ptr<Manager> m) {
    bool run = true;
    while (run) {
        std::cout << "\nМеню менеджера:\n"
                  << "1. Просмотр статуса заказа\n"
                  << "2. Утвердить заказ\n"
                  << "3. Обновить склад\n"
                  << "4. Выход\n";
        int c; std::cin >> c;
        switch (c) {
            case 1: m->viewOrderStatus(); break;
            case 2: m->approveOrder(); break;
            case 3: m->updateStock(); break;
            default: run = false;
        }
    }
}

void customerMenu(std::shared_ptr<Customer> c) {
    bool run = true;
    while (run) {
        std::cout << "\nМеню покупателя:\n"
                  << "1. Создать заказ\n"
                  << "2. Добавить товар в заказ\n"
                  << "3. Удалить товар из заказа\n"
                  << "4. Просмотр статуса заказа\n"
                  << "5. Оплатить заказ\n"
                  << "6. Отменить заказ\n"
                  << "7. Выход\n";
        int ch; std::cin >> ch;
        switch (ch) {
            case 1: c->createOrder(); break;
            case 2: c->addToOrder(); break;
            case 3: c->removeFromOrder(); break;
            case 4: c->viewOrderStatus(); break;
            case 5: c->makePayment(); break;
            case 6: c->cancelOrder(); break;
            default: run = false;
        }
    }
}

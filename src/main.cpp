#include "Menu.h"

int main() {
    std::string connStr = "host=localhost user=postgres password=pass dbname=store";
    DatabaseConnection<std::string> db(connStr);

    bool running = true;
    while (running) {
        std::cout << "Пожалуйста, выберите свою роль:\n"
                  << "1. Войти как Администратор\n"
                  << "2. Войти как Менеджер\n"
                  << "3. Войти как Покупатель\n"
                  << "4. Выход\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                auto admin = std::make_shared<Admin>(1, "Admin", db);
                adminMenu(admin);
                break;
            }
            case 2: {
                auto manager = std::make_shared<Manager>(2, "Manager", db);
                managerMenu(manager);
                break;
            }
            case 3: {
                auto customer = std::make_shared<Customer>(3, "Customer", db);
                customerMenu(customer);
                break;
            }
            default:
                running = false;
        }
    }
    return 0;
}

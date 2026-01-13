# Онлайн-магазин (C++ + PostgreSQL)

## 1. Что это за проект

Консольное приложение интернет-магазина с ролями:
- Администратор  
- Менеджер  
- Покупатель  

Используются: C++17(или 20), PostgreSQL, libpqxx, CMake.

---

## 2. Архитектура

Основные классы:
- `DatabaseConnection<T>` — шаблонный класс для работы с PostgreSQL (запросы, транзакции).
- `User` (абстрактный) и наследники `Admin`, `Manager`, `Customer`.
- `Order` и `OrderItem` (композиция: заказ владеет элементами).
- `Payment` и стратегии `PaymentStrategy`:
  - `CardPaymentStrategy`
  - `WalletPaymentStrategy`
  - `SBPPaymentStrategy`

Используются умные указатели:
- `std::unique_ptr` для `OrderItem` и `PaymentStrategy`.
- `std::shared_ptr` для хранения заказов у пользователя.

STL и лямбды:
- `std::find_if`, `std::copy_if`, `std::accumulate` для работы с коллекциями заказов.
- Лямбда-функции для фильтрации заказов и проверки прав доступа.

---

## 3. База данных

Таблицы:
- `users`, `products`, `orders`, `order_items`,
- `order_status_history`, `audit_log`.

Ограничения:
- `price > 0`, `stock_quantity >= 0`.
- `email` уникален.
- `role` и `status` ограничены через `CHECK`.[web:86][web:89]

Функции и процедуры (PL/pgSQL):
- `createOrder(...)` — создаёт заказ и позиции, считает `total_price`.
- `change_order_status(...)` — меняет статус, пишет историю и аудит.
- `getOrderStatus(order_id)` — вернёт статус заказа.
- `getUserOrderCount(user_id)`, `getTotalSpentByUser(user_id)`.
- `canReturnOrder(order_id)` — проверяет возможность возврата.
- `getOrderStatusHistory(order_id)`, `getAuditLogByUser(user_id)`.[web:92][web:95]

Триггеры:
- Обновление `order_date` при смене статуса заказа.
- Аудит операций с продуктами в `audit_log`.

---

## 4. Роли и меню

- **Администратор**  
  Управляет товарами, видит все заказы, меняет статусы, смотрит историю и аудит, формирует CSV‑отчёт.

- **Менеджер**  
  Утверждает заказы, обновляет склад, работает только со «своими» заказами.

- **Покупатель**  
  Создаёт заказы, добавляет/удаляет товары, оплачивает заказ выбранной стратегией, видит только свои заказы и историю.

При запуске программы сначала выбирается роль, затем показывается соответствующее текстовое меню.

---

## 5. Сборка и запуск

Требования:
- CMake ≥ 3.12  
- Компилятор с поддержкой C++17  
- PostgreSQL и установленная libpqxx

Сборка:

```bash
mkdir build
cd build
cmake ..
cmake --build .

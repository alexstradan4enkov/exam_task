INSERT INTO users(name, email, role, password_hash, loyalty_level)
VALUES
('Админ',   'admin@example.com',   'admin',   'hash', 1),
('Менеджер','manager@example.com', 'manager', 'hash', 0),
('Клиент',  'customer@example.com','customer','hash', 0);

INSERT INTO products(name, price, stock_quantity)
VALUES
('Товар1', 100.00, 10),
('Товар2', 200.00, 5);

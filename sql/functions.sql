Напишу всё SQL по блокам, чтобы не листать. Это минимальный набор, который можно дальше дополнять. Он разбит по файлам, как в задании.

sql/tables.sql
sql
CREATE TABLE users (
    user_id       SERIAL PRIMARY KEY,
    name          VARCHAR(100) NOT NULL,
    email         VARCHAR(100) NOT NULL UNIQUE,
    role          VARCHAR(20)  NOT NULL CHECK (role IN ('admin','manager','customer')),
    password_hash VARCHAR(255) NOT NULL,
    loyalty_level INTEGER      NOT NULL DEFAULT 0 CHECK (loyalty_level IN (0,1))
);

CREATE TABLE products (
    product_id     SERIAL PRIMARY KEY,
    name           VARCHAR(200) NOT NULL,
    price          DECIMAL(10,2) NOT NULL CHECK (price > 0),
    stock_quantity INTEGER NOT NULL CHECK (stock_quantity >= 0)
);

CREATE TABLE orders (
    order_id    SERIAL PRIMARY KEY,
    user_id     INTEGER NOT NULL REFERENCES users(user_id),
    status      VARCHAR(20) NOT NULL CHECK (status IN ('pending','completed','canceled','returned')),
    total_price DECIMAL(10,2) NOT NULL DEFAULT 0,
    order_date  TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id      INTEGER NOT NULL REFERENCES orders(order_id) ON DELETE CASCADE,
    product_id    INTEGER NOT NULL REFERENCES products(product_id),
    quantity      INTEGER NOT NULL CHECK (quantity > 0),
    price         DECIMAL(10,2) NOT NULL
);

CREATE TABLE order_status_history (
    history_id  SERIAL PRIMARY KEY,
    order_id    INTEGER NOT NULL REFERENCES orders(order_id),
    old_status  VARCHAR(20),
    new_status  VARCHAR(20),
    changed_at  TIMESTAMP NOT NULL DEFAULT NOW(),
    changed_by  INTEGER REFERENCES users(user_id)
);

CREATE TABLE audit_log (
    log_id        SERIAL PRIMARY KEY,
    entity_type   VARCHAR(20) NOT NULL,
    entity_id     INTEGER     NOT NULL,
    operation     VARCHAR(10) NOT NULL,
    performed_by  INTEGER,
    performed_at  TIMESTAMP NOT NULL DEFAULT NOW(),
    error_message TEXT
);
sql/functions.sql
sql
-- статус заказа по id
CREATE OR REPLACE FUNCTION getOrderStatus(p_order_id INT)
RETURNS VARCHAR AS $$
DECLARE
    v_status VARCHAR;
BEGIN
    SELECT status INTO v_status FROM orders WHERE order_id = p_order_id;
    RETURN v_status;
END;
$$ LANGUAGE plpgsql;

-- количество заказов пользователя
CREATE OR REPLACE FUNCTION getUserOrderCount(p_user_id INT)
RETURNS INT AS $$
DECLARE
    v_cnt INT;
BEGIN
    SELECT COUNT(*) INTO v_cnt FROM orders WHERE user_id = p_user_id;
    RETURN v_cnt;
END;
$$ LANGUAGE plpgsql;

-- общая сумма потраченных денег пользователем
CREATE OR REPLACE FUNCTION getTotalSpentByUser(p_user_id INT)
RETURNS NUMERIC AS $$
DECLARE
    v_sum NUMERIC;
BEGIN
    SELECT COALESCE(SUM(total_price), 0)
    INTO v_sum
    FROM orders
    WHERE user_id = p_user_id
      AND status IN ('completed','returned');
    RETURN v_sum;
END;
$$ LANGUAGE plpgsql;

-- можно ли вернуть заказ: завершён и не более 30 дней
CREATE OR REPLACE FUNCTION canReturnOrder(p_order_id INT)
RETURNS BOOLEAN AS $$
DECLARE
    v_status VARCHAR;
    v_date   TIMESTAMP;
BEGIN
    SELECT status, order_date
    INTO v_status, v_date
    FROM orders
    WHERE order_id = p_order_id;

    IF v_status = 'completed'
       AND v_date >= NOW() - INTERVAL '30 days' THEN
        RETURN TRUE;
    END IF;

    RETURN FALSE;
END;
$$ LANGUAGE plpgsql;

-- история статусов заказа
CREATE OR REPLACE FUNCTION getOrderStatusHistory(p_order_id INT)
RETURNS TABLE(
    order_id   INT,
    old_status VARCHAR,
    new_status VARCHAR,
    changed_at TIMESTAMP,
    changed_by INT
) AS $$
BEGIN
    RETURN QUERY
    SELECT order_id, old_status, new_status, changed_at, changed_by
    FROM order_status_history
    WHERE order_id = p_order_id
    ORDER BY changed_at;
END;
$$ LANGUAGE plpgsql;

-- действия пользователя из audit_log
CREATE OR REPLACE FUNCTION getAuditLogByUser(p_user_id INT)
RETURNS SETOF audit_log AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM audit_log
    WHERE performed_by = p_user_id
    ORDER BY performed_at DESC;
END;
$$ LANGUAGE plpgsql;

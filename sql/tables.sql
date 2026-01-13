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

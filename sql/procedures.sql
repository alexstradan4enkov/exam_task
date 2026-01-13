CREATE OR REPLACE FUNCTION createOrder(
    p_user_id INT,
    p_product_ids INT[],
    p_quantities INT[]
) RETURNS INT AS $$
DECLARE
    v_order_id INT;
    v_total    NUMERIC := 0;
    v_price    NUMERIC;
    i          INT;
BEGIN
    PERFORM pg_advisory_xact_lock(1); -- примитивная защита
    INSERT INTO orders(user_id, status, total_price)
    VALUES (p_user_id, 'pending', 0)
    RETURNING order_id INTO v_order_id;

    FOR i IN array_lower(p_product_ids,1)..array_upper(p_product_ids,1) LOOP
        SELECT price INTO v_price FROM products WHERE product_id = p_product_ids[i];
        IF NOT FOUND THEN
            RAISE EXCEPTION 'Product % not found', p_product_ids[i];
        END IF;

        INSERT INTO order_items(order_id, product_id, quantity, price)
        VALUES (v_order_id, p_product_ids[i], p_quantities[i], v_price);

        v_total := v_total + v_price * p_quantities[i];
    END LOOP;

    UPDATE orders SET total_price = v_total WHERE order_id = v_order_id;

    RETURN v_order_id;
END;
$$ LANGUAGE plpgsql;

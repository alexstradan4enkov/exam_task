CREATE OR REPLACE FUNCTION orders_status_trigger()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.status <> OLD.status THEN
        NEW.order_date := NOW();
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_orders_status_date
BEFORE UPDATE ON orders
FOR EACH ROW
EXECUTE FUNCTION orders_status_trigger();

-- триггер аудита для products
CREATE OR REPLACE FUNCTION products_audit_trigger()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        INSERT INTO audit_log(entity_type, entity_id, operation, performed_by)
        VALUES ('product', NEW.product_id, 'insert', NULL);
    ELSIF TG_OP = 'UPDATE' THEN
        INSERT INTO audit_log(entity_type, entity_id, operation, performed_by)
        VALUES ('product', NEW.product_id, 'update', NULL);
    ELSIF TG_OP = 'DELETE' THEN
        INSERT INTO audit_log(entity_type, entity_id, operation, performed_by)
        VALUES ('product', OLD.product_id, 'delete', NULL);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_products_audit
AFTER INSERT OR UPDATE OR DELETE ON products
FOR EACH ROW
EXECUTE FUNCTION products_audit_trigger();

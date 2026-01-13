#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>

template<typename T>
class DatabaseConnection {
public:
    explicit DatabaseConnection(const T& connStr)
        : conn(connStr) {}

    std::vector<std::vector<std::string>> executeQuery(const std::string& sql) {
        pqxx::work tx(conn);
        pqxx::result r = tx.exec(sql);
        std::vector<std::vector<std::string>> result;
        result.reserve(r.size());
        for (const auto& row : r) {
            std::vector<std::string> cols;
            cols.reserve(row.size());
            for (const auto& field : row) {
                cols.push_back(field.as<std::string>(""));
            }
            result.push_back(std::move(cols));
        }
        tx.commit();
        return result;
    }

    void executeNonQuery(const std::string& sql) {
        pqxx::work tx(conn);
        tx.exec(sql);
        tx.commit();
    }

    void beginTransaction() {
        if (transaction) return;
        transaction = std::make_unique<pqxx::work>(conn);
    }

    void commitTransaction() {
        if (transaction) {
            transaction->commit();
            transaction.reset();
        }
    }

    void rollbackTransaction() {
        if (transaction) {
            transaction->abort();
            transaction.reset();
        }
    }

    void createFunction(const std::string& sql) {
        executeNonQuery(sql);
    }

    void createTrigger(const std::string& sql) {
        executeNonQuery(sql);
    }

    bool getTransactionStatus() const {
        return static_cast<bool>(transaction);
    }

    ~DatabaseConnection() = default;

    pqxx::connection& getRawConnection() { return conn; }
    pqxx::work& getCurrentTx() { return *transaction; }

private:
    pqxx::connection conn;
    std::unique_ptr<pqxx::work> transaction;
};

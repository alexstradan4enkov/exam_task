#include "Report.h"
#include <fstream>
#include <iostream>

ReportGenerator::ReportGenerator(DatabaseConnection<std::string>& db)
    : db(db) {}

void ReportGenerator::generateAuditReport(const std::string& filePath) {
    // Здесь можно использовать либо VIEW/функцию, либо прямой SELECT
    std::string sql =
        "SELECT o.order_id, o.user_id, o.status, o.order_date, "
        "       h.old_status, h.new_status, h.changed_at, h.changed_by, "
        "       a.entity_type, a.entity_id, a.operation, a.performed_by, a.performed_at "
        "FROM orders o "
        "LEFT JOIN order_status_history h ON h.order_id = o.order_id "
        "LEFT JOIN audit_log a ON a.entity_type = 'order' AND a.entity_id = o.order_id "
        "ORDER BY o.order_id, h.changed_at, a.performed_at;";

    auto rows = db.executeQuery(sql);

    std::ofstream out(filePath);
    if (!out) {
        std::cerr << "Не удалось открыть файл отчёта\n";
        return;
    }

    // заголовок CSV
    out << "order_id,user_id,status,order_date,old_status,new_status,changed_at,"
           "changed_by,entity_type,entity_id,operation,performed_by,performed_at\n";

    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i) out << ",";
            out << row[i];
        }
        out << "\n";
    }

    std::cout << "Отчёт сохранён в " << filePath << "\n";
}

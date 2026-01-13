#pragma once
#include <string>
#include "DatabaseConnection.h"

class ReportGenerator {
public:
    explicit ReportGenerator(DatabaseConnection<std::string>& db);
    void generateAuditReport(const std::string& filePath);
private:
    DatabaseConnection<std::string>& db;
};

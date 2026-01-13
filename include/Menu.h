#pragma once
#include <memory>
#include "Admin.h"
#include "Manager.h"
#include "Customer.h"

void adminMenu(std::shared_ptr<Admin> admin);
void managerMenu(std::shared_ptr<Manager> manager);
void customerMenu(std::shared_ptr<Customer> customer);

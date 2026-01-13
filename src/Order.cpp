#include "Order.h"
#include <algorithm>

void Order::removeItem(int productId) {
    auto it = std::remove_if(items.begin(), items.end(),
        [productId](const std::unique_ptr<OrderItem>& it) {
            return it->getProductId() == productId;
        });
    items.erase(it, items.end());
}

double Order::calculateTotal() const {
    double total = 0.0;
    for (const auto& it : items) {
        total += it->getPrice() * it->getQuantity();
    }
    return total;
}

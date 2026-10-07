#include "order_book.hpp"
#include <iostream>

static void show(const std::vector<Trade>& ts) {
    for (auto& t : ts)
        std::cout << "  TRADE buy#" << t.buy_id << " sell#" << t.sell_id
                  << " @" << t.price << " x" << t.qty << "\n";
}

int main() {
    OrderBook b;
    b.add_limit(1, Side::Sell, 10003, 300);
    b.add_limit(2, Side::Sell, 10004, 200);
    b.add_limit(3, Side::Buy,  10001, 500);
    b.add_limit(4, Side::Buy,  10000, 100);
    std::cout << "Libro inicial:\n"; b.print(std::cout);

    std::cout << "\nCompra agresiva 10004 x 400:\n";
    show(b.add_limit(5, Side::Buy, 10004, 400));
    b.print(std::cout);

    std::cout << "\nCancelo la orden 3:\n";
    b.cancel(3);
    b.print(std::cout);
}

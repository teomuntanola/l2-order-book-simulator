#include "order_book.hpp"
#include <cassert>
#include <iostream>

int main() {
    { // sin cruce: queda en reposo
        OrderBook b;
        assert(b.add_limit(1, Side::Buy, 100, 10).empty());
        assert(b.best_bid() == 100 && b.has_order(1));
    }
    { // cruce total al precio de la orden en reposo
        OrderBook b;
        b.add_limit(1, Side::Sell, 100, 10);
        auto t = b.add_limit(2, Side::Buy, 105, 10);
        assert(t.size() == 1 && t[0].price == 100 && t[0].qty == 10);
        assert(!b.has_order(1) && !b.has_order(2));
    }
    { // cruce parcial: el resto queda en el libro
        OrderBook b;
        b.add_limit(1, Side::Sell, 100, 4);
        auto t = b.add_limit(2, Side::Buy, 100, 10);
        assert(t.size() == 1 && t[0].qty == 4);
        assert(b.has_order(2) && b.best_bid() == 100);
    }
    { // prioridad de tiempo en el mismo precio
        OrderBook b;
        b.add_limit(1, Side::Sell, 100, 5);
        b.add_limit(2, Side::Sell, 100, 5);
        auto t = b.add_limit(3, Side::Buy, 100, 5);
        assert(t.size() == 1 && t[0].sell_id == 1);
    }
    { // prioridad de precio: primero el ask más barato
        OrderBook b;
        b.add_limit(1, Side::Sell, 102, 5);
        b.add_limit(2, Side::Sell, 101, 5);
        auto t = b.add_limit(3, Side::Buy, 102, 5);
        assert(t[0].sell_id == 2 && t[0].price == 101);
    }
    { // cancelar
        OrderBook b;
        b.add_limit(1, Side::Buy, 100, 5);
        assert(b.cancel(1) && !b.cancel(1) && b.best_bid() == 0);
    }
    std::cout << "OK: todos los tests pasan\n";
}

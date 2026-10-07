#include "order_book.hpp"
#include <algorithm>
#include <iostream>

template <class Opp, class Crosses>
void OrderBook::match(Opp& opposite, Order& in, Crosses crosses, std::vector<Trade>& trades) {
    while (in.qty > 0 && !opposite.empty()) {
        auto lvl = opposite.begin();                 // mejor nivel del lado contrario
        if (!crosses(lvl->first, in.price)) break;   // ya no hay cruce
        Level& q = lvl->second;
        while (in.qty > 0 && !q.empty()) {
            Order& rest = q.front();                 // la más antigua primero
            Qty fill = std::min(in.qty, rest.qty);
            // El precio de ejecución es el de la orden en reposo.
            trades.push_back(in.side == Side::Buy
                ? Trade{in.id, rest.id, rest.price, fill}
                : Trade{rest.id, in.id, rest.price, fill});
            in.qty   -= fill;
            rest.qty -= fill;
            if (rest.qty == 0) { index_.erase(rest.id); q.pop_front(); }
        }
        if (q.empty()) opposite.erase(lvl);
    }
}

std::vector<Trade> OrderBook::add_limit(OrderId id, Side side, Price price, Qty qty) {
    std::vector<Trade> trades;
    if (qty == 0 || index_.count(id)) return trades;   // validación mínima
    Order in{id, side, price, qty};

    if (side == Side::Buy)
        match(asks_, in, [](Price ask, Price lim) { return ask <= lim; }, trades);
    else
        match(bids_, in, [](Price bid, Price lim) { return bid >= lim; }, trades);

    if (in.qty > 0) {                                   // lo que sobra queda en reposo
        auto place = [&](auto& book) {
            Level& lvl = book[price];
            lvl.push_back(in);
            index_[id] = {side, price, std::prev(lvl.end())};
        };
        if (side == Side::Buy) place(bids_); else place(asks_);
    }
    return trades;
}

bool OrderBook::cancel(OrderId id) {
    auto f = index_.find(id);
    if (f == index_.end()) return false;
    const Loc loc = f->second;
    auto erase_from = [&](auto& book) {
        auto lvl = book.find(loc.price);
        lvl->second.erase(loc.it);
        if (lvl->second.empty()) book.erase(lvl);
    };
    if (loc.side == Side::Buy) erase_from(bids_); else erase_from(asks_);
    index_.erase(f);
    return true;
}

void OrderBook::print(std::ostream& os, int depth) const {
    auto total = [](const Level& l) { Qty s = 0; for (auto& o : l) s += o.qty; return s; };
    os << "  ASKS\n";
    std::vector<std::pair<Price, Qty>> a;
    for (auto& [p, l] : asks_) { if ((int)a.size() == depth) break; a.push_back({p, total(l)}); }
    for (auto it = a.rbegin(); it != a.rend(); ++it) os << "    " << it->first << " x " << it->second << "\n";
    os << "  ----\n  BIDS\n";
    int n = 0;
    for (auto& [p, l] : bids_) { if (n++ == depth) break; os << "    " << p << " x " << total(l) << "\n"; }
}

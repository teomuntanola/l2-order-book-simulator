#pragma once
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

using OrderId = std::uint64_t;
using Price   = std::int64_t;   // en "ticks" (enteros): 10001 = 100.01. Nunca float.
using Qty     = std::uint32_t;

enum class Side { Buy, Sell };

struct Order { OrderId id; Side side; Price price; Qty qty; };
struct Trade { OrderId buy_id; OrderId sell_id; Price price; Qty qty; };

class OrderBook {
public:
    // Inserta una orden limit. Casa lo que cruce y deja el resto en el libro.
    std::vector<Trade> add_limit(OrderId id, Side side, Price price, Qty qty);
    // Cancela una orden en reposo. false si no existe.
    bool cancel(OrderId id);

    bool  has_order(OrderId id) const { return index_.count(id) > 0; }
    Price best_bid() const { return bids_.empty() ? 0 : bids_.begin()->first; }
    Price best_ask() const { return asks_.empty() ? 0 : asks_.begin()->first; }
    void  print(std::ostream& os, int depth = 5) const;

private:
    using Level = std::list<Order>;               // FIFO dentro de cada precio (time priority)
    std::map<Price, Level, std::greater<>> bids_; // mejor bid = precio más alto primero
    std::map<Price, Level> asks_;                 // mejor ask = precio más bajo primero

    struct Loc { Side side; Price price; Level::iterator it; };
    std::unordered_map<OrderId, Loc> index_;      // id -> posición, para cancelar en O(1)

    template <class Opp, class Crosses>
    void match(Opp& opposite, Order& in, Crosses crosses, std::vector<Trade>& trades);
};

# L2 Order Book Simulator (C++20)

Motor de matching con **price-time priority**: libro de órdenes limit con bids/asks,
ejecución de órdenes que cruzan y cancelación.

## Uso
    make test   # compila y pasa los tests
    ./demo      # ejemplo con un libro pequeño

## Diseño
- Precios enteros en ticks (sin floats).
- `std::map` por precio + `std::list` FIFO por nivel; índice `id -> iterator` para cancelar en O(1).
- Precio de ejecución = precio de la orden en reposo.

## Roadmap
- [ ] Órdenes market e IOC/FOK
- [ ] Modificar orden (modify)
- [ ] Benchmark (órdenes/segundo) y perfilado
- [ ] Sustituir `std::map`/`std::list` por estructuras sin allocations (arrays + pool)
- [ ] Replay de datos reales/sintéticos desde fichero

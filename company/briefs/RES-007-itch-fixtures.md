# RES-007: synthetic ITCH Add Order byte examples

Research agent artifact for L007. Two standalone payloads support the apprentice's
decoder; they are not a feed recording or an ordered market history. No parser,
encoder, or apprentice tests are supplied. Base: `2a8aa582d8acb227c151c305614fec0bdc6fa488`.

## Source and boundary

Layout checked 2026-09-29 against [Nasdaq TotalView-ITCH 5.0](https://nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf),
Data Types (page 4) and Add Order without MPID attribution (page 13).
Integers use unsigned big-endian encoding. Stock is eight ASCII bytes with
right-hand spaces. Price (4) is an integer with four implied decimal places.
Timestamp is nanoseconds since midnight, not Unix time.

Each file contains exactly one 36-byte `A` payload, beginning with `41` hex.
There is no transport header, length prefix, compression, or trailing newline.
Engineering owns the decoder contract and error precedence; these examples
cover valid inputs only.

## Literal bytes and expected fields

The byte sequences were chosen literally, independently of any implementation.
Binary files were written by converting each printed hex token to one byte;
no field encoder generated them. Offsets below are zero-based.

`tests/fixtures/itch50/add_order_buy.bin`:

```text
41 12 34 AB CD 01 02 03 04 05 06 80 00 00 01 02 03 04 05 42 00 01 02 03 5A 58 59 51 20 20 20 20 00 12 D6 87
```

`tests/fixtures/itch50/add_order_sell.bin`:

```text
41 AB CD 12 34 02 03 04 05 06 07 FE DC BA 98 76 54 32 10 53 01 02 03 04 51 56 58 5A 20 20 20 20 00 0F 42 41
```

| Field | Offset / bytes | Buy expected | Sell expected |
| --- | --- | --- | --- |
| Type | 0 / 1 | `A` | `A` |
| Stock locate | 1 / 2 | 4660 | 43981 |
| Tracking number | 3 / 2 | 43981 | 4660 |
| Timestamp, raw ns | 5 / 6 | 1108152157446 | 2211975595527 |
| Order reference | 11 / 8 | 9223372041183495173 | 18364758544493064720 |
| Side | 19 / 1 | `B` | `S` |
| Shares | 20 / 4 | 66051 | 16909060 |
| Stock, exact bytes | 24 / 8 | `ZXYQ    ` | `QVXZ    ` |
| Price, raw integer | 32 / 4 | 1234567 | 1000001 |

Both symbols have four trailing `20` bytes (spaces), no NUL terminator.
Human-readable display prices are 123.4567 and 100.0001 respectively; retaining
raw integers avoids rounding to cents. Both references exceed signed 64-bit
range. Unequal byte patterns expose byte-order mistakes; multi-byte shares
and the six-byte timestamp expose truncation. Sizes are synthetic stress values,
not claims of typical order sizes. Neither timestamp needs its top bit set:
both remain within a day.

## Interpretation and limitations

All values and symbol choices are invented. Any match to a real symbol is
incidental. No provider data, acquisition, licensing entitlement, or empirical
result is represented. A successful decode says what these bytes contain;
it does not validate a real order against exchange rules or reconstruct a book.

The timestamp alone supplies neither a date nor local receipt/availability time.
These isolated examples establish no session identity, sequence continuity,
order lifetime, execution, or causal information policy. The displayed order
price is not automatically an account mark or a price at which our account can
trade. Later book reconstruction, mark selection, and execution modeling are
separate decisions. Historical-source approval remains outstanding.

## Verification and handoff

Research checked file lengths, exact hex round trips, field-boundary slices,
and decimal values using PowerShell unsigned integer conversion. This verifies
the fixture artifact only; no apprentice decoder or runtime test suite was run.

Reproduce the byte inspection from the repository root in PowerShell:

```powershell
Get-Item tests/fixtures/itch50/*.bin | Select-Object Name,Length
Get-FileHash tests/fixtures/itch50/*.bin -Algorithm SHA256
Format-Hex tests/fixtures/itch50/add_order_buy.bin
Format-Hex tests/fixtures/itch50/add_order_sell.bin
[Convert]::ToUInt64('8000000102030405',16)
[Convert]::ToUInt64('fedcba9876543210',16)
```

Risk reviews the exact commit independently before integration. Engineering
owns the apprentice assignment and later runtime acceptance. Research does not
claim independent validation of its own artifact.

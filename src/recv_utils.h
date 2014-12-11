/**
 * @file
 *
 * Miscellaneous utilities for receiving SPEAD data.
 */

#ifndef SPEAD_RECV_UTILS_H
#define SPEAD_RECV_UTILS_H

namespace spead
{

/**
 * SPEAD stream receiver functionality.
 */
namespace recv
{

/**
 * Decodes an %ItemPointer into the ID, mode flag, and address/value.
 *
 * An %ItemPointer is encoded, from MSB to LSB, as
 * - a one bit mode flag (1 for immediate, 0 for address)
 * - an unsigned identifier
 * - either an integer value (in immediate mode) or a payload-relative address
 *   (in address mode).
 * The number of bits in the last field is given by @a heap_address_bits.
 *
 * The wire protocol uses big-endian, but this class assumes that the
 * conversion to host endian has already occurred.
 */
class pointer_decoder
{
private:
    int heap_address_bits;        ///< Bits for immediate/address field
    std::uint64_t address_mask;   ///< Mask selecting the immediate/address field
    std::uint64_t id_mask;        ///< Mask with number of bits for the ID field, shifted down

public:
    explicit pointer_decoder(int heap_address_bits)
    {
        this->heap_address_bits = heap_address_bits;
        this->address_mask = (std::uint64_t(1) << heap_address_bits) - 1;
        this->id_mask = (std::uint64_t(1) << (63 - heap_address_bits)) - 1;
    }

    /// Extract the ID from an item pointer
    std::int64_t get_id(std::uint64_t pointer) const
    {
        return (pointer >> heap_address_bits) & id_mask;
    }

    /**
     * Extract the address from an item pointer. At present, no check is
     * done to ensure that the mode is correct.
     */
    std::int64_t get_address(std::uint64_t pointer) const
    {
        return pointer & address_mask;
    }

    /**
     * Extract the immediate value from an item pointer. At present, no check
     * is done to ensure that the mode is correct.
     */
    std::int64_t get_immediate(std::uint64_t pointer) const
    {
        return get_address(pointer);
    }

    /// Determine whether the item pointer uses immediate mode
    bool is_immediate(std::uint64_t pointer) const
    {
        return pointer >> 63;
    }

    /// Return the number of bits for address/immediate given to the constructor
    int address_bits() const
    {
        return heap_address_bits;
    }
};

} // namespace recv
} // namespace spead

#endif // SPEAD_RECV_UTILS_H

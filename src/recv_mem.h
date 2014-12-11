/**
 * @file
 */

#ifndef SPEAD_RECV_MEM_H
#define SPEAD_RECV_MEM_H

#include <cstdint>
#include "recv_reader.h"

namespace spead
{
namespace recv
{

class reader;

/**
 * Reader class that feeds data from a memory buffer to a stream. The caller
 * must ensure that the underlying memory buffer is not destroyed before
 * this class.
 *
 * @note For simple cases, use @ref mem_to_stream instead. This class is
 * only necessary if one wants to plug in to a @ref Receiver.
 */
class mem_reader : public reader
{
private:
    /// Start of data
    const std::uint8_t *ptr;
    /// Length of data
    std::size_t length;

public:
    mem_reader(boost::asio::io_service &io_service, stream &s,
               const std::uint8_t *ptr, std::size_t length);
    virtual void start() override;
};

} // namespace recv
} // namespace spead

#endif // SPEAD_RECV_MEM_H

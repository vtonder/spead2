/**
 * @file
 */

#ifndef SPEAD_RECV_UDP_H
#define SPEAD_RECV_UDP_H

#include <cstdint>
#include <boost/asio.hpp>
#include "recv_reader.h"
#include "recv_stream.h"

namespace spead
{
namespace recv
{

/**
 * Asynchronous stream reader that receives packets over UDP.
 *
 * @todo Log errors somehow?
 */
class udp_reader : public reader
{
private:
    /// UDP socket we are listening on
    boost::asio::ip::udp::socket socket;
    /// Unused, but need to provide the memory for asio to write to
    boost::asio::ip::udp::endpoint endpoint;
    /// Buffer for asynchronous receive, of size @a max_size + 1.
    std::unique_ptr<uint8_t[]> buffer;
    /// Maximum packet size we will accept
    std::size_t max_size;

protected:
    /// Callback on completion of asynchronous receive
    void packet_handler(
        const boost::system::error_code &error,
        std::size_t bytes_transferred);

public:
    /// Maximum packet size, if none is explicitly passed to the constructor
    static constexpr std::size_t default_max_size = 9200;
    /// Socket receive buffer size, if none is explicitly passed to the constructor
    static constexpr std::size_t default_buffer_size = 8 * 1024 * 1024;

    /**
     * Constructor.
     *
     * @param io_service   IO service for the owning @ref Receiver
     * @param s            Wrapped stream
     * @param endpoint     Address on which to listen
     * @param max_size     Maximum packet size that will be accepted.
     * @param buffer_size  Requested socket buffer size. Note that the
     *                     operating system might not allow a buffer size
     *                     as big as the default.
     *
     * @todo Check that the io_service matches @ref start
     */
    explicit udp_reader(
        boost::asio::io_service &io_service,
        stream &s,
        const boost::asio::ip::udp::endpoint &endpoint,
        std::size_t max_size = default_max_size,
        std::size_t buffer_size = default_buffer_size);

    virtual void start() override;
    virtual void stop() override;
};

} // namespace recv
} // namespace spead

#endif // SPEAD_RECV_UDP_H

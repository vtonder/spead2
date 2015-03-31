Sending
-------
Unlike for receiving, each stream object can only use a single transport.
There is currently no support for collective operations where multiple
producers cooperate to construct a heap between them. It is still possible to
do multi-producer, single-consumer operation if the heap IDs are kept separate.

Because each stream has only one transport, there is a separate class for
each, rather than a generic `Stream` class. Because there is common
configuration between the stream classes, configuration is encapsulated in a
:py:class:`spead2.send.StreamConfig`.

.. autoclass:: spead2.send.StreamConfig(max_packet_size=1472, rate=0.0, burst_size=65536, max_heaps=4)

   :param int max_packet_size: Heaps will be split into packets of at most this size.
   :param double rate: Maximum transmission rate, in bytes per second, or 0
     to send as fast as possible.
   :param int burst_size: Bursts of up to this size will be sent as fast as
     possible. Setting this too large (larger than available buffer sizes)
     risks losing packets, while setting it too small may reduce throughput by
     causing more sleeps than necessary.
   :param int max_heaps: For asynchronous transmits, the maximum number of
     heaps that can be in-flight.

   The constructor arguments are also instance attributes.

Streams send pre-baked heaps, which can be constructed by hand, but are more
normally created from an :py:class:`~spead2.ItemGroup` by a
:py:class:`spead2.send.HeapGenerator`. To simplify cases where one item group
is paired with one heap generator, a convenience class
:py:class:`spead2.send.ItemGroup` is provided that inherits from both.

.. autoclass:: spead2.send.HeapGenerator

   .. automethod:: spead2.send.HeapGenerator.get_heap
   .. automethod:: spead2.send.HeapGenerator.get_end

Blocking send
^^^^^^^^^^^^^

.. py:class:: spead2.send.UdpStream(thread_pool, hostname, port, config, buffer_size=524288)

   Stream using UDP. Note that since UDP is an unreliable protocol, there is
   no guarantee that packets arrive.

   :param thread_pool: Thread pool handling the I/O
   :type thread_pool: :py:class:`spead2.ThreadPool`
   :param str hostname: Peer hostname
   :param int port: Peer port
   :param config: Stream configuration
   :type config: :py:class:`spead2.send.StreamConfig`
   :param int buffer_size: Socket buffer size. A warning is logged if this
     size cannot be set due to OS limits.

   .. py:method:: send_heap(heap)

      Sends a :py:class:`spead2.send.Heap` to the peer, and wait for
      completion. There is currently no indication of whether it successfully
      arrived.

.. py:class:: spead2.send.BytesStream(thread_pool, config)

   Stream that collects packets in memory and makes the concatenated stream
   available.

   :param thread_pool: Thread pool handling the I/O
   :type thread_pool: :py:class:`spead2.ThreadPool`
   :param config: Stream configuration
   :type config: :py:class:`spead2.send.StreamConfig`

   .. py:method:: send_heap(heap)

      Appends a :py:class:`spead2.send.Heap` to the memory buffer.

   .. py:method:: getvalue()

      Return a copy of the memory buffer.

      :rtype: :py:class:`bytes`


Asychronous send
^^^^^^^^^^^^^^^^

As for asychronous receives, asynchronous sends are managed by trollius_. A
stream can buffer up multiple heaps for asynchronous send, up to the limit
specified by `max_heaps` in the :py:class:`~spead2.send.StreamConfig`. If this
limit is exceeded, heaps will be dropped. There is currently no mechanism to
distinguish between heaps that were successfully sent and those that were
dropped on the sending side due to buffer space or OS errors, but in future
the futures returned by `async_send_heap` may raise errors.

.. _trollius: http://trollius.readthedocs.org/

.. autoclass:: spead2.send.trollius.UdpStream(thread_pool, hostname, port, config, buffer_size=524288, loop=None)

   .. automethod:: spead2.send.trollius.UdpStream.async_send_heap
   .. py:method:: flush

      Block until all enqueued heaps have been sent (or dropped).
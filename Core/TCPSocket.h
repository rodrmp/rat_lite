#pragma once
#include <memory>
#include <functional>
#include "ISocket.h"

namespace SL {
	namespace Remote_Access_Library {
		namespace INTERNAL {
			struct SocketImpl;
		}
		namespace Network {
			class Packet;
			class IBaseNetworkDriver;

			//this class is async so all calls return immediately and are later executed
			class TCPSocket: public ISocket {
			public:
				//factory to create a socket and connect to the destination address

				static std::shared_ptr<ISocket> ConnectTo(const char* host, const char* port, SL::Remote_Access_Library::Network::IBaseNetworkDriver* netevents);
				TCPSocket(std::shared_ptr<INTERNAL::SocketImpl>& impl);
				
				virtual ~TCPSocket();
				//adds the data to the internal queue, does not block and returns immediately. This calls compress before sending, and calls on_sent_callback once the packet has completed sending the packet
				virtual void send(std::shared_ptr<Packet>& pack, std::function<void()> on_sent_callback) override;
				//adds the data to the internal queue, does not block and returns immediately. This calls compress before sending 
				virtual void send(std::shared_ptr<Packet>& pack) override;
				//sends a request that the socket be closed. NetworkEvents::OnClose will be called when the call is successful
				virtual void close() override;
				void connect(const char* host, const char* port);


				//pending packets which are queued up and waiting to be sent
				virtual unsigned int get_OutgoingPacketCount() const override;
				//pending data which is queued up and waiting to be sent
				virtual unsigned int get_OutgoingByteCount() const override;

			private:
				std::shared_ptr<INTERNAL::SocketImpl> _SocketImpl;

				void do_read_header();
				void do_write();
				void do_write_header();
				void do_read_body();
				

			};
		}
	}
}
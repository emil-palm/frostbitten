require 'uri/generic'
require 'socket'
module URI
	class FrostbiteRCON < Generic
		COMPONENT = [:scheme,:host,:port]
	end
  	@@schemes['fbrcon'] = FrostbiteRCON
end

module Frostbitten
	class Server
		DEFAULTS = {
			# connect/read/write timeout for socket operations
			:socket_timeout => 0.5,
			
			:keepalive => true
		}

		attr_accessor :hostname, :port, :password
		attr_reader :sock, :sequence, :timeout

		def initialize(uri, options={})
			server_parsed = URI.parse(uri)
			self.hostname = server_parsed.host
			self.password = server_parsed.user
			self.port = server_parsed.port ||= 47200

			@sock = nil 
			@sequence = options[:sequence] ||= 0

			@options = DEFAULTS.merge(options)
		end

		def alive?
			return true if @sock
			return false
		end

		def connect
			return if alive?
			@sock = TCPSocket.open(self.hostname, self.port);
			if @options[:socket_timeout]
        		secs = Integer(@options[:socket_timeout])
        		usecs = Integer((@options[:socket_timeout] - secs) * 1_000_000)
        		optval = [secs, usecs].pack("l_2")
        		@sock.setsockopt Socket::SOL_SOCKET, Socket::SO_RCVTIMEO, optval
        		@sock.setsockopt Socket::SOL_SOCKET, Socket::SO_SNDTIMEO, optval
      		end

      		if @options[:keepalive]
      			@sock.setsockopt(Socket::SOL_SOCKET, Socket::SO_KEEPALIVE, true)
      		end
		end

		def close
			return unless alive?
			@sock.close
			@sock = nil
		end

		def send(message) 
			connect unless self.alive?
			unless message.header.is_response?
				@sequence += 1
			end

			message.header.sequence = self.sequence
			message.write @sock

			m = Message.new()
			m.read @sock

			unless m.words.first == "OK"
				m.words.first.tap do |word|
				 	if word == "UnknownCommand"
				 		raise NameError, "UnknownCommand raised by server"
				 	elsif word == "InvalidArguments"
				 		raise ArgumentEror, "InvalidArguments raised by server"
				 	else
				 		raise StandardError, "Server returned #{word}"
				 	end
				 end
			end
			return m
		end

		def auth

		end
	end
end
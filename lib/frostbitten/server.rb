require 'uri'
module URI
  class FrostbiteRCON < Generic
    DEFAULT_PORT = 47200
  end
  @@schemes['fbrcon'] = FrostbiteRCON
end

module Frostbitten
	class Server
		attr_accessor :hostname, :port, :password
		attr_reader :sock, :sequence, :timeout

		def initialize(uri, options={})
			server_parsed = URI.parse(uri)
			self.hostname = server_parsed.host
			self.password = server_parsed.user
			self.port = server_parsed.port
			@sock = nil
			@timeout = options[:timeout]
			@sequence = options[:sequence] ||= 0
		end
	end
end
require 'digest/md5'
require 'frostbitten/methods/admin'
require 'frostbitten/methods/normal'

# Author::    Emil Palm  (mailto:emil@x86.nu)
# Copyright:: Copyright (c) 2013 Emil Palm
# License::   Distributes under the same terms as Ruby

# This class will give you a shorthand to a Server object.
# and give you a easy way to execute methods on server
# that are predefined in this class.

module Frostbitten
	class Client
		# Access to the server object.
		attr_accessor :connection

		# Initialize class with URI 'fbrcon://password@ip:port'
		# Also accept options
		# { :keepalive = true/false, :socket_timeout => 1 }
		# will setup connection object.
		def initialize(uri=nil, options={})
			if uri
				self.connection = Frostbitten::Connection.new(uri,options)
			end
		end

		# Will create Frostbitten::Header and Frostbitten::Message 
		# objects for given commands. 
		# Accepts either a string or a array of strings to send.
		# Returns a list of words returned by server.
		def send(commands)
			return unless self.connection

			if commands.is_a? String
				commands = [commands]
			end

			header = Frostbitten::Header.new(:origin => :client, :response => false)
			msg = Frostbitten::Message.new(:words => commands, :header => header)
			message = connection.send(msg)
			return message.words[1..-1]
		end

		include Frostbitten::Methods::AdminCommands
		include Frostbitten::Methods::NormalCommands
		# :include:methods/admin.rb
		# :include:methods/normal.rb
	end
end
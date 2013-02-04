require 'frostbitten/native'
require 'frostbitten/version'
require 'frostbitten/server'
require 'frostbitten/client'
require 'frostbitten/player'

# Author::    Emil Palm  (mailto:emil@x86.nu)
# Copyright:: Copyright (c) 2013 Emil Palm
# License::   Distributes under the same terms as Ruby

# This module will give you a shorthand to a client object.
# very useful if your only connecting to one server

module Frostbitten
	# Setup the module with a URI 'fbrcon://password@ip:port'
	# Also accepts options { :keepalive = true/false, :socket_timeout => 1 }
	def self.setup(uri,options={})
		@client = Frostbitten::Client.new(uri,options)
	end

	# Method to return the client object if raw access is needed
	def self.conn
		return @client
	end

	# :nodoc:
	def self.method_missing(meth, *args, &block) 
		if @client and @client.respond_to?(meth)
			@client.public_send(meth, *args, &block)
		else
			super
		end
	end
end
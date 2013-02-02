require 'digest/md5'

module Frostbitten
	class Client
		attr_accessor :server


		def initialize(uri=nil, options={})
			if uri
				self.server = Frostbitten::Server.new(uri,options)
			end
		end

		def is_logged_in?
			@logged_in
		end

		def send(commands)
			return unless self.server

			if commands.is_a? String
				commands = [commands]
			end

			header = Frostbitten::Header.new(:origin => :client, :response => false)
			msg = Frostbitten::Message.new(:words => commands, :header => header)
			message = server.send(msg)
			return message.words
		end


		def auth(password=nil)
			unless password
				raise ArgumentError, "No password supplied" unless self.server and self.server.password
			end

			password = password ? password : self.server.password

			data = send("login.hashed")
			salt = [data.last].pack("H*")
			hashed = Digest::MD5.hexdigest("#{salt}#{password}")
			if send(["login.hashed", hashed])
				@logged_in = true
			else
				@logged_in = false
			end
		end
	end
end
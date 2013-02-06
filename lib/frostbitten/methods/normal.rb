require 'pp'
module Frostbitten
	module Methods
		# Methods not requiring admin rcon priveleges
		module NormalCommands
			# Authenticate connection using password or pregiven password in uri.
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

				return self
			end

			# Lists players and will return list of players
			def players(list="all")
				data = send(['listPlayers', list])
				if data
					cols = data.shift.to_i
					keys = data[0..cols-1]
					player = {}
					index = 0
					[].tap do |players|
						data[cols+1..-1].each do |value|
							player[keys[index]] = value
							index += 1
							if index == cols
								index = 0
								players << Frostbitten::Player.new(player)
							end
						end
					end
				end
			end

			# General server information
			def serverinfo
				hash = {}
				scoreentries = 0
				scores = []
				score = []
				score_data = send("serverinfo")
				score_data.each_with_index do |value,index|
					if index == 7
						scoreentries = value.to_i
					elsif (8..(7+scoreentries)).include? index
						score << value
						if (index-7) % scoreentries == 0
							scores << score
							score = []
						end
					else
						key = index + 1
						if scoreentries > 0 
							key = key - scoreentries
						end
						hash["data#{key}"] = value
					end
					hash["data8"] = scores
				end
				return Server.new(hash)
			end
		end
	end
end
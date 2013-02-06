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
				score_data = send("serverinfo")
				x = Frostbitten::Score.scores_from_list(score_data,7)
				score_data.insert(7,x)
				score_data.to_enum.with_index(1).each { |value,key| hash["data#{key}"] = value }
				return Server.new(hash)
			end
		end
	end
end